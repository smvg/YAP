
@{{BLOCK(logo)

@=======================================================================
@
@	logo, 64x32@4, 
@	+ palette 256 entries, not compressed
@	+ 32 tiles Metatiled by 8x4 not compressed
@	Total size: 512 + 1024 = 1536
@
@	Time-stamp: 2021-06-05, 17:28:14
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global logoTiles		@ 1024 unsigned chars
	.hidden logoTiles
logoTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.word 0x22222222,0x22222222,0x22111122,0x22111122,0x22333322,0x22333322,0x11333322,0x11333322
	.word 0x22222200,0x22222200,0x22112200,0x22112200,0x22332222,0x22332222,0x22331111,0x22331111
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x22222222,0x11111122,0x11111122,0x33333322,0x33333322,0x22333322,0x22333322
	.word 0x22222222,0x22222222,0x22221111,0x22221111,0x22223333,0x22223333,0x22113322,0x22113322
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x22222222,0x11112222,0x11112222,0x22332222,0x22332222,0x11331122,0x11331122
	.word 0x22222222,0x22222222,0x22111111,0x22111111,0x22333322,0x22333322,0x22333311,0x22333311

	.word 0x33332222,0x33332222,0x22221111,0x22221111,0x22222200,0x22222200,0x11112200,0x11112200
	.word 0x22333333,0x22333333,0x22333322,0x22333322,0x22333322,0x22333322,0x22333311,0x22333311
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x11333322,0x11333322,0x33333322,0x33333322,0x22223322,0x22223322,0x11223322,0x11223322
	.word 0x22333311,0x22333311,0x22333333,0x22333333,0x22332222,0x22332222,0x22332211,0x22332211
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x33333322,0x33333322,0x22333322,0x22333322,0x22333322,0x22333322,0x11333322,0x11333322
	.word 0x22222222,0x22222222,0x11111122,0x11111122,0x00000022,0x00000022,0x00000022,0x00000022

	.word 0x22222200,0x22222200,0x11111100,0x11111100,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x22222222,0x11111111,0x11111111,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00222222,0x00222222,0x00111111,0x00111111,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222200,0x22222200,0x11111100,0x11111100,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x22222222,0x22222222,0x11111111,0x11111111,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000022,0x00000022,0x00000011,0x00000011,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global logoPal		@ 512 unsigned chars
	.hidden logoPal
logoPal:
	.hword 0x0000,0x35AD,0x6FBB,0x56B5,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(logo)
