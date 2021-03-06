/*

 SHIP.C

 Tile Source File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : None.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 1

  Palette colors       : None.
  SGB Palette          : None.
  CGB Palette          : None.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/

#define shipSize 2

/* Start of tile array. */
unsigned char ship[] =
{
  // Left half of the ship
  0x7C,0x7C,0xFC,0xFC,0x2F,0x3F,0x41,0x7F,
  0x41,0x7F,0x2F,0x3F,0xFC,0xFC,0x7C,0x7C,
  // Right half of the ship
  0x60,0x60,0xF8,0xF8,0x9C,0xFC,0x0E,0xF6,
  0x0E,0xF6,0x9C,0xFC,0xF8,0xF8,0x60,0x60
};

/* End of SHIP.C */
