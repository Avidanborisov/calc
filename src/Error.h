#ifndef ERROR_H_
#define ERROR_H_

#include <stdarg.h>
#include <stdlib.h>

extern int errorOccurred;

#define error(fmt, ...)                                 \
	fprintf(stderr, "error: " fmt "\n", ##__VA_ARGS__); \
	errorOccurred = 1;                                  \

#endif /* ERROR_H_ */
