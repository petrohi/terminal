/*****************************************************************************************************************************

    main.c

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

********************************************************************************************************************************/


#include <plib.h>									// peripheral libraries
#include <string.h>
#include <ctype.h>
#include "config.h"                     // config pragmas
#include "main.h"
#include "terminal.h"
#include "terminal_config_ui.h"
#include "terminal_config_nvm.h"
#include "screen.h"
#include "vga.h"
#include "normal.h"
#include "bold.h"
#include "ps2.h"
#include "keys.h"
#include "buzzer.h"

//** USB INCLUDES ***********************************************************
#include "./USB/Microchip/Include/USB/usb.h"
#include "./USB/Microchip/Include/USB/usb_function_cdc.h"
#include "./USB/HardwareProfile.h"

#include "./USB/Microchip/Include/GenericTypeDefs.h"
#include "./USB/Microchip/Include/Compiler.h"
#include "./USB/usb_config.h"
#include "./USB/Microchip/Include/USB/usb_device.h"

#define KEYBOARD_RETRIES 3

extern void keyboard_init();
extern bool keyboard_set_leds(int num, int caps, int scroll, void (*yield)());
extern bool keyboard_test(void (*yield)());

extern struct ps2 *global_ps2;

void init_uart(void);

void CheckUSB(void);

void initTimer(void);

void BlinkLED(void);

#define SERIAL_RX_BUFFER_SIZE 8192
char uart_rx_buffer[SERIAL_RX_BUFFER_SIZE];
volatile size_t uart_rx_head = 0;

#define SERIAL_TX_BUFFER_SIZE 64
char uart_tx_buffer[SERIAL_TX_BUFFER_SIZE];
volatile size_t uart_tx_head = 0;
volatile size_t uart_tx_tail = 0;

#define LOCAL_BUFFER_SIZE 64
static character_t local_loop_buffer[LOCAL_BUFFER_SIZE];
static size_t local_loop_head = 0;

// declare the USB buffers
// these buffers are used by the USB I/O hardware
#define USB_DEVICE_RX_BUFFER_SIZE	64
#define USB_DEVICE_TX_BUFFER_SIZE	64
char UsbDeviceRxBuf[USB_DEVICE_RX_BUFFER_SIZE];
char UsbDeviceTxBuf[USB_DEVICE_TX_BUFFER_SIZE];

// Because the USB copies data direct to the uart Tx buf and sends data direct from the uart Rx queue
// it does not need its own I/O buffers (except for the hardware device buffers declared above).
// The only pointer that we need is this one which keep track of where we are while reading the uart Rx buffer
int USBSerialRxBufTail = 0;

volatile int LEDTimer = 0;
volatile int GeneralTimer;

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

static const struct bitmap_font normal_bitmap_font = {
    .height = FONT_HEIGHT,
    .width = FONT_WIDTH,
    .data = normal_font_data,
    .codepoints_length = sizeof(normal_font_codepoints) / sizeof(int),
    .codepoints = normal_font_codepoints,
    .codepoints_map = normal_font_codepoints_map,
};

static const struct bitmap_font bold_bitmap_font = {
    .height = FONT_HEIGHT,
    .width = FONT_WIDTH,
    .data = bold_font_data,
    .codepoints_length = sizeof(bold_font_codepoints) / sizeof(int),
    .codepoints = bold_font_codepoints,
    .codepoints_map = bold_font_codepoints_map,
};

#define CHAR_HEIGHT    16
#define CHAR_WIDTH     8

static struct screen screen_24_rows = {
    .format =
        {
            .rows = 24,
            .cols = 80,
        },
    .char_width = CHAR_WIDTH,
    .char_height = CHAR_HEIGHT,
    .buffer = NULL,
    .normal_bitmap_font = &normal_bitmap_font,
    .bold_bitmap_font = &bold_bitmap_font,
};

