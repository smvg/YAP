#include "effects.h"

int16_t __fade_lvl = 0;
short palette_lut[512*3*2] STR_IN_EWRAM;

/**
 * @brief Fades in. Will not exit until the effect has completed.
 * 
 */
void fade_in()
{
	__fxcnt = FX_T1_ALL | FX_MODE_DARK;

	while (__fade_lvl > 0) {
		swi_vblank();
		__fxval_int = --__fade_lvl;
	}
}

/**
 * @brief Fades out. Will not exit until the effect has completed.
 * 
 */
void fade_out()
{
	__fxcnt = FX_T1_ALL | FX_MODE_DARK;

	while (__fade_lvl < 16) {
		swi_vblank();
		__fxval_int = ++__fade_lvl;
	}
}

/**
 * @brief Increments the fading in effect by one. 
 * If value exceeds boundary, nothing happens.
 * 
 */
inline void fade_in_step()
{
	__fxcnt = FX_T1_ALL | FX_MODE_DARK;

	if (__fade_lvl > 0) {
		__fxval_int = --__fade_lvl;
	}
}

/**
 * @brief Increments the fading out effect by one. 
 * If value exceeds boundary, nothing happens.
 * 
 */
inline void fade_out_step()
{
	__fxcnt = FX_T1_ALL | FX_MODE_DARK;

	if (__fade_lvl < 16) {
		__fxval_int = ++__fade_lvl;
	}
}

/**
 * @brief Alternative fading in effect. Based on an implementation of the libgba library.
 * More resource intensive but effect can last longer without looking "bad". It updates
 * the palette of a sprite or background.
 * 
 * @param palAddr: Address of the palette to use.
 * @param newPalette: Final state of the effect.
 * @param palLen: Length of "new_palette".
 * @param frames: Length of the effect, measured in frames.
 */
void fade_in_palette(void *palAddr, const uint16_t *newPalette, int palLen, int frames)
{
	int x, r, g, b;
	int16_t *lut_ptr;
	volatile uint16_t *curr_palette;
	memset((void *)palAddr, 0, 512);

	lut_ptr = palette_lut;
	curr_palette = (volatile uint16_t*) palAddr;
	for (x = 0; x < palLen; x++) {
		*(lut_ptr++) = ((newPalette[x] & 0x1F) << 8) / frames;
		*(lut_ptr++) = (curr_palette[x] & 0x1F) << 8;

		*(lut_ptr++) = ((newPalette[x] >> 5 & 0x1F) << 8) / frames;
		*(lut_ptr++) = (curr_palette[x] >> 5 & 0x1F) << 8;

		*(lut_ptr++) = ((newPalette[x] >> 10 & 0x1F) << 8) / frames;
		*(lut_ptr++) = (curr_palette[x] >> 10 & 0x1F) << 8;

	}

	while (frames--) {

		swi_vblank();

		lut_ptr = palette_lut;		
		for (x = 0; x < palLen; x++) {
			r = *(lut_ptr++);
			r += *lut_ptr;
			*(lut_ptr++) = r;

			g = *(lut_ptr++);
			g += *lut_ptr;
			*(lut_ptr++) = g;

			b = *(lut_ptr++);
			b += *lut_ptr;
			*(lut_ptr++) = b;

			curr_palette[x] = (r >> 8) | ((g >> 8) << 5) | ((b >> 8) << 10);
		}
	}
}

/**
 * @brief Alternative fading out effect. Based on an implementation of the libgba library.
 * More resource intensive but effect can last longer without looking "bad". It updates
 * the palette of a sprite or background.
 * 
 * @param palAddr: Address of the palette to use.
 * @param palLen: Length of "new_palette".
 * @param frames: Length of the effect, measured in frames.
 */
void fade_out_palette(void *palAddr, int palLen, int frames)
{
	int x, r, g, b;
	uint16_t *curr_palette;
	int16_t *lut_ptr;

	curr_palette = (uint16_t*) palAddr;
	lut_ptr = palette_lut;
	for (x = 0; x < palLen; x++) {
		*(lut_ptr++) = -(((curr_palette[x] & 0x1F) << 8) / frames);
		*(lut_ptr++) = (curr_palette[x] & 0x1F) << 8;

		*(lut_ptr++) = -(((curr_palette[x] >> 5 & 0x1F) << 8) / frames);
		*(lut_ptr++) = (curr_palette[x] >> 5 & 0x1F) << 8;

		*(lut_ptr++) = -(((curr_palette[x] >> 10 & 0x1F) << 8) / frames);
		*(lut_ptr++) = (curr_palette[x] >> 10 & 0x1F) << 8;

	}

	while (frames--) {

		swi_vblank();

		lut_ptr = palette_lut;
		for (x = 0; x < palLen; x++) {
			r = *(lut_ptr++);
			r += *lut_ptr;
			*(lut_ptr++) = r;

			g = *(lut_ptr++);
			g += *lut_ptr;
			*(lut_ptr++) = g;

			b = *(lut_ptr++);
			b += *lut_ptr;
			*(lut_ptr++) = b;

			curr_palette[x] = (r >> 8) | ((g >> 8) << 5) | ((b >> 8) << 10);
		}
	}
}