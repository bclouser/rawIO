#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#if !defined(PEEK) && !defined(POKE)
#error Bad compile; Either PEEK or POKE must be defined 
#endif

#define MAP_SIZE 4096UL

int main(int argc, char *argv[])
{
	#ifdef POKE
	if(argc < 3){
		printf("\nUsage: poke <0xAddress> <0xValue>\n\n");
		return -EINVAL;
	}
	#endif
	if(argc < 2){
		#ifdef PEEK
		printf("\nUsage: peek <0xAddress>\n\n");
		return -EINVAL;
		#elif defined(POKE)
		printf("\nUsage: poke <0xAddress> <0xValue>\n\n");
		return -EINVAL;
		#endif
	}
	errno = 0;
	long address = strtoul(argv[1], NULL, 16);

	if(errno != 0){
		printf("Yeah, failed to convert that into a number\n");
		return errno; // might actually be returning errno for printf?
	}


	long pageaddr = address - (address%MAP_SIZE);
	long offset = address%MAP_SIZE;

	int memfd = open("/dev/mem", O_RDWR | O_SYNC);
 	void* page = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, pageaddr);
 	if(page == MAP_FAILED){
 		printf("Failed to memory map address\n");
 		return errno;
 	}

	void* mappedAddr = page + offset;
	//printf("mappedAddr = %08X\n", (unsigned int)mappedAddr);

#ifdef PEEK
	uint32_t arvo = *(uint32_t *)mappedAddr;
	printf("PEEK 0x%lX = 0x%X\n", address, arvo);	
#elif defined(POKE)
	uint32_t arvo = strtoul(argv[2], NULL, 0);
	*(uint32_t *)mappedAddr = arvo;
	printf("POKE 0x%lX = 0x%X\n", address, arvo);	
#endif 
	return 0;
}
