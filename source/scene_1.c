#include "scene_1.h"

#include "effects.h"
#include "utils.h"
#include "game.h"
#include "palette_bin.h"
#include "menu_tiles_bin.h"
#include "menu_bg_map_bin.h"
#include "menu_misc_map_bin.h"
#include "menu_ui_map_bin.h"
#include "menu_music_bin.h"
#include "cursor.h"
#include "font.h"
#include "text.h"
#include "rabbit.h"
#include "logo.h"
#include <string.h>

#ifdef BENCHMARK
int timer;
#endif

inline void init_menu() {

	fade_out();

	clear_oam();

#ifdef BENCHMARK
	// Start the timer to benchmark
	T3_CNT = TIMER_ENABLE | TIMER_FREQ_64;
#endif

	// Copy necessary assets
	dma_full_transfer_3(&tile_mem[4][0], rabbitTiles, BYTES_TO_WORDS(rabbitTilesLen));
	dma_full_transfer_3(&tile_mem[4][12], logoTiles, BYTES_TO_WORDS(logoTilesLen));
	dma_full_transfer_3(&tile_mem[4][cursorDefaultTileIdx], cursorTiles, BYTES_TO_WORDS(cursorTilesLen));
	dma_full_transfer_3(&tile_mem[4][textDefaultTileIdx], fontTiles, BYTES_TO_WORDS(fontTilesLen));

	dma_full_transfer_3(&pal_obj_mem[0], rabbitPal, BYTES_TO_WORDS(rabbitPalLen));
	dma_full_transfer_3(&pal_obj_mem[16], logoPal, BYTES_TO_WORDS(logoPalLen));
	dma_full_transfer_3(&pal_obj_mem[cursorDefaultPalIdx], cursorPal, BYTES_TO_WORDS(cursorPalLen));
	dma_full_transfer_3(&pal_obj_mem[textDefaultPalIdx], fontPalInverted, BYTES_TO_WORDS(fontPalInvertedLen));

	dma_full_transfer_3(&tile_mem[0][0], menu_tiles_bin, BYTES_TO_WORDS(menu_tiles_bin_size));
	dma_full_transfer_3(&map_mem[SCRBLK_IDX+0][0], menu_bg_map_bin, BYTES_TO_WORDS(menu_bg_map_bin_size));
	dma_full_transfer_3(&map_mem[SCRBLK_IDX+1][0], menu_misc_map_bin, BYTES_TO_WORDS(menu_misc_map_bin_size));
	dma_full_transfer_3(&map_mem[SCRBLK_IDX+2][0], menu_ui_map_bin, BYTES_TO_WORDS(menu_ui_map_bin_size));
	dma_full_transfer_3(pal_bg_mem, palette_bin, BYTES_TO_WORDS(palette_bin_size));


#ifdef BENCHMARK
	timer = T2_VAL;
	T3_CNT = 0x0000;
#endif

	// Initialize Display Control with the correct values
	__dispcnt = DC_RENDER_BG0 | DC_RENDER_BG1 | DC_RENDER_BG2 | DC_RENDER_OBJ | DC_OM_1D | DC_MODE_0;

	/* RESET SOUND */
	SND_CNT = SND_MASTER = 0;
	T0_CNT = T1_CNT = T2_CNT = 0;

	/* START SOUND */
	SND_CNT = 0x0B0F0000;
	SND_MASTER = SND_MASTER_ENABLE;

	dma_full_transfer_1(SND_FIFO_A, menu_music_bin);

	T2_VAL = 0xFFF8; // 8 seconds
	T2_CNT = TIMER_ENABLE | TIMER_CASCADE | TIMER_IRQ;
	T1_VAL = 0xC180; // Every 16K frames -> 1s
	T1_CNT = TIMER_ENABLE | TIMER_CASCADE;
	T0_VAL = 0xFBE8; // Song @ 16KHz
	T0_CNT = TIMER_ENABLE;

	/* POSITION INGAME MENU IN CENTER */
	*((volatile uint16_t*)BG_HOFF_3) = 200;
	*((volatile uint16_t*)BG_VOFF_3) = 230;

}

