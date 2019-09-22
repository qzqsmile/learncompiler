#include <float.h>
#include "c.h"

static Type type(int op, Type ty, int size, int align, void *sym);
static Symbol pointersym;


Type chartype;			/* char */
Type doubletype;		/* double */
Type floattype;			/* float */
Type inttype;			/* signed int */
Type longdouble;		/* long double */
Type longtype;			/* long */
Type longlong;			/* long long */
Type shorttype;			/* signed short int */
Type signedchar;		/* signed char */
Type unsignedchar;		/* unsigned char */
Type unsignedlong;		/* unsigned long int */
Type unsignedlonglong;		/* unsigned long long int */
Type unsignedshort;		/* unsigned short int */
Type unsignedtype;		/* unsigned int */
Type funcptype;			/* void (*)() */
Type charptype;			/* char* */
Type voidptype;			/* void* */
Type voidtype;			/* basic types: void */
Type unsignedptr;		/* unsigned type to hold void* */
Type signedptr;			/* signed type to hold void* */
Type widechar;			/* unsigned type that represents wchar_t */


static struct entry {
	struct type type;
	struct entry *link;
} *typetable[128];
static int maxlevel;

static Type xxinit(int op, char *name, Metrics m) {
	Symbol p = install(string(name), &types, GLOBAL, PERM);
	Type ty = type(op, 0, m.size, m.align, p);

	assert(ty->align == 0 || ty->size%ty->align == 0);
	p->type = ty;
	p->addressed = m.outofline;
	switch (ty->op) {
	case INT:
		p->u.limits.max.i = ones(8*ty->size)>>1;
		p->u.limits.min.i = -p->u.limits.max.i - 1;
		break;
	case UNSIGNED:
		p->u.limits.max.u = ones(8*ty->size);
		p->u.limits.min.u = 0;
		break;
	case FLOAT:
		if (ty->size == sizeof (float))
			p->u.limits.max.d =  FLT_MAX;
		else if (ty->size == sizeof (double))
			p->u.limits.max.d =  DBL_MAX;
		else
			p->u.limits.max.d = LDBL_MAX;
		p->u.limits.min.d = -p->u.limits.max.d;
		break;
	default: assert(0);
	}
	return ty;
}

void rmtypes(int lev) {
	if (maxlevel >= lev) {
		int i;
		maxlevel = 0;
		for (i = 0; i < NELEMS(typetable); i++) {
			struct entry *tn, **tq = &typetable[i];
			while ((tn = *tq) != NULL)
				if (tn->type.op == 1) // modified
					tq = &tn->link;
				else if (tn->type.u.sym && tn->type.u.sym->scope >= lev)
					*tq = tn->link;
				else {
					if (tn->type.u.sym && tn->type.u.sym->scope > maxlevel)
						maxlevel = tn->type.u.sym->scope;
					tq = &tn->link;
				}

		}
	}
}

static Type type(int op, Type ty, int size, int align, void *sym) {
	unsigned h = (op^((unsigned long)ty>>3))
&(NELEMS(typetable)-1);
	struct entry *tn;

	//this type if existed??
	if (op != FUNCTION && (op != ARRAY || size > 0))
		for (tn = typetable[h]; tn; tn = tn->link)
			if (tn->type.op    == op   && tn->type.type  == ty
			&&  tn->type.size  == size && tn->type.align == align
			&&  tn->type.u.sym == sym)
				return &tn->type;
	NEW0(tn, PERM);
	tn->type.op = op;
	tn->type.type = ty;
	tn->type.size = size;
	tn->type.align = align;
	tn->type.u.sym = sym;
	tn->link = typetable[h];
	typetable[h] = tn;
	return &tn->type;
}

