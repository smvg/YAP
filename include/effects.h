#ifndef EFFECTS_H
#define EFFECTS_H

#include <string.h>
#include "types.h"
#include "gba.h"

extern short palette_lut[512*3*2] STR_IN_EWRAM;
extern int16_t __fade_lvl;

void fade_in();
void fade_in_step();
void fade_out();
void fade_out_step();
void fade_in_palette(void*, const uint16_t*, int, int);
void fade_out_palette(void*, int, int);

#endif