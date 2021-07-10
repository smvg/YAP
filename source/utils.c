#include "utils.h"

static int32_t __curr_text_idx = textDefaultOAMIdx;

/**
 * @brief Flips a sprite in the X axis 
 * 
 * @param oam_idx: OAM index of the sprite
 */
inline void hflip_sprite(uint32_t oam_idx) {
	__oam[oam_idx].attr1 ^= 0x1000;
}

/**
 * @brief Flips a sprite in the Y axis 
 * 
 * @param oam_idx: OAM index of the sprite
 */
inline void vflip_sprite(uint32_t oam_idx) {
	__oam[oam_idx].attr1 ^= 0x2000;
}

/**
 * @brief Moves sprite to a desired Y coordinate
 * 
 * @param oam_idx: OAM index of the sprite
 * @param y: Y coordinate (NON-Fixed)
 */
inline void move_y(uint32_t oam_idx, int32_t y) {
	__oam[oam_idx].attr0 = (__oam[oam_idx].attr0 & ~0xFF) | (y & 0xFF);
}

/**
 * @brief Increases the Y coordinate of a sprite
 * 
 * @param oam_idx: OAM index of the sprite
 * @param y: Increase in the Y axis
 */
inline void incr_y(uint32_t oam_idx, int32_t y) {
	__oam[oam_idx].attr0 = (__oam[oam_idx].attr0 & ~0xFF) | (((__oam[oam_idx].attr0 & 0xFF) + y) & 0xFF);
}

/**
 * @brief Moves sprite to a desired X coordinate
 * 
 * @param oam_idx: OAM index of the sprite
 * @param x: X coordinate (NON-Fixed)
 */
inline void move_x(uint32_t oam_idx, int32_t x) {
	__oam[oam_idx].attr1 = (__oam[oam_idx].attr1 & ~0x01FF) | (x & 0x01FF);
}

/**
 * @brief Increases the X coordinate of a sprite
 * 
 * @param oam_idx: OAM index of the sprite
 * @param x: Increase in the X axis
 */
inline void incr_x(uint32_t oam_idx, int32_t x) {
	__oam[oam_idx].attr1 = (__oam[oam_idx].attr1 & ~0x01FF) | ((__oam[oam_idx].attr1 & 0x01FF) + x);
}

/**
 * @brief Checks if a key has been pressed
 * 
 * @param curr_key: Current key state
 * @param key: Key to check state
 * @return int: Returns 1 if the key has been pressed
 * and 0 if the key has not been pressed
 */
inline int key_press(int32_t curr_key, int32_t key) {
	return ~curr_key & key;
}

/**
 * @brief Checks if a key has been pressed that was not
 * previously registered
 * 
 * @param curr_key: Current key state
 * @param prev_key: Previous key state
 * @param key: Key to check
 * @return int: Returns 1 if the key has been pressed
 * and 0 if the key has not been pressed
 */
inline int key_press_first_time(int32_t curr_key, int32_t prev_key, int32_t key) {
	return (~(prev_key ^ ~curr_key) & ~curr_key) & key;
}

/**
 * @brief Checks if a key is being hold
 * 
 * @param curr_key: Current key state
 * @param prev_key: Previous key state
 * @param key: Key to check
 * @return int: Returns 1 if the key is being hold
 * and 0 if isn't
 */
inline int key_press_hold(int32_t curr_key, int32_t prev_key, int32_t key) {
	return (~(prev_key ^ curr_key) & ~curr_key) & key;
}

/**
 * @brief Checks if a key is no longer being pressed
 * 
 * @param curr_key: Current key state
 * @param prev_key: Previous key state
 * @param key: Key to check
 * @return int: Returns 1 if the key is no longer being pressed
 * and 0 if it is
 */
inline int key_press_no_more(int32_t curr_key, int32_t prev_key, int32_t key) {
	return (~(curr_key ^ ~prev_key) & ~prev_key) & key;
}

/**
 * @brief Changes the base tile of a sprite. Useful for animation.
 * 
 * @param oam_idx: OAM index of the sprite.
 * @param tidx: New tile index.
 */
inline void change_tidx(uint32_t oam_idx, uint32_t tidx) {
	__oam[oam_idx].attr2 = (__oam[oam_idx].attr2 & ~0x03FF) | (tidx & 0x03FF);
}

/**
 * @brief Flip visibility of a sprite.
 * 
 * @param oam_idx: OAM index of the sprite.
 */
inline void flip_hidden(uint32_t oam_idx) {
	__oam[oam_idx].attr0 ^= 0x0200;
}

