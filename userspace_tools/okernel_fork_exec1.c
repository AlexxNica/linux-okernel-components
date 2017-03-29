/*
 Copyright [2015-2017] Hewlett Packard Enterprise Development LP

This program is free software; you can redistribute it and/or modify it under the terms 
of version 2 of the GNU General Public License as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; 
if not, write to:
  Free Software Foundation, Inc.
  51 Franklin Street, Fifth Floor
  Boston, MA 02110-1301, USA.
*/


/* 
 * Fork and then exec from the child to run under a outer / inner kernel split.
 * This version uses an ioctl interface to switch to okernel (NR) mode.
 * C I Dalton 2015
 */

#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "okernel_ioctl.h"

#ifdef DEBUG
#define DPRINTF(fmt, ...) \
	do { printf("%d:%s " fmt, __LINE__, __FILE__, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
    do { } while (0)
#endif

#define LOG(fmt, ...) \
	do { printf("%d:%s " fmt, __LINE__, __FILE__, ## __VA_ARGS__); } while (0)


		
void usage(void)
{
	printf("okernel_fork_exec1 arg\n");
	return;
}


int a; 

int main(int argc, char **argv)
{
	pid_t pid;
	int fd;
	int b;

	
	if(argc < 2){
		usage();
		return 0;
	}
		
	pid = getpid();

	printf("started (%s) with pid (%ld)\n", argv[0], (long)pid);
	

#if 1
	if((fd = open(DEVICE_PATH, O_RDWR)) == -1 ){
		printf("Failed to open (%s) errno (%d)\n", DEVICE_PATH, errno);
		return -1;
	}

	printf("About to call OKERNEL_ON_CMD ioctl...\n");

	ioctl(fd, OKERNEL_ON_CMD, 1);

	asm volatile("xchg %bx, %bx");
	
	printf("Returned sucessefully from OKERNEL_ON_CMD ioctl.\n");

	asm volatile("xchg %bx, %bx");

	close(fd);
#endif
	asm volatile("xchg %bx, %bx");

	printf("About to fork (%s)\n", argv[0]);
	

	asm volatile("xchg %bx, %bx");
	
	printf("just before fork\n");
	
	fflush(stdout);
	
	if((pid = fork()) < 0) {
		printf("Fork failed - errno (%d)\n", errno);
	} else if (pid == 0){
		/* in child  - now exec the arg given */
		if(execvp(argv[1], &argv[1]) < 0){
			printf("execvp failed (%s) errno (%d)\n", argv[1], errno);
			asm volatile("xchg %bx, %bx");
			return -1;
		}
		/* Shouldn't get here! */
		printf("Shouldn't get here!\n");
		asm volatile("xchg %bx, %bx");
		return 0;
	} else {
		/* in parent */
		sleep(2);
		printf("In Parent\n");
		asm volatile("xchg %bx, %bx");
	}
	printf("pid (%ld) global a = (%d) stack local b = (%d)\n", (long)pid, a, b);
	printf("Now exit...\n");
	return 0;
}

