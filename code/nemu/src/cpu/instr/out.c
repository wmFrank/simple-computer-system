
#include "cpu/instr.h"

make_instr_func(out_a2dx_b)
{
	uint16_t dx = (uint16_t)(cpu.edx & 0xffff);
	pio_write(dx, 1, cpu.eax & 0xff);
	return 1; 
}
