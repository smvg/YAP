
@{{BLOCK(rabit_walk_anim_strip_6)

@=======================================================================
@
@	rabit_walk_anim_strip_6, 96x8@4, 
@	+ palette 256 entries, not compressed
@	+ 12 tiles Metatiled by 2x1 not compressed
@	Total size: 512 + 384 = 896
@
@	Time-stamp: 2021-06-01, 18:51:42
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global rabbitTiles		@ 384 unsigned chars
	.hidden rabbitTiles
rabbitTiles:
	.word 0x00000000,0x30000000,0x33000000,0x33000000,0x33330000,0x33333000,0x33333300,0x33333300
	.word 0x00000000,0x00003303,0x00003203,0x00003223,0x00003333,0x00001331,0x00003333,0x00003333
	.word 0x30000000,0x30000000,0x33000000,0x33000000,0x33330000,0x33333000,0x33333300,0x00333300
	.word 0x00003303,0x00003203,0x00003223,0x00003333,0x00003333,0x00001331,0x00003333,0x00000000
	.word 0x30000000,0x30000000,0x33000000,0x33000000,0x33330000,0x33333000,0x33333300,0x00333300
	.word 0x00003303,0x00032033,0x00032233,0x00033333,0x00013313,0x00033333,0x00033333,0x00030030
	.word 0x00000000,0x00000000,0x30000000,0x33330000,0x33333000,0x33333300,0x33333300,0x00003300
	.word 0x00033033,0x00032033,0x00032233,0x00033333,0x00013313,0x00033333,0x00033333,0x00300300

	.word 0x00000000,0x00000000,0x00000000,0x30330000,0x33333000,0x33333300,0x33333300,0x33000000
	.word 0x00033033,0x00032033,0x00032233,0x00033333,0x00013313,0x00033333,0x00033333,0x00033333
	.word 0x00000000,0x00000000,0x00000000,0x30000000,0x33333000,0x33333300,0x33333300,0x33333300
	.word 0x00000000,0x00003003,0x00032033,0x00032233,0x00013313,0x00033333,0x00033333,0x00033333

	.section .rodata
	.align	2
	.global rabbitPal		@ 512 unsigned chars
	.hidden rabbitPal
rabbitPal:
	.hword 0x0000,0x39B8,0x62D7,0x6BBB,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(rabit_walk_anim_strip_6)
