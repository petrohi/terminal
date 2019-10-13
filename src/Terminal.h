/****************************************************************************************
	Tracker.h

	Include file for the VT100 Terminal program


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
****************************************************************************************/

#define VERSION                 "1.3"                               // define the version number
#define YEAR			"2014"			            // and the year


// The main clock frequency for the chip
#define	CLOCKFREQ		(40000000ul)			    // This is set in in Configuration Bits.h

// The peripheral bus frequency
#define BUSFREQ			(CLOCKFREQ/1)			    // This is set in in Configuration Bits.h

// grab as much memory as possible for video and Rx buffers
#define BUFFER_SIZE             (29820)                             // this should be the largest possible size allowed by the compiler/linker


// General defines
#define forever                 1
#define true                    1
#define false                   0

#define INPUT                   1
#define OUTPUT                  0

#define P_INPUT			1				    // for setting the TRIS on I/O bits
#define P_OUTPUT		0
#define P_ON			1
#define P_OFF			0


#define dp(...) {char s[140];sprintf(s,  __VA_ARGS__); VideoPrintString(s); VideoPrintString("\r\n");}
#define dpUSB(...) {char s[140];sprintf(s,  __VA_ARGS__); USBPrintString(s); USBPrintString("\r\n"); mT4IntEnable(0); CheckUSB(); mT4IntEnable(1);}
#define uSec(us) { unsigned int i = ((((unsigned int)(us) * 1000) - 600) / (2000000000/CLOCKFREQ)); WriteCoreTimer(0); while(ReadCoreTimer() < i); }


// writing to the flash
#define FLASH_WRITE_WORD          0x4001      // Write a word
#define FLASH_PAGE_ERASE       	  0x4004      // Page erase
void FlashWrite(void *p, unsigned int wrd, unsigned int operation);
int GetFlashOption(const unsigned int *w);
void SetFlashOption(const unsigned int *w, int x);

extern void putSerial(int);
extern void putSerialString(char *p);
extern void putUSB(char);


//#define LOCAL_ECHO  false

extern volatile int GeneralTimer;

extern char *SerialRxBuf;
extern int RxBufferSize;
extern volatile int SerialRxBufHead;
extern volatile int SerialRxBufTail;
extern int USBSerialRxBufTail;

// global keyboard functions, defines and variables
extern void initKeyboard(void);
extern volatile int KeyDown;

extern void setLEDs(int num, int caps, int scroll);
extern volatile char CapsLock;
extern volatile char NumLock;

// the values returned by the standard control keys
#define TAB     	0x9
#define BKSP    	0x8
#define ENTER   	0xd
#define ESC     	0x1b

// the values returned by the function keys
#define F1      	0x91
#define F2      	0x92
#define F3      	0x93
#define F4      	0x94
#define F5      	0x95
#define F6      	0x96
#define F7      	0x97
#define F8      	0x98
#define F9      	0x99
#define F10     	0x9a
#define F11     	0x9b
#define F12     	0x9c

// the values returned by special control keys
#define UP		0x80
#define DOWN		0x81
#define LEFT		0x82
#define RIGHT		0x83
#define INSERT		0x84
#define DEL		0x7f
#define HOME		0x86
#define END		0x87
#define PUP		0x88
#define PDOWN		0x89
#define NUM_ENT		ENTER
#define SLOCK		0x8c
#define ALT		0x8b


// the option array used for storing set-up options
extern int Option[];

#define O_LINES24       0
#define O_PAL           2
#define O_1STOPBIT      3
#define O_PARITY        4
#define O_BAUDRATE      5
#define O_KEYBOARD      6
#define O_STARTUPMSG    7
#define O_SERIALINV     8

// definitions related to setting the keyboard type in O_KEYBOARD
#define O_KEYBOARD_US	-1
#define O_KEYBOARD_FR	0
#define O_KEYBOARD_GR	1
#define O_KEYBOARD_IT	2
#define O_KEYBOARD_BE	3
#define O_KEYBOARD_UK	4
#define O_KEYBOARD_ES	5

// definitions related to parity
#define O_PARITY_NONE   -1
#define O_PARITY_ODD    0
#define O_PARITY_EVEN   1

