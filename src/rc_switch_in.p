// Copyright (c) 2013 Owen McAree
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

.origin 0
.entrypoint START
#include "../include/pru.hp"

START:
// Preamble to set up OCP and shared RAM
    LBCO    r0, CONST_PRUCFG, 4, 4          // Enable OCP master port
    CLR     r0, r0, 4                       // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    SBCO    r0, CONST_PRUCFG, 4, 4
    MOV     r0, SHARED_RAM+0x020            // Configure the programmable pointer register for PRU1 by setting c28_pointer[15:0]
    MOV     r1, PRU0_CTRL+CTPPR0            // field to 0x0120.  This will make C28 point to 0x00012000 (PRU shared RAM).
    SBBO    r0, r1, 0, 4
// End of preamble

// Register values
//  Int 0 - Status register. 0 is no data, 1 is data
//  Int 1 - Data register, output as binary (24 bits, padded to 32 to fill an int)

    MOV     r0, 0                           // Zero status register
    MOV     r1, 0                           // Zero data register
    SBCO    r0, CONST_PRUSHAREDRAM, 0, 4    // Write out to shared RAM
MESSAGE_LOOP:
    MOV     r5, 0                           // Zero message repeat count register
    MOV     r6, 0                           // Zero previous message register
WAIT_FOR_SYNC:
    WBS     r31.t0                          // Wait for rising edge
    WBC     r31.t0                          // Wait for falling edge
    MOV     r2, 0                           // Zero counter of low time
    MOV     r3, 31*300000/15                // Minimum sync low time value (31 * 300000ns at 15ns/spin)
SYNC_SPIN:                                  // Look for sync period
    ADD     r2, r2, 1                       // Increment a counter
    QBBS    WAIT_FOR_SYNC, r31.t0           // If we get rising edge in sync period, out of sync, so reset
    QBGT    SYNC_SPIN, r2, r3               // If we reach 31 * 300ms (15ns/spin, 5ns/instruction), we're synced

    MOV     r4, 23                          // Counter of bits to receive
RECEIVE_BIT:                                // Start of receiving loop
    MOV     r2, 0                           // Zero counter of high cycles
    WBS     r31.t0                          // Wait for rising edge
BIT_SPIN:
    ADD     r2, r2, 1                       // Count high cycles (10ns/spin, 5ns/instruction)
    QBBS    BIT_SPIN, r31.t0                // Spin while input high

    MOV     r3, 700000/10                   // Threshold value for 0/1 (700000ns at 10ns/spin
    QBGT    BIT_0, r2, r3                   // If threshold is greater than value we have a zero
    QBLT    BIT_1, r2, r3                   // If threshold if less than value, we have a 1
    QBA     WAIT_FOR_SYNC                   // If neither are true (this can't happen!), go back to the start

BIT_0:                                      // If we have a zero
    CLR     r1, r4                          // Clear the corresponding bit of the output (not really needed as it's zero by default)
    QBA     DONE_BIT
BIT_1:                                      // If we have a one
    SET     r1, r4                          // Set the corresponding bit of the output
    QBA     DONE_BIT
DONE_BIT:
    QBEQ    DONE_MESSAGE, r4, 0             // No more bits to receive
    SUB     r4, r4, 1                       // Decrement bit to receive counter
    QBA     RECEIVE_BIT

DONE_MESSAGE:
    QBEQ    INCREMENT_REPEAT, r1, r6        // If previous message is equal than we need to increment repeat count
    MOV     r5, 0                           // This is the first message received, reset repeat count
    MOV     r6, r1                          // and store the read message
    QBA     WAIT_FOR_SYNC

INCREMENT_REPEAT:
    ADD     r5, r5, 1                       // Increment message repeat count
    QBGE    WAIT_FOR_SYNC, r5, 2            // We need 3 identical messages before continuing, so 2 repeat

    SBCO    r1, CONST_PRUSHAREDRAM, 4, 4    // Move data register in to shared memory
    MOV     r0, 1                           // Set status to 1
    SBCO    r0, CONST_PRUSHAREDRAM, 0, 4    // Move status register in to shared memory

    QBA     MESSAGE_LOOP                    // Got back to the start

    HALT
