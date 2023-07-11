#include "bgr.h"
#include "cur.h"
#include "geo.h"
#include "draw_3d.h"
#include "draw_tile.h"

Bgr::Bgr(Cur* cur) : cur(cur) {
	w = 1630; h = 860;
	tl = { 170, 0 }; dep = -100000;
	black = tile(w, h, dcol{}, 255);

	cm = cam(vp());
}

#include "my_def.h"

bool Bgr::hovered() const { return hvd == this; }
bool Bgr::wheeled() const { return whd == this; }

void Bgr::render_axis() {
	double hlen = 10 * cm.r / cm.scl;
	draw_3d_seg(scr, dscr, cm.look + vec3(hlen, 0, 0), cm.look, cm, dcol(255, 80, 80));
	draw_3d_seg(scr, dscr, cm.look + vec3(0, hlen, 0), cm.look, cm, dcol(80, 255, 80));
	draw_3d_seg(scr, dscr, cm.look + vec3(0, 0, hlen), cm.look, cm, dcol(80, 80, 255));
	draw_3d_seg(scr, dscr, cm.look - vec3(hlen, 0, 0), cm.look, cm, dcol(128, 40, 40));
	draw_3d_seg(scr, dscr, cm.look - vec3(0, hlen, 0), cm.look, cm, dcol(40, 128, 40));
	draw_3d_seg(scr, dscr, cm.look - vec3(0, 0, hlen), cm.look, cm, dcol(40, 40, 128));
}
void Bgr::render() {
	draw_tile_raw(scr, tl, scr.rect(), black, black.rect());
	if (cur->show_axis) { render_axis(); };
}
void Bgr::Update() {
	bool click = hovered() && msd[0] && !msd_old[0];
	if (click) { gsel = NULL; }

	dvec dmsp = msp - msp_old;
	if (dragged_mid) {
		dragged_mid = msd[1];
		if (dragged_mid) {
			if (gsel) {
				double theta = -dmsp.x * 0.005;
				double phi = dmsp.y * 0.005;
				gsel->tsf =
					mat3::rot(cm.mt.tsp().v2(), -theta) *
					mat3::rot(cm.mt.tsp().v0(), phi) * gsel->tsf;
			}
			else {
				cm.theta += -dmsp.x * 0.005;
				cm.phi += dmsp.y * 0.005;
			}
		}
	}
	else { dragged_mid = msd[1] && !msd_old[1] && wheeled(); }
	if (dragged_right) {
		dragged_right = msd[2];
		if (dragged_right) {
			if (gsel) {
				gsel->p = cm.mt * (gsel->p - cm.p);
				gsel->p.z -= dmsp.y / (cm.scl * cm.vp.h) * gsel->p.y;
				gsel->p.x += dmsp.x / (cm.scl * cm.vp.h) * gsel->p.y;
				gsel->p = cm.mt.tsp() * gsel->p + cm.p;
			}
			else {
				cm.look = cm.mt * (cm.look - cm.p);
				cm.look.z += dmsp.y / (cm.scl * cm.vp.h) * cm.look.y;
				cm.look.x -= dmsp.x / (cm.scl * cm.vp.h) * cm.look.y;
				cm.look = cm.mt.tsp() * cm.look + cm.p;
			}
		}
	}
	else { dragged_right = msd[2] && !msd_old[2] && wheeled(); }
	if (!kb) {
		if (kbd[L'Q']) { 
			if (gsel) {
				double psi = -1.2 * dt;
				gsel->tsf =
					mat3::rot(cm.mt.tsp().v1(), psi) * gsel->tsf;
			}
			else { cm.psi -= 1.2 * dt; }
			
		}
		if (kbd[L'E']) { 
			if (gsel) {
				double psi = 1.2 * dt;
				gsel->tsf =
					mat3::rot(cm.mt.tsp().v1(), psi) * gsel->tsf;
			}
			else { cm.psi += 1.2 * dt; }
		}
		if (kbd[L'A']) {
			if (gsel) { gsel->p.x -= 0.8 * dt * cm.r / cm.scl; }
			else { cm.look.x += 0.8 * dt * cm.r / cm.scl; }
		}
		if (kbd[L'D']) {
			if (gsel) { gsel->p.x += 0.8 * dt * cm.r / cm.scl; }
			else { cm.look.x -= 0.8 * dt * cm.r / cm.scl; }
		}
		if (kbd[L'W']) {
			if (gsel) { gsel->p.y -= 0.8 * dt * cm.r / cm.scl; }
			else { cm.look.y += 0.8 * dt * cm.r / cm.scl; }
		}
		if (kbd[L'S']) {
			if (gsel) { gsel->p.y += 0.8 * dt * cm.r / cm.scl; }
			else { cm.look.y -= 0.8 * dt * cm.r / cm.scl; }
		}
		if (kbd[L'R']) {
			if (gsel) { gsel->p.z -= 0.8 * dt * cm.r / cm.scl; }
			else { cm.look.z += 0.8 * dt * cm.r / cm.scl; }
		}
		if (kbd[L'F']) {
			if (gsel) { gsel->p.z += 0.8 * dt * cm.r / cm.scl; }
			else { cm.look.z -= 0.8 * dt * cm.r / cm.scl; }
		}
		if (kbd[L'T']) {
			cm.r = exp(log(cm.r) - 0.8 * dt);
		}
		if (kbd[L'G']) {
			cm.r = exp(log(cm.r) + 0.8 * dt);
		}
	}
	if (wheeled()) {
		cm.scl = exp(log(cm.scl) + msw * 0.1);
	}
	cm.theta = modf(cm.theta, 2 * PI);
	cm.psi = modf(cm.psi, 2 * PI);
	cm.phi = clmp(cm.phi, -PI / 2 + 0.001, PI / 2 - 0.001);

	cm.calc(); render();
}
void Bgr::PreUpdate() {
	bool ok = dhv <= dep && insd(msp, vp());
	if (ok) { dhv = dep; hvd = this; }

	ok = dwh <= dep && insd(msp, vp());
	if (ok) { dwh = dep; whd = this; }
}
