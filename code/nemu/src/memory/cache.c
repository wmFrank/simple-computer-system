#include "memory/cache.h"
#include "memory/memory.h"
#include <math.h>

CacheLine cache_block[1024]; //64KB

void init_cache()  //clear valid_bit
{
	for(int i = 0; i < 1024; i++)
		cache_block[i].valid_bit = 0;
}

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine *cache)      //find, compare, what if yes, what if no, what if full(random replace)
{
	uint32_t mark = (paddr >> 13) & 0x0007ffff;             //19bit
	uint32_t group = (paddr >> 6) & 0x0000007f;             //7bit
	uint32_t data_addr = paddr & 0x0000003f;                //6bit
	uint32_t line = group * 8;
	uint32_t ret = 0;                                       //result
	if(data_addr == 61 || data_addr == 62 || data_addr == 63)
	{
		ret = hw_mem_read(paddr, len);
	}
	else
	{
	bool flag1 = false;                                     //find or not
	bool flag2 = true;                                     //full or not
	for(int i = line; i < line + 8; i++)
	{
		if(cache_block[i].mark_bit == mark && cache_block[i].valid_bit == 1)
		{
			flag1 = true;
			if(len == 1)
			{
				ret = cache_block[i].data[data_addr];	
			}
			else if(len == 2)
			{
				ret = cache_block[i].data[data_addr] + cache_block[i].data[data_addr + 1] * pow(2, 8);
			}
			else if(len == 4)
			{
				ret = cache_block[i].data[data_addr] + cache_block[i].data[data_addr + 1] * pow(2, 8) + cache_block[i].data[data_addr + 2] * pow(2, 16) + cache_block[i].data[data_addr + 3] * pow(2, 24);
			}
			break;
		}
	}
	if(flag1 == false)
	{
		for(int i = line; i < line + 8; i++)
		{
			if(cache_block[i].valid_bit == 0)
			{
				flag2 = false;
				cache_block[i].valid_bit = 1;
				cache_block[i].mark_bit = mark;
				uint32_t init_addr = paddr & 0xffffffc0;
				for(int j = 0; j < 64; j++)
					cache_block[i].data[j] = hw_mem_read(init_addr + j, 1);
				if(len == 1)
				{
					ret = cache_block[i].data[data_addr];	
				}
				else if(len == 2)
				{
					ret = cache_block[i].data[data_addr] + cache_block[i].data[data_addr + 1] * pow(2, 8);
				}
				else if(len == 4)
				{
					ret = cache_block[i].data[data_addr] + cache_block[i].data[data_addr + 1] * pow(2, 8) + cache_block[i].data[data_addr + 2] * pow(2, 16) + cache_block[i].data[data_addr + 3] * pow(2, 24);
				}
				break;
			}
		}
	if(flag2 == true)
	{
		int number = line + rand() % 8;
		cache_block[number].valid_bit = 1;
		cache_block[number].mark_bit = mark;
		uint32_t init_addr = paddr & 0xffffffc0;
		for(int j = 0; j < 64; j++)
			cache_block[number].data[j] = hw_mem_read(init_addr + j, 1);
		if(len == 1)
		{
			ret = cache_block[number].data[data_addr];	
		}
		else if(len == 2)
		{
			ret = cache_block[number].data[data_addr] + cache_block[number].data[data_addr + 1] * pow(2, 8);
		}
		else if(len == 4)
		{
			ret = cache_block[number].data[data_addr] + cache_block[number].data[data_addr + 1] * pow(2, 8) + cache_block[number].data[data_addr + 2] * pow(2, 16) + cache_block[number].data[data_addr + 3] * pow(2, 24);
		}
	}
	}
	}
	return ret;
}

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache)         //find, compare, what if yes, what if no, how to deal with main_mem(write all)
{
	hw_mem_write(paddr, len, data);
	uint32_t mark = (paddr >> 13) & 0x0007ffff;             //19bit
	uint32_t group = (paddr >> 6) & 0x0000007f;             //7bit
	uint32_t data_addr = paddr & 0x0000003f;                //6bit
	uint32_t line = group * 8;
	uint8_t data0 = data & 0x000000ff;
	uint8_t data1 = (data >> 8) & 0x000000ff;
	uint8_t data2 = (data >> 16) & 0x000000ff;
	uint8_t data3 = (data >> 24) & 0x000000ff;
/*	if(data_addr == 0 || data_addr == 1 || data_addr == 2 || data_addr == 61 || data_addr == 62 || data_addr == 63)
	{
		ret = paddr_read(paddr, len);
	}*/
	if(((paddr & 0x3f) + len - 1) <= 0x3f)
	{
		for(int i = line; i < line + 8; i++)
		{
			if(cache_block[i].mark_bit == mark && cache_block[i].valid_bit == 1)
			{
				if(len == 1)
				{
					cache_block[i].data[data_addr] = data0;	
				}
				else if(len == 2)
				{
					cache_block[i].data[data_addr] = data0; cache_block[i].data[data_addr + 1] = data1;
				}
				else if(len == 4)
				{
					cache_block[i].data[data_addr] = data0; cache_block[i].data[data_addr + 1] = data1; cache_block[i].data[data_addr + 2] = data2; cache_block[i].data[data_addr + 3] = data3;
				}
				break;
			}
		}
	}
	else
	{
		if(len == 2)
		{
			paddr_t paddr_new = paddr + 1;
			cache_write(paddr_new, 1, data1, cache);
		}
		else if(len == 4)
		{
			if((paddr & 0x3f) == 0x3d)
			{
				paddr_t paddr_new = paddr + 3;
				cache_write(paddr_new, 1, data3, cache);
			}
			if((paddr & 0x3f) == 0x3e)
			{
				paddr_t paddr_new = paddr + 2;
				cache_write(paddr_new, 1, data2, cache);
				cache_write(paddr_new + 1, 1, data3, cache);
				
			}
			if((paddr & 0x3f) == 0x3f)
			{
				paddr_t paddr_new = paddr + 1;
				cache_write(paddr_new, 1, data1, cache);
				cache_write(paddr_new + 1, 1, data2, cache);
				cache_write(paddr_new + 2, 1, data3, cache);
			}
		}
	}
}



























