// Custom headers
#include "gba.h"
#include "config.h"

// Levels
#include "scene_0.h"
#include "scene_1.h"
#include "scene_2.h"
#include "scene_3.h"
#include "scene_4.h"
#include "scene_5.h"
#include "scene_6.h"
#include "scene_7.h"
#include "scene_8.h"

/**
 * @brief Main method of the game
 * 
 */
int main(void) {

	int opt = 0;

	// Enable VBlank & Timer 2 interrupts
	// both needed for the game
	*((volatile uint16_t*)DS_REG) |= DS_VBI;
	init_irq(IR_VBLANK | IR_TIMER2);
	
	#ifndef SKIP_TITLES
	init_title();
	run_title_logo();
	run_title_author_prog();
	run_title_author_art();
	run_title_author_music();
	#endif

	// Selection loop
	while (FOREVER) {
		if (opt == MAIN_MENU) {
			init_menu();
			opt = run_menu();
		} else if (opt == CONTROLS) {
			init_controls();
			opt = run_controls();
		} else if (opt == LEVEL_1_0) {
			init_level_1_0();
			opt = run_level_1_0();
		} else if (opt == LEVEL_1_1) {
			init_level_1_1();
			opt = run_level_1_1();
		} else if (opt == LEVEL_2_0) {
			init_level_2_0();
			opt = run_level_2_0();
		} else if (opt == LEVEL_2_1) {
			init_level_2_1();
			opt = run_level_2_1();
		} else if (opt == LEVEL_3_0) {
			init_level_3_0();
			opt = run_level_3_0();
		} else if (opt == LEVEL_3_1) {
			init_level_3_1();
			opt = run_level_3_1();
		}
	}

	return 0;
}
