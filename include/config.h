#ifndef CONFIG_H
#define CONFIG_H

#define CREDIT_FADE_PERIOD 120
#define CREDIT_BETWEEN_PERIOD 60
#define CREDIT_HOLD_PERIOD 120

#define LVL_TITLE_HOLD_PERIOD 180
#define END_GAME_HOLD_PERIOD 300

//#define SKIP_TITLES

#define heroDefaultOAMIdx 0
#define swordDefaultOAMIdx 1
#define heroDefaultTileIdx 0
#define heroDefaultPalIdx 0
#define heroDefaultPalbank (heroDefaultPalIdx >> 4)

#define enemyDefaultOAMIdx 13
#define slimeDefaultTileIdx 128
#define flyDefaultTileIdx 192
#define slimeDefaultPalIdx 16
#define flyDefaultPalIdx 32
#define slimeDefaultPalbank (slimeDefaultPalIdx >> 4)
#define flyDefaultPalbank (flyDefaultPalIdx >> 4)

#define cursorDefaultOAMIdx 2
#define uiDefaultBaseOAMIdx 3

#define heroBpp 8
#define heroAttrBpp 0
#define heroAttr0Size 0
#define heroAttr1Size 1
#define heroHeight 16
#define heroWidth 16

#define HERO_GOLD_X_SPEED 320
#define HERO_GOLD_JMP_SPEED 384
#define HERO_GOLD_JMP_DROP 64
#define HERO_GOLD_FALL_SPEED 384
#define HERO_GOLD_FALL_MULT 64

#define HERO_BLUE_X_SPEED 384
#define HERO_BLUE_JMP_SPEED 384
#define HERO_BLUE_JMP_DROP 64
#define HERO_BLUE_FALL_SPEED 256
#define HERO_BLUE_FALL_MULT 64

#define HERO_TRANS_FRAME 10

#define BASE_JMP_FRAME 10
#define MAX_JMP_FRAME 25

#define HERO_BLUE_Y_SPEED 600
#define HERO_BLUE_Y_DROP 16
#define HERO_GOLD_Y_SPEED 600
#define HERO_GOLD_Y_DROP 16

#define DMG_COOLDOWN 60

#define HERO_TRIGGER_FRAME 10
#define ENEMY_TRIGGER_FRAME 10

#define SCRBLK_IDX 21

#define textDefaultOAMIdx 77
#define textDefaultTileIdx 384
#define textDefaultPalIdx 240
#define textDefaultPalbank (textDefaultPalIdx >> 4)
#define MAX_CHARACTERS_PER_LINE 22

#define cursorDefaultPalIdx 224
#define cursorDefaultPalbank (cursorDefaultPalIdx >> 4)
#define cursorDefaultTileIdx 364

#define heartDefaultPalIdx 208
#define heartDefaultPalbank (heartDefaultPalIdx >> 4)
#define heartDefaultTileIdx 344

#define SPACE_TILE_IDX 1

#define MAIN_MENU 0
#define CONTROLS 1
#define LEVEL_1_0 2
#define LEVEL_1_1 3
#define LEVEL_2_0 4
#define LEVEL_2_1 5
#define LEVEL_3_0 6
#define LEVEL_3_1 7
#define END_GAME 8

#endif
