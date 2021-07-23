#include "game.h"

// Global hero variable
HERO_T __hero;
// Global enemy array
ENEMY_T __enemies[20];

// Delta parameters needed for the game loop
uint32_t __delta = 0;
uint32_t __last_delta_hero = 0;
uint32_t __last_delta_enemies = 0;

// Background offset. These variables are required
// since the registers for the offsets are RO
uint32_t __voff = 0;
uint32_t __hoff = 0;

// Previous & current keys (managed with get_key & save_key)
int32_t __prev_key = -1;
int32_t __curr_key = 0;

/**
 * @brief Assigns to __curr_key the current
 * combination of buttons pressed
 * 
 */
inline void get_key() {
	__curr_key = *((volatile uint16_t *)KY_REG);
}

/**
 * @brief Saves the previous key state
 * 
 */
inline void save_key() {
	__prev_key = __curr_key;
}

/**
 * @brief Resets game loop deltas
 * 
 */
inline void reset_deltas() {
	__delta = __last_delta_hero = __last_delta_enemies = 0;
}

/**
 * @brief Increases game loop delta
 * 
 */
inline void increase_delta() {
	__delta++;
}

/**
 * @brief Moves hero or camera, depending on the position
 * of the player.
 * 
 * @param inc_y: Difference in the Y-axis compared to previous state
 * @param inc_x: Difference in the X-axis compared to previous state
 */
inline void adjust_hero_position(int32_t *inc_y, int32_t *inc_x) {

	uint32_t curr_width, curr_height, curr_cnt;

	// First we have to know the current
	// dimensions of the background
	curr_cnt = BG_CNT_0 & 0xC000;
	curr_cnt = curr_cnt >> 14;

	if (curr_cnt == 0) {
		curr_width = curr_height = 256;
	} else if (curr_cnt == 1) {
		curr_width = 512;
		curr_height = 256;
	} else if (curr_cnt == 2) {
		curr_height = 512;
		curr_width = 256;
	} else {
		curr_width = curr_height = 512;
	}

	/* DO WE MOVE THE CAMERA OR THE SPRITE ? */

	// Y AXIS
	if (*inc_y < 0 && __hero.y + *inc_y < ((GBA_HEIGHT - 16) << 7)
		&& __voff + *inc_y >= 0 && __voff + *inc_y <= (FIXED(curr_height - GBA_HEIGHT))) {
		__voff += *inc_y;
		*((volatile uint16_t *)BG_VOFF_0) = __voff >> 8;
		*((volatile uint16_t *)BG_VOFF_1) = __voff >> 8;
	} else if (*inc_y > 0 && __hero.y + *inc_y > ((GBA_HEIGHT - 16) << 7)
		&& __voff + *inc_y >= 0 && __voff + *inc_y <= (FIXED(curr_height - GBA_HEIGHT))) {
		__voff += *inc_y;
		*((volatile uint16_t *)BG_VOFF_0) = __voff >> 8;
		*((volatile uint16_t *)BG_VOFF_1) = __voff >> 8;
	} else {
		__hero.y += *inc_y;
	}

	// X AXIS
	if (*inc_x > 0 && __hero.x + *inc_x > ((GBA_WIDTH - 16) << 7)
		&& __hoff + *inc_x >= 0 && __hoff + *inc_x <= (FIXED(curr_width - GBA_WIDTH))) {
		__hoff += *inc_x;
		*((volatile uint16_t *)BG_HOFF_0) = __hoff >> 8;
		*((volatile uint16_t *)BG_HOFF_1) = __hoff >> 8;
	} else if (*inc_x < 0 && __hero.x + *inc_x < ((GBA_WIDTH - 16) << 7)
		&& __hoff + *inc_x >= 0 && __hoff + *inc_x <= (FIXED(curr_width - GBA_WIDTH))) { 
		__hoff += *inc_x;
		*((volatile uint16_t *)BG_HOFF_0) = __hoff >> 8;
		*((volatile uint16_t *)BG_HOFF_1) = __hoff >> 8;
	} else {
		__hero.x += *inc_x;
	}

}

/**
 * @brief Uses hero state to visually update the sprite
 * 
 */
