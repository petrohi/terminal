/****************************************************************************************
    vga.h

	VGA implementation for the VT100 Terminal program


	Copyright (C) 2014-2019
	Geoff Graham (projects@geoffg.net) and Peter Hizalev (peter.hizalev@gmail.com)
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
       and Peter Hizalev (peter.hizalev@gmail.com)

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


#define VIDEO_LINES          525
#define VIDEO_V_SYNC         2
#define VIDEO_V_FRONT_PORCH  10
#define VIDEO_V_BACK_PORCH   33

#define VIDEO_PIXELS         800
#define VIDEO_H_SYNC         96
#define VIDEO_H_FRONT_PORCH  16
#define VIDEO_H_BACK_PORCH   48

#define VIDEO_LINE_T   	     (VIDEO_PIXELS * 2)
#define VIDEO_H_SYNC_T       (VIDEO_H_SYNC * 2)

#define VIDEO_V_PIXELS (VIDEO_LINES - VIDEO_V_FRONT_PORCH - VIDEO_V_SYNC - VIDEO_V_BACK_PORCH)
#define VIDEO_H_PIXELS (VIDEO_PIXELS - VIDEO_H_FRONT_PORCH - VIDEO_H_SYNC - VIDEO_H_BACK_PORCH)

#define CHAR_HEIGHT    16
#define CHAR_WIDTH     8

#define SCREEN_COLS    (VIDEO_H_PIXELS / CHAR_WIDTH)

#define TAB_SIZE       8 

extern int UnderlineChar;
extern int ReverseVideoChar;
extern int InvisibleChar;

extern int CursorRow;
extern int CursorCol;
extern int CursorOff;

extern int AutoLineWrap;

extern int ScreenRows();
extern void TestFont();
extern void ShowCursor(int cursor);
extern void MoveCursor(int row, int col);
extern void ClearEOL();
extern void ClearEOS();
extern void ClearBOL();
extern void ClearBOS();
extern void ClearScreen();
extern void ScrollUp();
extern void ScrollDown();

extern void PutChar(int c);

extern void InitVga(int marginLines);

extern void SetPixel(int x, int y);
extern void DrawLine(int x1, int y1, int x2, int y2);
extern void DrawBox(int x1, int y1, int x2, int y2, int fill);
extern void DrawCircle(int x, int y, int radius, int fill, float aspect);