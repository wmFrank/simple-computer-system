#ifndef __CACHE_H__
#define __CACHE_H__

#include <stdio.h>
#include "nemu.h"
#include <stdlib.h>

typedef struct
{
	uint8_t valid_bit;    //1bit(actual 4byte)
	uint32_t mark_bit;      //19bit(actual 4byte)
	uint8_t data[64];      //64byte(actual 64byte)
}CacheLine;

CacheLine cache_block[1024]; //64KB

void init_cache();  //clear valid_bit
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine *cache);       //find, compare, what if yes, what if no, what if full(random replace)
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache);         //find, compare, what if yes, what if no, how to deal with main_mem(write all)

#endif