void update_hero_render()
{

	uint32_t new_tid, target_delta;

	static uint32_t last_frame = 0;

	new_tid = 0;

	// Delta that triggers the next animation
	target_delta = __last_delta_hero + HERO_TRIGGER_FRAME;

	// If we reach the target we reset the 
	// counter and add 1 new frame to the animation
	if (target_delta == __delta) {
		__last_delta_hero = target_delta;
		last_frame++;
	}

	// Move both the sprite and the sword
	move_x(heroDefaultOAMIdx, __hero.x >> 8);
	move_y(heroDefaultOAMIdx, __hero.y >> 8);
	move_y(swordDefaultOAMIdx, (__hero.y >> 8) + 1);

	// If the hero has finished attacking, hide the sword
	if ((__hero.state & CURR_STATE_ATTACKING) && __hero.att_frame == 0)
	{
		ENABLE_HIDE_SPRITE(swordDefaultOAMIdx);
	}

	// Check if the hero is in middle of transforming
	if ((__hero.state & CURR_STATE_TRANS)) {

		// If we reach half of the animation, we use the new palette
		if (__hero.trans_frame == DIV2(HERO_TRANS_FRAME)) {

			if (__hero.type == HERO_BLUE_VARIATION) {
				dma_full_transfer_3(&pal_obj_mem[heroDefaultPalIdx], spritePalBlue, DIV4(spritePalLen));
			} else {
				dma_full_transfer_3(&pal_obj_mem[heroDefaultPalIdx], spritePalGold, DIV4(spritePalLen));
			}
		}

		// Update distorsion values
		__mosval = MOS_OBJ_H(1) | MOS_OBJ_V(1);

	} else if (__hero.state & PAST_STATE_TRANS) {
		__mosval = 0;
	}

	// If a new frame has appeared start from the beginning
	if (STATE_HAS_CHANGED(__hero.state)) {
		last_frame = 0;
	}

	// Move sowrd depending on the direction of the player
	if (__hero.dir == DIR_LEFT) {
		ENABLE_HFLIP_SPRITE(heroDefaultOAMIdx);
		ENABLE_HFLIP_SPRITE(swordDefaultOAMIdx);
		move_x(swordDefaultOAMIdx, ROUND(__hero.x) - 12);
	} else {
		DISABLE_HFLIP_SPRITE(heroDefaultOAMIdx);
		DISABLE_HFLIP_SPRITE(swordDefaultOAMIdx);
		move_x(swordDefaultOAMIdx, ROUND(__hero.x) + 12);
	}

	// Select correct animation
	if (__hero.state & CURR_STATE_RUN) {
		last_frame = (last_frame < 6) ? last_frame : 0;
		new_tid = heroDefaultTileIdx + 16 + last_frame * (DIV8(heroHeight) + DIV8(heroWidth)) * (heroAttrBpp + 1);
	} else if (__hero.state & CURR_STATE_ATTACKING) {
		last_frame = (last_frame < 4) ? last_frame : 0;
		new_tid = heroDefaultTileIdx + 80 + last_frame * (DIV8(heroHeight) + DIV8(heroWidth)) * (heroAttrBpp + 1);
		change_tidx(swordDefaultOAMIdx, new_tid);
		new_tid = heroDefaultTileIdx + 64 + last_frame * (DIV8(heroHeight) + DIV8(heroWidth)) * (heroAttrBpp + 1);
	} else if (__hero.state & CURR_STATE_JUMP) {
		last_frame = (last_frame < 3) ? last_frame : 0;
		new_tid = heroDefaultTileIdx + 40 + last_frame * (DIV8(heroHeight) + DIV8(heroWidth)) * (heroAttrBpp + 1);
	} else if (__hero.state & CURR_STATE_FALL) {
		last_frame = (last_frame < 3) ? last_frame : 0;
		new_tid = heroDefaultTileIdx + 52 + last_frame * (DIV8(heroHeight) + DIV8(heroWidth)) * (heroAttrBpp + 1);
	} else if (__hero.state & CURR_STATE_DEATH) {
		last_frame = (last_frame < 8) ? last_frame : 0;
		new_tid = heroDefaultTileIdx + 96 + last_frame * (DIV8(heroHeight) + DIV8(heroWidth)) * (heroAttrBpp + 1);
	} else {
		last_frame = (last_frame < 4) ? last_frame : 0;
		new_tid = heroDefaultTileIdx + last_frame * (DIV8(heroHeight) + DIV8(heroWidth)) * (heroAttrBpp + 1);
	}

	change_tidx(heroDefaultOAMIdx, new_tid);
}

