/***********************************************************************************************************************

Configuration Bitss.h

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

************************************************************************************************************************/


#ifndef CONFIGURATION_BITS_H
#define CONFIGURATION_BITS_H

#pragma config FNOSC = PRIPLL                                       // Oscillator Selection
                                                                    // PRI	is Primary oscillator (XT, HS, EC)
                                                                    // PRIPLL	is Primary oscillator (XT, HS, EC) w/ PLL
                                                                    // SOSC	is Secondary oscillator
                                                                    // LPRC	is Low power RC oscillator
                                                                    // FRC	is Fast RC oscillator
                                                                    // FRCPLL	is Fast RC oscillator w/ PLL
                                                                    // FRCDIV16	is Fast RC oscillator with divide by 16
                                                                    // FRCDIV	is Fast RC oscillator with divide

#pragma config POSCMOD = XT                                         // Primary Oscillator Selection
                                                                    // HS	is HS oscillator
                                                                    // EC	is EC oscillator
                                                                    // XT	is XT oscillator
                                                                    // OFF	is Disabled

#pragma config FPLLIDIV = DIV_2                                     // PLL Input Divide by 1, 2, 3, 4, 5, 6, 10 or 12
#pragma config FPLLMUL = MUL_20                                     // PLL Multiply by 15, 16, 17, 18, 19, 20, 21 or 24
#pragma config FPLLODIV = DIV_2                                     // PLL Output Divide by 1, 2, 4, 8, 16, 32, 64, or 256

#pragma config FPBDIV = DIV_1                                       // Peripheral Bus Clock Divide by 1, 2, 4 or 8

#pragma config FSOSCEN = OFF                                        // Secondary oscillator OFF or ON
#pragma config IESO = OFF                                           // Internal External Switchover (Two-Speed Start-up) OFF or ON
#pragma config OSCIOFNC = OFF                                       // CLKO output signal active on the OSCO pin. Select ON or OFF

#pragma config FCKSM = CSDCMD                                       // Clock Switching and Monitor Selection
                                                                    // CSECME	is Clock Switching Enabled, Clock Monitoring Enabled
                                                                    // CSECMD	is Clock Switching Enabled, Clock Monitoring Disabled
                                                                    // CSDCMD	is Clock Switching Disabled, Clock Monitoring Disabled


#pragma config PMDL1WAY = OFF                                       // Peripheral Module Disable Configuration
#pragma config IOL1WAY = OFF                                        // Peripheral Pin Select Configuration
                                                                    // ON        Allow only one reconfiguration (default)
                                                                    // OFF       Allow multiple reconfigurations

#pragma config JTAGEN = OFF                                         // JTAG Enable

#ifdef __DEBUG
#pragma config DEBUG = ON                                           // Background Debugger ON or OFF
#else
#pragma config DEBUG = OFF                                          // Background Debugger ON or OFF
#endif

#pragma config UPLLEN = ON                                          // USB PLL ON or OFF
#pragma config UPLLIDIV = DIV_2                                     // USB PLL Input Divide by 1, 2, 3, 4, 5, 6, 10 or 12

#pragma config FVBUSONIO = OFF                                      // USB VBUS_ON pin control
#pragma config FUSBIDIO = OFF                                       // USB USBID pin control

#pragma config FWDTEN = OFF                                         // Watchdog Timer ON or OFF
#pragma config WDTPS = PS128                                        // Watchdog Timer Postscale from 1:1 to 1:1,048,576
#pragma config WINDIS = OFF                                         // Watchdog Timer Window Enable (ON = Window Mode, OFF = Non Window Mode)
#pragma config FWDTWINSZ = WINSZ_75                                 // Watchdog Timer Window Size (75%, 50%, 37%, 25%)

#pragma config CP = OFF                                             // Code Protect Enable ON or OFF (prevents ANY read/write)
#pragma config BWP = OFF                                            // Boot Flash Write Protect OFF

#pragma config PWP = OFF                                            // Program Flash Write Protect ON, OFF or PWP1K to PWP32K in steps of 1K

#pragma config ICESEL = ICS_PGx3                                    // ICE/ICD Communications Channel Select

#endif  // CONFIGURATION_BITS_H
