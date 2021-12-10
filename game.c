#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

#include "game.h"

extern void init_music();
extern void start_music();

unsigned int playing, level, holes;
unsigned char game_status, gates;
unsigned int randomizator2 = 0;

extern unsigned char ghost00_spr[];
extern unsigned char ghost01_spr[];
extern unsigned char explorer00_spr[];
extern unsigned char explorer01_spr[];
extern unsigned char explorer02_spr[];
extern unsigned char explorer03_spr[];
extern unsigned char hole00_spr[];
extern unsigned char hole01_spr[];
extern unsigned char wall_u0[];
extern unsigned char wall_u1[];
extern unsigned char wall_d0[];
extern unsigned char wall_d1[];
extern unsigned char wall_l0[];
extern unsigned char wall_l1[];
extern unsigned char wall_r0[];
extern unsigned char wall_r1[];
extern unsigned char playfield00[];

SCB_REHV_PAL wall_d = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  0x0000,
  wall_d0,
  72, 85,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

SCB_REHV_PAL ghost = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&wall_d,
  ghost00_spr,
  20, 20,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

SCB_REHV_PAL explorer_spr = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&ghost,
  explorer00_spr,
  80, 80,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

SCB_REHV_PAL hole0 = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&explorer_spr,
  hole00_spr,
  40, 40,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

SCB_REHV_PAL hole1 = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&hole0,
  hole00_spr,
  40, 40,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

SCB_REHV_PAL hole2 = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&hole1,
  hole00_spr,
  100, 80,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

SCB_REHV_PAL wall_l = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&hole2,
  wall_l0,
  0, 0,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};
SCB_REHV_PAL wall_r = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&wall_l,
  wall_r0,
  152, 0,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};
SCB_REHV_PAL wall_u = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&wall_r,
  wall_u0,
  8, 0,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};


SCB_REHV_PAL playfield = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&wall_u,
  playfield00,
  8, 16,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

EXPLORER_TYPE explorer = {
	1072,
	1280,
	1, 2,
	20,
	WALK,
	600,
	DIR_RIGHT
};

void init_explorer(){
	explorer.vpos = 1072;			// vsize x16
	explorer.hpos = 1280;			// hsize x16
	explorer.tics = 10;			// tics for walk
	explorer.status = SEARCH;		// status
	explorer.statustics = 300;	// tics for status
	explorer.direction = DIR_LEFT;
	explorer_spr.data = explorer00_spr;
	explorer_spr.vpos = explorer.vpos/16;
	explorer_spr.hpos = explorer.hpos/16;
}

void init_level(){
	signed int vdiff, hdiff;
	unsigned int pop = 1;
	
	while(pop){
		hole0.hpos = rand()/341 + 32;
		hole0.vpos = rand()/862 + 32;			
		vdiff = explorer_spr.vpos - hole0.vpos;
		hdiff = explorer_spr.hpos - hole0.hpos;
		if (!(vdiff > -16 && vdiff < 4 && hdiff > -8 && hdiff < 8)){
			pop = 0;
		}
	}
	
	if(level < 4){
		gates = UP_GATE + DOWN_GATE + LEFT_GATE + RIGHT_GATE;
		holes = 1;
		hole1.data = hole01_spr;
		hole2.data = hole01_spr;
	}
	if(level >= 4 && level < 8){
		gates = rand() & 0x000F;
		holes = 2;
		hole1.data = hole00_spr;
		hole2.data = hole01_spr;
		}
	if(level >= 8){
		gates = rand() & 0x000F;
		holes = 3;
		hole1.data = hole00_spr;
		hole2.data = hole00_spr;
	}
	
	/*
	if(gates == UP_GATE + DOWN_GATE + LEFT_GATE + RIGHT_GATE){walls.data = walls00;}
	if(gates == LEFT_GATE + RIGHT_GATE){walls.data = walls01;}
	if(gates == UP_GATE + DOWN_GATE){walls.data = walls02;}
	*/
	wall_u.data = (gates & UP_GATE) 	? wall_u0 : wall_u1;
	wall_d.data = (gates & DOWN_GATE) 	? wall_d0 : wall_d1;
	wall_l.data = (gates & LEFT_GATE) 	? wall_l0 : wall_l1;
	wall_r.data = (gates & RIGHT_GATE) 	? wall_r0 : wall_r1;
}

void display_level(){
	
}

