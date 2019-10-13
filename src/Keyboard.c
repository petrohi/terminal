/****************************************************************************************
	Keyboard.c

	Keyboard processing for the VT100 Terminal program


	Copyright (C) 2014 Geoff Graham (projects@geoffg.net)
	All rights reserved.

	This file and the program created from it are FREE FOR COMMERCIAL AND
	NON-COMMERCIAL USE as long as the following conditions are aheared to.

	Copyright remains Geoff Graham's, and as such any Copyright notices in the
	code are not to be removed.  If this code is used in a product,  Geoff Graham
	should be given attribution as the author of the parts used.  This can be in
	the form of a textual message at program startup or in documentation (online
	or textual) provided with the program or product.

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions  are met:
	1. Redistributions of source code must retain the copyright notice, this list
	   of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice, this
	   list of conditions and the following disclaimer in the documentation and/or
	   other materials provided with the distribution.
	3. All advertising materials mentioning features or use of this software must
	   display the following acknowledgement:
	   This product includes software developed by Geoff Graham (projects@geoffg.net)

	THIS SOFTWARE IS PROVIDED BY GEOFF GRAHAM ``AS IS'' AND  ANY EXPRESS OR IMPLIED
	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT
	SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
	IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE.

	The licence and distribution terms for any publically available version or
	derivative of this code cannot be changed.  i.e. this code cannot simply be copied
	and put under another distribution licence (including the GNU Public Licence).

****************************************************************************************************************************

This routine is based on a technique and code presented by Lucio Di Jasio in his excellent book
"Programming 32-bit Microcontrollers in C - Exploring the PIC32".

Thanks to Muller Fabrice (France), Alberto Leibovich (Argentina) and the other contributors who provided the code for
the non US keyboard layouts

****************************************************************************************************************************/

#define INCLUDE_FUNCTION_DEFINES

#include <p32xxxx.h>
#include <plib.h>
#include "Terminal.h"

void setLEDs(int num, int caps, int scroll);
volatile char CapsLock;
volatile char NumLock;

const unsigned int KeyboardOption = 0xffffffff;						// used to store the keyboard layout output

// definition of the keyboard PS/2 state machine
#define PS2START    0
#define PS2BIT      1
#define PS2PARITY   2
#define PS2STOP     3

// PS2 KBD state machine and buffer
int PS2State;
unsigned char KBDBuf;
int KState, KCount, KParity;

volatile int KeyDown = -1;

// key codes that must be tracked for up/down state
#define CTRL  		0x14											// left and right generate the same code
#define L_SHFT  	0x12
#define R_SHFT  	0x59
#define CAPS    	0x58
#define NUML    	0x77

