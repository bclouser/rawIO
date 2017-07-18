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

#define MAP_SIZE 4096UL


int main(int argc, char *argv[])
{

	printf("User requested addr: %s\n", argv[1]);
	errno = 0;
	long address = strtoul(argv[1], NULL, 16);

	if(errno != 0){
		printf("Yeah, failed to convert that into a number\n");
		return -1;
	}

	printf("After converting to a number: %lX\n", address);

	long pageaddr = address & ~(MAP_SIZE - 1);
	long offset = address & (MAP_SIZE - 1);

	int memfd = open("/dev/mem", O_RDWR | O_SYNC);
 	void* page = mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, pageaddr);

 	if(page == NULL){
 		printf("Failed to memory map address\n");
 		return -1;
 	}

 	printf("page = %08X\n", (unsigned int)page);

	void* mappedAddr = page + offset;

	printf("mappedAddr = %08X\n", (unsigned int)mappedAddr);

#ifdef PEEK
	uint32_t arvo = *(uint32_t *)mappedAddr;
	printf("PEEK 0x%lX = 0x%X\n", address, arvo);	
#endif
#ifdef POKE
	uint32_t arvo = strtoul(argv[2], NULL, 0);
	*(uint32_t *)mappedAddr = arvo;
	printf("POKE 0x%lX = 0x%X\n", address, arvo);	
#endif 
	return 0;
}
