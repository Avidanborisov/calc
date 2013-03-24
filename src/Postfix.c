#include <stdio.h>
#include <stdlib.h>
#include <AQueue.h>
#include <AStack.h>
#include "Operator.h"
#include "Token.h"
#include "Error.h"

static int execOperator(AStack* values, Operator op);

double postfix(AQueue* expr)
{
	Token* t;
	AStack* values = AStruct->ANew(AStack);
	double* r;
	double result;

	while ((t = expr->dequeue(expr)) != NULL)
	{
		int ok = 1;

		switch (t->type)
		{
			case value:
				values->push(values, AStruct->ADup(t->token.v));
				break;

			case operator:
				ok = execOperator(values, t->token.op);
				break;

			default:
				break;
		}

		free(t);

		if (!ok)
		{
			break;
		}
	}

	if (values->size != 1)
	{
		error("Wrong number of arguments");
		values->destroy(values, free);
		expr->destroy(expr, NULL);
		return 0;
	}

	r = values->pop(values);
	result = *r;

	free(r);
	values->destroy(values, free);
	expr->destroy(expr, NULL);

	return result;
}

static int execOperator(AStack* values, Operator op)
{
	double result;
	double* v1;
	double* v2;

	switch (op.args)
	{
		case 1:
			v1 = values->pop(values);

			if (v1 == NULL)
			{
				return 0;
			}

			result = op.f.f1(*v1);
			free(v1);
			break;

		case 2:
			v2 = values->pop(values);
			v1 = values->pop(values);

			if (v1 == NULL || v2 == NULL)
			{
				free(v2);
				return 0;
			}

			result = op.f.f2(*v1, *v2);
			free(v1);
			free(v2);
			break;
	}

	values->push(values, AStruct->ADup(result));
	return 1;
}
