#include <stdio.h>
#include <stdlib.h>
#include <AQueue.h>
#include <AStack.h>
#include "Operator.h"
#include "Token.h"
#include "Error.h"

static void execOperator(AStack* values, Operator op);

double postfix(AQueue* expr)
{
	Token* t;
	AStack* values = AStruct->ANew(AStack);
	double* r;
	double result;

	while ((t = expr->dequeue(expr)) != NULL)
	{
		switch (t->type)
		{
			case value:
				values->push(values, AStruct->ADup(t->token.v));
				break;

			case operator:
				execOperator(values, t->token.op);
				break;

			default:
				break;
		}

		free(t);

		if (values->top(values) == NULL)
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

static void execOperator(AStack* values, Operator op)
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
				return;
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
				return;
			}

			result = op.f.f2(*v1, *v2);
			free(v1);
			free(v2);
			break;
	}

	values->push(values, AStruct->ADup(result));
}
