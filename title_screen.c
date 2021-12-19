#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

extern void std_functions();

unsigned int button_title;
extern unsigned char title_screen_bg[];
unsigned int randomizator = 0;

SCB_REHV_PAL screen_bg = {
  BPP_4 | TYPE_NORMAL, 
  REHV,
  0x01,
  0x0000,
  title_screen_bg,
  0, 0,
  0x0100, 0x0100,
  {0xD1,0x23,0x45,0x67,0x89,0xAB,0xC0,0xEF}
};

void screen_logic(){
	unsigned char joy;
	tgi_sprite(&screen_bg);
	tgi_outtextxy(56, 88, "Press A or B");
	
	//tgi_outtextxy(104, 0, "WIP 0.1");
	tgi_outtextxy(88, 64, "#LynxJam");
	tgi_outtextxy(120, 72, "2021");

	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button_title = 2;
	}
	else{
		if (button_title == 2) button_title = 0;
	}
	std_functions();
	tgi_updatedisplay();
}


void title_screen(){
	
	button_title = 1;
	tgi_clear();
	
	
	while(button_title){
		if (!tgi_busy()){
			screen_logic();
			randomizator++;
		}
	}	
	srand(randomizator);
}