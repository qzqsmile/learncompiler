#include "c.h"

static int warn;
int where = STMT;
//will move to another file later
int needconst;


Tree tree(int op, Type type, Tree left, Tree right) {
	Tree p;

	NEW0(p, where);
	p->op = op;
	p->type = type;
	p->kids[0] = left;
	p->kids[1] = right;
	return p;
}

Tree texpr(Tree (*f)(int), int tok, int a) {
	int save = where;
	Tree p;

	where = a;
	p = (*f)(tok);
	where = save;
	return p;
}


static Tree root1(Tree p) {
	// if (p == NULL)
	// 	return p;
	// if (p->type == voidtype)
	// 	warn++;
	// switch (generic(p->op)) {
	// case COND: {
	// 	Tree q = p->kids[1];
	// 	assert(q && q->op == RIGHT);
	// 	if (p->u.sym && q->kids[0] && generic(q->kids[0]->op) == ASGN)
	// 		q->kids[0] = root1(q->kids[0]->kids[1]);
	// 	else
	// 		q->kids[0] = root1(q->kids[0]);
	// 	if (p->u.sym && q->kids[1] && generic(q->kids[1]->op) == ASGN)
	// 		q->kids[1] = root1(q->kids[1]->kids[1]);
	// 	else
	// 		q->kids[1] = root1(q->kids[1]);
	// 	p->u.sym = 0;
	// 	if (q->kids[0] == 0 && q->kids[1] == 0)
	// 		p = root1(p->kids[0]);
	// 	}
	// 	break;
	// case AND: case OR:
	// 	if ((p->kids[1] = root1(p->kids[1])) == 0)
	// 		p = root1(p->kids[0]);
	// 	break;
	// case NOT:
	// 	if (warn++ == 0)
	// 		warning("expression with no effect elided\n");
	// 	return root1(p->kids[0]);
	// case RIGHT:
	// 	if (p->kids[1] == 0)
	// 		return root1(p->kids[0]);
	// 	if (p->kids[0] && p->kids[0]->op == CALL+B
	// 	&&  p->kids[1] && p->kids[1]->op == INDIR+B)
	// 		/* avoid premature release of the CALL+B temporary */
	// 		return p->kids[0];
	// 	if (p->kids[0] && p->kids[0]->op == RIGHT
	// 	&&  p->kids[1] == p->kids[0]->kids[0])
	// 		/* de-construct e++ construction */
	// 		return p->kids[0]->kids[1];
	// 	p = tree(RIGHT, p->type, root1(p->kids[0]), root1(p->kids[1]));
	// 	return p->kids[0] || p->kids[1] ? p : (Tree)0;
	// case EQ:  case NE:  case GT:   case GE:  case LE:  case LT: 
	// case ADD: case SUB: case MUL:  case DIV: case MOD:
	// case LSH: case RSH: case BAND: case BOR: case BXOR:
	// 	if (warn++ == 0)
	// 		warning("expression with no effect elided\n");
	// 	p = tree(RIGHT, p->type, root1(p->kids[0]), root1(p->kids[1]));
	// 	return p->kids[0] || p->kids[1] ? p : (Tree)0;
	// case INDIR:
	// 	if (p->type->size == 0 && unqual(p->type) != voidtype)
	// 		warning("reference to `%t' elided\n", p->type);
	// 	if (isptr(p->kids[0]->type) && isvolatile(p->kids[0]->type->type))
	// 		warning("reference to `volatile %t' elided\n", p->type);
	// 	/* fall thru */
	// case NEG: case BCOM: case FIELD:
	// 	if (warn++ == 0)
	// 		warning("expression with no effect elided\n");
	// 	return root1(p->kids[0]);
	// case ADDRL: case ADDRG: case ADDRF: case CNST:
	// 	if (needconst)
	// 		return p;
	// 	if (warn++ == 0)
	// 		warning("expression with no effect elided\n");
	// 	return NULL;
	// case CVF:
	// 	if (optype(p->op) == I
	// 	|| p->type->size < p->kids[0]->type->size)
	// 		if (warn++ == 0)
	// 			warning("expression with no effect elided\n");
	// 	return root1(p->kids[0]);
	// case CVI:
	// 	if ((optype(p->op) == U || optype(p->op) == I)
	// 	&& p->type->size < p->kids[0]->type->size
	// 	&& specific(p->kids[0]->op) != CALL+I)
	// 		if (warn++ == 0)
	// 			warning("expression with no effect elided\n");
	// 	return root1(p->kids[0]);
	// case CVU: case CVP:
	// 	if (optype(p->op) == U && p->type->size <  p->kids[0]->type->size
	// 	||  optype(p->op) == I && p->type->size <= p->kids[0]->type->size)
	// 		if (warn++ == 0)
	// 			warning("expression with no effect elided\n");
	// 	return root1(p->kids[0]);
	// case ARG: case ASGN: case CALL: case JUMP: case LABEL:
	// 	break;
	// default: assert(0);
	// }
	return p;
}

//section 12.9, so far will return a tree
Tree root(Tree p) {
	// warn = 0;
	return root1(p);
}
