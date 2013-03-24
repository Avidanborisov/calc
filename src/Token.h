#ifndef TOKEN_H_
#define TOKEN_H_

#include <AQueue.h>
#include "Operator.h"

typedef struct Token
{
	enum
	{
		operator,
		value,
		openP = '(',
		closeP = ')'
	} type;
	union
	{
		Operator op;
		double v;
	} token;
} Token;

AQueue* tokenize(char* s);

#endif /* TOKEN_H_ */
