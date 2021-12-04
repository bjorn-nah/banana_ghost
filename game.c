#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

unsigned int button;
extern unsigned char ghost00[];
extern unsigned char ghost01[];

SCB_REHV_PAL ghost = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  0x0000,
  ghost00,
  20, 20,
  0x0100, 0x0100,
  // 0 and D are inverted to make magenta transluent
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

void game_logic(){
	unsigned char joy;
	
	tgi_clear();
	tgi_outtextxy(16, 16, "This is the game!");
	
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
		ghost.data = ghost01;
	}
	else{
		ghost.data = ghost00;
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
		}
	}	

}