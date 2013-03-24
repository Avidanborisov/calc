#ifndef OPERATOR_H_
#define OPERATOR_H_

typedef struct Operator
{
	int preced;
	enum { left, right } assoc;
	int args;
	union
	{
		double (*f1)(double);
		double (*f2)(double, double);
	} f;
} Operator;

Operator getOperator(char** s);

#endif /* OPERATOR_H_ */
