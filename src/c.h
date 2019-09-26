#ifndef C_H
#define C_H

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#define NELEMS(a) ((int)(sizeof (a)/sizeof ((a)[0])))
#define isqual(t)     ((t)->op >= CONST)
#define unqual(t)     (isqual(t) ? (t)->type : (t))

#define	MAXLINE  512
#define	BUFSIZE 4096

#undef roundup
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))
#define NEW(p,a) ((p) = allocate(sizeof *(p), (a)))
#define NEW0(p,a) memset(NEW((p),(a)), 0, sizeof *(p))
#define isenum(t)  (unqual(t)->op == ENUM)
#define optype(op)  ((op)&0xF)
#define isfunc(t)     (unqual(t)->op == FUNCTION)
#define isarray(t)    (unqual(t)->op == ARRAY)
#define ones(n) ((n)>=8*sizeof (unsigned long) ? ~0UL : ~((~0UL)<<(n)))
#define extend(x,ty) ((x)&(1<<(8*(ty)->size-1)) ? (x)|((~0UL)<<(8*(ty)->size-1)) : (x)&ones(8*(ty)->size))
#define ischar(t)     ((t)->size == 1 && isint(t))
#define isint(t)      (unqual(t)->op == INT \
                    || unqual(t)->op == UNSIGNED)


#define gop(name,value) name=value<<4,
#define op(name,type,sizes)
enum {
#include "ops.h"
	LASTOP
};

typedef struct list *List;
typedef struct table *Table;
typedef struct type *Type;
typedef struct node *Node;


typedef struct coord {
	char *file;
	unsigned x, y;
} Coordinate;
typedef struct table *Table;

typedef struct field *Field;
typedef struct symbol *Symbol;
typedef struct tree *Tree;

typedef struct {
	unsigned printed:1;
	unsigned marked;
	unsigned short typeno;
	void *xt;
} Xtype;

#include "config.h"
enum { CONSTANTS=1, LABELS, GLOBAL, PARAM, LOCAL };


struct type {
	int op;
	Type type;
	int align;
	int size;
	union {
		Symbol sym;
		struct {
			unsigned oldstyle:1;
			Type *proto;
		} f;
	} u;
	Xtype x;
};

typedef struct metrics {
	unsigned char size, align, outofline;
} Metrics;

typedef struct interface {
	Metrics charmetric;
	Metrics shortmetric;
	Metrics intmetric;
	Metrics longmetric;
	Metrics longlongmetric;
	Metrics floatmetric;
	Metrics doublemetric;
	Metrics longdoublemetric;
	Metrics ptrmetric;
	Metrics structmetric;
	unsigned unsigned_char:1;

	void (*defsymbol)(Symbol);
	void (*local)(Symbol);
} Interface;

typedef union value {
	long i;
	unsigned long u;
	long double d;
	void *p;
	void (*g)(void);
} Value;

struct symbol {
	char *name;
	int scope;
	Coordinate src;
	//?
	Symbol up;
	// keep track of every use of every symbol, see exercise3.4
	List uses;
	// auto, register, static, extern, 
	int sclass;
	// unsigned structarg:1;

	unsigned addressed:1;
	// unsigned computed:1;
	unsigned temporary:1;
	unsigned generated:1;
	unsigned defined:1;
	// variables, functions, constants, structuture, union, enumeration
	Type type;
	//reference count, why float?
	float ref;
	//extra instructions
	union {
		struct {
			int label;
			Symbol equatedto;
		} l;
		struct {
			unsigned cfields:1;
			unsigned vfields:1;
			Table ftab;		/* omit */
			Field flist;
		} s;
		int value;
		Symbol *idlist;
		struct {
			Value min, max;
		} limits;
		struct {
			Value v;
			Symbol loc;
		} c;
		struct {
			Coordinate pt;
			int label;
			int ncalls;
			Symbol *callee;
		} f;
		int seg;
		Symbol alias;
		struct {
			Node cse;
			int replace;
			Symbol next;
		} t;
	} u;
	Xsymbol x;
};

struct tree {
	int op;
	Type type;
	Tree kids[2];
	Node node;
	union {
		Value v;
		Symbol sym;

		Field field;
	} u;
};


enum {
#define xx(a,b,c,d,e,f,g) a=b,
#define yy(a,b,c,d,e,f,g)
#include "token.h"
	LAST
};

enum {
	F=FLOAT,
	I=INT,
	U=UNSIGNED,
	P=POINTER,
	V=VOID,
	B=STRUCT
};

enum {
	AND=38<<4,
	NOT=39<<4,
	OR=40<<4,
	COND=41<<4,
	RIGHT=42<<4,
	FIELD=43<<4
};

extern char *firstfile;
extern char *file;
extern Coordinate src;
extern Table constants;
extern Table externals;
extern Table globals;
extern Table identifiers;
extern Table labels;
extern Table types;
extern Interface *IR;
extern Type inttype;
extern int Aflag;

enum { PERM=0, FUNC, STMT };

extern int where;
extern int t;
extern Type chartype;
extern Type doubletype;
extern Type floattype;
extern Type inttype;
extern Type longdouble;
extern Type longtype;
extern Type longlong;
extern Type shorttype;
extern Type signedchar;
extern Type unsignedchar;
extern Type unsignedlonglong;
extern Type unsignedlong;
extern Type unsignedshort;
extern Type unsignedtype;
extern Type charptype;
extern Type funcptype;
extern Type voidptype;
extern Type voidtype;
extern Type unsignedptr;
extern Type signedptr;
extern Type widechar;
extern unsigned char *limit;
extern char *line;
extern int lineno;
extern unsigned char *cp;
extern char *token;
extern Symbol tsym;
extern int t;

extern void error(const char *, ...);
extern void deallocate(unsigned a);
extern void *allocate(unsigned long n, unsigned a);
extern void *newarray(unsigned long m, unsigned long n, unsigned a);
extern char *stringn(const char *str, int len);
extern char *string(const char *str);
extern void error(const char *fmt, ...);
extern char *stringd(long n);
extern int eqtype(Type ty1, Type ty2, int ret);
extern void fprint(FILE *f, const char *fmt, ...);
extern Symbol newtemp(int sclass, int tc, int size);
extern Symbol temporary(int scls, Type ty);
extern Type btot(int op, int size);
extern Type promote(Type ty);
extern char *vtoa(Type ty, Value v);
extern Type func(Type ty, Type *proto, int style);
extern Type ptr(Type ty);
extern char *vtoa(Type ty, Value v);
extern char *stringf(const char *fmt, ...);
extern Symbol install(const char *name, Table *tpp, int level, int arena);
extern void rmtypes(int lev);
extern void warning(const char *fmt, ...);
extern int variadic(Type ty);
extern void input_init(int argc, char *argv[]);
extern void fillbuf(void);
extern void nextline(void);
extern Type array(Type ty, int n, int a); 
extern int gettok(void); 
extern void use(Symbol p, Coordinate src);
extern Symbol lookup(const char *name, Table tp);
extern void type_init(int argc, char *argv[]);
extern void skipto(int tok, char set[]);
extern void expect(int tok);
extern Tree tree(int op, Type type, Tree left, Tree right);
extern Tree texpr(Tree (*f)(int), int tok, int a);
extern Tree root(Tree p);
extern Tree expr1(int tok);
extern Tree asgntree(int, Tree, Tree);
extern Tree pointer(Tree p);
extern Tree value(Tree p);
extern Tree rightkid(Tree p);
extern void test(int tok, char set[]);
extern Tree incr(int op, Tree v, Tree e);

#endif