/**
 * @brief Similar to the previous function but updating enemies
 * 
 * @param num_enemies: Number of enemies.
 */
inline void update_enemies_render(uint32_t *num_enemies)
{

	int32_t ii, offset;
	uint32_t new_tid, target_delta;

	new_tid = 0;

	for (ii = 0; ii < *num_enemies; ii++) {

		// Delta that triggers the next animation
		target_delta = __enemies[ii].ref_delta + ENEMY_TRIGGER_FRAME;

		// If we reach the target we reset the 
		// counter and add 1 new frame to the animation
		if (target_delta == __delta) {
			__enemies[ii].curr_frame++;
			__enemies[ii].ref_delta = target_delta;
		}

		// The GBA allows for objects to have x > 240
		// but this is not the case for y. We have to make sure
		// we only update y if the enemy is within bounds
		offset = ROUND(__enemies[ii].y) - (int32_t)ROUND(__voff);
		
		if (offset >= -16 && offset <= 160) {
			move_y(enemyDefaultOAMIdx+ii, offset);
		}

		move_x(enemyDefaultOAMIdx+ii, ROUND(__enemies[ii].x) - ROUND(__hoff));

		NEW_STATE(__enemies[ii].state);

		if (__enemies[ii].life == 0) {
			SET_CURR_STATE(__enemies[ii].state, CURR_STATE_DEATH);
		}

		if (STATE_HAS_CHANGED(__enemies[ii].state)) {
			__enemies[ii].curr_frame = 0;
		}

		if (STATE_HAS_CHANGED(__enemies[ii].dir)) {
			hflip_sprite(enemyDefaultOAMIdx+ii);
		}

		if (__enemies[ii].type == ENEMY_SLIME) {

			if (__enemies[ii].state & CURR_STATE_DEATH) {

				__enemies[ii].curr_frame++;
				// If the death animation has finished, 
				// we swap the dead enemy for another,
				// to improve performance
				if (__enemies[ii].curr_frame >= 6) {
					offset = *num_enemies - 1;
					__enemies[ii] = __enemies[offset];
					__oam[enemyDefaultOAMIdx+ii].attr0 = __oam[enemyDefaultOAMIdx+offset].attr0;
					__oam[enemyDefaultOAMIdx+ii].attr1 = __oam[enemyDefaultOAMIdx+offset].attr1;
					__oam[enemyDefaultOAMIdx+ii].attr2 = __oam[enemyDefaultOAMIdx+offset].attr2;
					ENABLE_HIDE_SPRITE(enemyDefaultOAMIdx+offset);
					ii--; (*num_enemies)--;
					continue;
				} else {
					new_tid = (slimeDefaultTileIdx + 20) + __enemies[ii].curr_frame * ((16 >> 3) + (16 >> 3));
				}
			} else {
				__enemies[ii].curr_frame = (__enemies[ii].curr_frame < 5) ? __enemies[ii].curr_frame : 0;
				new_tid = slimeDefaultTileIdx + __enemies[ii].curr_frame * ((16 >> 3) + (16 >> 3));
			}

		} else if (__enemies[ii].type == ENEMY_FLY) {

			if (__enemies[ii].state & CURR_STATE_DEATH) {

				__enemies[ii].curr_frame++;
				// If the death animation has finished, 
				// we swap the dead enemy for another,
				// to improve performance
				if (__enemies[ii].curr_frame >= 6) {
					offset = *num_enemies - 1;
					__enemies[ii] = __enemies[offset];
					__oam[enemyDefaultOAMIdx+ii].attr0 = __oam[enemyDefaultOAMIdx+offset].attr0;
					__oam[enemyDefaultOAMIdx+ii].attr1 = __oam[enemyDefaultOAMIdx+offset].attr1;
					__oam[enemyDefaultOAMIdx+ii].attr2 = __oam[enemyDefaultOAMIdx+offset].attr2;
					ENABLE_HIDE_SPRITE(enemyDefaultOAMIdx+offset);
					ii--; (*num_enemies)--;
					continue;
				} else {
					new_tid = (flyDefaultTileIdx + 3) + __enemies[ii].curr_frame;
				}
			} else {
				__enemies[ii].curr_frame = (__enemies[ii].curr_frame < 3) ? __enemies[ii].curr_frame : 0;
				new_tid = flyDefaultTileIdx + __enemies[ii].curr_frame;
			}
		}

		change_tidx(enemyDefaultOAMIdx+ii, new_tid);
	}
}

