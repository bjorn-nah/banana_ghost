#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 
#include <peekpoke.h>

extern int title_screen();
extern int game();
extern int result_screen();

extern unsigned int pause;

// palette values can be found in .pal file provided by sprpck
static int palette[] =  {
  0x0000, 0x0008, 0x0800, 0x0808, 0x0080, 0x0088, 0x0880, 0x0888, 0x0CCC, 0x000F, 0x0F00, 0x0F0F, 0x00F0, 0x0000, 0x0FF0, 0x0FFF
};

unsigned int level;

void setpalette(const int* palette)
{
  char index;
  for (index = 0; index < 0x10; index++)
  {
    POKE(0xFDA0 + index, palette[index] >> 8);
    POKE(0xFDB0 + index, palette[index] & 0xFF);
  }
}

void init_music(void)
{
	asm("jsr SndInit");
}

void start_music(void)
{
	asm("jsr SndPauseOn");
	asm("lda musicptr+0");
	asm("ldy musicptr+1");
	asm("ldx #0");
	asm("jsr SndStartSound");
	asm("lda musicptr+2");
	asm("ldy musicptr+3");
	asm("ldx #1");
	asm("jsr SndStartSound");
	asm("lda musicptr+4");
	asm("ldy musicptr+5");
	asm("ldx #2");
	asm("jsr SndStartSound");
	asm("lda musicptr+6");
	asm("ldy musicptr+7");
	asm("ldx #3");
	asm("jsr SndStartSound");
	asm("jsr SndPauseOff");
}

void initialize()
{
	tgi_install(&tgi_static_stddrv);
	joy_install(&joy_static_stddrv);
	tgi_init();

	CLI();

	while (tgi_busy()) ;
	
	//tgi_setpalette(tgi_getdefpalette());
	setpalette(palette);
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(COLOR_BLACK);

	tgi_clear();
}

void reboot()
{
	asm("sei");
	asm("stz $FFF9");
	asm("jmp ($FFFC)");
}

void std_functions()
{
	if (kbhit()){
		switch (cgetc())
			{
			case 'F':
				tgi_flip();
				break;
			case 'R':
				reboot();
				break;
			case 'P':
				pause = !pause;
				break;

			default:
				break;
			}
	}
}



void main(void)
{
	initialize();

	while (1)
	{
		title_screen();
		game();
		result_screen();
	}
	return;
}