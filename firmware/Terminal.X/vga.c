#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plib.h>

#include "vga.h"

#define VGA_LINES          525
#define VGA_V_SYNC         2
#define VGA_V_PADDING      48
#define VGA_V_FRONT_PORCH  (10 + VGA_V_PADDING)
#define VGA_V_BACK_PORCH   (33 + VGA_V_PADDING)

#define VGA_PIXELS         800
#define VGA_H_SYNC         96
#define VGA_H_FRONT_PORCH  16
#define VGA_H_BACK_PORCH   48

#define VGA_LINE_T   	   (VGA_PIXELS * 2)
#define VGA_H_SYNC_T       (VGA_H_SYNC * 2)

#define VGA_V_LINES        (VGA_LINES - VGA_V_FRONT_PORCH - VGA_V_SYNC - VGA_V_BACK_PORCH)
#define VGA_H_PIXELS       (VGA_PIXELS - VGA_H_FRONT_PORCH - VGA_H_SYNC - VGA_H_BACK_PORCH)

#define VGA_H_BACK_PORCH_BYTES  (VGA_H_BACK_PORCH / 8)
#define VGA_H_FRONT_PORCH_BYTES (VGA_H_FRONT_PORCH / 8)
#define VGA_H_BYTES             (VGA_H_BACK_PORCH_BYTES + (VGA_H_PIXELS / 8) + VGA_H_FRONT_PORCH_BYTES)

#define VGA_V_FRONT_PORCH_SYNC            (VGA_V_FRONT_PORCH + VGA_V_SYNC)
#define VGA_V_FRONT_PORCH_SYNC_BACK_PORCH (VGA_V_FRONT_PORCH_SYNC + VGA_V_BACK_PORCH)

uint8_t vga_buffer[VGA_H_BYTES * VGA_V_LINES];

volatile uint8_t *vga_line = vga_buffer;
volatile size_t vga_cur_line = 0;

#define BIT_13 (1 << 13)

uint8_t *init_vga() {
    memset((void*)vga_buffer, 0x00, sizeof(vga_buffer));

    TRISBCLR = BIT_13; // B13 is the vertical sync output
    LATBSET =  BIT_13;
    
    PPSOutput(3, RPA4, SDO2); // A4 is the video output
    PPSInput(4, SS2, RPB9);   // B9 is the framing input
    PPSOutput(4, RPB14, OC3); // B14 is the horizontal sync output (ie, the output from OC3)    
    
    OpenOC3(OC_ON | OC_TIMER3_SRC | OC_CONTINUE_PULSE, 0, VGA_H_SYNC_T);
    OpenTimer3(T3_ON | T3_PS_1_1 | T3_SOURCE_INT, VGA_LINE_T - 1);
    SpiChnOpenEx(SPI_CHANNEL2, SPI_OPEN_MODE32 | SPICON_ON | SPICON_MSTEN | SPICON_FRMEN | SPICON_FRMSYNC | SPICON_FRMPOL | SPI_OPEN_DISSDI, SPI_OPEN2_IGNROV | SPI_OPEN2_IGNTUR, 2);

    DmaChnOpen(DMA_CHANNEL0, DMA_CHN_PRI0, DMA_OPEN_DEFAULT);
    DmaChnSetEventControl(DMA_CHANNEL0, DMA_EV_START_IRQ_EN | DMA_EV_START_IRQ(_SPI2_TX_IRQ));
    DmaChnSetTxfer(DMA_CHANNEL0, (void*)vga_line, (void *)&SPI2BUF, VGA_H_BYTES, 4, 4);
    
    mT3SetIntPriority(7);
    mT3IntEnable(1);
    
    return vga_buffer;
}

void __ISR(_TIMER_3_VECTOR, IPL7SOFT) T3Interrupt(void) {
    
    if (vga_cur_line < VGA_V_FRONT_PORCH) {
    }
    else if (vga_cur_line < VGA_V_FRONT_PORCH_SYNC) {
        LATBCLR = BIT_13;
    }
    else if (vga_cur_line < VGA_V_FRONT_PORCH_SYNC_BACK_PORCH) {
        LATBSET = BIT_13;
    }
    else {
        vga_line += VGA_H_BYTES;
        DCH0SSA = KVA_TO_PA((void*) vga_line);
        DmaChnEnable(DMA_CHANNEL0);
    }
    
    if (++vga_cur_line == VGA_LINES) {
        vga_cur_line = 0;
        vga_line = vga_buffer;
    }

    mT3ClearIntFlag();    											// clear the interrupt flag
}