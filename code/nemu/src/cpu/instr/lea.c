#include "cpu/instr.h"

make_instr_func(lea)
{
	OPERAND r, rm;
	rm.data_size = data_size;
	rm.type = OPR_REG;
	int len = 1;
	len += modrm_r_rm(eip+1, &rm,&r);
//	operand_read(&r);
	rm.val = r.addr;
	operand_write(&rm);
	return len;
}

	
