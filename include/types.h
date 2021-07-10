#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

typedef struct
{
	uint32_t data[8];
} TILE;

typedef uint16_t SCR_ENTRY;
typedef SCR_ENTRY SCREENBLOCK[1024];

typedef struct
{
	uint16_t attr0;
	uint16_t attr1;
	uint16_t attr2;
	int16_t fill;
} __attribute__((aligned(4))) OBJ_ATTR;

typedef struct
{
	uint16_t fill0[3];
	int16_t pa;
	uint16_t fill1[3];
	int16_t pb;
	uint16_t fill2[3];
	int16_t pc;
	uint16_t fill3[3];
	int16_t pd;
} __attribute__((aligned(4))) OBJ_AFFINE;

typedef TILE CHARBLOCK[512];

#define FOREVER 1

#define DIR_LEFT 1
#define DIR_RIGHT 0

#define CURR_STATE_IDLE 1
#define CURR_STATE_RUN 2
#define CURR_STATE_JUMP 4
#define CURR_STATE_FALL 8
#define CURR_STATE_ATTACK 16
#define CURR_STATE_ATTACKING 32
#define CURR_STATE_ATTACKED 64
#define CURR_STATE_DEATH 128
#define CURR_STATE_TRANS 256

#define PAST_STATE_IDLE CURR_STATE_IDLE << 16
#define PAST_STATE_RUN CURR_STATE_RUN << 16
#define PAST_STATE_JUMP CURR_STATE_JUMP << 16
#define PAST_STATE_FALL CURR_STATE_FALL << 16
#define PAST_STATE_ATTACK CURR_STATE_ATTACK << 16
#define PAST_STATE_ATTACKING CURR_STATE_ATTACKING << 16
#define PAST_STATE_ATTACKED CURR_STATE_ATTACKED << 16
#define PAST_STATE_DEATH CURR_STATE_DEATH << 16
#define PAST_STATE_TRANS CURR_STATE_TRANS << 16

#define INIT_STATE_IDLE (CURR_STATE_IDLE | PAST_STATE_IDLE)
#define INIT_STATE_RUN (CURR_STATE_RUN | PAST_STATE_RUN)
#define INIT_STATE_JUMP (CURR_STATE_JUMP | PAST_STATE_JUMP)
#define INIT_STATE_FALL (CURR_STATE_FALL | PAST_STATE_FALL)
#define INIT_STATE_ATTACK (CURR_STATE_ATTACK | PAST_STATE_ATTACK)
#define INIT_STATE_ATTACKING (CURR_STATE_ATTACKING | PAST_STATE_ATTACKING)
#define INIT_STATE_ATTACKED (CURR_STATE_ATTACKED | PAST_STATE_ATTACKED)
#define INIT_STATE_DEATH (CURR_STATE_DEATH | PAST_STATE_DEATH)

#define HERO_BLUE_VARIATION 0
#define HERO_GOLD_VARIATION 1

#define GET_PAST_STATE(s) (s >> 16)
#define GET_CURR_STATE(s) (s & 0xFFFF)
#define SET_CURR_STATE(s, c) (s = (s & ~0xFFFF) | ((c)&0xFFFF))
#define ADD_CURR_STATE(s, c) (s |= (c))
#define NEW_STATE(s) (s = s << 16)

#define STATE_HAS_CHANGED(s) (GET_CURR_STATE(s) != GET_PAST_STATE(s))

#define FIXED(n) ((n) << 8)
#define ROUND(n) ((n) >> 8)

typedef struct
{
	int32_t y;
	int32_t x;
	int32_t life;
	uint32_t dir;
	// 0 - Blue
	// 1 - Gold
	uint32_t type;
	int32_t att_frame;
	int32_t dmg_frame;
	int32_t jmp_frame;
	int32_t trans_frame;
	uint32_t state;
} HERO_T;

#define ENEMY_SLIME 1
#define ENEMY_FLY 2
typedef struct
{
	// 1- Slime
	// 2 - Fly
	uint32_t type;
	// 1 - Idle
	// 2 - Run
	// 4 - Attack
	// 8 - Attacked
	// 16 - Death
	// [31-16]: Current state
	// [15-0]: Past state
	uint32_t state;
	int32_t y, x;
	uint32_t dmg;
	int32_t life;
	uint32_t btidx;
	uint32_t speed;
	uint32_t ref_delta;
	int32_t curr_frame;
	int32_t dir;
} ENEMY_T;

#endif
