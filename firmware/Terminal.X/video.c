/****************************************************************************************
    video.c

	Video driver for the VT100 Terminal program


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



#include <p32xxxx.h>
#include <plib.h>
#include <string.h>
#include "main.h"
#include "video.h"
#include "fonts.h"
#include "vt100.h"

/*   The video generator is based on an idea and code by Lucio Di Jasio presented in his excellent book
     "Programming 32-bit Microcontrollers in C - Exploring the PIC32".
*/

// Parameters for VGA video with 31.5KHz horizontal scanning and 60Hz vertical refresh
// When using 36 lines we have 480x432 pixels.  This gives us 80 chars per line and 36 lines per screen
// When using 24 lines these values are changed in ConfigBuffers()
#define VGA_VRES	 	432                                         // Vert graphics resolution (pixels)
#define VGA_HRES	 	480                                         // Horiz graphics resolution (pixels)
#define VGA_H_OFFSET    5                                           // Number of blank pixels at the start of a line (horiz position)
#define VGA_HBUFF	 	((VGA_HRES + VGA_H_OFFSET + 31)/32)         // Number of words of memory to allocate for a horiz line
#define VGA_LINE_N   	525                                         // number of lines in VGA frame
#define VGA_LINE_T   	1270                                        // Tpb clock in a line (31.777us)
#define VGA_VSYNC_N  	2                                           // V sync lines
#define VGA_VBLANK_N 	(VGA_LINE_N - VGA_VRES - VGA_VSYNC_N)       // Nbr of blank lines
#define VGA_PREEQ_N  	((VGA_VBLANK_N/2) - 12)                     // Nbr blank lines at the bottom of the screen
#define VGA_POSTEQ_N 	VGA_VBLANK_N - VGA_PREEQ_N                  // Nbr blank lines at the top of the screen
#define VGA_PIX_T    	2                                           // Tpb clock per pixel
#define VGA_HSYNC_T  	152                                         // Tpb clock width horizontal pulse

// Common parameters for Composite video
#define C_PIX_T      	4                                           // Tpb clock per pixel
#define C_HSYNC_T    	187        									// Tpb clock width horizontal pulse
#define C_BLANKPAD   	8											// number of zero words (4 bytes each) before sending data
#define C_VSYNC_N    	3          									// V sync lines
#define C_H_OFFSET      8                                           // Number of blank pixels at the start of a line (horiz position)

// Parameters for PAL composite video
// Graphics is 388x216 pixels.  This gives us 48 chars per line and 18 lines per screen
#define PAL_VRES	    216											// Vert graphics resolution (pixels)
#define PAL_HRES	    288 										// Horiz graphics resolution (pixels)
//#define PAL_HBUFF	 	((PAL_HRES + C_H_OFFSET + 31)/32)           // Number of words of memory to allocate for a horiz line
#define PAL_HBUFF	 	((PAL_HRES + C_H_OFFSET + PAL_HRES/2 + C_H_OFFSET/2 + 31)/32)           // Number of words of memory to allocate for a horiz line
#define PAL_LINE_N     	312        									// number of lines in PAL frames
#define PAL_LINE_T     	2560       									// Tpb clock in a line (64us) 5115 //
#define PAL_VBLANK_N   	(PAL_LINE_N - PAL_VRES - C_VSYNC_N)  		// Nbr of blank lines
#define PAL_PREEQ_N    	((PAL_VBLANK_N/2) - 8)         				// Nbr blank lines at the bottom of the screen
#define PAL_POSTEQ_N   	(PAL_VBLANK_N - PAL_PREEQ_N)  				// Nbr blank lines at the top of the screen

// Parameters for NTSC composite video
// Graphics is 388x180 pixels.  This gives us 48 chars per line and 15 lines per screen
#define NTSC_VRES     	180											// Vert graphics resolution (pixels)
#define NTSC_HRES	    264											// Horiz graphics resolution (pixels)
#define NTSC_HBUFF	 	((NTSC_HRES + C_H_OFFSET + NTSC_HRES/2 + C_H_OFFSET/2 + 31)/32)          // Number of words of memory to allocate for a horiz line
#define NTSC_LINE_N     262	        								// number of lines in NTSC frames
#define NTSC_LINE_T     2540       									// Tpb clock in a line (63.55us)
#define NTSC_VBLANK_N   (NTSC_LINE_N - NTSC_VRES - C_VSYNC_N)  		// Nbr of blank lines
#define NTSC_PREEQ_N    ((NTSC_VBLANK_N/2) - 8)         			// Nbr blank lines at the bottom of the screen
#define NTSC_POSTEQ_N   (NTSC_VBLANK_N - NTSC_PREEQ_N)  			// Nbr blank lines at the top of the screen


