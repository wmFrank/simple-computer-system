#include "cpu/instr.h"

static void instr_execute_2op(){
	operand_read(&opr_src);
	operand_read(&opr_dest);
//	alu_sub(opr_src.val,sign_ext(opr_dest.val,opr_dest.data_size));
//	alu_sub(sign_ext(opr_src.val,opr_src.data_size),opr_dest.val);
	if(opr_src.data_size != opr_dest.data_size)
		alu_sub(sign_ext(opr_src.val,opr_src.data_size),sign_ext(opr_dest.val,opr_dest.data_size));
	else
		alu_sub(opr_src.val,opr_dest.val);
//	opr_dest.val = opr_src.val;
//	operand_write(&opr_dest);
}

make_instr_impl_2op(cmp,r,rm,b)
make_instr_impl_2op(cmp,r,rm,v)
make_instr_impl_2op(cmp,rm,r,b)
make_instr_impl_2op(cmp,rm,r,v)
make_instr_impl_2op(cmp,i,a,b)
make_instr_impl_2op(cmp,i,a,v)
make_instr_impl_2op(cmp,i,rm,b)
make_instr_impl_2op(cmp,i,rm,v)
make_instr_impl_2op(cmp,i,rm,bv)
