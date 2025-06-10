#include "D:\F256\llvm-mos\code\F256Archon\.builddir\trampoline.h"

#include "f256lib.h"
#include "../src/muBitmapFX.h"

void bitmapTitleFallRise(bool isDown, uint32_t blankBase) //isDown = true goes down, = false goes up
{
	bool isFallDone = false; //title screen falling is done
	uint8_t fallCount = 0;
	uint32_t start, offset;
	struct timer_t titleFallTimer;
	
	titleFallTimer.units = TIMER_FRAMES;
	titleFallTimer.cookie = TIMER_TITLEFALL_COOKIE;
	
	titleFallTimer.absolute = getTimerAbsolute(TIMER_FRAMES) + TIMER_TITLEFALL_DELAY;
	setTimer(&titleFallTimer);
	
	bitmapSetVisible(0,true);
	
	if(isDown)
	{
		start = blankBase;
		offset = -1;
	}
	else
	{
		start = (blankBase-((uint32_t)240*(uint32_t)320));
		offset = +1;
	}
	while(!isFallDone)
		{
		kernelNextEvent();
		if(kernelEventData.type == kernelEvent(timer.EXPIRED))
			{
			if(kernelEventData.timer.cookie == TIMER_TITLEFALL_COOKIE)
				{
					fallCount+=10;
					if(fallCount == 240) isFallDone = true;
	
					bitmapSetAddress(0,start + offset * fallCount * 320);
	
					titleFallTimer.absolute = getTimerAbsolute(TIMER_FRAMES) + TIMER_TITLEFALL_DELAY;
					setTimer(&titleFallTimer);
				}
			}
		}
}
