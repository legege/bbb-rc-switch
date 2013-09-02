/* Copyright (c) 2013 Owen McAree
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "pru.h"

// Entry point
int main() {
    // Initialise PRU
    PRU *myPRU0 = new PRU(0);
    myPRU0->stop();
    
    // Set up output data for PRU0
    // Offset of 2048 is there for legacy reasons (compatibility with node-pru)
    // Because this is shared memory we use offset of 50
    // So we don't conflict with PRU1
    myPRU0->setSharedMemoryInt(2048 + 50 + 0, 0);
    myPRU0->setSharedMemoryInt(2048 + 50 + 1, 0);
    myPRU0->setSharedMemoryInt(2048 + 50 + 2, 0);
    myPRU0->setSharedMemoryInt(2048 + 50 + 3, 1);
    myPRU0->setSharedMemoryInt(2048 + 50 + 4, 0);
    myPRU0->setSharedMemoryInt(2048 + 50 + 5, 0);
    myPRU0->setSharedMemoryInt(2048 + 50 + 6, 0xF);
    myPRU0->setSharedMemoryInt(2048 + 50 + 7, 0xF);
    myPRU0->setSharedMemoryInt(2048 + 50 + 8, 0xF);
    myPRU0->setSharedMemoryInt(2048 + 50 + 9, 0xF);
    myPRU0->setSharedMemoryInt(2048 + 50 + 10, 0);
    myPRU0->setSharedMemoryInt(2048 + 50 + 11, 0xF);
    
    // Wait a while to make sure PRU1 is happy
    usleep(1000000);
    
    // Execute PRU0 to send dummy signal
    myPRU0->execute("rc_switch_out.bin");    
}
 