/**
 * @brief Animate hearts & hide them if the hero takes damage
 * 
 */
inline void update_ui_render() {
	int ii;

	// Update animation every 2 frames
	if ((__delta & 1) && __aff[0].pa > FIXED(1)) {
		__aff[0].pa -= FIXED(1);
		__aff[0].pd -= FIXED(1);
	}

	for (ii = 2; ii >= __hero.life; ii--) {
		ENABLE_HIDE_AFF_SPRITE(uiDefaultBaseOAMIdx+ii);
	}
}

/**
 * @brief If the user uses the in-game menu,
 * we process the selected option
 * 
 * @param code: Pointer to the next level, if the users
 * selects "EXIT", the value will be overwritten with the code
 * for the main menu
 */
inline void update_pause_render(uint32_t *code)
{

	uint32_t cursor_frames, update_rate_cursor, selection;

	if (key_press_first_time(__curr_key, __prev_key, KY_START)) {
		save_key();
		__dispcnt |= DC_RENDER_BG3;
		cursor_frames = 0;
		selection = 0;
		update_rate_cursor = 10;
		DISABLE_HIDE_SPRITE(cursorDefaultOAMIdx);
		print_hcenter_4bpp("RESUME", 62, textDefaultTileIdx);
		print_hcenter_4bpp("EXIT", 94, textDefaultTileIdx);
		while (FOREVER) {
			swi_vblank();
			get_key();

			if ((--update_rate_cursor) == 0) {
				cursor_frames = (cursor_frames < 4) ? cursor_frames + 1 : 0;
				change_tidx(cursorDefaultOAMIdx, (cursor_frames << 2) + cursorDefaultTileIdx);
				update_rate_cursor = 10;
			}

			if (selection > 0 && key_press_first_time(__curr_key, __prev_key, KY_UP)) {
				incr_y(cursorDefaultOAMIdx, -32);
				selection--;
			} else if (selection < 1 && key_press_first_time(__curr_key, __prev_key, KY_DOWN)) {
				incr_y(cursorDefaultOAMIdx, +32);
				selection++;
			}
 

			if (key_press_first_time(__curr_key, __prev_key, KY_A)) {
				clear_text();
				__dispcnt ^= DC_RENDER_BG3;
				if (selection) {
					*code = MAIN_MENU;
					dma_stop_transfer_1();
				}
				ENABLE_HIDE_SPRITE(cursorDefaultOAMIdx);
				break;
			}

			save_key();
		}
	}
}

/**
 * @brief Depending on the players input, we update the hero 
 * 
 * @param inc_y: Difference in the Y-axis compared to previous state
 * @param inc_x: Difference in the X-axis compared to previous state
 */
