
@{{BLOCK(fly)

@=======================================================================
@
@	fly, 64x8@4, 
@	+ palette 256 entries, not compressed
@	+ 8 tiles not compressed
@	Total size: 512 + 256 = 768
@
@	Time-stamp: 2021-06-30, 19:25:47
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global flyTiles		@ 256 unsigned chars
	.hidden flyTiles
flyTiles:
	.word 0x11100011,0x13310131,0x13331331,0x13334431,0x01145510,0x00144410,0x00011100,0x00000000
	.word 0x01110110,0x13331331,0x13334431,0x13335531,0x01144410,0x00011100,0x00000000,0x00000000
	.word 0x00000000,0x00011100,0x01144410,0x13335531,0x13334431,0x13311131,0x01100010,0x00000000
	.word 0x11001100,0x13111310,0x13331331,0x01333441,0x00114551,0x00014441,0x00001110,0x00000000
	.word 0x11001100,0x13111310,0x13331331,0x01333000,0x00110000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x02222200,0x02222200,0x02222200,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x02000200,0x20000020,0x00000000,0x20000020,0x02000200,0x00000000,0x00000000,0x00000000
	.word 0x20000020,0x00000000,0x00000000,0x00000000,0x20000020,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global flyPal		@ 512 unsigned chars
	.hidden flyPal
flyPal:
	.hword 0x0000,0x18C6,0x7BDE,0x35FA,0x39B8,0x30F2,0x0000,0x0000

@}}BLOCK(fly)
