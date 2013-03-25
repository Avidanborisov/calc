#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "Operator.h"

#define OP_FUNC_BINARY(op, name) \
		double f##name(double a, double b) { return a op b; }
#define OP_FUNC_BINARY_CAST(op, name) \
		double f##name(double a, double b) { return (int)a op (int)b; }
#define OP_FUNC_UNARY(op, name) \
		double f##name(double a) { return op a; }
#define OP_FUNC_UNARY_CAST(op, name) \
		double f##name(double a) { return op (int)a; }

OP_FUNC_BINARY      (||, lor)
OP_FUNC_BINARY      (&&, land)
OP_FUNC_BINARY_CAST (| , bor)
OP_FUNC_BINARY_CAST (^ , bxor)
OP_FUNC_BINARY_CAST (& , band)
OP_FUNC_BINARY      (==, equ)
OP_FUNC_BINARY      (!=, nequ)
OP_FUNC_BINARY      (< , less)
OP_FUNC_BINARY      (<=, lesseq)
OP_FUNC_BINARY      (> , grt)
OP_FUNC_BINARY      (>=, grteq)
OP_FUNC_BINARY_CAST (<<, shl)
OP_FUNC_BINARY_CAST (>>, shr)
OP_FUNC_BINARY      (+ , add)
OP_FUNC_BINARY      (- , sub)
OP_FUNC_BINARY      (* , mul)
OP_FUNC_BINARY      (/ , div)
OP_FUNC_UNARY       (! , lnot)
OP_FUNC_UNARY_CAST  (~ , bnot)
OP_FUNC_UNARY       (++, inc)
OP_FUNC_UNARY       (--, dec)

static size_t oplen(const char* s)
{
	const char* ss = s;
	while (ispunct(*s) || isalpha(*s)) { s++; }

	return s - ss;
}

Operator getOperator(char** s)
{
	#define _(op) (((op) & 0x000000FF) << 24 | ((op) & 0x0000FF00) <<  8  \
	             | ((op) & 0x00FF0000) >>  8 | ((op) & 0xFF000000) >> 24) \
	             >> ((4 - (sizeof(#op) - 3)) * 8)

	#define OperatorTable() \
		op('||'  , 1 , left , 2, { .f2 = flor    }) \
		op('&&'  , 2 , left , 2, { .f2 = fland   }) \
		op('|'   , 3 , left , 2, { .f2 = fbor    }) \
		op('xor' , 4 , left , 2, { .f2 = fbxor   }) \
		op('&'   , 5 , left , 2, { .f2 = fband   }) \
		op('=='  , 6 , left , 2, { .f2 = fequ    }) \
		op('!='  , 6 , left , 2, { .f2 = fnequ   }) \
		op('<'   , 7 , left , 2, { .f2 = fless   }) \
		op('<='  , 7 , left , 2, { .f2 = flesseq }) \
		op('>'   , 7 , left , 2, { .f2 = fgrt    }) \
		op('>='  , 7 , left , 2, { .f2 = fgrteq  }) \
		op('<<'  , 8 , left , 2, { .f2 = fshl    }) \
		op('>>'  , 8 , left , 2, { .f2 = fshr    }) \
		op('+'   , 9 , left , 2, { .f2 = fadd    }) \
		op('-'   , 9 , left , 2, { .f2 = fsub    }) \
		op('*'   , 10, left , 2, { .f2 = fmul    }) \
		op('/'   , 10, left , 2, { .f2 = fdiv    }) \
		op('%'   , 10, left , 2, { .f2 = fmod    }) \
		op('!'   , 11, right, 1, { .f1 = flnot   }) \
		op('~'   , 11, right, 1, { .f1 = fbnot   }) \
		op('++'  , 11, right, 1, { .f1 = finc    }) \
		op('--'  , 11, right, 1, { .f1 = fdec    }) \
		op('^'   , 12, right, 2, { .f2 = pow     }) \
		op('sqrt', 12, right, 1, { .f1 = sqrt    }) \
		op('exp' , 12, right, 1, { .f1 = exp     }) \
		op('log' , 12, right, 1, { .f1 = log     }) \
		op('sin' , 12, right, 1, { .f1 = sin     }) \
		op('cos' , 12, right, 1, { .f1 = cos     }) \
		op('tan' , 12, right, 1, { .f1 = tan     }) \

	int len = oplen(*s);
	int in = *(int *)*s & (0xFFFFFFFF >> (4 - len) * 8);

	switch (in)
	{
		#define op(symbol, preced, assoc, args, f)                \
		    case _(symbol):                                       \
		    {                                                     \
		          Operator operator = { preced, assoc, args, f }; \
		          *s += sizeof(#symbol) - 3;                      \
		          return operator;                                \
		    }                                                     \

		OperatorTable();
	}

	return (Operator){ len, -1, 0, { 0 } }; /* No matching operator */
}
