#ifndef GAME_H
#define GAME_H

#include "types.h"
#include "gba.h"
#include "config.h"
#include "utils.h"
#include "sprite.h"

extern HERO_T __hero STR_IN_IWRAM;
extern ENEMY_T __enemies[20] STR_IN_IWRAM;

extern uint32_t __delta;
extern uint32_t __last_delta_hero;
extern uint32_t __last_delta_enemies;

extern uint32_t __voff;
extern uint32_t __hoff;

extern int32_t __prev_key;
extern int32_t __curr_key;

void get_key();

void save_key();

void reset_deltas();

void increase_delta();

void adjust_hero_position(int32_t *inc_y, int32_t *inc_x);

void update_hero_render();

void update_enemies_render(uint32_t *num_enemies);

void update_ui_render();

void update_pause_render(uint32_t *code);

void update_hero_state(int32_t *inc_y, int32_t *inc_x);

void update_enemies_state(uint32_t num_enemies);

void process_sprite_collisions(uint32_t num_enemies);

void reset_hero_to(int32_t y, int32_t x);

void build_hero();

void kill_hero();

void build_ui();

void reset_dynamic_bg_to(int32_t y, int32_t x);

void reset_static_bg_to(int32_t y, int32_t x);

void add_enemy_slime(uint32_t y, uint32_t x, uint32_t speed, uint32_t dir, uint32_t *num_enemies);

void add_enemy_fly(uint32_t y, uint32_t x, uint32_t speed, uint32_t dir, uint32_t *num_enemies);

#endif