// states of the vertical sync state machine
#define SV_PREEQ    0												// generating blank lines before the vert sync
#define SV_SYNC     1												// generating the vert sync
#define SV_POSTEQ   2												// generating blank lines after the vert sync
#define SV_LINE     3												// visible lines, send the video data out

// define the video buffer
// note that this can differ from the pixel resolution, for example for composite HRes is not an even multiple of 32 where HBuf is
int VBuf, HBuf;														// Global vert and horiz resolution of the video buffer
int VRes, HRes;														// Global vert and horiz resolution in pixels on the screen

// allocate the video buffer, the last part of this is also used for the serial Rx buffer
// simply use as much memory as we can
int VideoBuf[BUFFER_SIZE/4];

#if (VGA_VRES * VGA_HBUFF + 2048) > BUFFER_SIZE
    #error Not enough memory allocated
#endif

int vga;															// true if we are using the internal VGA video
unsigned int SvSyncT;												// used to determine the timing for the SV_SYNC state;

// the state machine is used to keep track of what part of the video frame we are generating (vert sysch, post equ, etc)
int VS[4] = { SV_SYNC, SV_POSTEQ, SV_LINE, SV_PREEQ };				// the next state table
int VC[4];															// the next counter table (initialise in initVideo() below)
volatile int VCount;												// counter for the number of lines in a frame
volatile int VState;												// the state of the state machine

const int zero[] = {0, 0, 0, 0, 0, 0, 0, 0};                        // 32 bytes of zero used to pad the start of composite video

const unsigned int PalVgaOption = 0xffffffff;					    // use to hold the pal/vga setting in flash

// these keep track of where the next char will be written
int CursorX, CursorY;												// position in pixels (home is 0, 0)
int CharPosX, CharPosY;												// position in characters (home is 1, 1)
int AutoLineWrap;													// global used to track if we want automatic line wrap
int Display24Lines;                                                 // true if we are displaying 24 lines

// the font table and the current height and width of the font
int fontNbr;

// details of the current font
unsigned int *fontPtr;                                              // pointer to a font using 32 bit words
int fontWidth;
int fontHeight;
int fontStart;
int fontEnd;
int fontScale;


