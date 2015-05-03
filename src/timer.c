
/*

Bits        Usage
6 and 7     Select channel :
                0 0 = Channel 0
                0 1 = Channel 1
                1 0 = Channel 2
                1 1 = Read-back command (8254 only)
4 and 5     Access mode :
                0 0 = Latch count value command
                0 1 = Access mode: lobyte only
                1 0 = Access mode: hibyte only
                1 1 = Access mode: lobyte/hibyte
1 to 3      Operating mode :
                0 0 0 = Mode 0 (interrupt on terminal count)
                0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                0 1 0 = Mode 2 (rate generator)
                0 1 1 = Mode 3 (square wave generator)
                1 0 0 = Mode 4 (software triggered strobe)
                1 0 1 = Mode 5 (hardware triggered strobe)
                1 1 0 = Mode 2 (rate generator, same as 010b)
                1 1 1 = Mode 3 (square wave generator, same as 011b)
0           BCD/Binary mode :
                0 = 16-bit binary
                1 = four-digit BCD

0x36 == 0011 0110 b
    00 : channel 0
    11 : low/high
    011 : mode 3
    0 : binary
*/

#include "types.h"
#include "idt.h"
#include "screen.h"
#include "printf.h"
#include "string.h"
#include "pic.h"
#include "io.h"

uint32 tick = 0;
uint32 freq = 0;

void timer_callback(void)
{
    uint16 cursor;

    tick ++;

    /* todo : grosse refacto, cursor pos + color + serial print */
    cursor = screen_get_cursor();
    screen_move_cursor(0, 70);
    printf("%d", tick);
    screen_move_cursor(cursor >> 8, cursor & 0xFF);

    pic_ack(IRQ_ID_TIMER);
}

void timer_init(void)
{
    uint16 freq, divisor;

    printf("Setting up the timer ...");

    /* set up timer */
    outb(0x43, 0x36);

    /* send the frequency divisor */
    freq = 1000;
    divisor = 1193180 / freq;
    outb(0x40, (divisor & 0xFF));
    outb(0x40, ((divisor >> 8) & 0xFF));

    /* enable interrupt handler */
    pic_int_handler_install(timer_callback, IRQ_VECT_TIMER, IRQ_ID_TIMER);

    printf(" ok\n");
}
