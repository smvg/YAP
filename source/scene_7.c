#include "scene_7.h"

#include "gba.h"
#include <string.h>

#include "effects.h"
#include "game.h"
#include "text.h"
#include "palette_bin.h"
#include "level_5_tiles_bin.h"
#include "ingame_tiles_bin.h"
#include "level_5_bg_map_bin.h"
#include "level_5_col_map_bin.h"
#include "level_5_misc_map_bin.h"
#include "ingame_ui_map_bin.h"
#include "level_music_bin.h"
#include "sprite.h"
#include "hearts.h"
#include "slime.h"
#include "fly.h"
#include "cursor.h"
#include "font.h"

inline void init_level_3_0()
{

	fade_out();

	// Save progress
	__gamepak[0] = LEVEL_3_0;

	dma_stop_transfer_1();
	SND_CNT = SND_MASTER = 0;
	T0_CNT = T1_CNT = T2_CNT = 0;

	clear_oam();

	// Copy necessary assets
	dma_full_transfer_3(&pal_obj_mem[heroDefaultPalIdx], spritePalBlue, BYTES_TO_WORDS(spritePalLen));
	dma_full_transfer_3(&pal_obj_mem[slimeDefaultPalIdx], slimePal, BYTES_TO_WORDS(slimePalLen));
	dma_full_transfer_3(&pal_obj_mem[flyDefaultPalIdx], flyPal, BYTES_TO_WORDS(flyPalLen));
	dma_full_transfer_3(&pal_obj_mem[heartDefaultPalIdx], heartsPal, BYTES_TO_WORDS(heartsPalLen));
	dma_full_transfer_3(&pal_obj_mem[cursorDefaultPalIdx], cursorPal, BYTES_TO_WORDS(cursorPalLen));
	dma_full_transfer_3(&pal_obj_mem[textDefaultPalIdx], fontPal, BYTES_TO_WORDS(fontPalLen));

	dma_full_transfer_3(&tile_mem[0][0], level_5_tiles_bin, level_5_tiles_bin_size >> 2);
	dma_full_transfer_3(&tile_mem[2][0], ingame_tiles_bin, ingame_tiles_bin_size >> 2);

	dma_full_transfer_3(&tile_mem[4][heroDefaultTileIdx], spriteTiles, BYTES_TO_WORDS(spriteTilesLen));
	dma_full_transfer_3(&tile_mem[4][slimeDefaultTileIdx], slimeTiles, BYTES_TO_WORDS(slimeTilesLen));
	dma_full_transfer_3(&tile_mem[4][flyDefaultTileIdx], flyTiles, BYTES_TO_WORDS(flyTilesLen));
	dma_full_transfer_3(&tile_mem[4][heartDefaultTileIdx], heartsTiles, BYTES_TO_WORDS(heartsTilesLen));
	dma_full_transfer_3(&tile_mem[4][cursorDefaultTileIdx], cursorTiles, BYTES_TO_WORDS(cursorTilesLen));
	dma_full_transfer_3(&tile_mem[4][textDefaultTileIdx], fontTiles, BYTES_TO_WORDS(fontTilesLen));

	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 0][0], &level_5_col_map_bin[0], level_5_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 1][0], &level_5_col_map_bin[4096], level_5_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 2][0], &level_5_col_map_bin[2048], level_5_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 3][0], &level_5_col_map_bin[6144], level_5_col_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 4][0], &level_5_misc_map_bin[0], level_5_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 5][0], &level_5_misc_map_bin[4096], level_5_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 6][0], &level_5_misc_map_bin[2048], level_5_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 7][0], &level_5_misc_map_bin[6144], level_5_misc_map_bin_size >> 4);
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 8][0], level_5_bg_map_bin, BYTES_TO_WORDS(level_5_bg_map_bin_size));
	dma_full_transfer_3(&map_mem[SCRBLK_IDX + 9][0], ingame_ui_map_bin, BYTES_TO_WORDS(ingame_ui_map_bin_size));

	dma_full_transfer_3(pal_bg_mem, palette_bin, BYTES_TO_WORDS(palette_bin_size));

	__dispcnt = DC_RENDER_OBJ | DC_OM_1D;

	print_hcenter_4bpp(INTRO_LEVEL_3, 50, textDefaultTileIdx);
	fade_in();
	halt_with_key(LVL_TITLE_HOLD_PERIOD, KY_A);
	fade_out();
	clear_text();

	dma_full_transfer_3(&pal_obj_mem[textDefaultPalIdx], fontPalInverted, BYTES_TO_WORDS(fontPalInvertedLen));
	__dispcnt = DC_RENDER_BG0 | DC_RENDER_BG1 | DC_RENDER_BG2 | DC_RENDER_OBJ | DC_OM_1D | DC_MODE_0;

	SND_CNT = 0x0B0F0000;
	SND_MASTER = SND_MASTER_ENABLE;

	dma_full_transfer_1(SND_FIFO_A, level_music_bin);

	T2_VAL = 0xFFF8; // 8 seconds
	T2_CNT = TIMER_ENABLE | TIMER_CASCADE | TIMER_IRQ;
	T1_VAL = 0xC180; // Every 16K frames -> 1s
	T1_CNT = TIMER_ENABLE | TIMER_CASCADE;
	T0_VAL = 0xFBE8; // Song @ 16KHz
	T0_CNT = TIMER_ENABLE;
}

inline int run_level_3_0()
{

	uint32_t num_enemies, next_scene;

	next_scene = LEVEL_3_1;

	// Sprites
	reset_hero_to(FIXED(32), FIXED(32));
	__hero.life = 3;
	build_hero();

	num_enemies = 0;

	BG_CNT_0 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+0) | BG_512x512;
	BG_CNT_1 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+4) | BG_512x512;
	BG_CNT_2 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+8) | BG_256x256;
	BG_CNT_3 = BG_PRIORITY(0) | BG_CBB(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+9) | BG_256x256;

	build_ui();

	reset_dynamic_bg_to(FIXED(0), 0);
	reset_static_bg_to(0, 0);

	add_enemy_slime(FIXED(32), FIXED(235), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(32), FIXED(160), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(48), FIXED(330), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(128), FIXED(400), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(128), FIXED(160), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(416), FIXED(235), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(416), FIXED(180), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(416), FIXED(32), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(128), FIXED(160), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_slime(FIXED(208), FIXED(400), FIXED(1), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(320), FIXED(160), FIXED(2), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(320), FIXED(200), FIXED(2), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(480), FIXED(176), FIXED(2), DIR_LEFT, &num_enemies);
	add_enemy_fly(FIXED(480), FIXED(288), FIXED(2), DIR_LEFT, &num_enemies);

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
		if ((__hero.y + __voff >= FIXED(472)
			&& __hero.x + __hoff <= FIXED(42))
			|| next_scene != LEVEL_3_1) {
			break;
		}

		save_key();
		increase_delta();
	}

	return next_scene;

}