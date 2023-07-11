#include "draw_px_seg.h"
#include "draw_px_tri.h"

// 好像这里经常发生溢出问题，不知能不能处理一下。
// 把 int 换成 long long 了，应该会有帮助。
bool pre_draw_px_seg(dvec& pa, dvec& pb, drect vp) {
	long long x0 = vp.left(); long long x1 = vp.right() - 1;
	long long y0 = vp.top(); long long y1 = vp.bottom() - 1;

	// 下面假设了不会发生除以 0 的问题。
#define TMP(a, b, x, y, bd, sym)\
if (p##a.x sym bd) {\
	p##a.y +=  (p##b.y - p##a.y) * (bd - p##a.x) / (p##b.x - p##a.x);\
	p##a.x = bd;\
}

	bool b0 = pa.x < x0 && pb.x < x0;
	bool b1 = pa.x > x1 && pb.x > x1;
	if (b0 || b1) { return false; }
	TMP(a, b, x, y, x0, < );
	TMP(a, b, x, y, x1, > );
	TMP(b, a, x, y, x0, < );
	TMP(b, a, x, y, x1, > );

	b0 = pa.y < y0 && pb.y < y0;
	b1 = pa.y > y1 && pb.y > y1;
	if (b0 || b1) { return false; }
	TMP(a, b, y, x, y0, < );
	TMP(a, b, y, x, y1, > );
	TMP(b, a, y, x, y0, < );
	TMP(b, a, y, x, y1, > );

#undef TMP
	return true;
}
void draw_px_seg
(tile& dest, dbuf& ds, dvec pa, dvec pb, double d, drect vp, dcol c) {
	if (vp.w <= 0 || vp.h <= 0) { return; }
	if (!pre_draw_px_seg(pa, pb, vp)) { return; }

	bool changed = false;
	int x = pa.x; int y = pa.y;
	int dx = abs(pb.x - pa.x);
	int dy = abs(pb.y - pa.y);
	int sx = (pb.x > pa.x) ? 1 : -1;
	int sy = (pb.y > pa.y) ? 1 : -1;
	if (dy > dx) { swap(dx, dy); changed = true; }

	int e = dx;
	int id = y * dest.w + x;
	if (ds[id] <= d) { ds[id] = d; dest.cols[id] = c; }

	rep(i, 0, dx) {
		e -= 2 * dy;
		if (changed) { y += sy; }
		else { x += sx; }
		if (e < 0) {
			e += 2 * dx;
			if (changed) { x += sx; }
			else { y += sy; }
		}
		int id = y * dest.w + x;
		if (ds[id] <= d) { ds[id] = d; dest.cols[id] = c; }
	}
}
void draw_px_seg
(tile& dest, dbuf& ds, vec2 pa, vec2 pb, double d, drect vp, dcol c) {
	draw_px_seg(dest, ds, (dvec)pa, (dvec)pb, d, vp, c);
}

bool pre_draw_px_seg
(dvec& pa, dvec& pb, double& da, double& db, cam const& cm) {
	long long x0 = cm.vp.left(); long long x1 = cm.vp.right() - 1;
	long long y0 = cm.vp.top(); long long y1 = cm.vp.bottom() - 1;

#define TMP(a, b, x, y, bd, sym)\
if (p##a.x sym bd) {\
	p##a.y += (p##b.y - p##a.y) * (bd - p##a.x) / (p##b.x - p##a.x);\
	d##a = interp((p##a.x - cm.ct.x) * d##a, (p##b.x - cm.ct.x) * d##b,\
		bd - cm.ct.x, d##a, d##b, d##a, d##b);\
	p##a.x = bd;\
}

	bool b0 = pa.x < x0&& pb.x < x0;
	bool b1 = pa.x > x1 && pb.x > x1;
	if (b0 || b1) { return false; }
	TMP(a, b, x, y, x0, < );
	TMP(a, b, x, y, x1, > );
	TMP(b, a, x, y, x0, < );
	TMP(b, a, x, y, x1, > );

	b0 = pa.y < y0&& pb.y < y0;
	b1 = pa.y > y1 && pb.y > y1;
	if (b0 || b1) { return false; }
	TMP(a, b, y, x, y0, < );
	TMP(a, b, y, x, y1, > );
	TMP(b, a, y, x, y0, < );
	TMP(b, a, y, x, y1, > );

#undef TMP
	return true;
}
void draw_px_seg(tile& dest, dbuf& ds, dvec pa, dvec pb,
	double da, double db, cam const& cm, dcol c) {
	if (cm.vp.w <= 0 || cm.vp.h <= 0) { return; }
	if (!pre_draw_px_seg(pa, pb, da, db, cm)) { return; }

	bool changed = false;
	int x = pa.x; int y = pa.y;
	int dx = abs(pb.x - pa.x);
	int dy = abs(pb.y - pa.y);
	int sx = (pb.x > pa.x) ? 1 : -1;
	int sy = (pb.y > pa.y) ? 1 : -1;
	double x0 = (pa.x - cm.ct.x) * da;
	double x1 = (pb.x - cm.ct.x) * db;
	double y0 = (pa.y - cm.ct.y) * da;
	double y1 = (pb.y - cm.ct.y) * db;
	if (dy > dx) { swap(dx, dy); changed = true; }

	int e = dx;
	int id = y * dest.w + x;
	double d = -da;
	if (ds[id] <= d) { ds[id] = d; dest.cols[id] = c; }

	rep(i, 0, dx) {
		e -= 2 * dy;
		if (changed) { y += sy; }
		else { x += sx; }
		if (e < 0) {
			e += 2 * dx;
			if (changed) { x += sx; }
			else { y += sy; }
		}
		int id = y * dest.w + x;
		if (changed) { d = -interp(y0, y1, y - cm.ct.y, da, db, da, db); }
		else { d = -interp(x0, x1, x - cm.ct.x, da, db, da, db); }
		if (ds[id] <= d) { ds[id] = d; dest.cols[id] = c; }
	}
}
