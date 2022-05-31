#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "monitor/symbol.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

uint32_t eval(int p, int q, bool *success);

enum {
	NOTYPE = 256, EQ, HEXNUM, NUM, REG, UEQ, AND, OR, NOT, NEG, DEREF,VARI

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"==", EQ},
	{"0[Xx][0-9a-fA-F]+",HEXNUM},
	{"[0-9]+",NUM},
	{"\\$e[abcd]x",REG},
	{"\\$e[bis]p",REG},
	{"\\$e[ds]i",REG},
	{"\\(",'('},
	{"\\)",')'},
	{"\\+",'+'},
	{"-",'-'},
	{"\\*",'*'},
	{"/",'/'},
	{"!=",UEQ},
	{"&&",AND},
	{"\\|\\|",OR},
	{"!",NOT},
	{"[_a-zA-Z][_0-9a-zA-Z]+",VARI}
	/*,{"&",'&'},
	{"\\|",'|'},
	{"~",'~'}*/
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch(rules[i].token_type) {
					case NOTYPE: break;
					case NUM:
					case HEXNUM:
					case VARI:
					case REG: tokens[nr_token].type = rules[i].token_type; strncpy(tokens[nr_token].str, substr_start, substr_len); tokens[nr_token].str[substr_len] = '\0'; nr_token ++; break;
					default: tokens[nr_token].type = rules[i].token_type; nr_token ++;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

static uint32_t num_to_int(char str[32])
{
	uint32_t sum = 0;
	for(int i = 0; str[i] != '\0'; i++)
		sum = sum * 10 + (str[i] - '0');
	return sum;
}

static uint32_t hexnum_to_int(char str[32])
{
	uint32_t sum = 0;
	for(int i = 2; str[i] != '\0'; i++)
	{
		switch(str[i])
		{
			case 'A':
		    case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F': sum = sum * 16 + (str[i] - '7'); break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f': sum = sum * 16 + (str[i] - 'W'); break;
			default: sum = sum * 16 + (str[i] - '0'); break;
		}
	}
	return sum;
}

static bool check_parentheses(int p, int q, bool *success)
{
	int flag = 0;
	int left = 0, right = 0;
	for(int i = p; i <= q; i++)
	{
		if(tokens[i].type == '(')
			left++;
		if(tokens[i].type == ')')
			right++;
		if(right == left && i != q)
			flag = 1;
		if(right > left)
		{
			*success = false;
			return false;
		}
	}
	if(left != right)
	{
		*success = false;
		return false;
	}
	else
	{
		if(flag == 0 && tokens[p].type == '(' && tokens[q].type == ')')
		{
			*success = true;
			return true;
		}
		else
		{
			*success = true;
			return false;
		}
	}
}

/*struct dominant_level
{
	int type;
	int level;
}levels[] = 
{
	{'(', 6},
	{')', 6},
	{NEG, 5},
	{DEREF, 5},
	{NOT, 5},
	{'*', 4},
	{'/', 4},
	{'+', 3},
	{'-', 3},
	{EQ, 2},
	{UEQ, 2},
	{AND, 1},
	{OR, 1}
};*/


static int dominant_pos(int p, int q)
{
	int pos = 0;
	int level = 7;
    int flag = 0;
	for(int i = p; i <= q; i++)
	{
		if(tokens[i].type == '(')
			flag++;
		else if(tokens[i].type == ')')
			flag--;
		else if(tokens[i].type == NUM || tokens[i].type == HEXNUM || tokens[i].type == REG || tokens[i].type == VARI)
			;
		else
		{
			if(flag == 0)
			{
				switch(tokens[i].type)
				{
					case NEG:
					case DEREF:
					case NOT: {if(level >= 5) pos = i;}break;
					case '*':
					case '/': {if(level >= 4) pos = i;}break;
					case '+':
					case '-': {if(level >= 3) pos = i;}break;
					case EQ:
					case UEQ: {if(level >= 2) pos = i;}break;
					case AND:  
					case OR: {if(level >= 1) pos = i;}break;
				}
			}
		}
	}
	return pos;
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

/*	printf("\nPlease implement expr at expr.c\n");
	assert(0);*/

	//scan array Token[] check '*'&'-' to see if they represent 'use pointer'&'negtive'
	for(int i = 0; i < nr_token; i++) 
	{
 		if(tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '+' || tokens[i - 1].type == '-' || tokens[i - 1].type == '*' || tokens[i - 1].type == '/' || tokens[i - 1].type == EQ || tokens[i - 1].type == UEQ || tokens[i - 1].type == AND || tokens[i - 1].type == OR || tokens[i - 1].type == NOT || tokens[i - 1].type == '(')) 
		{
			tokens[i].type = DEREF;
		}
		if(tokens[i].type == '-' && (i == 0 || tokens[i - 1].type == '+' || tokens[i - 1].type == '-' || tokens[i - 1].type == '*' || tokens[i - 1].type == '/' || tokens[i - 1].type == EQ || tokens[i - 1].type == UEQ || tokens[i - 1].type == AND || tokens[i - 1].type == OR || tokens[i - 1].type == NOT || tokens[i - 1].type == '('))
		{
			tokens[i].type = NEG;
		}
	}

/*	for(int k = 0; k < nr_token; k++)
	{
		printf("%d  ", tokens[k].type);
	}
	printf("\n");*/
	//use eval() to get the value of the expr

	return eval(0, nr_token - 1, success);
}