inline void update_hero_state(int32_t *inc_y, int32_t *inc_x)
{
	int32_t aux, speed_x, speed_y, drop_y;
	static int32_t jmp_speed;
	static int32_t add_jmp_frames = MAX_JMP_FRAME;

	*inc_x = *inc_y = 0;
	NEW_STATE(__hero.state);

	// Change stats with different variations
	if (__hero.type == HERO_BLUE_VARIATION) {
		speed_x = HERO_BLUE_X_SPEED;
		speed_y = HERO_BLUE_Y_SPEED;
		drop_y = HERO_BLUE_Y_DROP;
	} else {
		speed_x = HERO_GOLD_X_SPEED;
		speed_y = HERO_GOLD_Y_SPEED;
		drop_y = HERO_GOLD_Y_DROP;
	}

	// If player is attacking, continue with animation
	if (__hero.att_frame > 0) {
		__hero.att_frame--;
		__hero.state |= CURR_STATE_ATTACKING;
	}

	if (__hero.trans_frame-- > 0) {
		__hero.state |= CURR_STATE_TRANS;
	}

	// If the player is not attacking
	if (!(__hero.state & (CURR_STATE_ATTACKING | CURR_STATE_TRANS))) {

		// Transform if the player presses L or R
		if (__hero.type == 1 && key_press_first_time(__curr_key, __prev_key, KY_L)) {
			__hero.type = 0;
			__hero.trans_frame = HERO_TRANS_FRAME;
		} else if (__hero.type == 0 && key_press_first_time(__curr_key, __prev_key, KY_R)) {
			__hero.type = 1;
			__hero.trans_frame = HERO_TRANS_FRAME;
		}

		/* MOVING LEFT */
		if (key_press(__curr_key, KY_LEFT)) {
			__hero.dir = DIR_LEFT;
			SET_CURR_STATE(__hero.state, CURR_STATE_RUN);
			// If the player is in the air, slow down
			aux = (__hero.state & (PAST_STATE_JUMP | PAST_STATE_FALL)) ? 256 : speed_x;
			*inc_x = -move_hero_left(__hero.y + __voff, __hero.x + __hoff, aux, SPACE_TILE_IDX);
		}
		/* MOVING RIGHT */
		else if (key_press(__curr_key, KY_RIGHT)) {
			__hero.dir = DIR_RIGHT;
			SET_CURR_STATE(__hero.state, CURR_STATE_RUN);
			// If the player is in the air, slow down
			aux = (__hero.state & (PAST_STATE_JUMP | PAST_STATE_FALL)) ? 256 : speed_x;
			*inc_x = +move_hero_right(__hero.y + __voff, __hero.x + __hoff, aux, SPACE_TILE_IDX);
		}

		// If player presses B and he is not attacking, attack!
		if (__hero.type == 1 && !(__hero.state & (PAST_STATE_JUMP | PAST_STATE_FALL))
			&& key_press_first_time(__curr_key, __prev_key, KY_B)) {
			__hero.att_frame = (3 * HERO_TRIGGER_FRAME);
			DISABLE_HIDE_SPRITE(swordDefaultOAMIdx);
			SET_CURR_STATE(__hero.state, CURR_STATE_ATTACK | CURR_STATE_ATTACKING);
		}

		// If the player keeps holding the jumping button, the sprite jumps higher
		if (key_press_hold(__curr_key, __prev_key, KY_A) && --add_jmp_frames > 0) {
			__hero.jmp_frame += 1;
			jmp_speed -= drop_y;
		}

		// Is the player jumping ?
		if (__hero.state & (CURR_STATE_JUMP | PAST_STATE_JUMP)) {
			// If so, can he still jump ?
			if (--__hero.jmp_frame > 0) {
				SET_CURR_STATE(__hero.state, CURR_STATE_JUMP);
				*inc_y = -collision_hero_top(__hero.y + __voff, __hero.x + __hoff + FIXED(3), jmp_speed, SPACE_TILE_IDX);

				// If the player hits something on top, the hero goes down
				if (*inc_y == 0) {
					jmp_speed = drop_y;
					SET_CURR_STATE(__hero.state, CURR_STATE_FALL);
				}
			// If not, disable jumping animation
			} else {
				jmp_speed = drop_y;
				__hero.state &= ~CURR_STATE_JUMP;
			}
		}

		// If the hero is not jumping check if he has blocks underneath
		if (!(__hero.state & CURR_STATE_JUMP)) {

			*inc_y = +collision_hero_bottom(__hero.y + __voff, __hero.x + __hoff + FIXED(3), jmp_speed, SPACE_TILE_IDX);

			if (*inc_y > 0) {
				jmp_speed += drop_y;
				SET_CURR_STATE(__hero.state, CURR_STATE_FALL);
			} else {
				jmp_speed = drop_y;
				__hero.state &= ~CURR_STATE_FALL;
			}
		}

		// Trigger jumping for the first time
		if (__hero.type == 0 && !(__hero.state & (CURR_STATE_JUMP | CURR_STATE_FALL))
			&& key_press_first_time(__curr_key, __prev_key, KY_A)) {
			jmp_speed = speed_y;
			add_jmp_frames = MAX_JMP_FRAME;
			__hero.jmp_frame = BASE_JMP_FRAME;
			SET_CURR_STATE(__hero.state, CURR_STATE_JUMP);
		}

	}

	// Attacking animation
	if (__hero.dmg_frame > 0) {

		if (__hero.dmg_frame & 1) {
			flip_hidden(heroDefaultOAMIdx);
		}
		__hero.dmg_frame--;
		__hero.state |= CURR_STATE_ATTACKED;
	}
}