static struct screen screen_30_rows = {
    .format =
        {
            .rows = 30,
            .cols = 80,
        },
    .char_width = CHAR_WIDTH,
    .char_height = CHAR_HEIGHT,
    .buffer = NULL,
    .normal_bitmap_font = &normal_bitmap_font,
    .bold_bitmap_font = &bold_bitmap_font,
};

#define MAX_COLS 80
#define MAX_ROWS 30
#define TAB_STOPS_SIZE (MAX_COLS / 8)

static struct visual_cell visual_cells[MAX_ROWS * MAX_COLS];
uint8_t tab_stops[TAB_STOPS_SIZE];

struct screen *get_screen(struct format format) {
  if (format.cols == 80) {
    if (format.rows == 24)
      return &screen_24_rows;
    else if (format.rows == 30)
      return &screen_30_rows;
  }

  return NULL;
}

struct terminal *global_terminal = NULL;
struct terminal_config_ui *global_terminal_config_ui = NULL;

static void yield() {
  CheckUSB();

  if (global_ps2 && global_terminal) {
    terminal_keyboard_handle_key(
        global_terminal, global_ps2->lshift || global_ps2->rshift,
        global_ps2->lalt, global_ps2->ralt,
        global_ps2->lctrl || global_ps2->rctrl, global_ps2->keys[0]);
  }

  if (global_ps2->keys[0] != KEY_NONE)
    BlinkLED();
}

static void uart_transmit(character_t *characters, size_t size, size_t head) {
  if (!global_terminal->send_receive_mode) {
    while (size--) {
      local_loop_buffer[local_loop_head] = *characters;
      local_loop_head++;
      characters++;

      if (local_loop_head == LOCAL_BUFFER_SIZE)
        local_loop_head = 0;
    }
  }

  uart_tx_head = head;
  INTEnable(INT_SOURCE_UART_TX(UART2), INT_ENABLED);
}

static void screen_draw_codepoint_callback(struct format format, size_t row,
                                           size_t col, codepoint_t codepoint,
                                           enum font font, bool italic,
                                           bool underlined, bool crossedout,
                                           color_t active, color_t inactive) {
  screen_draw_codepoint(get_screen(format), row, col, codepoint, font,
                        italic, underlined, crossedout, active, inactive);
}

static void screen_clear_rows_callback(struct format format, size_t from_row,
                                       size_t to_row, color_t inactive) {
  screen_clear_rows(get_screen(format), from_row, to_row, inactive, yield);
}

static void screen_clear_cols_callback(struct format format, size_t row,
                                       size_t from_col, size_t to_col,
                                       color_t inactive) {
  screen_clear_cols(get_screen(format), row, from_col, to_col, inactive, yield);
}

static void screen_scroll_callback(struct format format, enum scroll scroll,
                                   size_t from_row, size_t to_row, size_t rows,
                                   color_t inactive) {
  screen_scroll(get_screen(format), scroll, from_row, to_row, rows, inactive,
                yield);
}

static void screen_shift_right_callback(struct format format, size_t row,
                                        size_t col, size_t cols,
                                        color_t inactive) {
  screen_shift_right(get_screen(format), row, col, cols, inactive, yield);
}

static void screen_shift_left_callback(struct format format, size_t row,
                                       size_t col, size_t cols,
                                       color_t inactive) {
  screen_shift_left(get_screen(format), row, col, cols, inactive, yield);
}

static void screen_test_callback(struct format format,
                                 enum screen_test screen_test) {
  struct screen *screen = get_screen(format);
  switch (screen_test) {
  case SCREEN_TEST_FONT1:
    screen_test_fonts(screen, FONT_NORMAL);
    break;
  case SCREEN_TEST_FONT2:
    screen_test_fonts(screen, FONT_BOLD);
    break;
  }
}

static void activate_config() {
  terminal_config_ui_activate(global_terminal_config_ui);
}