uint32_t eval(int p, int q, bool *success) {
 if(p > q) {
 /* Bad expression */
	*success = false;
	return 0;
 }
 else if(p == q) {
 /* Single token.
 * For now this token should be a number.
 * Return the value of the number.
 */
	switch(tokens[p].type)
   	{
		case NUM: return num_to_int(tokens[p].str);
		case HEXNUM: return hexnum_to_int(tokens[p].str);
		case REG: {
				if(strcmp(tokens[p].str,"$eax") == 0) return cpu.eax;
				else if(strcmp(tokens[p].str,"$ebx") == 0) return cpu.ebx;
				else if(strcmp(tokens[p].str,"$ecx") == 0) return cpu.ecx;
				else if(strcmp(tokens[p].str,"$edx") == 0) return cpu.edx;
				else if(strcmp(tokens[p].str,"$ebp") == 0) return cpu.ebp;
				else if(strcmp(tokens[p].str,"$esp") == 0) return cpu.esp;
   				else if(strcmp(tokens[p].str,"$esi") == 0) return cpu.esi;
				else if(strcmp(tokens[p].str,"$edi") == 0) return cpu.edi;
				else if(strcmp(tokens[p].str,"$eip") == 0) return cpu.eip;
				else {*success = false; return 0;}
			 }
		case VARI: return look_up_symtab(tokens[p].str, success);
		default: *success = false; return 0;
	}
 }
 else if(check_parentheses(p, q, success) == true && *success == true) {
 /* The expression is surrounded by a matched pair of parentheses.
 * If that is the case, just throw away the parentheses.
 */
 //printf("1111\n");
 return eval(p + 1, q - 1, success);
 }
 else if(*success == false){
 //printf("2222\n");
	return 0;
 }
 else {
 //printf("3333\n");
 //op = the position of dominant operator in the token expression;
 int op = dominant_pos(p, q);
 //	printf("%d\n",op);

  //printf("%d\n",tokens[op].type);
 if(tokens[op].type == DEREF || tokens[op].type == NOT || tokens[op].type == NEG) 
 {
 //	printf("4444\n");
	int val = eval(op + 1, q, success);
 //	printf("%d\n",val);
    switch(tokens[op].type)
	{
		case NEG: return -val;
		case DEREF: return vaddr_read(val,0,4);
		case NOT: return !(val);
		default: *success = false; return 0;
	}
 }
 else
 {
 	int val1 = eval(p, op - 1, success);
 //	printf("%d\n",val1);
 	int val2 = eval(op + 1, q, success);
 //	printf("%d\n",val2);
 	switch(tokens[op].type) 
    {
 	case '+': return val1 + val2;
 	case '-': return val1 - val2;
 	case '*': return val1 * val2;
 	case '/': return val1 / val2;
 	case EQ: return val1 == val2;
 	case UEQ: return val1 != val2;
 	case AND: return val1 && val2;
 	case OR: return val1 || val2;
 	default: *success = false; return 0;
 	}
 }
 }
}