void type_init(int argc, char *argv[]) {
	static int inited;
	int i;

// 	if (inited)
// 		return;
// 	inited = 1;
// 	if (!IR)
// 		return;
// 	for (i = 1; i < argc; i++) {
// 		int size, align, outofline;
// 		if (strncmp(argv[i], "-unsigned_char=", 15) == 0)
// 			IR->unsigned_char = argv[i][15] - '0';
// #define xx(name) \
// 		else if (sscanf(argv[i], "-" #name "=%d,%d,%d", &size, &align, &outofline) == 3) { \
// 			IR->name.size = size; IR->name.align = align; \
// 			IR->name.outofline = outofline; }
// 	xx(charmetric)
// 	xx(shortmetric)
// 	xx(intmetric)
// 	xx(longmetric)
// 	xx(longlongmetric)
// 	xx(floatmetric)
// 	xx(doublemetric)
// 	xx(longdoublemetric)
// 	xx(ptrmetric)
// 	xx(structmetric)
// #undef xx
// 	}
#define xx(v,name,op,metrics) v=xxinit(op,name,IR->metrics)
	xx(chartype,        "char",              IR->unsigned_char ? UNSIGNED : INT,charmetric);
    xx(chartype,        "char",              INT, charmetric);
	xx(doubletype,      "double",            FLOAT,   doublemetric);
	xx(floattype,       "float",             FLOAT,   floatmetric);
	xx(inttype,         "int",               INT,     intmetric);
	xx(longdouble,      "long double",       FLOAT,   longdoublemetric);
	xx(longtype,        "long int",          INT,     longmetric);
	xx(longlong,        "long long int",     INT,     longlongmetric);
	xx(shorttype,       "short",             INT,     shortmetric);
	xx(signedchar,      "signed char",       INT,     charmetric);
	xx(unsignedchar,    "unsigned char",     UNSIGNED,charmetric);
	xx(unsignedlong,    "unsigned long",     UNSIGNED,longmetric);
	xx(unsignedshort,   "unsigned short",    UNSIGNED,shortmetric);
	xx(unsignedtype,    "unsigned int",      UNSIGNED,intmetric);
	xx(unsignedlonglong,"unsigned long long",UNSIGNED,longlongmetric);
#undef xx
	{
		Symbol p;
		p = install(string("void"), &types, GLOBAL, PERM);
		voidtype = type(VOID, NULL, 0, 0, p);
		p->type = voidtype;
	}
	pointersym = install(string("T*"), &types, GLOBAL, PERM);
	pointersym->addressed = IR->ptrmetric.outofline;
	pointersym->u.limits.max.p = (void*)ones(8*IR->ptrmetric.size);
	pointersym->u.limits.min.p = 0;
	voidptype = ptr(voidtype);
	funcptype = ptr(func(voidtype, NULL, 1));
	charptype = ptr(chartype);
#define xx(v,t) if (v==NULL && t->size==voidptype->size && t->align==voidptype->align) v=t
	xx(unsignedptr,unsignedshort);
	xx(unsignedptr,unsignedtype);
	xx(unsignedptr,unsignedlong);
	xx(unsignedptr,unsignedlonglong);
	if (unsignedptr == NULL)
		unsignedptr = type(UNSIGNED, NULL, voidptype->size, voidptype->align, voidptype->u.sym);
	xx(signedptr,shorttype);
	xx(signedptr,inttype);
	xx(signedptr,longtype);
	xx(signedptr,longlong);
	if (signedptr == NULL)
		signedptr = type(INT, NULL, voidptype->size, voidptype->align, voidptype->u.sym);
#undef xx
	widechar = unsignedshort;
	for (i = 0; i < argc; i++) {
#define xx(name,type) \
		if (strcmp(argv[i], "-wchar_t=" #name) == 0) \
			widechar = type;
		xx(unsigned_char,unsignedchar)
		xx(unsigned_int,unsignedtype)
		xx(unsigned_short,unsignedshort)
	}
#undef xx
}

int eqtype(Type ty1, Type ty2, int ret) {
	if (ty1 == ty2)
		return 1;
	if (ty1->op != ty2->op)
		return 0;
	switch (ty1->op) {
	case ENUM: case UNION: case STRUCT:
	case UNSIGNED: case INT: case FLOAT:
		return 0;
	case POINTER:  return eqtype(ty1->type, ty2->type, 1);
	case VOLATILE: case CONST+VOLATILE:
	case CONST:    return eqtype(ty1->type, ty2->type, 1);
	case ARRAY:    if (eqtype(ty1->type, ty2->type, 1)) {
		       	if (ty1->size == ty2->size)
		       		return 1;
		       	if (ty1->size == 0 || ty2->size == 0)
		       		return ret;
		       }
		       return 0;
	case FUNCTION: if (eqtype(ty1->type, ty2->type, 1)) {
		       	Type *p1 = ty1->u.f.proto, *p2 = ty2->u.f.proto;
		       	if (p1 == p2)
		       		return 1;
		       	if (p1 && p2) {
		       		for ( ; *p1 && *p2; p1++, p2++)
					if (eqtype(unqual(*p1), unqual(*p2), 1) == 0)
						return 0;
				if (*p1 == NULL && *p2 == NULL)
					return 1;
		       	} else {
		       		if (variadic(p1 ? ty1 : ty2))
					return 0;
				if (p1 == NULL)
					p1 = p2;
				for ( ; *p1; p1++) {
					Type ty = unqual(*p1);
					if (promote(ty) != (isenum(ty) ? ty->type : ty))
						return 0;
				}
				return 1;
		       	}
		       }
		       return 0;
	}
	assert(0); return 0;
}


Type btot(int op, int size) {
#define xx(ty) if (size == (ty)->size) return ty;
	switch (optype(op)) {
	case F:
		xx(floattype);
		xx(doubletype);
		xx(longdouble);
		assert(0); return 0;
	case I:
		if (chartype->op == INT)
			xx(chartype);
		xx(signedchar);
		xx(shorttype);
		xx(inttype);
		xx(longtype);
		xx(longlong);
		assert(0); return 0;
	case U:
		if (chartype->op == UNSIGNED)
			xx(chartype);
		xx(unsignedchar);
		xx(unsignedshort);
		xx(unsignedtype);
		xx(unsignedlong);
		xx(unsignedlonglong);
		assert(0); return 0;
	case P:
		xx(voidptype);
		xx(funcptype);
		assert(0); return 0;
	}
#undef xx
	assert(0); return 0;
}

Type func(Type ty, Type *proto, int style) {
	if (ty && (isarray(ty) || isfunc(ty)))
		error("illegal return type `%t'\n", ty);
	ty = type(FUNCTION, ty, 0, 0, NULL);
	ty->u.f.proto = proto;
	ty->u.f.oldstyle = style;
	return ty;
}

Type array(Type ty, int n, int a) {
	assert(ty);
	if (isfunc(ty)) {
		error("illegal type `array of %t'\n", ty);
		return array(inttype, n, 0);
	}
	if (isarray(ty) && ty->size == 0)
		error("missing array size\n");
	if (ty->size == 0) {
		if (unqual(ty) == voidtype)
			error("illegal type `array of %t'\n", ty);
		else if (Aflag >= 2)
			warning("declaring type array of %t' is undefined\n", ty);

	} else if (n > INT_MAX/ty->size) {
		error("size of `array of %t' exceeds %d bytes\n",
			ty, INT_MAX);
		n = 1;
	}
	return type(ARRAY, ty, n*ty->size,
		a ? a : ty->align, NULL);
}

Type promote(Type ty) {
	ty = unqual(ty);
	switch (ty->op) {
	case ENUM:
		return inttype;
	case INT:
		if (ty->size < inttype->size)
			return inttype;
		break;
	case UNSIGNED:
		if (ty->size < inttype->size)
			return inttype;
		if (ty->size < unsignedtype->size)
			return unsignedtype;
		break;
	case FLOAT:
		if (ty->size < doubletype->size)
			return doubletype;
	}
	return ty;
}

int variadic(Type ty) {
	if (isfunc(ty) && ty->u.f.proto) {
		int i;
		for (i = 0; ty->u.f.proto[i]; i++)
			;
		return i > 1 && ty->u.f.proto[i-1] == voidtype;
	}
	return 0;
}


Type ptr(Type ty) {
	return type(POINTER, ty, IR->ptrmetric.size,
		IR->ptrmetric.align, pointersym);
}


