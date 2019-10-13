/****************************************************************************************
 Video.h

	Header file for the video driver for the VT100 Terminal program


	Copyright (C) 2014 Geoff Graham (projects@geoffg.net)
	All rights reserved.

	This file and the program created from it are FREE FOR COMMERCIAL AND
	NON-COMMERCIAL USE as long as the following conditions are aheared to.

	Copyright remains Geoff Graham's, and as such any Copyright notices in the
	code are not to be removed.  If this code is used in a product, Geoff Graham
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
****************************************************************************************/

// Global variables provided by Video.c
extern int VRes, HRes;						    // Global vert and horiz resolution in pixels on the screen
extern int screenWidth, screenHeight;				    // Global vert and horiz resolution in the currently selected font
extern int VBuf, HBuf;						    // Global vert and horiz resolution of the video buffer
extern int CursorX, CursorY;
extern int CharPosX, CharPosY;
extern int MMCharPos;
extern int vga;							    // true if we are using the internal monochrome vga

extern int VideoBuf[];						    // video buffer

#define VCHARS (VRes / (fontHeight * fontScale))                    // number of lines in a screenfull

// Facilities provided by Video.c
extern void MMCursor(int b);
extern void ClearScreen(void);
extern void initVideo(void);
extern void initFont(int);
extern void plot(int x, int y, int b);
extern int pixel(int x, int y);
extern void DrawLine(int x1, int y1, int x2, int y2, int colour) ;
extern void DrawCircle(int x, int y, int radius, int fill, int colour, float aspect) ;
extern void DrawBox(int x1, int y1, int x2, int y2, int fill, int colour);
extern void VideoPutc(char c);
extern void DisplayString(char *p);
extern void ScrollUp(void);
extern void ScrollDown(void);
extern void DrawChar0(int x, int y, int c);
extern void ShowCursor(int show);
extern void ConfigBuffers(int lines24);
extern int AutoLineWrap;                                            // true if auto line wrap is on
extern int Display24Lines;                                          // true if displaying 24 lines

#define CONFIG_PAL		0b111                               // PAL is the default for the Maximite
#define CONFIG_NTSC		0b001
#define CONFIG_DISABLED         0b010
extern const unsigned int PalVgaOption;

// define the blink rate for the cursor
extern int CursorOff;
extern volatile int CursorTimer;
#define CURSOR_OFF		350											// cursor off time in mS
#define CURSOR_ON		650											// cursor on time in mS

// cursor definition
typedef enum {C_OFF = 0, C_STANDARD, C_INSERT } Cursor_e ;
extern Cursor_e Cursor;
extern int AutoLineWrap;		// true if auto line wrap is on
extern int PrintPixelMode;      // global used to track the pixel mode when we are printing

// the font table and the current height and width of the font
extern int fontNbr;

// details of the current font
extern unsigned int *fontPtr;                                              // pointer to a font using 32 bit words
extern int fontWidth;
extern int fontHeight;
extern int fontStart;
extern int fontEnd;
extern int fontScale;


