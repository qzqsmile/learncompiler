#include "c.h"

int errcnt   = 0;
int errlimit = 20;

void error(const char *fmt, ...) {
	// va_list ap;

	// if (errcnt++ >= errlimit) {
	// 	errcnt = -1;
	// 	error("too many errors\n");
	// 	exit(1);
	// }
	// va_start(ap, fmt);
	// if (firstfile != file && firstfile && *firstfile)
	// 	fprint(stderr, "%s: ", firstfile);
	// fprint(stderr, "%w: ", &src);
	// vfprint(stderr, NULL, fmt, ap);
	// va_end(ap);
    printf("test");
}

/* warning - issue warning error message */
void warning(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	// if (wflag == 0) {
	// 	errcnt--;
	// 	error("warning: ");
	// 	vfprint(stderr, NULL, fmt, ap);
	// }
	va_end(ap);
}