/**************************************************************************************************
Initialise the video components
***************************************************************************************************/
void initVideo(void) {

    AutoLineWrap = 1;

    // test if there is a monitor plugged into the VGA connector
    CNPUASET = (1 << 4);                                            // set a pullup on the video output pin
    uSec(300);                                                      // allow it to settle
    vga = !PORTAbits.RA4;                                           // the pin will be pulled low if the monitor is there
    CNPUACLR = (1 << 4);

    ////////////////////////////
    // setup SPI2 which is the video generator.  the output of this module is a stream of bits which are the pixels in a horiz scan line
    if(vga)
        PPSOutput(3, RPA4, SDO2);                                   // B5 is the video out for VGA
    else
        PPSOutput(3, RPB2, SDO2);                                   // B2 is the video out for composite
    PPSInput(4, SS2, RPB9);                                         // B9 is the framing input
    #define P_VIDEO_SPI         2                                   // the SPI peripheral used for video
    #define P_SPI_INPUT         SPI2BUF                             // input buffer for the SPI peripheral
    #define P_SPI_INTERRUPT     _SPI2_TX_IRQ                        // interrupt used by the SPI peripheral when it needs more data

    ////////////////////////////
    // the horizontal sync uses Timer 3 and Output Compare 3 to generate the pulse and interrupt level 7
    PPSOutput(4, RPB14, OC3);                                       // B14 is the horizontal sync output (ie, the output from OC3)
    #define P_VID_OC_OPEN       OpenOC3                             // the function used to open the output compare
    #define P_VID_OC_REG        OC3R                                // the output compare register

    ////////////////////////////
    // the vertical sync uses B13
    TRISBCLR = (1<<13);                                             // Vert sync output used by VGA
    #define P_VERT_SET_HI		LATBSET = (1 << 13)                 // set vert sync hi
    #define P_VERT_SET_LO		LATBCLR = (1 << 13)                 // set vert sync lo

    // calculate the paramaters for each video mode and setup Timer 3 to generate the horiz synch and interrupt on its leading edge
    if(vga) {
        // set up for VGA output
	    HRes = VGA_HRES;                                            // HRes is the number of horiz pixels to use
	    HBuf = VGA_HBUFF * 32;                                      // HBuf is the horiz buffer size (in pixels)
        ConfigBuffers(Option[O_LINES24]);                           // setup the buffer pointers and VBuf, VRes
		VC[0] = VGA_VSYNC_N;										// setup the table used to count lines
		VC[1] = VGA_POSTEQ_N;
	    P_VID_OC_OPEN(OC_ON | OC_TIMER3_SRC | OC_CONTINUE_PULSE, 0, VGA_HSYNC_T);	// enable the output compare which is used to time the width of the horiz sync pulse
	    OpenTimer3( T3_ON | T3_PS_1_1 | T3_SOURCE_INT, VGA_LINE_T-1);	            // enable timer 3 and set to the horizontal scanning frequency
	}
    else if(Option[O_PAL]) {
		// this is for the PAL composite output and is the same as VGA with timing differences
	    VBuf = VRes = PAL_VRES;
	    HRes = PAL_HRES;
	    HBuf = PAL_HBUFF * 32;                                      // HBuf is the horiz buffer size (in pixels)
        ConfigBuffers(0);
	    SvSyncT = PAL_LINE_T - C_HSYNC_T;
		VC[0] = C_VSYNC_N;
		VC[1] = PAL_POSTEQ_N;
		VC[2] = PAL_VRES;
		VC[3] = PAL_PREEQ_N;
	    P_VID_OC_OPEN(OC_ON | OC_TIMER3_SRC | OC_CONTINUE_PULSE, 0, C_HSYNC_T);	    // enable the output compare which is used to time the width of the horiz sync pulse
	    OpenTimer3(T3_ON | T3_PS_1_1 | T3_SOURCE_INT, PAL_LINE_T-1);	            // enable timer 3 and set to the horizontal scanning frequency
    }
	else {
		// this is for the NTSC composite output and is similar again
	    VBuf = VRes = NTSC_VRES;
	    HRes = NTSC_HRES;
	    HBuf = NTSC_HBUFF * 32;                                     // HBuf is the horiz buffer size (in pixels)
        ConfigBuffers(0);
	    SvSyncT = NTSC_LINE_T - C_HSYNC_T;
		VC[0] = C_VSYNC_N;
		VC[1] = NTSC_POSTEQ_N;
		VC[2] = NTSC_VRES;
		VC[3] = NTSC_PREEQ_N;
	    P_VID_OC_OPEN(OC_ON | OC_TIMER3_SRC | OC_CONTINUE_PULSE, 0, C_HSYNC_T);	    // enable the output compare which is used to time the width of the horiz sync pulse
	    OpenTimer3(T3_ON | T3_PS_1_1 | T3_SOURCE_INT, NTSC_LINE_T-1);	            // enable timer 3 and set to the horizontal scanning frequency
	}

    // set priority level 7 for the timer 3 interrupt (horiz synch) and enable it
    mT3SetIntPriority(7);
    mT3IntEnable(1);

    // initialise the state machine and set the count so that the first interrupt will increment the state
	VState = SV_PREEQ;
    VCount = 1;

    // setup the SPI channel then DMA channel which will copy the memory bitmap buffer to the SPI channel
    if(vga) {
        // open the SPI in framing mode.  Note that SPI_OPEN_DISSDI will disable the input (which we do not need)
	    SpiChnOpen(P_VIDEO_SPI, SPICON_ON | SPICON_MSTEN | SPICON_MODE32 | SPICON_FRMEN | SPICON_FRMSYNC | SPICON_FRMPOL | SPI_OPEN_DISSDI, VGA_PIX_T);
        SPI2CON2 = (1<<9) | (1<<8);                                         // instruct the SPI module to ignore any errors that might occur
        DmaChnOpen(1, 1, DMA_OPEN_DEFAULT);                                 // setup DMA 1 to send data to SPI channel 2
        DmaChnSetEventControl(1, DMA_EV_START_IRQ_EN | DMA_EV_START_IRQ(P_SPI_INTERRUPT));
	    DmaChnSetTxfer(1, (void*)VideoBuf, (void *)&P_SPI_INPUT, HBuf/8, 4, 4);
    } else {
	    SpiChnOpen(P_VIDEO_SPI, SPICON_ON | SPICON_MSTEN | SPICON_MODE32 | SPICON_FRMEN | SPICON_FRMSYNC | SPICON_FRMPOL | SPI_OPEN_DISSDI, C_PIX_T);
        SPI2CON2 = (1<<9) | (1<<8);                                         // instruct the SPI module to ignore any errors that might occur
        DmaChnOpen(1, 1, DMA_OPEN_DEFAULT);    	                            // setup DMA 1 is the blank padding at the start of a scan line
        DmaChnSetEventControl(1, DMA_EV_START_IRQ_EN | DMA_EV_START_IRQ(P_SPI_INTERRUPT));
	    DmaChnSetTxfer(1, (void*)zero, (void *)&P_SPI_INPUT, C_BLANKPAD, 4, 4);
    	DmaChnOpen( 0, 0, DMA_OPEN_DEFAULT);		                        // setup DMA 0 to pump the data from the graphics buffer to the SPI peripheral
    	DmaChnSetEventControl(0, DMA_EV_START_IRQ_EN | DMA_EV_START_IRQ(P_SPI_INTERRUPT));
	    DmaChnSetTxfer(0, (void*)VideoBuf, (void *)&P_SPI_INPUT, HBuf/8 + 6, 4, 4);
    	DmaChnSetControlFlags(0, DMA_CTL_CHAIN_EN | DMA_CTL_CHAIN_DIR);    	// chain DMA 0 so that it will start on completion of the DMA 1 transfer
    }
}