static void write_config(const volatile struct terminal_config *terminal_config_copy) {
  NVMErasePage((void *)&terminal_config);

  for (size_t i = 0; i < sizeof(struct terminal_config) / 4; ++i)
    NVMWriteWord(((uint32_t *)&terminal_config) + i,
                 *(((uint32_t *)terminal_config_copy) + i));
}

static void keyboard_set_leds_callback(struct lock_state state) {
  size_t retries = KEYBOARD_RETRIES;
  while (retries-- && !keyboard_set_leds(state.caps, state.num, state.scroll, yield))
    ;
}

int main(int argc, char* argv[]) {
  int ch;

  // initial setup of the I/O ports
  ANSELA = 0;
  ANSELB = 0;         // Default all pins to digital
  mJTAGPortEnable(0); // turn off jtag

  // setup the CPU
  SYSTEMConfigPerformance(CLOCKFREQ); // System config performance
  mOSCSetPBDIV(OSC_PB_DIV_1); // fix the peripheral bus to the main clock speed

  // clear all port I/O (they are not cleared by a software reset)
  LATA = LATB = 0;
  TRISA = TRISB = 0xffffffff;
  CNENA = CNENB = CNCONA = CNCONB = 0;
  CNPUA = CNPUB = CNPDA = CNPDB = 0;

  TRISBbits.TRISB5 = 0;
  LATBCLR = (1 << 5); // turn on the power LED
  uSec(1000);         // settling time

  keyboard_init();
  screen_24_rows.buffer = screen_30_rows.buffer =
      init_vga(terminal_config.format_rows);

  struct terminal terminal;
  struct terminal_callbacks callbacks = {
      .keyboard_set_leds = keyboard_set_leds_callback,
      .uart_transmit = uart_transmit,
      .screen_draw_codepoint = screen_draw_codepoint_callback,
      .screen_clear_rows = screen_clear_rows_callback,
      .screen_clear_cols = screen_clear_cols_callback,
      .screen_scroll = screen_scroll_callback,
      .screen_shift_left = screen_shift_left_callback,
      .screen_shift_right = screen_shift_right_callback,
      .screen_test = screen_test_callback,
      .reset = SoftReset,
      .yield = yield,
      .activate_config = activate_config,
      .write_config = write_config};
  terminal_init(&terminal, &callbacks, visual_cells, tab_stops, TAB_STOPS_SIZE,
                &terminal_config, uart_tx_buffer, SERIAL_TX_BUFFER_SIZE);
  global_terminal = &terminal;

  INTEnableSystemMultiVectoredInt();

  initBuzzer();
  initTimer();
  init_uart();
  USBDeviceInit();

  INTEnableInterrupts();

  uSec(1000000);

  struct terminal_config_ui terminal_config_ui;
  global_terminal_config_ui = &terminal_config_ui;
  terminal_config_ui_init(&terminal_config_ui, &terminal, &terminal_config);

  bool keyboard_detected = false;
  size_t retries = KEYBOARD_RETRIES;
  while (retries-- && !(keyboard_detected = keyboard_test(yield)))
    ;

  if (keyboard_detected) {
    terminal_keyboard_update_leds(&terminal);
  } else {
    terminal_uart_receive_string(&terminal,
                                 "PS/2 keyboard is not detected!\r\n");
  }

  size_t local_loop_tail = 0;
  size_t uart_rx_tail = 0;

  while (1) {
    yield();
    terminal_screen_update(&terminal);
    terminal_keyboard_repeat_key(&terminal);

    if (terminal_config_ui.activated)
      continue;

    {
      size_t head = local_loop_head;

      if (local_loop_tail != head) {
        size_t size = 0;
        if (local_loop_tail < head)
          size = head - local_loop_tail;
        else
          size = head + (LOCAL_BUFFER_SIZE - local_loop_tail);

        while (size--) {
          character_t character = local_loop_buffer[local_loop_tail];
          terminal_uart_receive_character(&terminal, character);
          local_loop_tail++;

          if (local_loop_tail == LOCAL_BUFFER_SIZE)
            local_loop_tail = 0;
        }
      }
    }

    {
      size_t head = uart_rx_head;

      if (head != uart_rx_tail) {
        int size = 0;
        if (uart_rx_tail < head)
          size = head - uart_rx_tail;
        else
          size = head + (SERIAL_RX_BUFFER_SIZE - uart_rx_tail);

        terminal_uart_flow_control(&terminal, size);

        while (size--) {
          yield();

          if (terminal_config_ui.activated)
            break;

          terminal_uart_flow_control(&terminal, size);

          character_t character = uart_rx_buffer[uart_rx_tail];
          uart_rx_tail++;

          terminal_uart_receive_character(&terminal, character);
          if (uart_rx_tail == SERIAL_RX_BUFFER_SIZE)
            uart_rx_tail = 0;
        }
      } else {
        terminal_uart_flow_control(&terminal, 0);
      }
    }
  }
}

