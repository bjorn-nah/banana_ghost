#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 

extern unsigned int level;
extern void std_functions();

unsigned int button_result;

void end_logic(){
	unsigned char joy;
	char text[20];
	
	tgi_clear();
	
	if(level > 16){
		tgi_outtextxy(16, 16, "CONGRATULATIONS!");
		tgi_outtextxy(8, 32, "You have completed");
		tgi_outtextxy(8, 40, "enough levels to");
		tgi_outtextxy(8, 48, "show this screen.");
		tgi_outtextxy(8, 62, "Thank you");
		tgi_outtextxy(56, 70, "for playing!");
	}else{
		tgi_outtextxy(44, 32, "GAME OVER");
		tgi_outtextxy(8, 48, "You reach level");
		itoa(level, text, 10);
		tgi_outtextxy(136, 48, text);
	}
	
	tgi_outtextxy(56, 88, "Press A or B");

	joy = joy_read(JOY_1);
	if (JOY_BTN_1(joy) || JOY_BTN_2(joy) ) {
		button_result = 2;
	}
	else{
		if (button_result == 2) button_result = 0;
	}
	std_functions();
	tgi_updatedisplay();
}


void result_screen(){
	
	button_result = 1;
	tgi_clear();	
	
	while(button_result){
		if (!tgi_busy()){
			end_logic();
		}
	}
}