void explorer_logic(){
	
	explorer.statustics--;
	explorer.tics--;
	
	if(explorer.status == SEARCH){
		if(explorer.tics == 0){
			if(explorer.direction == DIR_LEFT){
				explorer.direction = DIR_RIGHT;
			}
			else{
				explorer.direction = DIR_LEFT;
			}
			explorer.tics = 120;
		}
		if(explorer.statustics == 0){
			explorer.statustics = 600;
			explorer.status = WALK;
			explorer.vspeed = rand()/2048 - 8;
			explorer.hspeed = rand()/2048 - 8;
		}
	}

	if(explorer.status == WALK){
		if(explorer.tics == 0){
			if(explorer_spr.data == explorer00_spr){
				explorer_spr.data = explorer01_spr;
			}
			else
			{
				explorer_spr.data = explorer00_spr;
			}
			explorer.tics = 20;
		}
		if(explorer.hspeed > 0){
			explorer.direction = DIR_RIGHT;
		}
		else
		{
			explorer.direction = DIR_LEFT;
		}
		if(explorer.statustics == 0){
			explorer.statustics = 600;
			explorer.status = SEARCH;
		}
	}
	if(explorer.status == PANIC){
		if(explorer.tics == 0){
			if(explorer_spr.data == explorer02_spr){
				explorer_spr.data = explorer03_spr;
			}
			else
			{
				explorer_spr.data = explorer02_spr;
			}
			explorer.tics = 10;
		}
		if(explorer.hspeed > 0){
			explorer.direction = DIR_RIGHT;
		}
		else
		{
			explorer.direction = DIR_LEFT;
		}
		if(explorer.statustics == 0){
			explorer.statustics = 600;
			explorer.status = SEARCH;
		}
	}
	if(explorer.status == SCARED){
			explorer.statustics = 600;
			explorer.status = PANIC;
			explorer.vspeed = explorer_spr.vpos - ghost.vpos;
			explorer.hspeed = explorer_spr.hpos - ghost.hpos;
			explorer.tics = 10;
	}
	
	if(explorer.status == FALL){
			explorer.statustics = 120;
			explorer.status = FALLING;
			explorer.tics = 10;
			explorer_spr.data = explorer02_spr;
	}
	
	if(explorer.status == FALLING){
			if(explorer.tics == 0){
				if(explorer.direction == DIR_LEFT){
					explorer.direction = DIR_RIGHT;
				}
				else{
					explorer.direction = DIR_LEFT;
				}
				explorer.tics = 10;
			}
			if(explorer.statustics == 0){
				playing = 0;
			}
	}
	
	if(explorer.status == PANIC || explorer.status == WALK){
		explorer.vpos += explorer.vspeed;
		explorer.hpos += explorer.hspeed;
		explorer_spr.vpos = explorer.vpos/16;
		explorer_spr.hpos = explorer.hpos/16;
		if(explorer_spr.vpos < 8 || explorer_spr.vpos > 102-8){
			explorer.vspeed = -explorer.vspeed;
		}
		if(explorer_spr.hpos < 8 || explorer_spr.hpos > 160-8){
			explorer.hspeed = -explorer.hspeed;
		}
	}
	
	explorer_spr.vpos = explorer.vpos/16;
	explorer_spr.hpos = explorer.hpos/16;
	
	if(explorer.direction == DIR_LEFT){
		explorer_spr.sprctl0 = BPP_4 | TYPE_NORMAL;
	}
	else
	{
		explorer_spr.sprctl0 = BPP_4 | TYPE_NORMAL | HFLIP;
	}

}

void physics(){
	signed int vdiff, hdiff;
	vdiff = explorer_spr.vpos - hole0.vpos;
	hdiff = explorer_spr.hpos - hole0.hpos;
	if ((vdiff > -14 && vdiff < 2) && (hdiff > -8 && hdiff < 8) && explorer.status != FALLING){
		explorer.status = FALL;
	}
	
	// gates
	if(explorer_spr.hpos > 72 && explorer_spr.hpos < 88 && explorer_spr.vpos<10 && (gates & UP_GATE)){
		game_status = LEVEL_UP;
	}
	if(explorer_spr.hpos > 72 && explorer_spr.hpos < 88 && explorer_spr.vpos>92 && (gates & DOWN_GATE)){
		game_status = LEVEL_UP;
	}
	if(explorer_spr.hpos < 10 && explorer_spr.vpos > 43 && explorer_spr.vpos < 59 && (gates & LEFT_GATE)){
		game_status = LEVEL_UP;
	}
	if(explorer_spr.hpos > 150 && explorer_spr.vpos > 43 && explorer_spr.vpos < 59 && (gates & RIGHT_GATE)){
		game_status = LEVEL_UP;
	}
	
}

void game_logic(){
	unsigned char joy;
	
	tgi_clear();
	//tgi_outtextxy(16, 16, "This is the game!");
	
	
	tgi_sprite(&playfield);
	tgi_sprite(&wall_u);
	tgi_sprite(&wall_l);
	tgi_sprite(&wall_r);
	tgi_sprite(&hole0);
	tgi_sprite(&hole1);
	tgi_sprite(&hole2);
	tgi_sprite(&explorer_spr);
	tgi_sprite(&ghost);
	tgi_sprite(&wall_d);
	
	joy = joy_read(JOY_1);
	
	if (JOY_UP(joy) && ghost.vpos > 8) {
		ghost.vpos--;
	}
	if (JOY_DOWN(joy) && ghost.vpos < 102-8) {
		ghost.vpos++;
	}
	if (JOY_LEFT(joy) && ghost.hpos > 8) {
		ghost.hpos--;
		ghost.sprctl0 = BPP_4 | TYPE_NORMAL;
	}
	if (JOY_RIGHT(joy)&& ghost.hpos < 160-8) {
		ghost.hpos++;
		ghost.sprctl0 = BPP_4 | TYPE_NORMAL | HFLIP;
	}
	if (JOY_BTN_2(joy)) {
		ghost.data = ghost01_spr;
		if (explorer.status != PANIC && explorer.status != FALL && explorer.status != FALLING){
			explorer.status = SCARED;
		}
	}
	else{
		ghost.data = ghost00_spr;
	}
	
	/*
	if (JOY_BTN_1(joy)) {
		playing = 2;
	}
	else{
		if (playing == 2) playing = 0;
	}*/
}


void game(){
	char text[20];
	
	playing = 1;
	level = 1;
	game_status = NORMAL;
	tgi_clear();
	init_explorer();
	init_level();
	
	init_music();
	start_music();
	
	while(playing){
		if (!tgi_busy())
		{
			if(game_status == LEVEL_UP){
				level++;
				game_status = NORMAL;
				init_explorer();
				init_level();
				srand(randomizator2);
			}
			game_logic();
			explorer_logic();
			physics();
			itoa(level, text, 10);
			tgi_outtextxy(8, 2, text);
			tgi_updatedisplay();
			randomizator2++;
		}
	}	
	
	asm("jsr SndStopAll");

}