void init_uart(void) {
  PPSInput(2, U2RX, RPB1);
  PPSOutput(4, RPB0, U2TX);

  int cfg1 = UART_ENABLE_PINS_TX_RX_ONLY;
  if (terminal_config.serial_inverted)
    cfg1 |= (UART_INVERT_RECEIVE_POLARITY | UART_INVERT_TRANSMIT_POLARITY);

  int cfg2 = UART_DATA_SIZE_8_BITS;

  switch (terminal_config.stop_bits) {
  case STOP_BITS_1:
    cfg2 |= UART_STOP_BITS_1;
    break;
  case STOP_BITS_2:
    cfg2 |= UART_STOP_BITS_2;
    break;
  }

  switch (terminal_config.parity) {
  case PARITY_NONE:
    cfg2 |= UART_PARITY_NONE;
    break;
  case PARITY_EVEN:
    cfg2 |= UART_PARITY_EVEN;
    break;
  case PARITY_ODD:
    cfg2 |= UART_PARITY_ODD;
    break;
  }

  int baud = terminal_config_get_baud_rate(&terminal_config);

  UARTConfigure(UART2, cfg1);
  UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL |
                             UART_INTERRUPT_ON_RX_NOT_EMPTY);
  UARTSetLineControl(UART2, cfg2);
  UARTSetDataRate(UART2, BUSFREQ, baud);
  UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

  INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_3);
  INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_0);
  INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
}

void __ISR(_UART2_VECTOR, IPL3AUTO) uart_handler(void) {

  if (INTGetFlag(INT_SOURCE_UART_RX(UART2))) {
    while (UARTReceivedDataIsAvailable(UART2)) {
      if (UARTGetLineStatus(UART2) & 0b1110) {
        UARTGetDataByte(UART2);
        U2STACLR = 0b1110;
        continue;
      }

      uart_rx_buffer[uart_rx_head++] = UARTGetDataByte(UART2);

      if (uart_rx_head >= SERIAL_RX_BUFFER_SIZE)
        uart_rx_head = 0;
    }

    BlinkLED();
    INTClearFlag(INT_SOURCE_UART_RX(UART2));
  }

  if (INTGetFlag(INT_SOURCE_UART_TX(UART2))) {
    while (UARTTransmitterIsReady(UART2) && uart_tx_tail != uart_tx_head) {

      INTDisableInterrupts();

      UARTSendDataByte(UART2, uart_tx_buffer[uart_tx_tail++]);

      INTEnableInterrupts();

      if (uart_tx_tail >= SERIAL_TX_BUFFER_SIZE)
        uart_tx_tail = 0;
    }

    if (uart_tx_tail == uart_tx_head)
      INTEnable(INT_SOURCE_UART_TX(UART2), INT_DISABLED);

    INTClearFlag(INT_SOURCE_UART_TX(UART2));
  }
}

/*********************************************************************************************
* USB I/O functions
**********************************************************************************************/



