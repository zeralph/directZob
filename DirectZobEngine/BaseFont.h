#ifndef DZOB_BASEFONT_H
#define DZOB_BASEFONT_H

/* GIMP RGB C-Source image dump (font2.c) */
#include "Types.h"
namespace directZob 
{
static const struct {
  uint  	 width;
  uint  	 height;
  uint		 nbCharWidth;
  uint		 nbCharHeight;
  u8 		 pixel_data[16512];

} BaseFont = {
  256, 
  64, 
  32, 
  8,
  "0000000001111110011111100110110000010000001110000001000000000000111111110000000011111111000011110011110000111111001111110001100010000000000000100001100000100100011111110011111000000000000110000001000000010000000000000000000000000000000000000000000000000000"
"0000000010000001111111111111111000111000011111000001000000000000111111110011110011000011000000110100001000100001001000011101101111100000000011100011110000100100100100100110001100000000001111000011100000010000000110000011000000000000001001000001000011111110"
"0000000010100101110110111111111001111100001110000011100000011000111001110110011010011001000001010100001000111111001111110011110011111000001111100111111000100100100100100011100000000000011111100111110000010000000011000110000001000000011001100011100011111110"
"0000000010000001111111111111111011111110111111100111110000111100110000110100001010111101011111010100001000100000001000011110011111111110111111100001100000100100011100100100010000000000000110000101010001010100111111101111111001000000111111110111110001111100"
"0000000010111101110000110111110001111100111111101111111000111100110000110100001010111101100001000011110000100000001000111110011111111000001111100001100000100100000100100100010001111110011111100001000001111100000011000110000001000000011001101111111000111000"
"0000000010011001111001110011100000111000110101100111110000011000111001110110011010011001100001000001100001100000011001110011110011100000000011100111111000000000000100100011100001111110001111000001000000111000000110000011000001111110001001001111111000010000"
"0000000010000001111111110001000000010000000100000001000000000000111111110011110011000011100001000111111011100000111001101101101110000000000000100011110000100100000100101100110001111110000110000001000000010000000000000000000000000000000000000000000000000000"
"0000000001111110011111100000000000000000001110000011100000000000111111110000000011111111011110000001100011000000110000000001100000000000000000000001100000000000000000000111100000000000111111110000000000000000000000000000000000000000000000000000000000000000"
"0000000000010000001001000010010000011000000000000011000000010000000100000010000000000000000000000000000000000000000000000000000000111100000100000011110000111100000010000111111000011100011111100011110000111100000000000000000000001000000000000001000000111100"
"0000000000111000001001000010010000111110011000100100100000010000001000000001000001000100000100000000000000000000000000000000001001000010001100000100001001000010000110000100000000100000010000100100001001000010000100000001000000010000000000000000100001000010"
"0000000000111000001001000111111001000000011001000011000000100000010000000000100000111000000100000000000000000000000000000000010001000110010100000000001000000010001010000111110001000000000001000100001001000010000100000001000000100000011111100000010000000010"
"0000000000010000000000000010010000111100000010000101011000000000010000000000100011111110011111000000000001111110000000000000100001001010000100000000110000011100010010000000001001111100000010000011110000111110000000000000000001000000000000000000001000000100"
"0000000000010000000000000111111000000010000100001000100000000000010000000000100000111000000100000000000000000000000000000001000001010010000100000011000000000010111111100000001001000010000100000100001000000010000000000000000000100000000000000000010000001000"
"0000000000000000000000000010010001111100001001101000100000000000001000000001000001000100000100000001000000000000000100000010000001100010000100000100001001000010000010000100001001000010000100000100001000000100000100000001000000010000011111100000100000000000"
"0000000000010000000000000010010000011000010001100111011000000000000100000010000000000000000000000001000000000000000100000100000000111100011111000111111000111100000111000011110000111100000100000011110000111000000100000001000000001000000000000001000000001000"
"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000"
"0011110000011000011111000001110001111000011111100111111000011100010000100011100000001110011000100111000001100011011000100001100001111100001111000111110000111100011111110100001001000001010000010100000101000001011111110111100010000000011110000001000000000000"
"0100001000100100001000100010001000100100001000100010001000100010010000100001000000000100001001000010000001010101010100100010010000100010010000100010001001000010010010010100001001000001010000010010001000100010010000100100000001000000000010000010100000000000"
"0101111001000010001000100100000000100010001010000010100001000000010000100001000000000100001010000010000001001001010010100100001000100010010000100010001001000000000010000100001001000001010000010001010000010100000001000100000000100000000010000100010000000000"
"0101001001000010001111000100000000100010001110000011100001000000011111100001000000000100001100000010000001000001010001100100001000111100010000100011110000111100000010000100001001000001010010010000100000001000000010000100000000010000000010001000001000000000"
"0101111001111110001000100100000000100010001010000010100001001110010000100001000001000100001010000010000001000001010000100100001000100000010010100010100000000010000010000100001000100010010010010001010000001000000100000100000000001000000010000000000000000000"
"0100000001000010001000100010001000100100001000100010000000100010010000100001000001000100001001000010001001000001010000100010010000100000001111000010010001000010000010000100001000010100010010010010001000001000001000010100000000000100000010000000000000000000"
"0011110001000010011111000001110001111000011111100111000000011110010000100011100000111000011000110111111001000001010000100001100001110000000000110111001000111100000111000011110000001000001101100100000100011100011111110111100000000010011110000000000000000000"
"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111111"
"0001000000000000011000000000000000000110000000000000110000000000011000000001000000000010011000000011000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000110000010000001100000011001000000000"
"0001000000000000001000000000000000000010000000000001001000000000001000000000000000000000001000000001000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000001000000010000000010000100110000001000"
"0000100000111100001000000011110000000010001111000001000000111101001011000011000000000110001001000001000001110110010111000011110001101100001101100110110000111110011111000100001001000001010000010100010001000010011111000001000000010000000010000000000000010100"
"0000000000000010001011100100001000111010010000100011100001000010001100100001000000000010001010000001000001001001011000100100001000110010010011000011001001000000000100000100001001000001010010010010100001000010000010000110000000000000000001100000000000100010"
"0000000000111110001100010100000001000110011111100001000001000010001000100001000000000010001100000001000001001001010000100100001000110010010011000010001000111100000100000100001000100010010010010001000001000010000100000001000000010000000010000000000001000001"
"0000000001000010001100010100001001000110010000000001000000111110001000100001000001000010001010000001000001001001010000100100001000101100001101000010000000000010000100100100011000010100010010010010100000111110001000000001000000010000000010000000000001000001"
"0000000000111111001011100011110000111011001111000011100000000010011000100011100001000010001001100011100001001001010000100011110000100000000001000111000001111100000011000011101000001000001101100100010000000010011111000000110000010000001100000000000001111111"
"0000000000000000000000000000000000000000000000000000000001111100000000000000000000111100000000000000000000000000000000000000000001110000000011100000000000000000000000000000000000000000000000000000000001111100000000000000000000000000000000000000000000000000"
"0011110000000000000011000011110001000010001100000001000000000000001111000100001000110000001001000111110000110000010000100001100000001100000000000001111100011000000000000010000000011000001000000000000001000010010000100000100000011000010001001111100000011100"
"0100001001000100000000000100001000000000000000000000000000000000010000100000000000000000000000001000001000000000000110000001100000000000000000000010010000100100010000100001000000100100000100000100001000011000000000000000100000100100001010000100110000010010"
"0100000000000000001111000011100000111000001110000011100000111100001111000011110000111100000110000011000000011000001001000000000001111100001100110100010000000000000000000000000000000000000000000000000000100100010000100011111000100000011111000111100000010000"
"0100001001000100010000100000010000000100000001000000010001000000010000100100001001000010000010000001000000001000010000100011110000100000000011000111111100111100001111000011110001000010010000100100001001000010010000100100000001110000000100000100010001111100"
"0011110001000100011111100011110000111100001111000011110001000000011111100111111001111110000010000001000000001000011111100100001000111000001111110100010001000010010000100100001001000010010000100100001001000010010000100100000000100000011111000100111100010000"
"0000110001000100010000000100010001000100010001000100010000111100010000000100000001000000000010000001000000001000010000100111111000100000010001000100010001000010010000100100001001000010010000100011111000100100010000100011111001000010000100000100010000010000"
"0000001000111110001111000011111000111110001111100011111000000110001111000011110000111100000111000011100000011100010000100100001001111100001110110100011100111100001111000011110000111100001111000000001000011000001111000000100001111100000100000100010110010000"
"0011110000000000000000000000000000000000000000000000000000011100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011110000000000000000000000100000000000000000001110011001100000"
"0000110000001100000001000000000000110010001101000011110000111000000100000000000000000000010000100100001000000000000000000000000000100010010101011101101100010000000100000001000000010100000000000000000000010100000101000000000000010100000101000001000000000000"
"0000000000000000000010000000010001001100010011000100010001000100000000000000000000000000110001001100010000010000000100100100100010001000101010100111011100010000000100000001000000010100000000000000000000010100000101000000000000010100000101000001000000000000"
"0011100000011000000000000000100000000000000000000100010001000100000100000000000000000000010010000100101000000000001001000010010000100010010101011101101100010000000100001111000000010100000000001111000011110100000101001111110011110100000101001111000000000000"
"0000010000001000001111000100001001111100011000100011111000111000001000000111111001111110111101101111011000010000010010000001001010001000101010101110111000010000000100000001000000010100000000000001000000000100000101000000010000000100000101000001000000000000"
"0011110000001000010000100100001001000010010100100000000000000000010000000100000000000010001010010010101000010000001001000010010000100010010101011101101100010000111100001111000011110100111111001111000011110100000101001111010011111100111111001111000011110000"
"0100010000001000010000100100001001000010010010100111111001111100010000100100000000000010010000110101111100010000000100100100100010001000101010100111011100010000000100000001000000010100000101000001000000010100000101000001010000000000000000000000000000010000"
"0011111000011100001111000011110001000010010001100000000000000000001111000000000000000000100011001000001000010000000000000000000000100010010101011101101100010000000100000001000000010100000101000001000000010100000101000001010000000000000000000000000000010000"
"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111110000001000000000000000000000000010001000101010101110111000010000000100000001000000010100000101000001000000010100000101000001010000000000000000000000000000010000"
"0001000000010000000000000001000000000000000100000001000000010100000101000000000000010100000000000001010000000000000101000001000000010100000000000000000000010100000100000000000000000000000101000001000000010000000000001111111100000000111100000000111111111111"
"0001000000010000000000000001000000000000000100000001000000010100000101000000000000010100000000000001010000000000000101000001000000010100000000000000000000010100000100000000000000000000000101000001000000010000000000001111111100000000111100000000111111111111"
"0001000000010000000000000001000000000000000100000001111100010100000101110001111111110111111111110001011111111111111101111111111100010100111111110000000000010100000111110001111100000000000101001111111100010000000000001111111100000000111100000000111111111111"
"0001000000010000000000000001000000000000000100000001000000010100000100000001000000000000000000000001000000000000000000000000000000010100000000000000000000010100000100000001000000000000000101000001000000010000000000001111111100000000111100000000111111111111"
"0001111111111111111111110001111111111111111111110001111100010111000111110001011111111111111101110001011111111111111101111111111111111111111111111111111100011111000111110001111100011111111111111111111111110000000111111111111111111111111100000000111100000000"
"0000000000000000000100000001000000000000000100000001000000010100000000000001010000000000000101000001010000000000000101000000000000000000000100000001010000000000000000000001000000010100000101000001000000000000000100001111111111111111111100000000111100000000"
"0000000000000000000100000001000000000000000100000001000000010100000000000001010000000000000101000001010000000000000101000000000000000000000100000001010000000000000000000001000000010100000101000001000000000000000100001111111111111111111100000000111100000000"
"0000000000000000000100000001000000000000000100000001000000010100000000000001010000000000000101000001010000000000000101000000000000000000000100000001010000000000000000000001000000010100000101000001000000000000000100001111111111111111111100000000111100000000"
"0000000000000000000000000000000001111110000000000000000000000000011111000001100000011000000111000000000000000010000011000011110000000000000100000001000000001000000011000001000000011000000000000011000000000000000000000000111101111000001100000000000000000000"
"0000000000111100011111100011111101000010000000000100010000110011000100000010010000100100001000000110001000000100000100000100001001111110000100000000100000010000000100100001000000011000001100100100100000000000000000000000100001000100010010000000000000000000"
"0011000101000010010000100101010000100000001111100100010001001100001110000100001001000010000110001001010100111100001000000100001000000000011111000000010000100000000100100001000000000000010011000100100000000000000000000000100001000100000100000011110000000000"
"0100101001111100010000000001010000011000010010000100010000001000010001000111111001000010001111001000100101001010001111000100001001111110000100000000100000010000000100000001000001111110000000000011000000011000000000000000100001000100001000000011110000000000"
"0100010001000010010000000001010000100000010010000111101000001000010001000100001000100100010000101001010101010010001000000100001000000000000100000001000000001000000100000001000000000000001100100000000000011000000110000000100001000100011110000011110000000000"
"0100101001111100010000000001010001000010010010000100000000001000001110000010010000100100010000100110001000111100000100000100001001111110000000000000000000000000000100001001000000011000010011000000000000000000000000001100100000000000000000000011110000000000"
"0011000101000000010000000001010001111110001100000100000000001000000100000001100001100110001111000000000001000000000011000100001000000000011111000111111001111110000100001001000000011000000000000000000000000000000000000010100000000000000000000000000000000000"
"0000000001000000000000000000000000000000000000001000000000000000011111000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000100000110000000000000000000000000000000000000000000000001100000000000000000000000000000000000"

};
}
#endif