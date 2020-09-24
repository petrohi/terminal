/****************************************************************************************

    keyboard.c

    Keyboard processing for the VT100 Terminal program


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

****************************************************************************************************************************

This routine is based on a technique and code presented by Lucio Di Jasio in his excellent book
"Programming 32-bit Microcontrollers in C - Exploring the PIC32".

Thanks to Muller Fabrice (France), Alberto Leibovich (Argentina) and the other contributors who provided the code for
the non US keyboard layouts

****************************************************************************************************************************/

#define INCLUDE_FUNCTION_DEFINES

#include <p32xxxx.h>
#include <plib.h>
#include "main.h"
#include "ps2.h"

struct ps2 ps2;

struct ps2 *global_ps2;

const unsigned int KeyboardOption = 0xffffffff;						// used to store the keyboard layout output

// definition of the keyboard PS/2 state machine
#define PS2START    0
#define PS2BIT      1
#define PS2PARITY   2
#define PS2STOP     3


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
void keyboard_init(void) {
  P_PS2CLK_TRIS = 1;
  P_PS2DAT_TRIS = 1; // make sure out inputs are in fact inputs

  PPSInput(2, INT3, RPB8);
  ConfigINT3(EXT_INT_PRI_2 | FALLING_EDGE_INT | EXT_INT_ENABLE);

  ps2_init(&ps2);
  global_ps2 = &ps2;
}

/***************************************************************************************************
sendCommand - Send a command to to keyboard.
 Note that when we want to signal a logic high we turn the pin into an input which means that the
 pullup will ensure a high.  When we want to signal a logic low we turn the pin into an output and
 because we have loaded the output latch with a zero the output will be pulled low.
****************************************************************************************************/
static bool send_command(int cmd) {
  int i, j;
  bool success = false;

  // calculate the parity and add to the command as the 9th bit
  for (j = i = 0; i < 8; i++)
    j += ((cmd >> i) & 1);
  cmd = (cmd & 0xff) | (((j + 1) & 1) << 8);

  P_PS2_INTERRUPT(false); // disable interrupt while we play
  P_PS2CLK_OUT = 0;  // when configured as an output the clock line will be low
  P_PS2DAT_OUT = 0;  // same for data
  P_PS2CLK_TRIS = 0; // clock low
  uSec(150);
  P_PS2DAT_TRIS = 0; // data low
  uSec(2);
  P_PS2CLK_TRIS = 1; // release the clock (goes high)
  uSec(2);

  GeneralTimer = 500; // timeout of 500mS
  while (P_PS2CLK)
    if (GeneralTimer == 0)
      goto timeout; // wait for the keyboard to pull the clock low

  // send each bit including parity
  for (i = 0; i < 9; i++) {
    P_PS2DAT_TRIS = (cmd & 1); // set the data bit
    while (!P_PS2CLK)
      if (GeneralTimer == 0)
        goto timeout; // wait for the keyboard to bring the clock high
    while (P_PS2CLK)
      if (GeneralTimer == 0)
        goto timeout; // wait for clock low
    cmd >>= 1;
  }

  P_PS2DAT_TRIS = 1; // release the data line
  while (P_PS2DAT)
    if (GeneralTimer == 0)
      goto timeout; // wait for the keyboard to pull data low (ACK)
  while (P_PS2CLK)
    if (GeneralTimer == 0)
      goto timeout; // wait for the clock to go low
  while (!P_PS2CLK || !P_PS2DAT)
    if (GeneralTimer == 0)
      goto timeout; // finally wait for both the clock and data to go high (idle
                    // state)

  success = true;

timeout:
  P_PS2CLK_TRIS = 1;
  P_PS2DAT_TRIS = 1; // reset the data & clock to inputs in case a keyboard was
                     // not plugged in
  P_PS2_INTERRUPT(true); // re enable interrupt
  uSec(5000);
  return success;
}

bool keyboard_test() {
  bool success = send_command(0xee);
  if (!success)
    return false;

  GeneralTimer = 500;
  while (ps2.response == 0 && GeneralTimer != 0)
    ;
  return (ps2.response == PS2_ECHO_ACK);
}

// set the keyboard LEDs
bool keyboard_set_leds(bool caps, bool num, bool scroll) {
  bool success = send_command(0xed); // Set/Reset Status Indicators Command
  if (!success)
    return false;

  GeneralTimer = 500;
  while (ps2.response == 0 && GeneralTimer != 0)
    ;
  if (ps2.response != PS2_COMMAND_ACK)
    return false;

  return send_command((((int)caps & 1) << 2) | (((int)num & 1) << 1) |
                      ((int)scroll & 1)); // set the various LEDs
}

/***************************************************************************************************
change notification interrupt service routine
****************************************************************************************************/
void __ISR(_EXTERNAL_3_VECTOR, ipl2) INT3Interrupt(void) {
  int d;

  // PS2 KBD state machine and buffer
  static int state = PS2START, count, parity;
  static unsigned char code = 0;

  // Make sure it was a falling edge
  if (P_PS2CLK == 0) {
    // Sample the data
    d = P_PS2DAT;
    switch (state) {
    default:
    case PS2START:
      if (!d) {      // PS2DAT == 0
        count = 8;  // init bit counter
        parity = 0; // init parity check
        code = 0;
        state = PS2BIT;
      }
      break;

    case PS2BIT:
      code >>= 1; // shift in data bit
      if (d)
        code |= 0x80;  // PS2DAT == 1
      parity ^= code; // calculate parity
      if (--count <= 0)
        state = PS2PARITY; // all bit read
      break;

    case PS2PARITY:
      if (d)
        parity ^= 0x80;  // PS2DAT == 1
      if (parity & 0x80) // parity odd, continue
        state = PS2STOP;
      else
        state = PS2START;
      break;

    case PS2STOP:
      if (d) { // PS2DAT == 1
        ps2_handle_code(&ps2, code);
        code = 0;
      }
      state = PS2START;
      break;
    }
  }
  // clear interrupt flag
  mINT3ClearIntFlag();
}
