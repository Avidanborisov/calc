#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include "Infix.h"
#include "Error.h"

double infix(AQueue* expr)
{
	AQueue* output = AStruct->ANew(AQueue);
	AStack* operators = AStruct->ANew(AStack);
	Token* t;

	while ((t = expr->dequeue(expr)) != NULL)
	{
		switch (t->type)
		{
			case value:
				output->enqueue(output, t);
				break;

			case operator:
				while (operators->size > 0)
				{
					Token* tk = operators->top(operators);

					if (tk->type != openP)
					{
						Operator new = t->token.op;
						Operator top = tk->token.op;

						if (new.preced - top.preced + (int)new.assoc <= 0)
						{
							output->enqueue(output, operators->pop(operators));
							continue;
						}
					}

					break;
				}

				operators->push(operators, t);
				break;

			case openP:
				operators->push(operators, t);
				break;

			case closeP:
				while (operators->size > 0 &&
						((Token *)operators->top(operators))->type != openP)
				{
					output->enqueue(output, operators->pop(operators));
				}

				if (operators->size == 0)
				{
					error("Mismatched parentheses");
					expr->destroy(expr, free);
					operators->destroy(operators, free);
					output->destroy(output, free);
					return 0;
				}

				free(operators->pop(operators));
		}
	}

	while (operators->size > 0)
	{
		output->enqueue(output, operators->pop(operators));
	}

	expr->destroy(expr, NULL);
	operators->destroy(operators, NULL);

	return postfix(output);
}
