**TLDR;**

This product is a kit based on the original [Geoff Graham design](http://geoffg.net/terminal.html).

If you only need the PCB you can buy it [here](https://www.tindie.com/products/petrohi/geoffs-vt100-terminal-pcb/).

**ATTENTION! Version 3 of the ASCII Terminal has following significant changes to the original Geoff's design:**

- Full compatibility with VT-100 and XTerm tested with [vttest](https://invisible-island.net/vttest/);
- All new SETUP utility available on Ctrl+Alt+Del;
- Configurable UTF-8, ISO 8859 and IBM PC 437 support;
- Configurable VT-220 and PC/Sun keyboards compatibility;
- Regular, bold, underlined and strike-through [Terminus Unicode fonts](http://terminus-font.sourceforge.net/);
- Blinking and inverted visual attributes;
- Configurable ANSI color-to-monochrome transformation;
- Support for historic DEC graphic characters;
- VGA output follows strict 25MHz pixel clock to get standard 640x480 resolution at 60Hz refresh rate;
- 8x16 characters and 24 or 30 lines screen buffers configurable in SETUP utility;
- Full range of baud rates from 110 up to 1,500,000;
- Scroll-lock and XON/XOFF flow control;
- PS/2 keyboard detection at startup;
- Monochrome color is selectable with jumpers instead of solder bridges;
- Only US, UK, DE, SE, FR and BE keyboard layouts are supported (new layouts are easy to add--just let me know);
- Composite PAL/NTSC output is not supported;
- Serial communication speed is not configurable with jumpers, the only available configuration is with SETUP utility;
- Streamlined 4″x1.5″ board with VGA, PS/2 and USB ports on one side;
- Firmware source code and KiCad designs are [open source](https://github.com/petrohi/terminal). Contributions are welcome!

**Description**

This is VT-100 and XTerm compatible video terminal implemented on the PIC32 microcontroller. It has a serial interface with TTL or RS-232 signal levels, input from a standard PS/2 keyboard, and output to a VGA monitor. There is also a USB interface that supports serial over USB and acts as a USB-to-serial converter.

**What is included?**

- 2 layer PCB
- PIC32MX270F256B-I/SP-ND microcontroller
- MCP1700-3302E/TO-ND voltage regulator
- Red LED
- 20MHz Crystal
- Ceramic Capacitors 27pF 50V (2)
- Ceramic Capacitors 0.1uF 50V (2)
- Ceramic Capacitors 10uF 10% 16V (3)
- Resistor 82Ω
- Resistor 220Ω
- Resistors 4.7KΩ (4)
- Resistor 10KΩ
- Resistor 100KΩ
- USB B-Type Socket
- DE-15 Female Connector
- 6 Pin Mini DIN Female Connector
- 4 way Header Connector
- 2 way Header Connector
- DIP socket
- Header Pins (16)
- Jumpers (4)