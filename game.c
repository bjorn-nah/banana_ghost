#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

unsigned int button;

void game_logic(){
	unsigned char joy;
	
	tgi_outtextxy(16, 16, "This is the game!");
	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button = 2;
	}
	else{
		if (button == 2) button = 0;
	}
}


void game(){
	
	button = 1;
	tgi_clear();
	while(button){
		game_logic();
	}	

}