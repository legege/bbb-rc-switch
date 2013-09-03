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

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "pru.h"

PRU *pru;

void my_handler(int s){
    printf("Caught signal %d\n",s);
    pru->stop();
    exit(1); 
}

// Entry point
int main(int argc, char* argv[]) {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // Initialise PRU
    int p = 0;
    if (argv[1] != NULL) {
        p = atoi(argv[1]);
    }

    printf("PRU #%d\n", p);
    pru = new PRU(p);
    pru->stop();

    pru->execute("rc_switch_in.bin");    

    while(1) {

        // Wait until PRU status is set to 1, indicating data has arrived
        while (pru->getSharedMemoryInt(2048 + 0) < 1) {
        }
    
        // Get data
        int data = pru->getSharedMemoryInt(2048 + 1);
        printf("%d\n", data);
        
        // Clear the status register so we know data has been dealt with
        pru->setSharedMemoryInt(2048 + 0, 0);
    }
}
 
