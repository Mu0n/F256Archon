#define F256LIB_IMPLEMENTATION
#include "f256lib.h"
#include "../src/muUtils.h" //contains helper functions I often use
#include "../src/muMidi.h"  //contains basic MIDI functions I often use
#include "../src/timer0.h"  //contains basic cpu based timer0 functions I often use
#include "../src/muMidiPlay.h"  //contains the function to play an embedded .dim file
#include "../src/muBitmapFX.h" //contains the fall and rise Bitmap effect

#define MIDI_BASE   0x40000 //gives a nice 07ffkb until the parsed version happens
#define MIDI_PARSED 0x50000 //end of ram is 0x7FFFF, gives a nice 256kb of parsed midi

#define PAL_BASE 0x10000
#define BMP_BASE 0x10400
#define BLANK_BASE 0x23000
#define MUSIC_BASE 0x50000

EMBED(title,     "../assets/title.pal",  0x10000); //1kb
EMBED(paltitle,  "../assets/title.bin" , 0x10400); //75kb
EMBED(music,     "../assets/archon.dim", 0x50000); //about 8kb
//0x12C00 of blank space

//STRUCTS

void setup(void);

void setup()
{	
//	uint8_t machineCheck;
	uint32_t a;
	
	openAllCODEC(); //if the VS1053b is used, this might be necessary for some board revisions	

	//wipeBitmapBackground(0x2F,0x2F,0x2F);
	POKE(MMU_IO_CTRL, 0x00);
	      // XXX GAMMA  SPRITE   TILE  | BITMAP  GRAPH  OVRLY  TEXT
	POKE(VKY_MSTR_CTRL_0, 0b00001111); //sprite,graph,overlay,text
	      // XXX XXX  FON_SET FON_OVLY | MON_SLP DBL_Y  DBL_X  CLK_70
	POKE(VKY_MSTR_CTRL_1, 0b00010100); //font overlay, double height text, 320x240 at 60 Hz;
	POKE(MMU_IO_CTRL,1);  //MMU I/O to page 1

	// Set up CLUT0.
	for(uint16_t c=0;c<1023;c++) 
	{
		POKE(VKY_GR_CLUT_0+c, FAR_PEEK(PAL_BASE+c));
	}
	POKE(MMU_IO_CTRL,0);
	
	bitmapSetActive(0);
	bitmapSetAddress(0,BLANK_BASE);
	bitmapSetCLUT(0);
	
	bitmapSetVisible(0,false);
	
//	machineCheck=PEEK(0xD6A7)&0x3F;

	for(a = 0 ; a<((uint32_t)320*(uint32_t)240); a++) FAR_POKE(BLANK_BASE+a,0); //empty out a whole screen of bitmap after the loaded title screen bmp

	resetInstruments(false); //resets all channels to piano, for sam2695
	midiShutUp(false); //ends trailing previous notes if any, for sam2695
}



int main(int argc, char *argv[]) {
	bool isDone = false; //to know when to exit the main loop; done playing
	
	textGotoXY(0,0);printf("Please wait...");
	setup(); //codec, timer and  graphic setup
	textGotoXY(0,0);printf("              ");
//INTRO SECTION
//title bitmap fall animation		
	bitmapTitleFallRise(true, BLANK_BASE); 
//play song
	playEmbeddedDim(MUSIC_BASE);
	midiShutAllChannels(false);	
	bitmapTitleFallRise(false, BLANK_BASE); 
	
	
	textGotoXY(0,0);printf("[Insert Game Here]!");
//MAIN LOOP SECTION
	while(!isDone)
		{
		kernelNextEvent();
		if(kernelEventData.type == kernelEvent(key.PRESSED))
			{
			if(kernelEventData.key.raw == 0x92) isDone = true; //ESC, top left back arrow to quit
			}
		}	
	return 0;
	}
	