inline int run_menu() {

	uint32_t bg_hoff = 0;
	uint32_t increment = 0;
	uint32_t cursor_frames = 0;
	uint32_t rabbit_frames = 0;
	uint32_t update_rate_cursor = 10;
	uint32_t update_rate_rabbit = 5;

	// Specify parameters for each background
	BG_CNT_0 = BG_PRIORITY(2) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+0) | BG_256x256;
	BG_CNT_1 = BG_PRIORITY(1) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+1) | BG_256x256;
	BG_CNT_2 = BG_PRIORITY(0) | BG_COLOR_8BPP | BG_SBB(SCRBLK_IDX+2) | BG_256x256;

	*((volatile uint16_t*)BG_HOFF_0) = 0;
	*((volatile uint16_t*)BG_HOFF_1) = 0;
	*((volatile uint16_t*)BG_HOFF_2) = DIV2(256 - GBA_WIDTH);
	*((volatile uint16_t*)BG_VOFF_0) = 0;
	*((volatile uint16_t*)BG_VOFF_1) = 0;
	*((volatile uint16_t*)BG_VOFF_2) = 0;

	int32_t selection = 0;

	// Rabbit
	__oam[1].attr0 = BUILD_ATTR0(136, 0, 0, 0, 0, 1);
	__oam[1].attr1 = BUILD_ATTR1(40, 0, 0, 0, 0);
	__oam[1].attr2 = BUILD_ATTR2(0, 0, 0);

	// Logo
	__oam[2].attr0 = BUILD_ATTR0(0, 3, 0, 0, 0, 1);
	__oam[2].attr1 = BUILD_ATTR1((DIV2(GBA_WIDTH)-64), 0, 0, 0, 3);
	__oam[2].attr2 = BUILD_ATTR2(12, 0, 1);

	// Cursor
	__oam[0].attr0 = BUILD_ATTR0(80, 0, 0, 0, 0, 0);
	__oam[0].attr1 = BUILD_ATTR1(40, 0, 0, 0, 1);
	__oam[0].attr2 = BUILD_ATTR2(cursorDefaultTileIdx, 0, cursorDefaultPalbank);

	// Affine matrix used for the logo
	__aff[0].pa = 170; __aff[0].pb = 0;
	__aff[0].pc = 0; __aff[0].pd = 170;

	// Text
	print_hcenter_4bpp("PLAY", 84, textDefaultTileIdx);
	print_hcenter_4bpp("CONTROLS", 116, textDefaultTileIdx);

	while (FOREVER) {

		swi_vblank();

		fade_in_step();

		get_key();

		/* BUTTON SELECTION */
		if (key_press_first_time(__curr_key, __prev_key, KY_UP) && selection > 0) {
			incr_y(0, -32);
			selection--;
		}

		if (key_press_first_time(__curr_key, __prev_key, KY_DOWN) && selection < 1) {
			incr_y(0, 32);
			selection++;
		}

		// Option selected
		if (key_press(__curr_key, KY_A)) {
			break;
		}

		// DEV: Deletes previous saves
		if (key_press(__curr_key, KY_SELECT)) {
			__gamepak[0] = 0;
		}

		/* UPDATE ANIMATIONS */
		if ((--update_rate_rabbit) == 0) {
			rabbit_frames = (rabbit_frames < 5) ? rabbit_frames + 1 : 0;
			change_tidx(1, (rabbit_frames << 1));
			update_rate_rabbit = 5;
		}

		if ((--update_rate_cursor) == 0) {
			cursor_frames = (cursor_frames < 4) ? cursor_frames + 1 : 0;
			change_tidx(0, (cursor_frames << 2) + cursorDefaultTileIdx);
			update_rate_cursor = 10;
		}

		// Update background and rabbit every 2 frames
		increment ^= 1;

		if (increment) {
			bg_hoff += 1;
			*((volatile uint16_t*)BG_HOFF_0) = bg_hoff;
			*((volatile uint16_t*)BG_HOFF_1) = bg_hoff;
			incr_x(1, 1);
		}

		save_key();

#ifdef BENCHMARK
		print_int_4bpp(timer, 5, 5, 0, 15);
#endif

	}

	dma_stop_transfer_1();

	// If player selected "PLAY", check if there are any saved games
	if (selection == 0) {
		selection = (__gamepak[0]) ? __gamepak[0] : (uint8_t)LEVEL_1_0;	
		selection = (selection >= END_GAME) ? LEVEL_1_0 : selection;
	}

	return selection;

}
