#include "scene_0.h"
#include "title_screen_1.h"

#include "effects.h"
#include "utils.h"
#include "font.h"
#include "text.h"
#include "config.h"
#include <string.h>

/**
 * @brief Copy necessary assets
 * 
 */
inline void init_title() {
	dma_full_transfer_3((void*)__vram, title_screen_1Bitmap, BYTES_TO_WORDS(title_screen_1BitmapLen));
	dma_full_transfer_3(&tile_mem[4][textDefaultTileIdx], fontTiles, BYTES_TO_WORDS(fontTilesLen));
}

/**
 * @brief University logo
 * 
 */
inline void run_title_logo() {

	__dispcnt = DC_RENDER_BG2 | DC_MODE_4;

	fade_in_palette((void*)__palette, title_screen_1Pal, 34, CREDIT_FADE_PERIOD);
	halt_with_key(CREDIT_HOLD_PERIOD, KY_A);
	fade_out_palette((void*)__palette, 34, CREDIT_FADE_PERIOD);
	halt_with_key(CREDIT_BETWEEN_PERIOD, KY_A);
}

/**
 * @brief Author's name
 * 
 */
inline void run_title_author_prog() {

	__dispcnt = DC_RENDER_BG0 | DC_RENDER_OBJ | DC_OM_1D;
	
	clear_oam();

	// PROGRAMMING BY SAM MARTIN VARGAS GIAGNOCAVO
	print_hcenter_4bpp(CREDIT_AUTHOR, 50, textDefaultTileIdx);
	fade_in_palette((void*)&pal_obj_mem[textDefaultPalIdx], fontPal, 16, CREDIT_FADE_PERIOD);
	halt_with_key(CREDIT_HOLD_PERIOD, KY_A);
	fade_out_palette((void*)&pal_obj_mem[textDefaultPalIdx], 16, CREDIT_FADE_PERIOD);
	halt_with_key(CREDIT_BETWEEN_PERIOD, KY_A);

}

/**
 * @brief Names of the artists that created the game's art
 * 
 */
inline void run_title_author_art() {

	__dispcnt = DC_RENDER_BG0 | DC_RENDER_OBJ | DC_OM_1D;

	clear_oam();

	// ART BY DAGON & EEVE SOMEPX
	print_hcenter_4bpp(CREDIT_ART, 50, textDefaultTileIdx);
	fade_in_palette((void*)&pal_obj_mem[textDefaultPalIdx], fontPal, 16, CREDIT_FADE_PERIOD);
	halt_with_key(CREDIT_HOLD_PERIOD, KY_A);
	fade_out_palette((void*)&pal_obj_mem[textDefaultPalIdx], 16, CREDIT_FADE_PERIOD);
	halt_with_key(CREDIT_BETWEEN_PERIOD, KY_A);
}

/**
 * @brief Names of the artists that created the game's music
 * 
 */
inline void run_title_author_music() {

	__dispcnt = DC_RENDER_BG0 | DC_RENDER_OBJ | DC_OM_1D;

	clear_oam();
	
	// MUSIC BY NINJA
	print_hcenter_4bpp(CREDIT_MUSIC, 50, textDefaultTileIdx);
	fade_in_palette((void*)&pal_obj_mem[textDefaultPalIdx], fontPal, 16, CREDIT_FADE_PERIOD);
	halt(CREDIT_HOLD_PERIOD);
	fade_out_palette((void*)&pal_obj_mem[textDefaultPalIdx], 16, CREDIT_FADE_PERIOD);
	halt_with_key(CREDIT_BETWEEN_PERIOD, KY_A);

}
