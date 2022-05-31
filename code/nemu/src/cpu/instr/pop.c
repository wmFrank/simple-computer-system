#include "cpu/instr.h"

static void instr_execute_1op()
{
	OPERAND obj;
	obj.type = OPR_MEM;
	obj.sreg = SREG_CS;
	obj.data_size = data_size;
	obj.addr = cpu.gpr[4]._32;
	operand_read(&obj);
	opr_src.val = obj.val;
	operand_write(&opr_src);
    cpu.gpr[4]._32 = alu_add(4, cpu.gpr[4]._32);
}

make_instr_impl_1op(pop,r,v)


