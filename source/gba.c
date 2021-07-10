#include "gba.h"

/**
 * @brief Calls the BIOS function that halts the CPU until VBlank is reached.
 * 
 */
inline void swi_vblank() {
   asm volatile("SWI 0x05" ::: "r0", "r1", "r2", "r3");
}

/**
 * @brief Function in charge of processing interrupts.
 * In this implementation, only VBlank and Timer 2 are used.
 *  
 */
ARM void handle_irq() {
	uint32_t curr_enable_reg, curr_confirm;

	curr_enable_reg = IR_ENABLE_REG;
	curr_confirm = curr_enable_reg & IR_CONFIRM_REG;

	IR_CONFIRM_REG = curr_confirm;
	IR_CHECK_REG |= curr_confirm;

	IR_MASTER_REG= 0;
	IR_ENABLE_REG &= ~curr_confirm;

	if (curr_confirm & IR_TIMER2) {
		*((volatile uint32_t*)0x040000C4) = 0;
		*((volatile uint32_t*)0x040000C4) = 0xB6400000;
	}

	IR_ENABLE_REG= curr_enable_reg;
	IR_MASTER_REG= 1;
}

/**
 * @brief Inits the interrupts specified in flags.
 * 
 * @param flags: Flags to enable in the Interrupt Enable Reg. 
 */
void init_irq(uint32_t flags) {
   IR_FUNCT_ADDR = (fnptr)handle_irq;
   IR_ENABLE_REG |= flags;
   IR_MASTER_REG = 1;
}

/* @brief Hides all sprites in OAM. */
inline void clear_oam() {
	uint32_t *x;
	for (x = (uint32_t*)MEM_OAM; x < (uint32_t*)MEM_OAM+0x3FF; x+=2)
		*x = 2 << 8;
}

/**
 * @brief Checks whether DMA channel 1 is active.
 * 
 * @return int: Returns 1 if active and 0 if not.
 */
int dma_is_active_1()
{
	return *((volatile uint32_t*)0x040000C4) & 0x8000;
}

/**
 * @brief Disables DMA channel 1.
 * 
 */
void dma_stop_transfer_1()
{
	*((volatile uint32_t*)0x040000C4) = 0;
}

/**
 * @brief Starts a 32 bit width transfer in DMA channel 1.
 * This is used for audio only, therefore no size needs
 * to be specified (it's supposed to use the FIFO buffer).
 * 
 * @param dst: Destination address.
 * @param src: Source address.
 */
void dma_full_transfer_1(uint32_t dst, const void *src)
{
	*((volatile uint32_t*)0x040000BC) = (uint32_t)src;
	*((volatile uint32_t*)0x040000C0) = dst;
	*((volatile uint32_t*)0x040000C4) = 0xB6400000;
}

/**
 * @brief Starts a 32 bit width transfer in DMA channel 3 (general pourpose)
 * 
 * @param dst: Destination address.
 * @param src: Source address.
 * @param size: Size of the transfer.
 */
void dma_full_transfer_3(const void *dst, const void *src, uint32_t size)
{
	*((volatile uint32_t*)0x040000D4) = (uint32_t)src;
	*((volatile uint32_t*)0x040000D8) = (uint32_t)dst;
	*((volatile uint32_t*)0x040000DC) = 0x84000000 | (size & 0xFFFF);
}