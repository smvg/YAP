#ifndef UTILS_H
#define UTILS_H

#include "gba.h"
#include "types.h"
#include "config.h"

#define SPACE 32

#define ENABLE_HFLIP_SPRITE(i) (__oam[i].attr1 |= 0x1000)
#define DISABLE_HFLIP_SPRITE(i) (__oam[i].attr1 &= 0xEFFF)

#define ENABLE_HIDE_SPRITE(i) (__oam[i].attr0 |= 0x0200)
#define DISABLE_HIDE_SPRITE(i) (__oam[i].attr0 &= 0xFDFF)

#define ENABLE_HIDE_AFF_SPRITE(i) (__oam[i].attr0 = (__oam[i].attr0 & 0xFCFF) | 0x0200)
#define DISABLE_HIDE_AFF_SPRITE(i) (__oam[i].attr0 = (__oam[i].attr0 & 0xFCFF) | 0x0100)

#define DIV2(n) ((n) >> 1)
#define DIV4(n) ((n) >> 2)
#define DIV8(n) ((n) >> 3)
#define MUL2(n) ((n) << 1)
#define MUL4(n) ((n) << 2)
#define MUL8(n) ((n) << 3)

#define ROUND_NEXT_MULTIPLE_8(x) ((x+7) & (-8))

#define BYTES_TO_WORDS(n) DIV4(n)

void hflip_sprite(uint32_t);

void vflip_sprite(uint32_t);

void flip_hidden(uint32_t);

void move_y(uint32_t, int32_t);

void incr_y(uint32_t, int32_t);

void move_x(uint32_t, int32_t);

void incr_x(uint32_t, int32_t);

int key_press(int32_t, int32_t);
int key_press_first_time(int32_t, int32_t, int32_t);
int multikey_press_first_time(int32_t, int32_t, int32_t);
int key_press_hold(int32_t, int32_t, int32_t);
int key_press_no_more(int32_t, int32_t, int32_t);

uint32_t get_abs(int32_t n);

void change_tidx(uint32_t, uint32_t);

uint32_t move_hero_left(int32_t, int32_t, int32_t, uint32_t);
uint32_t move_hero_right(int32_t, int32_t, int32_t, uint32_t);

// uint32_t move_enemy_left(int32_t, int32_t, int32_t, int32_t, uint32_t);
// uint32_t move_enemy_right(int32_t, int32_t, int32_t, int32_t, uint32_t);
uint32_t move_enemy_left(ENEMY_T*, int32_t, uint32_t);
uint32_t move_enemy_right(ENEMY_T*, int32_t, uint32_t);

uint32_t collision_hero_left(int32_t, int32_t, int32_t, uint32_t);
uint32_t collision_hero_right(int32_t, int32_t, int32_t, uint32_t);
uint32_t collision_hero_bottom(int32_t, int32_t, int32_t, uint32_t);
uint32_t collision_hero_top(int32_t, int32_t, int32_t, uint32_t);

uint32_t detect_collision_hero_y(int32_t, int32_t, uint32_t);
uint32_t detect_collision_hero_x(int32_t, int32_t, uint32_t);

// Get screen index 
uint16_t se_index_fast(uint16_t tx, uint16_t ty);

void print_text_4bpp(const char*, int, int, int);

void print_text_hcenter_4bpp(const char*, int, int, int, int);
void print_hcenter_4bpp(const char*, int, int);
void print_int_4bpp(int, int, int, int, int);
void clear_text();

void halt(uint32_t frames);
void halt_with_key(uint32_t frames, uint32_t key);

#endif
