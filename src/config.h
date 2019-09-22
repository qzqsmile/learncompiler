#include "c.h"

typedef struct {
	Symbol vbl;
	short set;
	short number;
	unsigned mask;
} *Regnode;

typedef struct {
	char *name;
	unsigned int eaddr;  /* omit */
	int offset;
	Node lastuse;
	int usecount;
	Regnode regnode;
	Symbol *wildcard;
} Xsymbol;

