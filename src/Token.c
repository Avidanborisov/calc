#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <AQueue.h>
#include "Token.h"
#include "Error.h"

static char* nextToken(char* s, Token* t);

AQueue* tokenize(char* s)
{
	AQueue* expr = AStruct->ANew(AQueue);
	Token t;

	while ((s = nextToken(s, &t)) != NULL)
	{
		if (errorOccurred)
		{
			expr->destroy(expr, free);
			return NULL;
		}

		expr->enqueue(expr, AStruct->ADup(t));
	}

	return expr;
}

static char* nextToken(char* s, Token* t)
{
	char* end;

	while ((isspace(*s))) { s++; }

	if (*s == openP || *s == closeP)
	{
		t->type = *s;
		return ++s;
	}

	if (!isdigit(*s) && *s != '\0')
	{
		if ((t->token.op = getOperator(&s)).args > 0)
		{
			t->type = operator;
			return s;
		}

		error("Unknown operator: %.*s", t->token.op.preced, s);
		return NULL;
	}

	if (t->token.v = strtod(s, &end), s != end)
	{
		t->type = value;
		return end;
	}

	return NULL;
}
