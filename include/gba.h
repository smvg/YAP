#ifndef GBA_H
#define GBA_H

#include "types.h"

// Specifise the target of the function
#define ARM __attribute__((__target__("arm")))
#define THUMB __attribute__((__target__("thumb")))

// Memory addresses of interest
#define MEM_IO 0x04000000
#define MEM_PALETTE 0x05000000
#define MEM_VRAM 0x06000000
#define MEM_OAM 0x07000000
#define MEM_GAMEPAK 0x0E000000

// VCounter
#define VCounter (volatile uint16_t*)0x04000006

// Screen dimensions
#define GBA_WIDTH 240
#define GBA_HEIGHT 160

// Stores data in External RAM
#define STR_IN_EWRAM __attribute__((section(".ewram")))
// Stores data in Internal RAM
#define STR_IN_IWRAM __attribute__((section(".iwram")))
// Stores unitialised in External RAM
#define STR_IN_EWRAM_GLOBALS __attribute__((section(".sbss")))

// Call function from Internal RAM
#define CALL_FROM_IWRAM __attribute__((section(".iwram"), long_call))
// Call function from External RAM
#define CALL_FROM_EWRAM __attribute__((section(".ewram"), long_call))

// Pointers to previous addresses
#define __vram ((volatile uint16_t*)MEM_VRAM)
#define __io ((volatile uint16_t*)MEM_IO)
#define __oam ((volatile OBJ_ATTR*)MEM_OAM)
#define __aff ((volatile OBJ_AFFINE*)MEM_OAM)
#define __palette ((volatile uint16_t*)MEM_PALETTE)
#define __gamepak ((volatile uint8_t*)MEM_GAMEPAK)
#define __dispcnt *__io

/* EFFECTS */

// Mosaic
#define __mosval *((volatile uint16_t*)0x0400004C)
#define MOS_BG_H(x) (x) 
#define MOS_BG_V(x) (x << 4)
#define MOS_OBJ_H(x) (x << 8) 
#define MOS_OBJ_V(x) (x << 12)

// Other effects, such as fade & blend
#define __fxcnt *((volatile uint16_t*)0x04000050)
#define __fxval_bld *((volatile uint16_t*)0x04000052)
#define __fxval_int *((volatile uint16_t*)0x04000054)
#define FX_MODE_BLEND 0x0040
#define FX_MODE_BRIGHT 0x0080
#define FX_MODE_DARK 0x00C0

// Selectors for the effects (target 1)
#define FX_T1_BG0 0x0001
#define FX_T1_BG1 0x0002
#define FX_T1_BG2 0x0004
#define FX_T1_BG3 0x0008
#define FX_T1_OBJ 0x0010
#define FX_T1_BCK 0x0020
#define FX_T1_ALL 0x003F

// Selectors for the effects (target 2)
#define FX_T2_BG0 0x0100
#define FX_T2_BG1 0x0200
#define FX_T2_BG2 0x0400
#define FX_T2_BG3 0x0800
#define FX_T2_OBJ 0x1000
#define FX_T2_BCK 0x2000
#define FX_T2_ALL 0x3F00

// Pointers to useful locations (based on TONC)
#define tile_mem ((CHARBLOCK*)MEM_VRAM)
#define map_mem ((SCREENBLOCK*)MEM_VRAM)
#define pal_bg_mem ((short*)MEM_PALETTE)
#define pal_obj_mem ((short*)(MEM_PALETTE+0x0200))

/* DISPLAY CONTROL REGISTER */
#define DC_REG 0x04000000

// Available modes for the DC register
#define DC_MODE 0x0007
#define DC_MODE_0 0x0
#define DC_MODE_1 0x1
#define DC_MODE_2 0x2
#define DC_MODE_3 0x3
#define DC_MODE_4 0x4
#define DC_MODE_5 0x5

// Selector for checking whether game is running
// in a GBA or GB(C)
#define DC_GB 0x06

// Page select, useful for supported bitmap modes
#define DC_PS 0x10
#define DC_PS_0 0x00
#define DC_PS_1 0x10

#define DC_HB 0x20

// Object mapping: 1D or 2D
#define DC_OM 0x40
#define DC_OM_1D 0x40
#define DC_OM_2D 0x00

// Screen blank
#define DC_SB 0x80

// Rendering mode for background & sprites.
#define DC_RENDER 0x1F00
#define DC_RENDER_BG0 0x0100
#define DC_RENDER_BG1 0x0200
#define DC_RENDER_BG2 0x0400
#define DC_RENDER_BG3 0x0800
#define DC_RENDER_OBJ 0x1000

// Available Windows (0 + 1 + Object)
#define DC_WINDOW 0xE000
#define DC_WINDOW_0 0x2000
#define DC_WINDOW_1 0x4000
#define DC_WINDOW_O 0x8000

/* DISPLAY STATUS REGISTER */
#define DS_REG 0x04000004

// Scanline related
#define DS_VBR 0x0001 
#define DS_HBR 0x0002 
#define DS_VSR 0x0004 
#define DS_VBI 0x0008 
#define DS_HBI 0x0010 
#define DS_VCI 0x0020 
#define DS_VCT 0xFF00

/* BACKGROUNDS */
#define BG_REG 0x04000008

#define BG_CNT_0 *((volatile uint16_t*)BG_REG + 0)
#define BG_CNT_1 *((volatile uint16_t*)BG_REG + 1)
#define BG_CNT_2 *((volatile uint16_t*)BG_REG + 2)
#define BG_CNT_3 *((volatile uint16_t*)BG_REG + 3) 

