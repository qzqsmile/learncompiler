#include "c.h"

void fprint(FILE *f, const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	// vfprint(f, NULL, fmt, ap);
	va_end(ap);
    
}


/* stringf - formatted output to a saved string */
char *stringf(const char *fmt, ...) {
	char buf[1024];
	va_list ap;

	va_start(ap, fmt);
	// vfprint(NULL, buf, fmt, ap);
	fprintf(NULL, buf, fmt, ap);
	va_end(ap);
	return string(buf);
}