void ConfigBuffers(int lines24) {
    if(vga) {
        Display24Lines = lines24;                                   // set the 24 line flag
        if(lines24) {                                               // and adjust the vertical paramaters accordingly
            VBuf = VRes = (VGA_VRES / 3) * 2;
            VC[2] = VGA_VRES - 1;
            VC[3] = VGA_PREEQ_N + 1;
        } else {
            VBuf = VRes = VGA_VRES;
            VC[2] = VGA_VRES;
            VC[3] = VGA_PREEQ_N;
        }
    } else {
        Display24Lines = false;                                     // composite cannot have 24 lines
    }
    SerialRxBuf = (char *)&VideoBuf[(VBuf * HBuf)/32];              // this is the end of the area used for video
    RxBufferSize = BUFFER_SIZE - ((VBuf * HBuf)/8) - 4;             // we take what is left for the Rx buffer (-4 is to allow for word/byte alignment)
    SerialRxBufHead = 0;
    SerialRxBufTail = 0;
    USBSerialRxBufTail = 0;
}


/**************************************************************************************************
Timer 3 interrupt.
Used to generate the horiz and vert sync pulse under control of the state machine
***************************************************************************************************/
void __ISR(_TIMER_3_VECTOR, IPL7SOFT) T3Interrupt(void) {
    static int *VPtr;
    static unsigned char AlternateField = 0;                        // used to count odd/even fields in composite video
    static unsigned int LineCnt = 1;

    switch ( VState) {    											// vertical state machine
        case SV_PREEQ:  // 0            							// prepare for the new horizontal line
            VPtr = VideoBuf;
            LineCnt = 1;
            break;

        case SV_SYNC:   // 1
            if(!vga) {                                              // if the video is composite
                P_VID_OC_REG = SvSyncT;                             // start the vertical sync pulse for composite
            	VCount += AlternateField;                           // in composite video the alternative field has one extra line
            	AlternateField ^= 1;
            }
            P_VERT_SET_LO;										    // start the vertical sync pulse for vga
            break;

        case SV_POSTEQ: // 2
        	if(!vga) P_VID_OC_REG = C_HSYNC_T; 			            // end of the vertical sync pulse for composite
            P_VERT_SET_HI;                                          // end of the vertical sync pulse for vga
            break;

        case SV_LINE:   // 3
            P_SPI_INPUT = 0;                                        // preload the SPI with 4 zero bytes to pad the start of the video
            if(vga)
                DCH1SSA = KVA_TO_PA((void*) (VPtr));                // update the DMA1 source address (DMA1 is used for VGA data)
            else
                DCH0SSA = KVA_TO_PA((void*) (VPtr - 1));            // update the DMA0 source address (DMA0 is used for composite data)
            if(!Display24Lines || LineCnt++ % 3) VPtr += HBuf/32;   // move the pointer to the start of the next line
            DmaChnEnable(1);                                        // arm the DMA transfer
            break;
   }

    if (--VCount == 0) {											// count down the number of lines
        VCount = VC[VState&3];										// set the new count
        VState = VS[VState&3];    									// and advance the state machine
    }

    mT3ClearIntFlag();    											// clear the interrupt flag
}



