#include "cpu/instr.h"

make_instr_func(in_dx2a_b)
{
	uint16_t dx = (uint16_t)(cpu.edx & 0xffff);
	uint32_t data = pio_read(dx, 1);
	cpu.eax = (cpu.eax & 0xffffff00) + (data & 0xff);
	return 1; 
}

make_instr_func(in_dx2a_v)
{
	uint16_t dx = (uint16_t)(cpu.edx & 0xffff);
	uint32_t data = 0;
	if(data_size == 32)
	{
		//printf("%x\n",dx);
		//printf("111111111111111111111111\n");
		data = pio_read(dx, 4);
		cpu.eax = data & 0xffffffff;
	}
	else if(data_size == 16)
	{
		//printf("222222222222222222222222\n");
		data = pio_read(dx, 2);
		cpu.eax = (cpu.eax & 0xffff0000) + (data & 0xffff);
	}
	return 1; 
}
