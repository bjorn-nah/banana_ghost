#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

#include "game.h"

unsigned int button;
extern unsigned char ghost00_spr[];
extern unsigned char ghost01_spr[];
extern unsigned char explorer00_spr[];
extern unsigned char explorer01_spr[];
extern unsigned char explorer02_spr[];
extern unsigned char explorer03_spr[];
extern unsigned char playfield00[];

SCB_REHV_PAL ghost = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  0x0000,
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

SCB_REHV_PAL playfield = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  (char *)&explorer_spr,
  playfield00,
  0, 0,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

EXPLORER_TYPE explorer = {
	640,
	640,
	1, 2,
	20,
	WALK,
	600,
	DIR_RIGHT
};

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
	else{
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
	
	if(explorer.direction == DIR_LEFT){
		explorer_spr.sprctl0 = BPP_4 | TYPE_NORMAL;
	}
	else
	{
		explorer_spr.sprctl0 = BPP_4 | TYPE_NORMAL | HFLIP;
	}

}

void game_logic(){
	unsigned char joy;
	
	tgi_clear();
	//tgi_outtextxy(16, 16, "This is the game!");
	
	
	tgi_sprite(&playfield);
	tgi_sprite(&explorer_spr);
	tgi_sprite(&ghost);
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
		if (explorer.status != PANIC){
			explorer.status = SCARED;
		}
	}
	else{
		ghost.data = ghost00_spr;
	}
	
	if (JOY_BTN_1(joy)) {
		button = 2;
	}
	else{
		if (button == 2) button = 0;
	}
	tgi_updatedisplay();
}


void game(){
	
	button = 1;
	tgi_clear();
	
	while(button){
		if (!tgi_busy())
		{
			game_logic();
			explorer_logic();
		}
	}	

}