/**************************************************************************************************
Turn on or off a single pixel in the graphics buffer
***************************************************************************************************/
void plot(int x, int y, int b) {
    int t;
    if(x >= 0 && x < HRes && y >= 0 && y < VRes) {
        if(vga)
            x += VGA_H_OFFSET;                                                      // this positions the start of the video
        else {
            // for composite video we need to duplicate every second pixel to get the aspect correct ratio
            // this means that we must multiple the x cordinate by 1.5
            // but that leaves gaps in the pixels so we need to figure out if there will be a gap and, if so,
            // turn on a second pixel to fill in the gap
            t = ((x << 1) + x) >> 1;                                                // multiply by 1.5
            if(((((x + 1) << 1) + (x + 1)) >> 1) - t > 1) {                         // will there be a gap?
                x = t + C_H_OFFSET + 1;                                             // this positions the start of the video
                if(b == 0)
                    VideoBuf[y * (HBuf/32) + x/32] &= ~(0x80000000>>(x & 0x1f));   	// turn off the pixel
                else if(b == -1)
                    VideoBuf[y * (HBuf/32) + x/32] ^= (0x80000000>>(x & 0x1f));		// invert the pixel
                else
                    VideoBuf[y * (HBuf/32) + x/32] |= (0x80000000>>(x & 0x1f));		// turn on the pixel
            }
            // if there is no gap we can proceed normally
            x = t + C_H_OFFSET;                                                     // this positions the start of the video
        }

      	if(b == 0)
            VideoBuf[y * (HBuf/32) + x/32] &= ~(0x80000000>>(x & 0x1f));   		    // turn off the pixel
        else if(b == -1)
        	VideoBuf[y * (HBuf/32) + x/32] ^= (0x80000000>>(x& 0x1f));			    // invert the pixel
        else
    	    VideoBuf[y * (HBuf/32) + x/32] |= (0x80000000>>(x & 0x1f));			    // turn on the pixel
    }
}



/**************************************************************************************************
scroll the screen
***************************************************************************************************/
void ScrollUp(void) {
   	int i;
	int *pd;
   	int *ps;

    pd = VideoBuf;
    ps = pd + (HBuf/32) * (fontHeight * fontScale);
    for(i=0; i < (HBuf/32) * (VRes - (fontHeight * fontScale)); i++) *pd++ = *ps++;// scroll up
    for(i=0; i<(HBuf/32) * (fontHeight * fontScale); i++) *pd++ = 0;              // clear the new line
}



void ScrollDown(void) {
	int *pd;
   	int *ps;

    pd = VideoBuf + ((VRes * HBuf)/32) - 1;
    ps = pd - ((HBuf/32) * (fontHeight * fontScale));
    while(ps >= VideoBuf) *pd-- = *ps--;                                                        // scroll down
    for(pd = VideoBuf; pd < VideoBuf + (HBuf/32) * (fontHeight * fontScale); pd++) *pd = 0;     // clear the new line
}




