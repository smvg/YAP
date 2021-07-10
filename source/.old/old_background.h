
//{{BLOCK(background)

//======================================================================
//
//	background, 512x256@4, 
//	+ palette 256 entries, not compressed
//	+ 275 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x32 
//	Total size: 512 + 8800 + 4096 = 13408
//
//	Time-stamp: 2021-03-07, 01:19:21
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKGROUND_H
#define GRIT_BACKGROUND_H

#define backgroundTilesLen 8800
extern const unsigned int backgroundTiles[2200];

#define backgroundMapLen 4096
extern const unsigned short backgroundMap[2048];

#define backgroundPalLen 512
extern const unsigned short backgroundPal[256];

#endif // GRIT_BACKGROUND_H

//}}BLOCK(background)