/**
 * @brief Updates the enemies state by checking collisions in the environment.
 * 
 * @param num_enemies: Number of enemies to process.
 */
inline void update_enemies_state(uint32_t num_enemies)
{
	uint32_t ii, inc, check_fall;
	for (ii = 0; ii < num_enemies; ii++) {

		NEW_STATE(__enemies[ii].dir);

		// Does the enemy navigate by air ?
		if (__enemies[ii].type == ENEMY_SLIME) {
			check_fall = 1;
		} else {
			check_fall = 0;
		} // else if ...

		if (GET_PAST_STATE(__enemies[ii].dir) == DIR_LEFT) {
			inc = - move_enemy_left(&__enemies[ii], check_fall, SPACE_TILE_IDX);
		} else {
			inc = + move_enemy_right(&__enemies[ii], check_fall, SPACE_TILE_IDX);
		}

		// Flip sprite if necessary
		if (__enemies[ii].speed && !inc) {
			__enemies[ii].dir |= GET_PAST_STATE(__enemies[ii].dir) ^ 1;
		} else {
			__enemies[ii].dir |= GET_PAST_STATE(__enemies[ii].dir);
		}

		__enemies[ii].x += inc;
	}
}

/**
 * @brief 
 * 
 * @param num_enemies 
 */
inline void process_sprite_collisions(uint32_t num_enemies) {
	uint32_t ii, offx, offy;

	// If the hero is being attacked we don't bother taking damage
	if (__hero.state & CURR_STATE_ATTACKED) {
		return;
	}

	// Might change depending on the enemy,
	// will keep these values for now
	offy = 2048;
	offx = 4096;

	for (ii = 0; ii < num_enemies; ii++) {

		// Is the enemy within range of the player ?
		if (get_abs(__enemies[ii].y - __hero.y - __voff) < offy) {

			// Check sword collision with enemies
			if ((__hero.state & CURR_STATE_ATTACK) && __hero.dir == DIR_LEFT
					&& __hero.x + __hoff - FIXED(12) - __enemies[ii].x < offx) {
				__enemies[ii].life--;
			} else if ((__hero.state & CURR_STATE_ATTACK) && __hero.dir == DIR_RIGHT
					&& __enemies[ii].x - __hero.x - __hoff - FIXED(12) < offx) {
				__enemies[ii].life--;
			// Check player collision with enemies
			} else if (get_abs(__enemies[ii].x - __hero.x - __hoff) < (offx - FIXED(3))) {
				SET_CURR_STATE(__hero.state, CURR_STATE_ATTACKED);
				__hero.dmg_frame = DMG_COOLDOWN;
				__hero.life--;
			}

		}
	}
}

/**
 * @brief Resets hero to a certain location
 * 
 * @param y: Y coordinate (Fixed)
 * @param x: X coordinate (Fixed)
 */
void reset_hero_to(int32_t y, int32_t x)
{
	__hero.y = y;
	__hero.x = x;
	__hero.dir = DIR_RIGHT;
	__hero.dmg_frame = 0;
	__hero.att_frame = 0;
	__hero.type = HERO_BLUE_VARIATION;
	__hero.state = INIT_STATE_IDLE;
}