/**************************************************************************************************
clear the screen
***************************************************************************************************/
void ClearScreen(void) {
    ShowCursor(false);                                              // turn off the cursor to prevent it from getting confused
	memset(VideoBuf, 0, VBuf*(HBuf/8));
}



// Write a char on the video using the graphic bit font
void DrawChar(int x, int y, int c) {                                // x and y are in pixels
    int j, k, h, w;                     							// Loop counters, j for horizontal, y for vertical
	int t;

	if(c < fontStart || c > fontEnd) return;

    for(j = 0; j < fontWidth; j++)                                  // Loop through each word (ie, horizontal scan line)
        for(k = 0; k < fontHeight; k++) {                           // Loop through the bits in the word (ie, vertical pixels)
            t = ((*(fontPtr + ((c - fontStart) * fontWidth) + j)) & (1<<k)) >> k;
            if(AttribUL && k == fontHeight - 3) t = 1;              // implement underline
            if(AttribInvis) t = 0;                                  // don't draw if invisible
            if(AttribRV) t = !t;                                    // reverse video (if required)
            for(h = 0; h < fontScale; h++)
                for(w = 0; w < fontScale; w++)
                    plot(x + (j * fontScale) + w, y + (k * fontScale) + h, t);	// Draw the pixel
        }
}




// select a font
void initFont(int fnt) {
    ShowCursor(false);                                              // turn off the cursor to prevent it from getting confused
    if(fnt == 1) {
        fontPtr = (unsigned int *)fontOne;
        fontHeight = 12;
        fontWidth = 6;
        fontStart = ' ';
        fontEnd = '~';
        fontScale = 1;
        fontNbr = 1;
    }

    if(fnt == 2) {
        fontPtr = (unsigned int *)fontTwo;
        fontHeight = 20;
        fontWidth = 13;
        fontStart = ' ';
        fontEnd = '~';
        fontScale = 1;
        fontNbr = 2;
    }

    if(fnt == 3) {
        fontPtr = (unsigned int *)fontTwo;
        fontHeight = 20;
        fontWidth = 13;
        fontStart = ' ';
        fontEnd = '~';
        fontScale = 2;
        fontNbr = 3;
    }


}


// Put a char onto the video screen
void VideoPutc(char c) {
    if(c & 0x80) return;                                            // don't print anything with the top bit set

    ShowCursor(false);                                              // turn off the cursor to prevent fragments of it cluttering the screen

    if(AutoLineWrap) {
		if(CursorX < 0) CursorX = 0;
		if(CursorY < 0) CursorY = 0;
		if(CursorY > VRes) CursorY = VRes;
	}

	if(c >= fontStart && c <= fontEnd) {
    	// if it is a printable chars chect if we need to scroll at the end of the line
		if(CursorX + (fontWidth * fontScale) > HRes && AutoLineWrap) {// if we are at the end of a line
			CursorX = 0;												// wrap to the next
			CursorY += (fontHeight * fontScale);
		}
	} else {
	    // if it is not a printable char we need to check for control chars
    	if(c == '\r') CursorX = 0; 									// for a return set the horizontal position to zero

    	if(c == '\n') {
    		CursorY += (fontHeight * fontScale); 					// for a line feed add the char height to the vert position
    	}

    	if(c == '\t')  												// for a tab move the horiz position to the next tab spot
    		CursorX = ((CursorX + ((fontWidth * fontScale) << 3)) >> 3) << 3;

    	if(c == '\b') {												// for a backspace move the horiz position back by a char width
    		CursorX -= (fontWidth * fontScale);
    		if(CursorX <0) CursorX = 0;
    	}
    }

    // if we are beyond the bottom of the screen scroll up the previous lines
	if(CursorY + (fontHeight * fontScale) > VRes + 1 && AutoLineWrap) {
    	ScrollUp();
		CursorY -= (fontHeight * fontScale);
	}

    // now we are ready to draw the character
	if(c >= fontStart && c <= fontEnd) {
		DrawChar(CursorX, CursorY, c);
		CursorX += (fontWidth * fontScale);							// and update our position
	}

    CharPosX = (CursorX / (fontWidth * fontScale)) + 1;				// update the horizontal character position
    CharPosY = (CursorY / (fontHeight * fontScale)) + 1;			// update the horizontal character position
}



