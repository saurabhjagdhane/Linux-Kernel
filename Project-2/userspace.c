#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __NR_my_syscall 359


int main()
{

	char dataBuffer[800]; // initialize dataBuffer
	int numTasks; // store number of processes in system
	int looper = 0; 
	numTasks = syscall(__NR_my_syscall,looper, dataBuffer); // Calling first time 

	//printf("%d\n", numTasks); //Just to check whether no of processes
	printf("  PID TTY          TIME CMD\n"); // Headers for processes

	// check if looper = 0 so it can enter the loop in the first place
	while (looper < numTasks){
		memset(dataBuffer, 0, 800); // set dataBuffer to null (reset)
		int ret = syscall(__NR_my_syscall, looper, dataBuffer); // call syscall for every task
		printf("%s\n", dataBuffer); 
		looper++; //increment dataBuffer
	}

	return 0;
}
