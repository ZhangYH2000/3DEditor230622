#include "tile.h"

dcol dcol::from_str(wstring const& s) {
	dcol c; int i = 0;
	c.r = fetch_num(i, s);
	c.g = fetch_num(i, s);
	c.b = fetch_num(i, s);
	return c;
}
wstring tw(dcol c) {
	return L"(" + tw(c.r) + L"," + tw(c.g) + L"," + tw(c.b) + L")";
}

tile::tile(int w, int h, dcol c, BYTE a) :
	w(w), h(h), cols(n(), c), as(n(), a) {}
int tile::n() const { return w * h; }
drect tile::rect() const { return { w, h }; }
void tile::fill(dcol c, BYTE a) {
	rep(i, 0, n()) { cols[i] = c; }
	memset(as.data(), a, n());
}

void tile::save(wstring const& nm) const {
	FILE* f = wfopen(nm, L"wb");
	fwt(w); fwt(h);
	fwts(&cols[0], n());
	fwts(&as[0], n());
	fclose(f);
}
tile tile::load(wstring const& nm, bool* ok) {
	FILE* f = wfopen(nm, L"rb");
	if (ok) { *ok = f; }
	if (!f) { return {}; }

	int w = 0, h = 0;
	frd(w); frd(h);

	tile tmp(w, h);
	frds(&tmp.cols[0], tmp.n());
	frds(&tmp.as[0], tmp.n());
	fclose(f); return tmp;
}

void copy_tile(tile& dest, tile const& src, drect vp_src) {
	rep(i, 0, dest.w) rep(j, 0, dest.h) {
		dvec d_pnt(i, j);
		dvec s_pnt = d_pnt *
			dvec(vp_src.w, vp_src.h) / dvec(dest.w, dest.h);

		int dp = d_pnt.y * dest.w + d_pnt.x;
		int sp = (s_pnt.y + vp_src.top()) * src.w +
			s_pnt.x + vp_src.left();
		dest.cols[dp] = src.cols[sp];
		dest.as[dp] = src.as[sp];
	}
}
