#include "par.h"

param::param(wstring const& nm) : nm(nm) {}
void param::save(FILE* f) {
	int sz = 0;
	fwtv(nm); fwt(val);
}
param param::load(FILE* f) {
	param p; int sz = 0;
	frdv(p.nm); frd(p.val);
	return p;
}
