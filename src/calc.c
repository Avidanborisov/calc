#include <stdio.h>
#include <string.h>
#include "Error.h"
#include "Token.h"
#include "Postfix.h"
#include "Infix.h"

#define MAXLINE 256

typedef double (*Notation)(AQueue* expr);
void calculate(char* expr, Notation notation);

int main(int argc, char* argv[])
{
	char line[MAXLINE];
	Notation notation = infix;

	if (argc > 1)
	{
		char* expr = argv[1];

		if (strcmp(argv[1], "-p") == 0)
		{
			notation = postfix;
			expr = argv[2];
		}

		calculate(expr, notation);
		return errorOccurred;
	}

	while (printf("> "), fgets(line, MAXLINE, stdin) != NULL)
	{
		if (line[0] == '\n')
		{
			continue;
		}

		calculate(line, infix);
	}

	putchar('\n');
	return 0;
}

void calculate(char* expr, Notation notation)
{
	errorOccurred = 0;
	AQueue* tokens = tokenize(expr);
	double result = 0;

	if (tokens != NULL)
	{
		result = notation(tokens);
	}

	if (!errorOccurred)
	{
		printf("%g\n", result);
	}
}
