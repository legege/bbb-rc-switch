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
    PRU *myPRU1 = new PRU(1);
    myPRU1->stop();

        // Run PRU1, this will block until PRU0 pulls pin high at start of sync
        myPRU1->execute("rc_switch_in.bin");    

    while(1) {

        // Wait until PRU1 status is set to 1, indicating data has arrived
        while (myPRU1->getSharedMemoryInt(2048 + 0) < 1) {
        }
    
        // Get data
        int data = myPRU1->getSharedMemoryInt(2048 + 1);
        printf("%d\n", data);
        
        // Clear the status register so we know data has been dealt with
        myPRU1->setSharedMemoryInt(2048 + 0, 0);
    }
}
 
