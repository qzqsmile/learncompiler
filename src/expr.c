#include "c.h"


static char prec[] = {
#define xx(a,b,c,d,e,f,g) c,
#define yy(a,b,c,d,e,f,g) c,
#include "token.h"
};
static int oper[] = {
#define xx(a,b,c,d,e,f,g) d,
#define yy(a,b,c,d,e,f,g) d,
#include "token.h"
};

static Tree expr2(void);
static Tree expr3(int);
static Tree nullcheck(Tree);
static Tree postfix(Tree);
static Tree unary(void);
static Tree primary(void);
static Type super(Type ty);


Tree rightkid(Tree p) {
	while (p && p->op == RIGHT)
		if (p->kids[1])
			p = p->kids[1];
		else if (p->kids[0])
			p = p->kids[0];
		else
			assert(0);
	assert(p);
	return p;
}

// used in assign cond
Tree value(Tree p) {
	// int op = generic(rightkid(p)->op);

	// if (p->type != voidtype
	// && (op==AND || op==OR || op==NOT || op==EQ || op==NE
	// ||  op== LE || op==LT || op== GE || op==GT))
	// 	p = condtree(p, consttree(1, inttype),
	// 		consttree(0, inttype));
	return p;
}

//assign expression, tok is expected token
Tree expr(int tok) {
	static char stop[] = { IF, ID, '}', 0 };
	Tree p = expr1(0);

	while (t == ',') {
		Tree q;
		t = gettok();
		q = pointer(expr1(0));
		p = tree(RIGHT, q->type, root(value(p)), q);
	}
	// instead of test set, test only one token is more precise
	if (tok)	
		test(tok, stop);
	return p;
}

Tree expr0(int tok) {
	return root(expr(tok));
}


Tree expr1(int tok) {
	static char stop[] = { IF, ID, 0 };
	Tree p = expr2();

	if (t == '='
	|| (prec[t] >=  6 && prec[t] <=  8)
	|| (prec[t] >= 11 && prec[t] <= 13)) {
		int op = t;
		t =  gettok();
		if (oper[op] == ASGN)
			p = asgntree(ASGN, p, value(expr1(0)));
		else
			{
				// += /= ...opertors will hit this branch
				expect('=');
				//build a a tree p is left and expr1(0) is right
				p = incr(op, p, expr1(0));
			}
	}
	if (tok)	
		test(tok, stop);
	return p;
}
Tree incr(int op, Tree v, Tree e) {
	// will call addtree function in encode.c
	// return asgntree(ASGN, v, (*optree[op])(oper[op], v, e));
    return v;
}

//array pointer or function pointer
Tree pointer(Tree p) {
	// if (isarray(p->type))
	// 	/* assert(p->op != RIGHT || p->u.sym == NULL), */
	// 	p = retype(p, atop(p->type));
	// else if (isfunc(p->type))
	// 	p = retype(p, ptr(p->type));
	return p;
}


static Tree expr2(void) {
    Tree p = NULL;
	// Tree p = expr3(4);

	// if (t == '?') {
	// 	Tree l, r;
	// 	Coordinate pts[2];
	// 	// test ? a : b if test if a function, is
	// 	if (Aflag > 1 && isfunc(p->type))
	// 		warning("%s used in a conditional expression\n",
	// 			funcname(p));
	// 	p = pointer(p);
	// 	t = gettok();
	// 	pts[0] = src;
	// 	// end in symbol :
	// 	l = pointer(expr(':'));
	// 	pts[1] = src;
	// 	r = pointer(expr2());
	// 	//??????
	// 	if (generic(p->op) != CNST && events.points)
	// 		{
	// 			apply(events.points, &pts[0], &l);
	// 			apply(events.points, &pts[1], &r);
	// 		}
	// 	p = condtree(p, l, r);
	// }
	return p;
}