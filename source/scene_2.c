#include "scene_2.h"

#include "gba.h"
#include <string.h>

#include "effects.h"
#include "game.h"
#include "palette_bin.h"
#include "controls_tiles_bin.h"
#include "controls_bg_map_bin.h"
#include "controls_col_map_bin.h"
#include "controls_ui_map_bin.h"
#include "sprite.h"
#include "font.h"

inline void init_controls()
{
	/* RESET SOUND */
	SND_CNT = SND_MASTER = 0;
	T0_CNT = T1_CNT = T2_CNT = 0;

	fade_out();

	clear_oam();

	// Copy necessary assets
	dma_full_transfer_3(&pal_obj_mem[0], spritePalBlue, BYTES_TO_WORDS(spritePalLen));
	dma_full_transfer_3(&pal_obj_mem[textDefaultPalIdx], fontPal, BYTES_TO_WORDS(fontPalLen));

	dma_full_transfer_3(&tile_mem[0][0], controls_tiles_bin, BYTES_TO_WORDS(controls_tiles_bin_size));

	dma_full_transfer_3(&tile_mem[4][0], spriteTiles, BYTES_TO_WORDS(spriteTilesLen));
	dma_full_transfer_3(&tile_mem[4][textDefaultTileIdx], fontTiles, BYTES_TO_WORDS(fontTilesLen));

	dma_full_transfer_3(&map_mem[SCRBLK_IDX+0][0], controls_col_map_bin, BYTES_TO_WORDS(controls_col_map_bin_size));
	dma_full_transfer_3(&map_mem[SCRBLK_IDX+1][0], controls_ui_map_bin, BYTES_TO_WORDS(controls_ui_map_bin_size));
	dma_full_transfer_3(&map_mem[SCRBLK_IDX+2][0], controls_bg_map_bin, BYTES_TO_WORDS(controls_bg_map_bin_size));

	dma_full_transfer_3(pal_bg_mem, palette_bin, BYTES_TO_WORDS(palette_bin_size));

	__dispcnt = DC_RENDER_BG0 | DC_RENDER_BG1 | DC_RENDER_BG2 | DC_RENDER_OBJ | DC_OM_1D | DC_MODE_0;

}

inline int run_controls()
{

	uint32_t  next_level;

	next_level = MAIN_MENU;

	reset_hero_to(FIXED(80), FIXED(208));
	build_hero();

	BG_CNT_0 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+0) | BG_256x256;
	BG_CNT_1 = BG_PRIORITY(1) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+1) | BG_256x256;
	BG_CNT_2 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+2) | BG_256x256;

	reset_dynamic_bg_to(FIXED(8), FIXED(0));
	reset_static_bg_to(0, 0);

	reset_deltas();

	int32_t inc_x, inc_y;
	inc_x = inc_y = 0;

	print_hcenter_4bpp("MOVE", 12, textDefaultTileIdx);
	print_hcenter_4bpp("TRANSFORM", 44, textDefaultTileIdx);
	print_hcenter_4bpp("JUMP", 76, textDefaultTileIdx);
	print_hcenter_4bpp("ATTACK", 108, textDefaultTileIdx);
	print_hcenter_4bpp("PAUSE AND EXIT", 140, textDefaultTileIdx);

	fade_in();

	while (FOREVER) {

		swi_vblank();
		get_key();

		__hero.y += inc_y;
		__hero.x += inc_x;
		update_hero_render();
		update_hero_state(&inc_y, &inc_x);

		if (key_press(__curr_key, KY_START)) {
			break;
		}

		save_key();
		increase_delta();
	}

	return next_level;
}