/**
 * @brief Get the absolute value of a number
 * 
 * @param n: Number.
 * @return uint32_t: Absolute value.
 */
inline uint32_t get_abs(int32_t n) {
	int32_t mask = n >> 31;
	return ((n ^ mask) - mask);
}

/**
 * @brief Move hero to the left unless there is a collision
 * 
 * @param y: Y coordinate of the player (Fixed)
 * @param x: X coordinate of the player (Fixed)
 * @param inc: X axis increase from the player (Fixed)
 * @param space_tile: Index of empty tile
 * @return uint32_t: Returns he amount the player can move to the left 
 */
inline uint32_t move_hero_left(int32_t y, int32_t x, int32_t inc, uint32_t space_tile) {
	int32_t new_x, old_x, old_y;
	uint32_t ii, dd, tile, tt;

	old_y = ROUND(y);
	old_x = ROUND(x);
	new_x = ROUND(x - inc);

	// Redondeamos hacia arriba el tile más próximo
	old_y = (old_y + 7);

	// Cuantos tiles de diferencia hay ?
	tt = DIV8(old_x) - DIV8(new_x);

	// Recorremos los tiles que tiene por delante
	for (dd = 0; dd <= tt; dd++) {
		for (ii = 0; ii < 2; ii++) {
			tile = se_index_fast(DIV8(old_x)-dd, DIV8(old_y)+ii);
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) != space_tile) {
				return FIXED(old_x - (old_x & (-8)) + ((dd-1) << 3) + 3);
			}
		}
	}

	return inc;

}

/**
 * @brief Move hero to the right unless there is a collision
 * 
 * @param y: Y coordinate of the player (Fixed)
 * @param x: X coordinate of the player (Fixed)
 * @param inc: X axis increase from the player (Fixed)
 * @param space_tile: Index of empty tile
 * @return uint32_t: Returns he amount the player can move to the right 
 */
inline uint32_t move_hero_right(int32_t y, int32_t x, int32_t inc, uint32_t space_tile) {
	int32_t new_x, old_x, old_y;
	uint32_t ii, dd, tile, tt;

	old_y = ROUND(y);
	old_x = ROUND(x) + 16;
	new_x = ROUND(x + inc) + 16;

	// Get nearest tile
	old_y = (old_y + 7);

	// Tile distance between both points
	tt = DIV8(new_x) - DIV8(old_x);

	// Check all of the tiles that might collision
	for (dd = 0; dd <= tt; dd++) {
		for (ii = 0; ii < 2; ii++) {
			tile = se_index_fast(DIV8(old_x)+dd, DIV8(old_y)+ii);
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) != space_tile) {
				return FIXED(((old_x + 7) & (-8)) - old_x + ((dd-1) << 3) + 3);
			}
		}
	}

	return inc;
}

/**
 * @brief Move enemy to the left unless there is a collision
 * 
 * @param enemy: Pointer of the enemy
 * @param check_fall: If set, the enemy will not be able to float
 * @param space_tile: Index of empty tile
 * @return uint32_t: Returns he amount the enemy can move to the left 
 */
inline uint32_t move_enemy_left(ENEMY_T *enemy, int32_t check_fall, uint32_t space_tile) {
	int32_t new_x, old_x, old_y;
	uint32_t ii, dd, tile, tt, height;

	old_y = enemy->y >> 8;
	old_x = enemy->x >> 8;
	new_x = (enemy->x - enemy->speed) >> 8;

	// Get nearest tile
	old_y = (old_y + 7);

	// Tile distance between both points
	tt = (old_x >> 3) - (new_x >> 3);

	if (enemy->type == ENEMY_SLIME) {
		height = 2;
	} else {
		height = 1;
	} // Room for more enemies ?

	// Check all of the tiles that might collision
	for (dd = 0; dd <= tt; dd++) {

		if (check_fall) {
			tile = se_index_fast((old_x >> 3)-dd, (old_y >> 3)+height);
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) == space_tile) {
				return 0;
			}
		}

		ii = height;
		while (ii--) {
			tile = se_index_fast((old_x >> 3)-dd, (old_y >> 3)+ii);
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) != space_tile) {
				return 0;
			}
		}

	}

	return enemy->speed;
}

/**
 * @brief Move enemy to the right unless there is a collision
 * 
 * @param enemy: Pointer of the enemy
 * @param check_fall: If set, the enemy will not be able to float
 * @param space_tile: Index of empty tile
 * @return uint32_t: Returns he amount the enemy can move to the right 
 */