// control the cursor
void ShowCursor(int show) {
	static int visible = false;

	if(CursorOff || CursorTimer > CURSOR_ON) show = false;
	if(visible == show) return;                                     // no change required so skip the rest
	DrawLine(CursorX - 1, CursorY + (fontHeight * fontScale) - 3, CursorX + (fontWidth * fontScale) - 1, CursorY + (fontHeight * fontScale) - 3, -1);	// invert the underline
	visible = show;
}



void DisplayString(char *p) {
   while(*p) VideoPutc(*p++);
}



/**************************************************************************************************
Draw a line on a the video output
	x1, y1 - the start coordinate
	x2, y2 - the end coordinate
	colour - zero for erase, non zero to draw
***************************************************************************************************/
#define abs( a)     (((a)> 0) ? (a) : -(a))

void DrawLine(int x1, int y1, int x2, int y2, int colour) {
    int  x, y, addx, addy, dx, dy;
    int P;
    int i;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    x = x1;
    y = y1;

    if(x1 > x2)
       addx = -1;
    else
       addx = 1;

    if(y1 > y2)
       addy = -1;
    else
       addy = 1;

    if(dx >= dy) {
       P = 2*dy - dx;
       for(i=0; i<=dx; ++i) {
          plot(x, y, colour);
          if(P < 0) {
             P += 2*dy;
             x += addx;
          } else {
             P += 2*dy - 2*dx;
             x += addx;
             y += addy;
          }
       }
    } else {
       P = 2*dx - dy;
       for(i=0; i<=dy; ++i) {
          plot(x, y, colour);
          if(P < 0) {
             P += 2*dx;
             y += addy;
          } else {
             P += 2*dx - 2*dy;
             x += addx;
             y += addy;
          }
       }
    }
}




/**********************************************************************************************
Draw a box on the video output
     x1, y1 - the start coordinate
     x2, y2 - the end coordinate
     fill  - 0 or 1
     colour - 0 or 1
***********************************************************************************************/
void DrawBox(int x1, int y1, int x2, int y2, int fill, int colour) {

   if(fill) {
      int y, ymax;                          // Find the y min and max
      if(y1 < y2) {
         y = y1;
         ymax = y2;
      } else {
         y = y2;
         ymax = y1;
      }

      for(; y<=ymax; ++y)                    // Draw lines to fill the rectangle
         DrawLine(x1, y, x2, y, colour);
   } else {
      DrawLine(x1, y1, x2, y1, colour);      	// Draw the 4 sides
      DrawLine(x1, y2, x2, y2, colour);
      DrawLine(x1, y1, x1, y2, colour);
      DrawLine(x2, y1, x2, y2, colour);
   }
}



/***********************************************************************************************
Draw a circle on the video output
	x, y - the center of the circle
	radius - the radius of the circle
	fill - non zero
	colour - to use for both the circle and fill
	aspect - the ration of the x and y axis (a float).  0.83 gives an almost perfect circle
***********************************************************************************************/
void DrawCircle(int x, int y, int radius, int fill, int colour, float aspect) {
   int a, b, P;
   int A, B;
   int asp;

   a = 0;
   b = radius;
   P = 1 - radius;
   asp = aspect * (float)(1 << 10);

   do {
     A = (a * asp) >> 10;
     B = (b * asp) >> 10;
     if(fill) {
         DrawLine(x-A, y+b, x+A, y+b, colour);
         DrawLine(x-A, y-b, x+A, y-b, colour);
         DrawLine(x-B, y+a, x+B, y+a, colour);
         DrawLine(x-B, y-a, x+B, y-a, colour);
      } else {
         plot(A+x, b+y, colour);
         plot(B+x, a+y, colour);
         plot(x-A, b+y, colour);
         plot(x-B, a+y, colour);
         plot(B+x, y-a, colour);
         plot(A+x, y-b, colour);
         plot(x-A, y-b, colour);
         plot(x-B, y-a, colour);
      }

      if(P < 0)
         P+= 3 + 2*a++;
      else
         P+= 5 + 2*(a++ - b--);

    } while(a <= b);
}
