#ifndef MUBITMAPFX_H
#define MUBITMAPFX_H

#include "f256lib.h"
#include "../src/muUtils.h" //contains helper functions I often use

#define TIMER_FRAMES 0
#define TIMER_SECONDS 1
#define TIMER_TITLEFALL_COOKIE 2
#define TIMER_TITLEFALL_DELAY 2

void bitmapTitleFallRise(bool, uint32_t);
#endif // MUBITMAPFX_H