inline uint32_t move_enemy_right(ENEMY_T *enemy, int32_t check_fall, uint32_t space_tile) {
	int32_t new_x, old_x, old_y;
	uint32_t ii, dd, tile, tt, height, width;

	if (enemy->type == ENEMY_SLIME) {
		height = 2;
		width = 2;
	} else {
		height = 1;
		width = 1;
	} // Room for more enemies ?

	old_y = ROUND(enemy->y);
	old_x = ROUND(enemy->x) + MUL8(width);
	new_x = ROUND(enemy->x + enemy->speed) + MUL8(width);

	// Get nearest tile
	old_y = (old_y + 7);

	// Tile distance between both points
	tt = DIV8(new_x) - DIV8(old_x);

	// Check all of the tiles that might collision
	for (dd = 0; dd <= tt; dd++) {

		if (check_fall) {
			tile = se_index_fast(DIV8(old_x)+dd, DIV8(old_y)+height);
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) == space_tile) {
				return 0;
			}
		}

		ii = height;
		while (ii--) {
			tile = se_index_fast(DIV8(old_x)+dd, DIV8(old_y)+ii);
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) != space_tile) {
				return 0;
			}
		}

	}

	return enemy->speed;
}

/**
 * @brief Checks if there is collision underneath the player
 * 
 * @param y: Y coordinate of the player (Fixed)
 * @param x: X coordinate of the player (Fixed)
 * @param inc: Y axis increase from the player (Fixed)
 * @param space_tile: Index of empty tile
 * @return uint32_t: Returns he amount the player can move downward
 */
inline uint32_t collision_hero_bottom(int32_t y, int32_t x, int32_t inc, uint32_t space_tile) {
	int32_t new_x, new_y;
	uint32_t ii, dd, tile, tt;

	new_y = ROUND(y) + 16 - 1;
	new_x = ROUND(x);

	// Get the nearest tile
	tt = ROUND(inc+255);

	for (dd = 1; dd <= tt; dd++) {
		for (ii = 0; ii < 2; ii++) {
			tile = se_index_fast((new_x >> 3)+ii, (new_y+dd) >> 3);
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) != space_tile) {
				return ((dd-1) << 8) + ((y+255) & (~0xFF)) - y - 1;
			}
		}
	}

	return inc;
}

/**
 * @brief Checks if there is collision on top of the player
 * 
 * @param y: Y coordinate of the player (Fixed)
 * @param x: X coordinate of the player (Fixed)
 * @param inc: Y axis increase from the player (Fixed)
 * @param space_tile: Index of empty tile
 * @return uint32_t: Returns he amount the player can move upward
 */
inline uint32_t collision_hero_top(int32_t y, int32_t x, int32_t inc, uint32_t space_tile) {
	uint32_t ii, dd, tile, tt;

	y = ROUND(y);
	x = ROUND(x);

	// Get the nearest tile
	tt = ROUND(inc+255);

	for (dd = 1; dd <= tt; dd++) {
		for (ii = 0; ii < 2; ii++) {
			tile = se_index_fast(DIV8(x)+ii, DIV8(y-dd));
			if ((map_mem[SCRBLK_IDX][tile] & 0x01FF) != space_tile) {
				return FIXED(dd-1);
			}
		}
	}

	return inc;
}


/**
 * @brief Get screen index for non affine backgrounds. Based on TONC.
 * 
 * @param tx: X coordinate
 * @param ty: Y coordinate
 * @return uint16_t: Returns map index.
 */
inline uint16_t se_index_fast(uint16_t tx, uint16_t ty) {
	uint16_t n;
	uint32_t curr_cnt;

	curr_cnt = BG_CNT_0 & 0xC000;
	curr_cnt = curr_cnt >> 14;

	n = tx + (ty << 5);
	if (tx >= 32) n += 0x03E0;
	if (ty >= 32 && curr_cnt == 3) n += 0x0400;
	return n;
}

/* @brief Print text (TEXT HAS TO BE ALL CAPS).
   @param text: Text that will be displayed.
   @param sy: Starting y.
   @param sx: Starting x.
   @param oam_index: Base tile index. Font sprites have to start here.
   */
void print_text_4bpp(const char *text, int sy, int sx, int btid) {
	char c;
	while ((c = *text++)) {
		// Numbers
		if (c < 58 && c > 47) {
			__oam[__curr_text_idx].attr0 = BUILD_ATTR0(sy,0,0,0,0,0);
			__oam[__curr_text_idx].attr1 = BUILD_ATTR1(sx,0,0,0,0);
			__oam[__curr_text_idx].attr2 = BUILD_ATTR2((btid + c - 22),0,textDefaultPalbank);
			__curr_text_idx = (__curr_text_idx == 127) ? textDefaultOAMIdx : __curr_text_idx + 1;
		// Text
		} else if (c > 64) {
			__oam[__curr_text_idx].attr0 = BUILD_ATTR0(sy,0,0,0,0,0);
			__oam[__curr_text_idx].attr1 = BUILD_ATTR1(sx,0,0,0,0);
			__oam[__curr_text_idx].attr2 = BUILD_ATTR2((btid + c - 65),0,textDefaultPalbank);
			__curr_text_idx = (__curr_text_idx == 127) ? textDefaultOAMIdx : __curr_text_idx + 1;
		}

		// "else if" cause we ignore \s
		sx += 8;
	}
}