/**
 * @brief Automates creating the main protagonist's sprite
 * 
 */
void build_hero()
{

	__oam[heroDefaultOAMIdx].attr0 = BUILD_ATTR0(__hero.y >> 8, 0, 0, 1, heroAttrBpp, heroAttr0Size);
	__oam[heroDefaultOAMIdx].attr1 = BUILD_ATTR1(__hero.x >> 8, 0, 0, 0, heroAttr1Size);
	__oam[heroDefaultOAMIdx].attr2 = BUILD_ATTR2(heroDefaultTileIdx, 1, 0);

	__oam[swordDefaultOAMIdx].attr0 = BUILD_ATTR0(__hero.y >> 8, 2, 0, 0, heroAttrBpp, heroAttr0Size);
	__oam[swordDefaultOAMIdx].attr1 = BUILD_ATTR1((__hero.x >> 8)+12, 0, 0, 0, heroAttr1Size);
	__oam[swordDefaultOAMIdx].attr2 = BUILD_ATTR2((heroDefaultTileIdx+160), 1, heroDefaultPalbank);

}

/**
 * @brief Dedicated function for animating the hero's death.
 * Stops everything else.
 * 
 */
void kill_hero()
{
	uint32_t ii;
	for (ii = 0; ii < 70; ii++) {
		NEW_STATE(__hero.state);
		__hero.state |= CURR_STATE_DEATH;
		update_hero_render();
		swi_vblank();
		increase_delta();
	}
}

/**
 * @brief Automates initializing the UI. Populates the Affine Matrix
 * & the OAM with the necessary tiles.
 * 
 */
void build_ui()
{

	__aff[0].pa = FIXED(8); __aff[0].pb = 0;
	__aff[0].pc = 0; __aff[0].pd = FIXED(8);

	// Cursor
	__oam[cursorDefaultOAMIdx].attr0 = BUILD_ATTR0(58, 2, 0, 0, 0, 0);
	__oam[cursorDefaultOAMIdx].attr1 = BUILD_ATTR1(66, 0, 0, 0, 1);
	__oam[cursorDefaultOAMIdx].attr2 = BUILD_ATTR2(cursorDefaultTileIdx, 0, cursorDefaultPalbank);

	// Heart 0
	__oam[uiDefaultBaseOAMIdx].attr0 = BUILD_ATTR0(5, 1, 0, 0, 0, 0);
	__oam[uiDefaultBaseOAMIdx].attr1 = BUILD_ATTR1(5, 0, 0, 0, 1);
	__oam[uiDefaultBaseOAMIdx].attr2 = BUILD_ATTR2(heartDefaultTileIdx, 0, heartDefaultPalbank);

	// Heart 1
	__oam[uiDefaultBaseOAMIdx+1].attr0 = BUILD_ATTR0(5, 1, 0, 0, 0, 0);
	__oam[uiDefaultBaseOAMIdx+1].attr1 = BUILD_ATTR1(26, 0, 0, 0, 1);
	__oam[uiDefaultBaseOAMIdx+1].attr2 = BUILD_ATTR2(heartDefaultTileIdx, 0, heartDefaultPalbank);

	// Heart 2
	__oam[uiDefaultBaseOAMIdx+2].attr0 = BUILD_ATTR0(5, 1, 0, 0, 0, 0);
	__oam[uiDefaultBaseOAMIdx+2].attr1 = BUILD_ATTR1(47, 0, 0, 0, 1);
	__oam[uiDefaultBaseOAMIdx+2].attr2 = BUILD_ATTR2(heartDefaultTileIdx, 0, heartDefaultPalbank);

}

/**
 * @brief Resets backgrounds 0 & 1 to a certain offset
 * 
 * @param y: Y offset (Fixed)
 * @param x: X offset (Fixed)
 */
void reset_dynamic_bg_to(int32_t y, int32_t x)
{
	__voff = y;
	__hoff = x;

	*((volatile uint16_t*)BG_HOFF_0) = __hoff >> 8;
	*((volatile uint16_t*)BG_VOFF_0) = __voff >> 8;
	*((volatile uint16_t*)BG_HOFF_1) = __hoff >> 8;
	*((volatile uint16_t*)BG_VOFF_1) = __voff >> 8;
}

