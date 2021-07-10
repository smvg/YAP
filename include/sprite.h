
//{{BLOCK(sprite)

//======================================================================
//
//	sprite, 512x16@4, 
//	+ palette 256 entries, not compressed
//	+ 128 tiles Metatiled by 2x2 not compressed
//	Total size: 512 + 4096 = 4608
//
//	Time-stamp: 2021-06-06, 04:05:16
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SPRITE_H
#define GRIT_SPRITE_H

#define spriteTilesLen 4096
extern const unsigned int spriteTiles[1024];

#define spritePalLen 16
extern const unsigned short spritePalRed[8];
extern const unsigned short spritePalBlue[8];
extern const unsigned short spritePalGold[8];

#endif // GRIT_SPRITE_H

//}}BLOCK(sprite)