// Background offset registers
#define BG_HOFF_0 0x04000010
#define BG_HOFF_1 0x04000014 
#define BG_HOFF_2 0x04000018 
#define BG_HOFF_3 0x0400001C 

#define BG_VOFF_0 0x04000012 
#define BG_VOFF_1 0x04000016 
#define BG_VOFF_2 0x0400001A 
#define BG_VOFF_3 0x0400001E 

// Background parameters. Used in combination with BG_CNT_X
#define BG_PRIORITY(p) (p & 0x03)
#define BG_CBB(n) ((n) << 2)
#define BG_MOSAIC (1 << 6)
#define BG_COLOR_4BPP (0 << 7)
#define BG_COLOR_8BPP (1 << 7)
#define BG_SBB(n) ((n) << 8)
#define BG_AFFINE_WRAP (1 << 13)
#define BG_512x512 (3 << 14)
#define BG_256x512 (2 << 14)
#define BG_512x256 (1 << 14)
#define BG_256x256 (0 << 14)

// Keypad registers
#define KY_REG 0x04000130

#define KY_L 0x0200
#define KY_R 0x0100
#define KY_DOWN 0x0080
#define KY_UP 0x0040
#define KY_LEFT_RIGHT 0x0030
#define KY_LEFT 0x0020
#define KY_RIGHT 0x0010
#define KY_START 0x0008
#define KY_SELECT 0x0004
#define KY_B 0x0002
#define KY_A 0x0001


/* INTERRUPT RELATED */

// Interrupt Check Flag
#define IR_CHECK_REG *((volatile uint16_t*)0x03007FF8)
// Function address for IRQ
#define IR_FUNCT_ADDR *((fnptr*)0x03007FFC)
// Enable Register
#define IR_ENABLE_REG *((volatile uint16_t*)0x04000200)
//Acknowledge Register
#define IR_CONFIRM_REG *((volatile uint16_t*)0x04000202)
// Master enable
#define IR_MASTER_REG *((volatile uint16_t*)0x04000208)

// Interrupts for VBlank and the 4 timers
#define IR_VBLANK 0x0001
#define IR_TIMER0 0x0008
#define IR_TIMER1 0x0010
#define IR_TIMER2 0x0020
#define IR_TIMER3 0x0040

/* SOUND */
#define SND_MASTER *((volatile uint16_t*)0x04000084)
#define SND_MASTER_ENABLE 0x0080
#define SND_CNT *((volatile uint32_t*)0x04000080)
#define SND_FIFO_A 0x040000A0
#define SND_FIFO_B 0x040000A4

/* TIMERS */
#define T0_VAL *((volatile uint16_t*)0x04000100)
#define T0_CNT *((volatile uint16_t*)0x04000102)
#define T1_VAL *((volatile uint16_t*)0x04000104)
#define T1_CNT *((volatile uint16_t*)0x04000106)
#define T2_VAL *((volatile uint16_t*)0x04000108)
#define T2_CNT *((volatile uint16_t*)0x0400010A)
#define T3_VAL *((volatile uint16_t*)0x0400010C)
#define T3_CNT *((volatile uint16_t*)0x0400010E)

// Timer parameters
#define TIMER_FREQ_1 0x0000
#define TIMER_FREQ_64 0x0001
#define TIMER_FREQ_256 0x0002
#define TIMER_FREQ_1024 0x0003
#define TIMER_ENABLE 0x0080
#define TIMER_IRQ 0x0040
#define TIMER_CASCADE 0x0004


/* BUILDERS BASED ON TONC'S LIBRARY */
/* @brief Build Attribute 0. This does not sanitize params.
   @param y: Y coordinate.
   @param om: Affine object mode. 00 (Normal rendering), 01 (Sprite is an affine)
   10 (Disables rendering) & 11 (Affine sprite using double rendering area).
   @param gm: Flags for special effects. 00 (Normal rendering), 01 (Alpha blending
   ) & 10 (Object part of the object window. Sprite isn't rendered but serves as a
   mask for bgs and other sprites).
   @param mos: Enables mosaic effect. 
   @param cm: Color mode. 4bpp if cleared and 8bpp if set.
   @param sh: Sprite shape.
*/
#define BUILD_ATTR0(y,om,gm,mos,cm,sh) (((y) & 0xFF) | om << 8 | gm << 10 | mos << 12 | cm << 13 | sh << 14)

/* @brief Build Attribute 1. This does not sanitize params.
   @param x: X coordinate.
   @param aid: Affine index. Valid only if attr0{8} is set.
   @param hf: Horizontal flip.
   @param vf: Vertical flip. 
   @param sz: Sprite shape.
*/
#define BUILD_ATTR1(x,aid,hf,vf,sz) (((x) & 0x01FF) | aid << 9 | hf << 12 | vf << 13 | sz << 14)

/* @brief Build Attribute 2. This does not sanitize params.
   @param tid: Base tile index.
   @param pr: Priority. Higher priorities are drawn first.
   @param pb: Palette bank to use when 16-color mode is set.
*/
#define BUILD_ATTR2(tid,pr,pb) (tid | pr << 10 | pb << 12)

typedef void (*fnptr)(void);

void swi_vblank();

ARM void handle_irq();

void init_irq(uint32_t);

void clear_oam();

int dma_is_active_1();
void dma_stop_transfer_1();
void dma_full_transfer_1(uint32_t, const void*);
void dma_full_transfer_3(const void*, const void*, uint32_t);

#endif
