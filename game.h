// status define
#define SEARCH		0x00
#define WALK		0x01
#define SCARED		0x02
#define PANIC		0x03

#define DIR_LEFT	0x13
#define DIR_RIGHT	0x14

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