/**
 * @brief Resets backgrounds 1 & 2 to a certain offset
 * 
 * @param y: Y offset (Fixed)
 * @param x: X offset (Fixed)
 */
void reset_static_bg_to(int32_t y, int32_t x)
{
	*((volatile uint16_t*)BG_HOFF_2) = x >> 8;
	*((volatile uint16_t*)BG_VOFF_2) = y >> 8;
}

/**
 * @brief Function that adds a ENEMY_SLIME_T to the enemy pool
 * 
 * @param y: Y coordinate (fixed)
 * @param x: X coordinate (fixed)
 * @param speed: Units / frame
 * @param dir: Initial direction of the sprite
 * @param num_enemies: Pointer to update the enemy count
 */
void add_enemy_slime(uint32_t y, uint32_t x, uint32_t speed, uint32_t dir, uint32_t *num_enemies)
{
	__enemies[*num_enemies].type = ENEMY_SLIME;
	__enemies[*num_enemies].state = INIT_STATE_IDLE;
	__enemies[*num_enemies].y = y;
	__enemies[*num_enemies].x = x;
	__enemies[*num_enemies].dmg = 1;
	__enemies[*num_enemies].life = 1;
	__enemies[*num_enemies].btidx = 96;
	__enemies[*num_enemies].dir = dir << 16 | dir;
	__enemies[*num_enemies].curr_frame = 0;
	__enemies[*num_enemies].speed = speed;
	__enemies[*num_enemies].ref_delta = 0;

	y = ROUND(__enemies[*num_enemies].y - __voff);
	x = ROUND(__enemies[*num_enemies].x - __hoff);

	y = (y > GBA_HEIGHT) ? GBA_HEIGHT : y;

	__oam[enemyDefaultOAMIdx+*num_enemies].attr0 = BUILD_ATTR0(y, 0, 0, 0, 0, 0);
	__oam[enemyDefaultOAMIdx+*num_enemies].attr1 = BUILD_ATTR1(x, 0, 0, 0, 1);
	__oam[enemyDefaultOAMIdx+*num_enemies].attr2 = BUILD_ATTR2(slimeDefaultTileIdx, 1, slimeDefaultPalbank);
	*num_enemies += 1;
}

/**
 * @brief Function that adds a ENEMY_FLY_T to the enemy pool
 * 
 * @param y: Y coordinate (fixed)
 * @param x: X coordinate (fixed)
 * @param speed: Units / frame
 * @param dir: Initial direction of the sprite
 * @param num_enemies: Pointer to update the enemy count
 */
void add_enemy_fly(uint32_t y, uint32_t x, uint32_t speed, uint32_t dir, uint32_t *num_enemies)
{
	__enemies[*num_enemies].type = ENEMY_FLY;
	__enemies[*num_enemies].state = INIT_STATE_IDLE;
	__enemies[*num_enemies].y = y;
	__enemies[*num_enemies].x = x;
	__enemies[*num_enemies].dmg = 1;
	__enemies[*num_enemies].life = 1;
	__enemies[*num_enemies].btidx = 96;
	__enemies[*num_enemies].dir = dir << 16 | dir;
	__enemies[*num_enemies].curr_frame = 0;
	__enemies[*num_enemies].speed = speed;
	__enemies[*num_enemies].ref_delta = 0;

	y = ROUND(__enemies[*num_enemies].y - __voff);
	x = ROUND(__enemies[*num_enemies].x - __hoff);

	y = (y > GBA_HEIGHT) ? GBA_HEIGHT : y;

	__oam[enemyDefaultOAMIdx+*num_enemies].attr0 = BUILD_ATTR0(y, 0, 0, 0, 0, 0);
	__oam[enemyDefaultOAMIdx+*num_enemies].attr1 = BUILD_ATTR1(x, 0, 0, 0, 0);
	__oam[enemyDefaultOAMIdx+*num_enemies].attr2 = BUILD_ATTR2(flyDefaultTileIdx, 1, flyDefaultPalbank);
	*num_enemies += 1;
}