// this is a map of the keycode characters and the character to be returned for the keycode
const char keyCodes[7][128]=
            // US Layout
         {
             {                                                                          //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,     '`',       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'q',     '1',       0,       //16-23    10-07
                0,       0,     'z',     's',     'a',     'w',     '2',       0,       //24-31    18-1F
                0,     'c',     'x',     'd',     'e',     '4',     '3',       0,       //32-39    20-27
                0,     ' ',     'v',     'f',     't',     'r',     '5',       0,       //40-48    28-2F
                0,     'n',     'b',     'h',     'g',     'y',     '6',       0,       //48-56    30-37
                0,       0,     'm',     'j',     'u',     '7',     '8',       0,       //56-63    38-3F
                0,     ',',     'k',     'i',     'o',     '0',     '9',       0,       //64-71    40-47
                0,     '.',     '/',     'l',     ';',     'p',     '-',       0,       //72-79    48-4F
                0,       0,    '\'',       0,     '[',     '=',       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     ']',      0,     0x5c,       0,       0,       //88-95    58-5F
                0,       0,       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // FR Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,       0,       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'a',     '&',       0,       //16-23    10-07
                0,       0,     'w',     's',     'q',     'z',       0,       0,       //24-31    18-1F
                0,     'c',     'x',     'd',     'e',     '\'',    '"',       0,       //32-39    20-27
                0,     ' ',     'v',     'f',     't',     'r',     '(',       0,       //40-48    28-2F
                0,     'n',     'b',     'h',     'g',     'y',     '-',       0,       //48-56    30-37
                0,       0,     ',',     'j',     'u',       0,     '_',       0,       //56-63    38-3F
                0,     ';',     'k',     'i',     'o',       0,       0,       0,       //64-71    40-47
                0,     ':',     '!',     'l',     'm',     'p',     ')',       0,       //72-79    48-4F
                0,       0,       0,       0,     '^',     '=',       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '$',      0,      '*',       0,       0,       //88-95    58-5F
                0,     '<',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // GR Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,     '^',       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'q',     '1',       0,       //16-23    10-07
                0,       0,     'y',     's',     'a',     'w',     '2',       0,       //24-31    18-1F
                0,     'c',     'x',     'd',     'e',     '4',     '3',       0,       //32-39    20-27
                0,     ' ',     'v',     'f',     't',     'r',     '5',       0,       //40-47    28-2F
                0,     'n',     'b',     'h',     'g',     'z',     '6',       0,       //48-55    30-37
                0,       0,     'm',     'j',     'u',     '7',     '8',       0,       //56-63    38-3F
                0,     ',',     'k',     'i',     'o',     '0',     '9',       0,       //64-71    40-47
                0,     '.',     '-',     'l',       0,     'p',       0,       0,       //72-79    48-4F
                0,       0,       0,       0,       0,     '\'',      0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '+',      0,      '#',       0,       0,       //88-95    58-5F
                0,     '<',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // IT Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,    0x5c,       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'q',     '1',       0,       //16-23    10-07
                0,       0,     'z',     's',     'a',     'w',     '2',       0,       //24-31    18-1F
                0,     'c',     'x',     'd',     'e',     '4',     '3',       0,       //32-39    20-27
                0,     ' ',     'v',     'f',     't',     'r',     '5',       0,       //40-48    28-2F
                0,     'n',     'b',     'h',     'g',     'y',     '6',       0,       //48-56    30-37
                0,       0,     'm',     'j',     'u',     '7',     '8',       0,       //56-63    38-3F
                0,     ',',     'k',     'i',     'o',     '0',     '9',       0,       //64-71    40-47
                0,     '.',     '-',     'l',       0,     'p',    '\'',       0,       //72-79    48-4F
                0,       0,       0,       0,       0,       0,       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '+',       0,       0,       0,       0,       //88-95    58-5F
                0,     '<',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // BE Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,       0,       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'a',     '&',       0,       //16-23    10-07
                0,       0,     'w',     's',     'q',     'z',       0,       0,       //24-31    18-1F
                0,     'c',     'x',     'd',     'e',     '\'',    '"',       0,       //32-39    20-27
                0,     ' ',     'v',     'f',     't',     'r',     '(',       0,       //40-48    28-2F
                0,     'n',     'b',     'h',     'g',     'y',       0,       0,       //48-56    30-37
                0,       0,     ',',     'j',     'u',       0,     '!',       0,       //56-63    38-3F
                0,     ';',     'k',     'i',     'o',       0,       0,       0,       //64-71    40-47
                0,     ':',     '=',     'l',     'm',     'p',     ')',       0,       //72-79    48-4F
                0,       0,       0,       0,     '^',     '-',       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '$',       0,       0,       0,       0,       //88-95    58-5F
                0,     '<',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // UK Layout
             {                                                                          //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,     '`',       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,    0x5C,    CTRL,     'q',     '1',       0,       //16-23    10-07
                0,       0,     'z',     's',     'a',     'w',     '2',       0,       //24-31    18-1F
                0,     'c',     'x',     'd',     'e',     '4',     '3',       0,       //32-39    20-27
                0,     ' ',     'v',     'f',     't',     'r',     '5',       0,       //40-48    28-2F
                0,     'n',     'b',     'h',     'g',     'y',     '6',       0,       //48-56    30-37
                0,       0,     'm',     'j',     'u',     '7',     '8',       0,       //56-63    38-3F
                0,     ',',     'k',     'i',     'o',     '0',     '9',       0,       //64-71    40-47
                0,     '.',     '/',     'l',     ';',     'p',     '-',       0,       //72-79    48-4F
                0,       0,    '\'',       0,     '[',     '=',    0x5C,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     ']',     '#',     '#',       0,       0,       //88-95    58-5F
                0,    0x5C,       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
		  // ES Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,    0x5c,       0,       //08-15	   08-0F	  0x5C is backslash
                0,     ALT,  L_SHFT,      0,     CTRL,     'q',     '1',       0,       //16-23    10-07
                0,       0,     'z',     's',     'a',     'w',     '2',       0,       //24-31    18-1F
                0,     'c',     'x',     'd',     'e',     '4',     '3',       0,       //32-39    20-27
                0,     ' ',     'v',     'f',     't',     'r',     '5',       0,       //40-47    28-2F
                0,     'n',     'b',     'h',     'g',     'y',     '6',       0,       //48-55    30-37
                0,       0,     'm',     'j',     'u',     '7',     '8',       0,       //56-63    38-3F
                0,     ',',     'k',     'i',     'o',     '0',     '9',       0,       //64-71    40-47
                0,     '.',     '-',     'l',       0,     'p',    '\'',       0,       //72-79	   48-4F
                0,       0,    '\'',       0,    0x60,       0,       0,       0,       //80-87    50-57	  0x60 a single quote
             CAPS,  R_SHFT,   ENTER,     '+',       0,       0,       0,       0,       //88-95    58-5F
                0,     '<',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            }
        };

// this map is with the shift key pressed
const char keySCodes[7][128] =
            // US Layout
        {
            {                                                                           //Base 10   Hex
                0,       F9,       0,      F5,      F3,      F1,      F2,     F12,      //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,     '~',       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'Q',     '!',       0,       //16-23    10-07
                0,       0,     'Z',     'S',     'A',     'W',     '@',       0,       //24-31    18-1F
                0,     'C',     'X',     'D',     'E',     '$',     '#',       0,       //32-39    20-27
                0,     ' ',     'V',     'F',     'T',     'R',     '%',       0,       //40-47    28-2F
                0,     'N',     'B',     'H',     'G',     'Y',     '^',       0,       //48-55    30-37
                0,       0,     'M',     'J',     'U',     '&',     '*',       0,       //56-63    38-3F
                0,     '<',     'K',     'I',     'O',     ')',     '(',       0,       //64-71    40-47
                0,     '>',     '?',     'L',     ':',     'P',     '_',       0,       //72-79    48-4F
                0,       0,    '\"',       0,     '{',     '+',       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '}',       0,     '|',       0,       0,       //88-95    58-5F
                0,       0,       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
               '0',    '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // FR Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,       0,       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'A',     '1',       0,       //16-23    10-07
                0,       0,     'W',     'S',     'Q',     'Z',     '2',       0,       //24-31    18-1F
                0,     'C',     'X',     'D',     'E',     '4',     '3',       0,       //32-39    20-27
                0,     ' ',     'V',     'F',     'T',     'R',     '5',       0,       //40-47    28-2F
                0,     'N',     'B',     'H',     'G',     'Y',     '6',       0,       //48-55    30-37
                0,       0,     '?',     'J',     'U',     '7',     '8',       0,       //56-63    38-3F
                0,     '.',     'K',     'I',     'O',     '0',     '9',       0,       //64-71    40-47
                0,     '/',       0,     'L',     'M',     'P',       0,       0,       //72-79    48-4F
                0,       0,     '%',       0,       0,     '+',       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,       0,       0,       0,       0,       0,       //88-95    58-5F
                0,     '>',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
               '0',    '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // GR Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,       0,       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'Q',     '!',       0,       //16-23    10-07
                0,       0,     'Y',     'S',     'A',     'W',     '\"',      0,       //24-31    18-1F
                0,     'C',     'X',     'D',     'E',     '$',       0,       0,       //32-39    20-27
                0,     ' ',     'V',     'F',     'T',     'R',     '%',       0,       //40-47    28-2F
                0,     'N',     'B',     'H',     'G',     'Z',     '&',       0,       //48-55    30-37
                0,       0,     'M',     'J',     'U',     '/',     '(',       0,       //56-63    38-3F
                0,     ';',     'K',     'I',     'O',     '=',     ')',       0,       //64-71    40-47
                0,     ':',     '_',     'L',       0,     'P',     '?',       0,       //72-79    48-4F
                0,       0,       0,       0,       0,       0,       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '*',       0,       0,       0,       0,       //88-95    58-5F
                0,     '>',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
               '0',    '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // IT Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,     '|',       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'Q',     '!',       0,       //16-23    10-07
                0,       0,     'Z',     'S',     'A',     'W',    '\"',       0,       //24-31    18-1F
                0,     'C',     'X',     'D',     'E',     '$',       0,       0,       //32-39    20-27
                0,     ' ',     'V',     'F',     'T',     'R',     '%',       0,       //40-47    28-2F
                0,     'N',     'B',     'H',     'G',     'Y',     '&',       0,       //48-55    30-37
                0,       0,     'M',     'J',     'U',     '/',     '(',       0,       //56-63    38-3F
                0,     ';',     'K',     'I',     'O',     '=',     ')',       0,       //64-71    40-47
                0,     ':',     '_',     'L',       0,     'P',     '?',       0,       //72-79    48-4F
                0,       0,       0,       0,       0,     '^',       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '*',       0,       0,       0,       0,       //88-95    58-5F
                0,     '>',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
               '0',    '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // BE Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,       0,       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,      0,     CTRL,     'A',     '1',       0,       //16-23    10-07
                0,       0,     'W',     'S',     'Q',     'Z',     '2',       0,       //24-31    18-1F
                0,     'C',     'X',     'D',     'E',     '4',     '3',       0,       //32-39    20-27
                0,     ' ',     'V',     'F',     'T',     'R',     '5',       0,       //40-47    28-2F
                0,     'N',     'B',     'H',     'G',     'Y',     '6',       0,       //48-55    30-37
                0,       0,     '?',     'J',     'U',     '7',     '8',       0,       //56-63    38-3F
                0,     '.',     'K',     'I',     'O',     '0',     '9',       0,       //64-71    40-47
                0,     '/',     '+',     'L',     'M',     'P',       0,       0,       //72-79    48-4F
                0,       0,     '%',       0,       0,     '_',       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '*',       0,       0,       0,       0,       //88-95    58-5F
                0,     '>',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
               '0',    '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
            // UK Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,     '~',       0,       //08-15    08-0F
                0,     ALT,  L_SHFT,     '|',    CTRL,     'Q',     '!',       0,       //16-23    10-07
                0,       0,     'Z',     'S',     'A',     'W',     '\"',      0,       //24-31    18-1F
                0,     'C',     'X',     'D',     'E',     '$',     '#',       0,       //32-39    20-27
                0,     ' ',     'V',     'F',     'T',     'R',     '%',       0,       //40-47    28-2F
                0,     'N',     'B',     'H',     'G',     'Y',     '^',       0,       //48-55    30-37
                0,       0,     'M',     'J',     'U',     '&',     '*',       0,       //56-63    38-3F
                0,     '<',     'K',     'I',     'O',     ')',     '(',       0,       //64-71    40-47
                0,     '>',     '?',     'L',     ':',     'P',     '_',       0,       //72-79    48-4F
                0,       0,     '@',       0,     '{',     '+',     '|',       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '}',     '~',     '~',       0,       0,       //88-95    58-5F
                0,     '|',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
               '0',    '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            },
		    // ES Layout
            {                                                                           //Base 10   Hex
                0,      F9,       0,      F5,      F3,      F1,      F2,     F12,       //00-07    00-07
                0,     F10,      F8,      F6,      F4,     TAB,    0x5C,       0,       //08-15    08-0F	 0x5C is backslash
                0,     ALT,  L_SHFT,      0,     CTRL,     'Q',     '!',       0,       //16-23    10-07
                0,       0,     'Z',     'S',     'A',     'W',    '\"',       0,       //24-31    18-1F
                0,     'C',     'X',     'D',     'E',     '$',       0,       0,       //32-39    20-27
                0,     ' ',     'V',     'F',     'T',     'R',     '%',       0,       //40-47    28-2F
                0,     'N',     'B',     'H',     'G',     'Y',     '&',       0,       //48-55    30-37
                0,       0,     'M',     'J',     'U',     '/',     '(',       0,       //56-63    38-3F
                0,     ';',     'K',     'I',     'O',     '=',     ')',       0,       //64-71    40-47
                0,     ':',     '_',     'L',       0,     'P',     '?',       0,       //72-79	   48-4F
                0,       0,       0,       0,     '^',       0,       0,       0,       //80-87    50-57
             CAPS,  R_SHFT,   ENTER,     '*',       0,       0,       0,       0,       //88-95    58-5F
                0,     '>',       0,       0,       0,       0,    BKSP,       0,       //96-103   60-67
                0,     '1',       0,     '4',     '7',       0,       0,       0,       //104-111  68-6F
              '0',     '.',     '2',     '5',     '6',     '8',     ESC,       NUML,    //112-119  70-77
              F11,     '+',     '3',     '-',     '*',     '9',       0,       0        //120-127  78-7F
            }
        };

// this map is for when the keycode preceeded by 0xe0
const char keyE0Codes[56] =
            // General Layout on all Keyboard for the Keypad
            {                                                                           //Base 10
                0,     END,       0,    LEFT,    HOME,       0,       0,       0,    	//104-111
           INSERT,     DEL,    DOWN,       0,   RIGHT,      UP,     ESC,    NUML,    	//112-119
              F11,     '+',   PDOWN,     '-',     '*',     PUP,   SLOCK,       0,    	//120-127
            };

const char keyE0Codes_ES[56] =
            // Layout for spanish Keyboard for the Keypad
            {                                                                           //Base 10
                0,     END,       0,    LEFT,    HOME,       0,       0,       0,    	//104-111
           INSERT,     DEL,    DOWN,     PUP,   RIGHT,      UP,     ESC,    NUML,    	//112-119
              F11,       0,   PDOWN,       0,       0,       0,   SLOCK,       0,    	//120-127
            };


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Hardware definitions for the keyboard
// If you change the I/O pis allocated to the keybord you should edit these defines and the code in initKeyboard().
// The rest of the keyboard driver is independent of the physical I/O pins used.
#define P_PS2CLK			PORTBbits.RB8			// Keyboard clock
#define P_PS2CLK_TRIS       TRISBbits.TRISB8        // tris
#define P_PS2CLK_OUT        LATBbits.LATB8          // output latch for the clock

// for the clock we use INT3, this is set in initKeyboard().
#define P_PS2_INTERRUPT(a)  mINT3IntEnable(a)       // ensble/disable they keyboard interrupt

#define P_PS2DAT			PORTBbits.RB7			// Keyboard data
#define P_PS2DAT_TRIS       TRISBbits.TRISB7        // tris
#define P_PS2DAT_OUT        LATBbits.LATB7          // output latch for the data



/***************************************************************************************************
initKeyboard
Initialise the keyboard.
****************************************************************************************************/
void initKeyboard(void) {

    P_PS2CLK_TRIS = 1; P_PS2DAT_TRIS = 1;                           // make sure out inputs are in fact inputs

    PPSInput(2, INT3, RPB8);
    ConfigINT3(EXT_INT_PRI_2 | FALLING_EDGE_INT | EXT_INT_ENABLE);

    // initialise variabls
    PS2State = PS2START;
    CapsLock = 0;  NumLock = 1;
    KeyDown = -1;

    // set the LEDs on the keyboard
    setLEDs(CapsLock, NumLock, 0);
}



/***************************************************************************************************
sendCommand - Send a command to to keyboard.
 Note that when we want to signal a logic high we turn the pin into an input which means that the
 pullup will ensure a high.  When we want to signal a logic low we turn the pin into an output and
 because we have loaded the output latch with a zero the output will be pulled low.
****************************************************************************************************/
void sendCommand(int cmd) {
    int i, j;

    // calculate the parity and add to the command as the 9th bit
    for(j = i = 0; i < 8; i++) j += ((cmd >> i) & 1);
    cmd = (cmd & 0xff) | (((j + 1) & 1) << 8);

 	P_PS2CLK_OUT = 0;                                               // when configured as an output the clock line will be low
 	P_PS2DAT_OUT = 0;                                               // same for data
 	P_PS2CLK_TRIS = 0;                                              // clock low
 	uSec(150);
 	P_PS2DAT_TRIS = 0;                                              // data low
 	uSec(2);
 	P_PS2CLK_TRIS = 1;                                              // release the clock (goes high)
 	uSec(2);

 	GeneralTimer = 500;                                             // timeout of 500mS
 	while(P_PS2CLK) if(GeneralTimer == 0) return;                   // wait for the keyboard to pull the clock low

 	// send each bit including parity
 	for(i = 0; i < 9; i++) {
        P_PS2DAT_TRIS = (cmd & 1);                                  // set the data bit
     	while(!P_PS2CLK) if(GeneralTimer == 0) return;              // wait for the keyboard to bring the clock high
     	while(P_PS2CLK)  if(GeneralTimer == 0) return;              // wait for clock low
     	cmd >>= 1;
    }

    P_PS2DAT_TRIS = 1;                                              // release the data line
    while(P_PS2DAT) if(GeneralTimer == 0) return;                   // wait for the keyboard to pull data low (ACK)
 	while(P_PS2CLK) if(GeneralTimer == 0) return;                   // wait for the clock to go low
 	while(!P_PS2CLK || !P_PS2DAT) if(GeneralTimer == 0) return;     // finally wait for both the clock and data to go high (idle state)
}


// set the keyboard LEDs
void setLEDs(int caps, int num, int scroll) {
    P_PS2_INTERRUPT(false);       								    // disable interrupt while we play
    sendCommand(0xED);                                              // Set/Reset Status Indicators Command
    uSec(50000);
    sendCommand(((caps & 1) << 2) | ((num & 1) << 1) | (scroll & 1));// set the various LEDs
    P_PS2CLK_TRIS = 1; P_PS2DAT_TRIS = 1;                           // reset the data & clock to inputs in case a keyboard was not plugged in
    uSec(5000);
    P_PS2_INTERRUPT(true);       								    // re enable interrupt
}



/***************************************************************************************************
change notification interrupt service routine
****************************************************************************************************/
void __ISR( _EXTERNAL_3_VECTOR , ipl2) INT3Interrupt(void) {

    int d;
	unsigned char c = 0;

	static char LShift = 0;
	static char RShift = 0;
	static char Ctrl = 0;
	static char AltGrDown = 0;
	static char KeyUpCode = false;
	static char KeyE0 = false;
	static unsigned char Code = 0;

     // Make sure it was a falling edge
    if(P_PS2CLK == 0)
    {
	    // Sample the data
	    d = P_PS2DAT;
        switch(PS2State){
            default:
            case PS2START:
                if(!d) {                							// PS2DAT == 0
                    KCount = 8;         							// init bit counter
                    KParity = 0;        							// init parity check
                    Code = 0;
                    PS2State = PS2BIT;
                }
                break;

            case PS2BIT:
                Code >>= 1;            								// shift in data bit
                if(d) Code |= 0x80;                					// PS2DAT == 1
                KParity ^= Code;      								// calculate parity
                if (--KCount <= 0) PS2State = PS2PARITY;   			// all bit read
                break;

            case PS2PARITY:
                if(d) KParity ^= 0x80;                				// PS2DAT == 1
                if (KParity & 0x80)    								// parity odd, continue
                    PS2State = PS2STOP;
                else
                    PS2State = PS2START;
                break;

            case PS2STOP:
                if(d) {                 							// PS2DAT == 1
                    //dp("%X", Code);
	                if(Code == 0xaa)                                // self test code (a keyboard must have just been plugged in)
	                    setLEDs(CapsLock, NumLock, 0);              // so initialise it
	                else if(Code == 0xf0)                           // a key has been released
	                	KeyUpCode = true;
	                else if(Code == 0xe0)                           // extended keycode prefix
	                	KeyE0 = true;
	                else {
		                // Process a scan code from the keyboard into an ASCII character.
		                // It then inserts the char into the keyboard queue.

    					// for the US keyboard and the right Alt key we need to make it the same as the left Alt key
    					if(Option[O_KEYBOARD] == O_KEYBOARD_US && KeyE0 && Code == 0x11) KeyE0 = false;

                        // if a key has been released we are only interested in resetting state keys
                        if(KeyUpCode) {
                            if(Code == L_SHFT) LShift = 0;                      // left shift button is released
                            else if(Code == R_SHFT) RShift = 0;                 // right shift button is released
                            else if(Code == CTRL) Ctrl = 0;			            // left control button is released
                            else if(KeyE0 && Code == 0x11) AltGrDown = 0;       // release the AltGr key on non US keyboards
                            //else if(Code == KeyDownCode) KeyDown = 0;           // normal char so record that it is no longer depressed
                            goto SkipOut;

                        }

                        // we are only here if the key has been pressed (NOT released)
                        if(Code == L_SHFT) { LShift = 1; goto SkipOut; }        // left shift button is pressed
                        if(Code == R_SHFT) { RShift = 1; goto SkipOut; }		// right shift button is pressed
                        if(Code == CTRL) { Ctrl = 1; goto SkipOut; }			// left control button is pressed
                        if(Code == CAPS) {                                      // caps or num lock pressed
                            setLEDs(CapsLock = !CapsLock, NumLock, 0);
                            goto SkipOut;
                        }
                        if(Code == NUML) {                                      // caps or num lock pressed
                            setLEDs(CapsLock, NumLock = !NumLock, 0);
                            goto SkipOut;
                        }
                        if(KeyE0 && Code == 0x11) {AltGrDown = 1; goto SkipOut;}// AltGr key pressed on non US Keyboard

                        // now get the character into c.  Why, oh why, are scan codes so random?
                        if(!KeyE0 && Code == 0x83)
                            c = 0x97;										    // a special case, function key F7
                        else if(KeyE0 && Code == 0x4A)
                            c = '/';                                            // another special case, this time the keypad forward slash
                        else if(KeyE0 && Code == 0x5A)
                            c = NUM_ENT;                                        // yet another special case, this time the keypad enter key
                        else if((KeyE0 || !NumLock) && Code >= 104 && Code < 0x80 && !AltGrDown) {// a keycode from the numeric keypad
                            LShift = 0;                                         // when num lock LED is on codes are preceeded by left shift
                            if(Option[O_KEYBOARD] == O_KEYBOARD_ES)
						        c = keyE0Codes_ES[Code - 104];
						    else
						        c = keyE0Codes[Code - 104];
					    }
                        else if((Code >= 0x15 && Code < 0x62) && AltGrDown != 0) // a keycode preceeded by Alt-Gr
                          switch(Option[O_KEYBOARD])
                          {                                  			// an international keycode pressed with
                            case O_KEYBOARD_US:                          	// the AltGr key
                              break;                        			// no code for US keyboard
                            case O_KEYBOARD_FR:                          	// French Keyboard
                              switch (Code)
                              {
                                case 0x45:
                                  c = 0x40;       // @
                                  AltGrDown = 0;
                                  break;
                                case 0x25:
                                  c = 0x7b;       // {
                                  AltGrDown = 0;
                                  break;
                                case 0x2e:
                                  c = 0x5b;       // [
                                  AltGrDown = 0;
                                  break;
                                case 0x55:
                                  c = 0x7d;       // }
                                  AltGrDown = 0;
                                  break;
                                case 0x4e:
                                  c = 0x5d;       // ]
                                  AltGrDown = 0;
                                  break;
                                case 0x3e:
                                  c = 0x5c;       // '\'
                                  AltGrDown = 0;
                                  break;
                                case 0x1e:
                                  c = 0x7e;       // ~
                                  AltGrDown = 0;
                                  break;
                                case 0x36:
                                  c = 0x7c;       // |
                                  AltGrDown = 0;
                                  break;
                                case 0x26:
                                  c = 0x23;       // #
                                  AltGrDown = 0;
                                  break;
                                default:
                                  c = 0;		  // invalid code
                                  AltGrDown = 0;
                                  break;
                              }
                              break;
                            case O_KEYBOARD_GR:                          // German Keyboard
                              switch (Code)
                              {
                                case 0x15:
                                  c = 0x40;       // @
                                  AltGrDown = 0;
                                  break;
                                case 0x3d:
                                  c = 0x7b;       // {
                                  AltGrDown = 0;
                                  break;
                                case 0x3e:
                                  c = 0x5b;       // [
                                  AltGrDown = 0;
                                  break;
                                case 0x45:
                                  c = 0x7d;       // }
                                  AltGrDown = 0;
                                  break;
                                case 0x46:
                                  c = 0x5d;       // ]
                                  AltGrDown = 0;
                                  break;
                                case 0x4e:
                                  c = 0x5c;       // '\'
                                  AltGrDown = 0;
                                  break;
                                case 0x5b:
                                  c = 0x7e;       // ~
                                  AltGrDown = 0;
                                  break;
                                case 0x61:
                                  c = 0x7c;       // |
                                  AltGrDown = 0;
                                  break;
                                default:
                                  c = 0;		  // invalid code
                                  AltGrDown = 0;
                                  break;
                              }
                              break;
                            case O_KEYBOARD_IT:                          // Italian Keyboard
                              switch (Code)
                              {
                                case 0x4C:
                                  c = 0x40;       // @
                                  AltGrDown = 0;
                                  break;
                                case 0x54:
                                  c = 0x5b;       // [
                                  AltGrDown = 0;
                                  break;
                                case 0x5b:
                                  c = 0x5d;       // ]
                                  AltGrDown = 0;
                                  break;
                                case 0x52:
                                  c = 0x23;       // #
                                  AltGrDown = 0;
                                  break;
                                default:
                                  c = 0;		  // invalid code
                                  AltGrDown = 0;
                                  break;
                              }
                              break;
                            case O_KEYBOARD_BE:                          // Belgian Keyboard
                              switch (Code)
                              {
                                case 0x1e:
                                  c = 0x40;       // @
                                  AltGrDown = 0;
                                  break;
                                case 0x46:
                                  c = 0x7b;       // {
                                  AltGrDown = 0;
                                  break;
                                case 0x54:
                                  c = 0x5b;       // [
                                  AltGrDown = 0;
                                  break;
                                case 0x45:
                                  c = 0x7d;       // }
                                  AltGrDown = 0;
                                  break;
                                case 0x5b:
                                  c = 0x5d;       // ]
                                  AltGrDown = 0;
                                  break;
                                case 0x1a:
                                  c = 0x5c;       // '\'
                                  AltGrDown = 0;
                                  break;
                                case 0x4a:
                                  c = 0x7e;       // ~
                                  AltGrDown = 0;
                                  break;
                                case 0x16:
                                  c = 0x7c;       // |
                                  AltGrDown = 0;
                                  break;
                                case 0x26:
                                  c = 0x23;       // #
                                  AltGrDown = 0;
                                  break;
                                default:
                                  c = 0;		  // invalid code
                                  AltGrDown = 0;
                                  break;
                              }
                              break;
							case O_KEYBOARD_ES:                          // Spanish Keyboard
                              switch (Code)
                              {
                                case 0x1E:
                                  c = 0x40;       // @
                                  AltGrDown = 0;
                                  break;
                                case 0x54:
                                  c = 0x5b;       // [
                                  AltGrDown = 0;
                                  break;
                                case 0x5b:
                                  c = 0x5d;       // ]
                                  AltGrDown = 0;
                                  break;
                                case 0x26:
                                  c = 0x23;       // #
                                  AltGrDown = 0;
                                  break;
								case 0x52:
                                  c = 0x7b;       // {
                                  AltGrDown = 0;
                                  break;
								case 0x5d:
                                  c = 0x7d;       // }
                                  AltGrDown = 0;
                                  break;
								case 0x16:
                                  c = 0x7c;       // |
                                  AltGrDown = 0;
                                  break;
								case 0x0e:
                                  c = 0x5c;       // '\'
                                  AltGrDown = 0;
                                  break;
                                case 0x25:
                                  c = 0x7e;       // ~
                                  AltGrDown = 0;
                                  break;
                                default:
                                  c = 0;		  // invalid code
                                  AltGrDown = 0;
                                  break;
                              }
                              break;
                          }
                        else {
                            switch (Option[O_KEYBOARD])
                            {
                              case O_KEYBOARD_US:
                                if(LShift || RShift)
                                    c = keySCodes[0][Code%128];			// a keycode preceeded by a shift
                                else
                                    c = keyCodes[0][Code%128];			// just a keycode
                                break;
                              case O_KEYBOARD_FR:
                                if(LShift || RShift)
                                    c = keySCodes[1][Code%128];			// a keycode preceeded by a shift
                                else
                                    c = keyCodes[1][Code%128];			// just a keycode
                                break;
                              case O_KEYBOARD_GR:
                                if(LShift || RShift)
                                    c = keySCodes[2][Code%128];			// a keycode preceeded by a shift
                                else
                                    c = keyCodes[2][Code%128];			// just a keycode
                                break;
                              case O_KEYBOARD_IT:
                                if(LShift || RShift)
                                    c = keySCodes[3][Code%128];			// a keycode preceeded by a shift
                                else
                                    c = keyCodes[3][Code%128];			// just a keycode
                                break;
                              case O_KEYBOARD_BE:
                                if(LShift || RShift)
                                    c = keySCodes[4][Code%128];			// a keycode preceeded by a shift
                                else
                                    c = keyCodes[4][Code%128];			// just a keycode
                                break;
                              case O_KEYBOARD_UK:
                                if(LShift || RShift)
                                    c = keySCodes[5][Code%128];			// a keycode preceeded by a shift
                                else
                                    c = keyCodes[5][Code%128];			// just a keycode
                                break;
							  case O_KEYBOARD_ES:
                                if(LShift || RShift)
                                    c = keySCodes[6][Code%128];			// a keycode preceeded by a shift
                                else
                                    c = keyCodes[6][Code%128];			// just a keycode
                                break;
                            }
                        }

                        if(!c) goto SkipOut;

                        if(c <= 0x7F) {									// a normal character
                          if(CapsLock && c >= 'a' && c <= 'z') c -= 32;	// adj for caps lock
                          if(Ctrl) c &= 0x1F;							// adj for control
                        }
                        else	{										// must be a function key or similar
                          if(LShift || RShift) c |= 0b00100000;
                          if(Ctrl) c |= 0b01000000;
                        }

    				    KeyDown = c;

                        SkipOut:
                        // end lump of self contained code
                        //////////////////////////////////////////////////////////////////////////////////////////////////////////
	                	KeyUpCode = false;
	                	KeyE0 = false;
	                }
	            Code = 0;
                }
                PS2State = PS2START;
                break;
	    }
	}
    // clear interrupt flag
    mINT3ClearIntFlag();    										// Clear the interrupt flag

}
