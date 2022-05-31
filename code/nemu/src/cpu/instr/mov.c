#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
	//printf("%x\n",cpu.eip);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)

make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

make_instr_func(mov_cr2r_v)
{
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	len += modrm_r_rm(eip + 1, &r, &rm);

	rm.type = OPR_CREG;
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);
	return len;
}

make_instr_func(mov_r2cr_v)
{
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	len += modrm_r_rm(eip + 1, &rm, &r);
	
	rm.type = OPR_CREG;
	operand_read(&r);
	rm.val = r.val;
	operand_write(&rm);
	return len;
}

make_instr_func(mov_rm2s_v)
{
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	len += modrm_r_rm(eip + 1, &r, &rm);

	r.type = OPR_SREG;
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	load_sreg(cpu.segReg[r.addr].index);
	return len;
}

/*make_instr_func(mov_cr2r_v)
{
	//printf("111111111111111111111111111111111111111\n");
	//printf("%x\n",cpu.eip);

	OPERAND rel1;
	rel1.type = OPR_MEM;
	//rel1.sreg = SREG_CS;
	rel1.data_size = 8;
	rel1.addr = eip + 1;
	operand_read(&rel1);
	uint32_t index = (rel1.val) & 0x00000007;
	//printf("cr2r index = %x\n",index);

	OPERAND r, rm;
	//rm.type = OPR_CREG;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	len += modrm_r_rm(eip + 1, &r, &rm);
	printf("rm.addr == %x\n",rm.addr);
	printf("rm.val == %x\n",rm.val);
	//operand_read(&rm);
	if(index == 0)
	{
		r.val = cpu.cr0.val;
		printf("cpu.cr0.val == %x\n",cpu.cr0.val);
	}
	else if(index == 3)
	{
		r.val = cpu.cr3.val;
		printf("cpu.cr3.val == %x\n",cpu.cr3.val);
	}
	operand_write(&r);
	//printf("%d\n",len);
	return len;
}*/

/*make_instr_func(mov_r2cr_v)
{
	//printf("2222222222222222222222222222222222222222\n");
	//printf("%x\n",cpu.eip);
	OPERAND rel1;
	rel1.type = OPR_MEM;
	//rel1.sreg = SREG_CS;
	rel1.data_size = 8;
	rel1.addr = eip + 1;
	operand_read(&rel1);
	uint32_t index = (rel1.val >> 3) & 0x00000007;
	//printf("r2cr index = %x\n",index);

	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;
	int len = 1;
	len += modrm_r_rm(eip + 1, &rm, &r);
	operand_read(&r);
	if(index == 0)
		cpu.cr0.val = r.val;
	else if(index == 3)
	{
		cpu.cr3.val = r.val;
//		cpu.cr3.reserved = 0;
	}
	//operand_write(&rm);
	//printf("%d\n",len);
	return len;
}*/

/*make_instr_func(mov_rm2s_v)
{
	//printf("4444444444444444444444444444444444444444444\n");
	//printf("%x\n",cpu.eip);
	OPERAND rel1;
	rel1.type = OPR_MEM;
	//rel1.sreg = SREG_CS;
	rel1.data_size = 8;
	rel1.addr = eip + 1;
	operand_read(&rel1);
	uint32_t sreg = (rel1.val >> 3) & 0x00000007;

	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;
	int len = 1;
	len += modrm_r_rm(eip + 1, &r, &rm);
	operand_read(&rm);
	cpu.segReg[sreg].val = rm.val;
	//operand_write(&rm);
	
	load_sreg(cpu.segReg[sreg].index);
	//printf("sreg == %x    cpu.segReg[sreg].index == %x\n",sreg, cpu.segReg[sreg].index);
	//printf("%d\n",len);
	return len;
}*/

/*
make_instr_func(mov_r2rm_b) {
	src.data_size = 8;
	dest.data_size = 8;

	len += modrm_r_rm(eip + 1, &src, &dest);
	
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//operand_read(&r);
	//rm.val = r.val;
	//operand_write(&rm);
	execute();
	return len;
}*/



/*make_instr_func(mov_r2rm_v) {
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//printf("mov_r2rm rm addr = 0x%08x\n", rm.addr);
	//operand_read(&r);
	//rm.val = r.val;
	//operand_write(&rm);
	execute(&r, &rm);
	return len;
}

make_instr_func(mov_rm2r_b) {
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	//operand_read(&rm);
	//r.val = rm.val;
	//operand_write(&r);
	execute(&rm, &r);
	return len;
}

make_instr_func(mov_rm2r_v) {
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);

	//operand_read(&rm);
	//r.val = rm.val;
	//operand_write(&r);
	execute(&rm, &r);
	return len;
}

make_instr_func(mov_i2rm_b) {
	OPERAND rm, imm;
	rm.data_size = 8;
	imm.data_size = 8;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);
	//operand_read(&imm);
	//rm.val = imm.val;
	//operand_write(&rm);
	execute(&imm, &rm);
	return len + 1; 
}

make_instr_func(mov_i2rm_v) {
	OPERAND rm, imm;
	rm.data_size = data_size;
	imm.data_size = data_size;

	int len = 1;
	// read ModR/M for src and dest
	len += modrm_rm(eip + 1, &rm);

	imm.type = OPR_IMM;
	imm.addr = eip + len;
	imm.data_size = data_size;
	// printf("addr imm %08x, len=%d\n", imm.addr, len);

	//operand_read(&imm);
	//rm.val = imm.val;
	//operand_write(&rm);
	execute(&imm, &rm);
	return len + data_size / 8;
}
make_instr_func(mov_i2r_b) {
	OPERAND r, imm;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	imm.data_size = 8;

	//operand_read(&imm);
	//r.val = imm.val;
	//operand_write(&r);

	execute(&imm, &r);
	return 2;
}

make_instr_func(mov_i2r_v) {
	OPERAND r, imm;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = opcode & 0x7;

	imm.type = OPR_IMM;
	imm.addr = eip + 1;
	imm.data_size = data_size;

	//operand_read(&imm);
	//r.val = imm.val;
	//operand_write(&r);

	execute(&imm, &r);
	return 1 + data_size / 8;
}

make_instr_func(mov_off2a_b) {
	OPERAND r, m;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = REG_AL;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = 8;

	operand_read(&m);
	r.val = m.val;
	operand_write(&r);
	return 5;
}


make_instr_func(mov_off2a_v) {
	OPERAND r, m;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = REG_AX;
	
	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = data_size;

	operand_read(&m);
	r.val = m.val;
	operand_write(&r);
	return 5;
}

make_instr_func(mov_a2off_b) {
	OPERAND r, m;
	r.data_size = 8;
	r.type = OPR_REG;
	r.addr = REG_AL;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = 8;

	operand_read(&r);
	m.val = r.val;
	operand_write(&m);
	return 5;
}

make_instr_func(mov_a2off_v) {
	OPERAND r, m;
	r.data_size = data_size;
	r.type = OPR_REG;
	r.addr = REG_AX;

	m.type = OPR_MEM;
	m.sreg = SREG_DS;
	m.addr = instr_fetch(eip + 1, 4);
	m.data_size = data_size;

	operand_read(&r);
	m.val = r.val;
	operand_write(&m);
	return 5;
}

*/
make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}
