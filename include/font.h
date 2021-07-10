
//{{BLOCK(font)

//======================================================================
//
//	font, 64x40@4, 
//	+ palette 256 entries, not compressed
//	+ 40 tiles not compressed
//	Total size: 512 + 1280 = 1792
//
//	Time-stamp: 2021-06-05, 18:44:18
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FONT_H
#define GRIT_FONT_H

#define fontTilesLen 1280
extern const unsigned int fontTiles[320];

#define fontPalLen 16
extern const unsigned short fontPal[8];

#define fontPalInvertedLen 16
extern const unsigned short fontPalInverted[8];

#endif // GRIT_FONT_H

//}}BLOCK(font)
