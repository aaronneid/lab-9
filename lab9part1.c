/* ECE3220 Lab 9 Part 2
 * Author: Aaron Neidlinger */

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	int fd;		// for the file descriptor of the special file we need to open.
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR

    fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
	if(fd == -1){
		printf("\n error\n");
	    return(-1);  // failed open
	}

	// We need to map Address 0x80840000 (beginning of a memory page)
	// An entire memory page is mapped. We need to point to the beginning of the page.
	BasePtr = (unsigned long*)mmap(NULL,getpagesize(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
    if(BasePtr == MAP_FAILED){
    	printf("\n Unable to map memory space \n");
    	return(-2);
    }  // failed mmap

	// To access other registers in the page, we need to offset the base pointer to reach the
	// corresponding addresses. Those can be found in the board's manual.
	PBDR = BasePtr + 1;		// Address of port B DR is 0x80840004
    PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014

    *PBDDR |= 0xE0;			// configures port B7 as output (Green LED)
	*PBDDR &= 0xFFFFFFFE;	// configures port B0 as input (first push button). You could use: &= ~0x01

	// The program will turn on the green LED, sleep for a while, then off, sleep, then on again, then off.
	// You could use loops, if you wanted/needed.
	
    *PBDR |= 0x80;	// ON: write a 1 to port B0. Mask all other bits.
	sleep(1);		// How can you sleep for less than a second?
    *PBDR &= ~0x80;	// OFF: write a 0 to port B0. Mask all other bits.
    sleep(1);

	for(int i = 0; i < 3; i++){
		*PBDR |= 0x20;		//red on
		sleep(1);
		*PBDR &= 0xDF;		//red off
		sleep(1);		
	}
	
	for(int i = 0; i < 4; i++){
		*PBDR |= 0x40;		//yellow on
		sleep(1);
		*PBDR &= 0xBF;		//yellow off
		sleep(1);		
	}
	
	for(int i = 0; i < 4; i++){
		*PBDR |= 0x40;		//yellow on
		*PBDR |= 0x20;		//red on
		*PBDR |= 0x80;	//green on
		sleep(1);
		*PBDR &= 0xBF;		//yellow off
		*PBDR &= ~0x80;	//green off
		*PBDR &= 0xDF;		//red off
		sleep(1);		
	}

	// If you wanted to read the status of Port B0, how could you do it?

    close(fd);	// close the special file
    return 0;
}

