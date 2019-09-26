#include "c.h"

Tree asgntree(int op, Tree l, Tree r) {
	Type aty, ty;

	// r = pointer(r);
	// ty = assign(l->type, r);
	// if (ty)
	// 	r = cast(r, ty);
	// else {
	// 	typeerror(ASGN, l, r);
	// 	if (r->type == voidtype)
	// 		r = retype(r, inttype);
	// 	ty = r->type;
	// }
	// if (l->op != FIELD)
	// 	l = lvalue(l);
	// aty = l->type;
	// if (isptr(aty))
	// 	aty = unqual(aty)->type;
	// if ( isconst(aty)
	// ||  isstruct(aty) && unqual(aty)->u.sym->u.s.cfields)
	// 	if (isaddrop(l->op)
	// 	&& !l->u.sym->computed && !l->u.sym->generated)
	// 		error("assignment to const identifier `%s'\n",
	// 			l->u.sym->name);
	// 	else
	// 		error("assignment to const location\n");
	// if (l->op == FIELD) {
	// 	long n = 8*l->u.field->type->size - fieldsize(l->u.field);
	// 	if (n > 0 && isunsigned(l->u.field->type))
	// 		r = bittree(BAND, r,
	// 			cnsttree(r->type, (unsigned long)fieldmask(l->u.field)));
	// 	else if (n > 0) {
	// 		if (r->op == CNST+I) {
	// 			n = r->u.v.i;
	// 			if (n&(1<<(fieldsize(l->u.field)-1)))
	// 				n |= ~0UL<<fieldsize(l->u.field);
	// 			r = cnsttree(r->type, n);
	// 		} else
	// 			r = shtree(RSH,
	// 				shtree(LSH, r, cnsttree(inttype, n)),
	// 				cnsttree(inttype, n));
	// 	}
	// }
	// if (isstruct(ty) && isaddrop(l->op) && iscallb(r))
	// 	return tree(RIGHT, ty,
	// 		tree(CALL+B, ty, r->kids[0]->kids[0], l),
	// 		idtree(l->u.sym));
	// return tree(mkop(op,ty), ty, l, r);
    return l;
}