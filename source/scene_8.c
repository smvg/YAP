#include "scene_8.h"

#include "gba.h"
#include <string.h>

#include "effects.h"
#include "game.h"
#include "text.h"
#include "palette_bin.h"
#include "level_6_tiles_bin.h"
#include "ingame_tiles_bin.h"
#include "level_bg_0_map_bin.h"
#include "level_6_col_map_bin.h"
#include "level_6_misc_map_bin.h"
#include "ingame_ui_map_bin.h"
#include "level_music_bin.h"
#include "sprite.h"
#include "hearts.h"
#include "slime.h"
#include "fly.h"
#include "cursor.h"
#include "font.h"

inline void init_level_3_1()
{

	fade_out();

	clear_oam();

	// Copy necessary assets
	dma_full_transfer_3(&pal_obj_mem[heroDefaultPalIdx], spritePalBlue, BYTES_TO_WORDS(spritePalLen));
	dma_full_transfer_3(&pal_obj_mem[slimeDefaultPalIdx], slimePal, BYTES_TO_WORDS(slimePalLen));
	dma_full_transfer_3(&pal_obj_mem[flyDefaultPalIdx], flyPal, BYTES_TO_WORDS(flyPalLen));
	dma_full_transfer_3(&pal_obj_mem[heartDefaultPalIdx], heartsPal, BYTES_TO_WORDS(heartsPalLen));
	dma_full_transfer_3(&pal_obj_mem[cursorDefaultPalIdx], cursorPal, BYTES_TO_WORDS(cursorPalLen));
	dma_full_transfer_3(&pal_obj_mem[textDefaultPalIdx], fontPalInverted, BYTES_TO_WORDS(fontPalInvertedLen));

	dma_full_transfer_3(&tile_mem[0][0], level_6_tiles_bin, BYTES_TO_WORDS(level_6_tiles_bin_size));
	dma_full_transfer_3(&tile_mem[2][0], ingame_tiles_bin, BYTES_TO_WORDS(ingame_tiles_bin_size));

	dma_full_transfer_3(&tile_mem[4][heroDefaultTileIdx], spriteTiles, BYTES_TO_WORDS(spriteTilesLen));
	dma_full_transfer_3(&tile_mem[4][slimeDefaultTileIdx], slimeTiles, BYTES_TO_WORDS(slimeTilesLen));
	dma_full_transfer_3(&tile_mem[4][flyDefaultTileIdx], flyTiles, BYTES_TO_WORDS(flyTilesLen));
	dma_full_transfer_3(&tile_mem[4][heartDefaultTileIdx], heartsTiles, BYTES_TO_WORDS(heartsTilesLen));
	dma_full_transfer_3(&tile_mem[4][cursorDefaultTileIdx], cursorTiles, BYTES_TO_WORDS(cursorTilesLen));
	dma_full_transfer_3(&tile_mem[4][textDefaultTileIdx], fontTiles, BYTES_TO_WORDS(fontTilesLen));

	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 0][0], &level_6_col_map_bin[0], level_6_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 1][0], &level_6_col_map_bin[4096], level_6_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 2][0], &level_6_col_map_bin[2048], level_6_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 3][0], &level_6_col_map_bin[6144], level_6_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 4][0], &level_6_misc_map_bin[0], level_6_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 5][0], &level_6_misc_map_bin[4096], level_6_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 6][0], &level_6_misc_map_bin[2048], level_6_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 7][0], &level_6_misc_map_bin[6144], level_6_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 8][0], level_bg_0_map_bin, BYTES_TO_WORDS(level_bg_0_map_bin_size));
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 9][0], ingame_ui_map_bin, BYTES_TO_WORDS(ingame_ui_map_bin_size));

	dma_full_transfer_3(pal_bg_mem, palette_bin, BYTES_TO_WORDS(palette_bin_size));

	__dispcnt = DC_RENDER_BG0 | DC_RENDER_BG1 | DC_RENDER_BG2 | DC_RENDER_OBJ | DC_OM_1D | DC_MODE_0;

}

inline int run_level_3_1()
{

	uint32_t num_enemies, next_scene;

	next_scene = END_GAME;

	reset_hero_to(FIXED(32), FIXED(34));
	build_hero();

	num_enemies = 0;

	BG_CNT_0 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+0) | BG_512x512;
	BG_CNT_1 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+4) | BG_512x512;
	BG_CNT_2 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+8) | BG_256x256;
	BG_CNT_3 = BG_PRIORITY(0) | BG_CBB(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+9) | BG_256x256;

	build_ui();

	reset_dynamic_bg_to(FIXED(352), 0);
	reset_static_bg_to(0, 0);

	add_enemy_fly(FIXED(484), FIXED(400), FIXED(2), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(368), FIXED(320), FIXED(2), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(304), FIXED(480), DIV2(FIXED(1)), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(272), FIXED(272), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(272), FIXED(176), DIV2(FIXED(1)), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(224), FIXED(112), FIXED(2), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(144), FIXED(144), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(96), FIXED(176), FIXED(2), DIR_LEFT, &num_enemies);

	reset_deltas();

	int32_t inc_x, inc_y;
	inc_x = inc_y = 0;

	fade_in();

	// Game loop
	while (FOREVER) {

		swi_vblank();
		get_key();

		adjust_hero_position(&inc_y, &inc_x);
		process_sprite_collisions(num_enemies);
		update_hero_render();
		update_hero_state(&inc_y, &inc_x);
		update_enemies_render(&num_enemies);
		update_enemies_state(num_enemies);
		update_ui_render();
		update_pause_render(&next_scene);

		// Hero condition
		if (__hero.life == 0) {
			next_scene = LEVEL_3_0;
			kill_hero();
		}

		// Scene condition
		if ((__hero.y + __voff <= FIXED(50)
			&& __hero.x + __hoff >= FIXED(424))
			|| next_scene != END_GAME) {
			break;
		}

		save_key();
		increase_delta();
	}

	// END GAME
	if (next_scene == END_GAME) {
		fade_out();
		clear_oam();
		__dispcnt = DC_RENDER_OBJ | DC_OM_1D;
		dma_full_transfer_3(&pal_obj_mem[textDefaultPalIdx], fontPal, fontPalLen >> 2);
		print_hcenter_4bpp(END_GAME_MSG, 50, textDefaultTileIdx);
		fade_in();
		halt(END_GAME_HOLD_PERIOD);
		// Reset save
		__gamepak[0] = 0;
		next_scene = MAIN_MENU;
	}

	dma_stop_transfer_1();

	return next_scene;
}
