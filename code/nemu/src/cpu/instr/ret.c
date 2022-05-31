#include "cpu/instr.h"

make_instr_func(ret)
{
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;
	obj.addr = cpu.esp;
	operand_read(&obj);
	cpu.eip = obj.val;
	cpu.esp = cpu.esp + 0x4;
	return 0;
}

make_instr_func(ret_i_v)
{
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;
	obj.addr = cpu.esp;
	operand_read(&obj);
//	cpu.eip = obj.val;
	uint32_t mid = (uint32_t)instr_fetch(eip+1,2);
	cpu.eip = obj.val;
	cpu.esp = cpu.esp + 4 + mid;
	return 0;
}