/**
 * @brief Displays text on the center of the screen.
 * 
 * @param text: Text to display.
 * @param sy: Y coordinate.
 * @param btid: Base tile id of the text.
 */
void print_hcenter_4bpp(const char *text, int sy, int btid) {
	int i, offx;
	char *start_line, *end_line;

	start_line = (char*)text;
	end_line = NULL;

	while (end_line == NULL || *end_line != '\0') {

		// We move to the next line & make
		// sure we don't break any words or
		// ignore new lines specified by the user
		for (i = 0; i < MAX_CHARACTERS_PER_LINE; i++) {
			
			if (start_line[i] == '\0' || start_line[i] == '\n') {
				end_line = &start_line[i]; break;
			}
			else if (start_line[i] == SPACE) {
				end_line = &start_line[i];
			}
		}

		// Offset to center the line
		offx = (GBA_WIDTH - ((end_line - start_line) << 3)) >> 1;
		while (start_line != end_line) {

			// Numbers
			if (*start_line < 58 && *start_line > 47) {
				__oam[__curr_text_idx].attr0 = BUILD_ATTR0(sy,0,0,0,0,0);
				__oam[__curr_text_idx].attr1 = BUILD_ATTR1(offx,0,0,0,0);
				__oam[__curr_text_idx].attr2 = BUILD_ATTR2((btid + *start_line - 22),0,textDefaultPalbank);
				__curr_text_idx = (__curr_text_idx == 127) ? textDefaultOAMIdx : __curr_text_idx + 1;
			// Text
			} else if (*start_line > 64) {
				__oam[__curr_text_idx].attr0 = BUILD_ATTR0(sy,0,0,0,0,0);
				__oam[__curr_text_idx].attr1 = BUILD_ATTR1(offx,0,0,0,0);
				__oam[__curr_text_idx].attr2 = BUILD_ATTR2((btid + *start_line - 65),0,textDefaultPalbank);
				__curr_text_idx = (__curr_text_idx == 127) ? textDefaultOAMIdx : __curr_text_idx + 1;
			}

			// "else if" cause we ignore \s
			offx += 8; start_line++;
		}

		// Increase Y offset
		sy += 8;

		// Ignore last character
		start_line++;
	}

}

/**
 * @brief Prints a number on screen. NOT EFFICIENT.
 * 
 * @param number: Number to display.
 * @param sy: Y coordinate.
 * @param sx: X coordinate.
 * @param btid: Base tile id of text.
 * @param oam_index: OAM index to start with.
 */
void print_int_4bpp(int number, int sy, int sx, int btid, int oam_index) {

	char c;
	char result[10];
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = number;
		number /= 10;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - number * 10)];
	} while ( number );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}

	ptr = result;
	while ((c = *ptr++)) {
		__oam[oam_index].attr0 = BUILD_ATTR0(sy,0,0,0,0,0);
		__oam[oam_index].attr1 = BUILD_ATTR1(sx,0,0,0,0);
		__oam[oam_index].attr2 = BUILD_ATTR2((btid+c-22),0,0);
		oam_index++; sx += 8;
	}
}

/**
 * @brief Clears all the text.
 * 
 */
void clear_text() {
	uint32_t ii;

	for (ii = textDefaultOAMIdx; ii < 128; ii++) {
		ENABLE_HIDE_SPRITE(ii);
	}
}

/**
 * @brief Equivalent of "sleep". The device waits for a certain
 * amount of time.
 * 
 * @param frames: Time to wait
 */
void halt(uint32_t frames) {
	while (frames--) swi_vblank();
}

/**
 * @brief Equivalent of "sleep". The device waits for a certain
 * amount of time until the specified key is pressed.
 * 
 * @param frames: Time to wait
 * @param key: Key that allows the user to exit the function
 */
void halt_with_key(uint32_t frames, uint32_t key) {
	uint32_t curr_key;

	while (frames--) {
		swi_vblank();
		curr_key = *((volatile uint16_t *)KY_REG);
		if (key_press(curr_key, key)) {
			break;
		}
	}
}
