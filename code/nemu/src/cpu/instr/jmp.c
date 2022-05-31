#include "cpu/instr.h"

make_instr_func(jmp_near) {
		//printf("jump_near..........................\n");
        OPERAND rel;
        rel.type = OPR_IMM;
		rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
//	print_asm_1("jmp", "", 2, &rel);
	print_asm_1("jmp", "", 1 + data_size / 8, &rel);

	cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_short_){
	OPERAND rel;
	rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
	rel.data_size = 8;
	rel.addr = eip + 1;

	operand_read(&rel);

	int offset = sign_ext(rel.val, 8);
	print_asm_1("jmp", "", 1 + 1, &rel);

	cpu.eip += offset;
	
	return 2;
}

make_instr_func(jmp_v)
{
	OPERAND src;
	src.data_size = data_size;
	
	int len = 1;
	len += modrm_rm(eip + 1, &src);
	
	operand_read(&src);
	cpu.eip = src.val;
	return 0;
}

make_instr_func(ljmp)
{
	//printf("3333333333333333333333333333333333333333\n");
	//printf("%x\n",cpu.eip);
	/*OPERAND rel;
	rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
	rel.data_size = 32;
	rel.addr = eip + 1;

	operand_read(&rel);*/
	
	//int offset = sign_ext(rel.val,data_size);
	//cpu.eip = rel.val;
	uint32_t offset = laddr_read(eip + 1, 4);
	cpu.eip = offset;	
	//printf("%x\n",cpu.eip);
	/*OPERAND rel2;
	rel2.type = OPR_IMM;
	rel2.sreg = SREG_CS;
	rel2.data_size = 16;
	rel2.addr = eip + 5;
	operand_read(&rel2);*/
	uint32_t data = laddr_read(eip + 5, 2);
	cpu.cs.val = data;
	//printf("%x\n",data);
	load_sreg(cpu.cs.index);
	return 0;
}


