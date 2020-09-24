/****************************************************************************************

	main.h

	VT100 Terminal program


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

// The main clock frequency for the chip
#define	CLOCKFREQ		(50000000ul)			    // This is set in in Configuration Bits.h

// The peripheral bus frequency
#define BUSFREQ			(CLOCKFREQ/1)			    // This is set in in Configuration Bits.h

#define INPUT                   1
#define OUTPUT                  0

#define P_INPUT			1				    // for setting the TRIS on I/O bits
#define P_OUTPUT		0
#define P_ON			1
#define P_OFF			0


#define dp(...) {char s[140];sprintf(s,  __VA_ARGS__); VideoPrintString(s); VideoPrintString("\r\n");}
#define dpUSB(...) {char s[140];sprintf(s,  __VA_ARGS__); USBPrintString(s); USBPrintString("\r\n"); mT4IntEnable(0); CheckUSB(); mT4IntEnable(1);}
#define uSec(us) { unsigned int i = ((((unsigned int)(us) * 1000) - 600) / (2000000000/CLOCKFREQ)); WriteCoreTimer(0); while(ReadCoreTimer() < i); }

extern volatile int GeneralTimer;
