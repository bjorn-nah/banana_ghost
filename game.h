// status define
#define SEARCH		0x00
#define WALK		0x01
#define SCARED		0x02
#define PANIC		0x03
#define FALL		0x04
#define FALLING		0x05

#define DIR_LEFT	0x13
#define DIR_RIGHT	0x14

#define NORMAL		0x20
#define LEVEL_UP	0x21

#define UP_GATE		0x01
#define DOWN_GATE	0x02
#define LEFT_GATE	0x04
#define RIGHT_GATE	0x08

#define EXIT		0x80
#define VISITED		0x10



typedef struct EXPLORER_TYPE {
	signed int vpos;			// vsize x8
	signed int hpos;			// hsize x8
	signed int hspeed;			// horizontal speed
	signed int vspeed;			// vertical speed
	unsigned int tics;			// tics for walk
	unsigned char status;		// status
	unsigned int statustics;	// tics for status
	unsigned char direction;
} EXPLORER_TYPE;

typedef struct MAP_LIST {
	SCB_REHV_PAL *first;
} MAP_LIST;