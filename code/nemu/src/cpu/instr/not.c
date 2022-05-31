#include "cpu/instr.h"

static void instr_execute_1op()
{
	operand_read(&opr_src);
	switch(opr_src.data_size)
	{
		case 8:opr_src.val = opr_src.val ^ 0xff;break;
		case 16:opr_src.val = opr_src.val ^ 0xffff;break;
		case 32:opr_src.val = opr_src.val ^ 0xffffffff;break;
	}
	operand_write(&opr_src);
}

make_instr_impl_1op(not,rm,b)
make_instr_impl_1op(not,rm,v)
