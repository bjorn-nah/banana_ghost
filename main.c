#include <6502.h>
#include <lynx.h>
#include <tgi.h>
#include <stdlib.h>
#include <conio.h>
#include <joystick.h> 
#include <peekpoke.h>

extern int title_screen();
extern int game();

// palette values can be found in .pal file provided by sprpck
static int palette[] =  {
  0x0000, 0x0008, 0x0800, 0x0808, 0x0080, 0x0088, 0x0880, 0x0888, 0x0CCC, 0x000F, 0x0F00, 0x0F0F, 0x00F0, 0x0000, 0x0FF0, 0x0FFF
};

void setpalette(const int* palette)
{
  char index;
  for (index = 0; index < 0x10; index++)
  {
    POKE(0xFDA0 + index, palette[index] >> 8);
    POKE(0xFDB0 + index, palette[index] & 0xFF);
  }
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



void main(void)
{
	initialize();

	while (1)
	{
		title_screen();
		game();
	}
	return;
}