/******************************************************************************************
Check the USB for work to be done.
Used to send and get data to or from the USB interface.
Each call takes typically 6uS but sometimes it can be up to 600uS.
*******************************************************************************************/
void CheckUSB(void) {
	int i, numBytesRead;
	if(U1OTGSTAT & 0b1000) {													  // is there 5V on the USB?
	    USBDeviceTasks();													      // do any USB work

        if(USBGetDeviceState() == CONFIGURED_STATE) {

            // get data from the USB interface
            // figure out how much space is left in the uart TX buf and if it is less that the USB buffer try and get the USB data
            if(((uart_tx_tail - uart_tx_head + SERIAL_TX_BUFFER_SIZE) % SERIAL_TX_BUFFER_SIZE) <= USB_DEVICE_RX_BUFFER_SIZE) {
                numBytesRead = getsUSBUSART(UsbDeviceRxBuf,USB_DEVICE_RX_BUFFER_SIZE);// check for data to be read
                if(numBytesRead > 0) {                                             // if we have some data,
                    for(i = 0; i < numBytesRead; i++)
                        terminal_uart_transmit_character(global_terminal, UsbDeviceRxBuf[i]); // copy it into the uart output queue
                    BlinkLED();
                }
            }

            // send any data waiting to go
			if((uart_rx_head != USBSerialRxBufTail) && mUSBUSARTIsTxTrfReady()) {		  // next, check for data to be sent
                for(i = 0; uart_rx_head != USBSerialRxBufTail && i < USB_DEVICE_TX_BUFFER_SIZE; i++) {
                    UsbDeviceTxBuf[i] = uart_rx_buffer[USBSerialRxBufTail++];                 // copy the char to the device buffer
                    if(USBSerialRxBufTail >= SERIAL_RX_BUFFER_SIZE) USBSerialRxBufTail = 0;
                }
				putUSBUSART(UsbDeviceTxBuf,i);	                                  // and send it
			}
		    CDCTxService();													      // do the actual send/receive
		}
	}
}



/******************************************************************************************
BOOL USER_USB_CALLBACK_EVENT_HANDLER
This function is called from the USB stack to notify a user application that a USB event
occured.  This callback is in interrupt context when the USB_INTERRUPT option is selected.

Args:  event - the type of event
       *pdata - pointer to the event data
       size - size of the event data

This function was derived from the demo CDC program provided by Microchip
*******************************************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED:
            CDCInitEP();
             break;
//        case EVENT_SET_DESCRIPTOR:
//            break;
        case GRG_EVENT_EP0_REQUEST:
            break;
        case EVENT_SOF:
            break;
        case EVENT_SUSPEND:
            break;
        case EVENT_RESUME:
            break;
        case EVENT_BUS_ERROR:
            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }
    return TRUE;
}





/*********************************************************************************************
* Timer functions
**********************************************************************************************/




/***************************************************************************************************
InitTimer - Initialise the 1 mSec timer used for internal timekeeping.
****************************************************************************************************/
void initTimer(void) {
 	// setup timer 4
    PR4 = 1000 * (BUSFREQ/2/1000000) - 1;                           // 1 mSec
    T4CON = 0x8010;         										// T4 on, prescaler 1:2
    mT4SetIntPriority(1);  											// lower priority
    mT4ClearIntFlag();      										// clear interrupt flag
    mT4IntEnable(1);       											// enable interrupt
}



/****************************************************************************************************************
Timer 4 interrupt processor
This fires every mSec and is responsible for tracking the time and the counts of various timing variables
*****************************************************************************************************************/

void __ISR( _TIMER_4_VECTOR, IPL1AUTO) T4Interrupt(void) {

    if(LEDTimer)
        if(--LEDTimer < 25) LATBCLR = (1<<5);

    StopBuzzerWhenTimeout();

    if(GeneralTimer) GeneralTimer--;
    
    terminal_timer_tick(global_terminal);
    
    // Clear the interrupt flag
    mT4ClearIntFlag();
}


void BlinkLED(void) {
    if(LEDTimer  == 0) {
        LEDTimer = 75;
        LATBSET = (1<<5);
    }
}
