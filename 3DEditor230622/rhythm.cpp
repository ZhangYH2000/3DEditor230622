#include "rhythm.h"
#include "app.h"

// 这里写得比较乱，计划马上把这部分重构一下。

void channel::save(FILE* f) const {
	fwt(typ_wv);
	if (typ_wv == WV_USR) {
		int sz = user.size(); fwt(sz);
		fwts(user.data(), sz);
	}
	fwt(vol); fwt(typ_dc); fwt(dec); fwt(pow);
	fwt(inc); fwt(fix_tune); fwt(tune); fwt(ph_mtp);
}
channel channel::load(FILE* f) {
	channel ch;
	frd(ch.typ_wv);
	if (ch.typ_wv == WV_USR) {
		int sz = 0; frd(sz);
		ch.user = vector<double>(sz);
		frds(ch.user.data(), sz);
	}
	frd(ch.vol); frd(ch.typ_dc); frd(ch.dec); frd(ch.pow);
	frd(ch.inc); frd(ch.fix_tune); frd(ch.tune); frd(ch.ph_mtp);
	return ch;
}
void track::save(FILE* f) const {
	int sz = 0;
	fwtv(nm);
	sz = inst.size(); fwt(sz);
	for (auto ch : inst) { ch.save(f); }

	sz = sheet.size(); fwt(sz);
	for (auto const& nr : sheet) {
		fwt(nr.t0); fwt(nr.t1); fwt(nr.tune);
	}
}
track track::load(FILE* f) {
	track tk; int sz = 0; 

	frdv(tk.nm); frd(sz);
	rep(i, 0, sz) { tk.inst.push_back(channel::load(f)); }

	frd(sz);
	rep(i, 0, sz) {
		double t0, t1; int tune;
		frd(t0); frd(t1); frd(tune);
		tk.sheet.push_back({ tune, t0, t1 });
	}
	return tk;
}
track load_track_old(FILE* f) {
	track tk; int sz = 0;
	// 直接存名字是绝对错误的！
	frd(tk.nm); frd(sz);
	rep(i, 0, sz) { tk.inst.push_back(channel::load(f)); }

	frd(sz);
	rep(i, 0, sz) {
		double t0, t1; int tune;
		frd(t0); frd(t1); frd(tune);
		tk.sheet.push_back({ tune, t0, t1 });
	}
	return tk;
}

void rhythm::save(wstring const& nm) const {
	FILE* f = wfopen(nm, L"wb");
	if (!f) { return; }

	fwt(n_oct); fwt(zero_oct);
	fwt(spd); fwt(bias_tune); fwt(t_end);
	for (auto inst : drum) {
		int sz = inst.size(); fwt(sz);
		for (auto const& ch : inst) { ch.save(f); }
	}

	int sz = tracks.size(); fwt(sz);
	for (auto tr : tracks) { tr.save(f); }
	fclose(f);
}
rhythm rhythm::load(wstring const& nm) {
	rhythm ry; FILE* f = wfopen(nm, L"rb");
	if (!f) { return {}; }

	frd(ry.n_oct); frd(ry.zero_oct);
	frd(ry.spd); frd(ry.bias_tune); frd(ry.t_end);
	ry.drum = vector<instr>(12 * ry.n_oct);
	for (auto& inst : ry.drum) {
		inst.clear(); int sz = 0; frd(sz);
		rep(i, 0, sz) { inst.push_back(channel::load(f)); }
	}

	int sz = 0; frd(sz);
	rep(i, 0, sz) { ry.tracks.push_back(track::load(f)); }
	fclose(f); return ry;
}
rhythm load_rhythm_old(wstring const& nm) {
	rhythm ry; FILE* f = wfopen(nm, L"rb");
	if (!f) { return {}; }

	ry.n_oct = 6; ry.zero_oct = 2;
	frd(ry.spd); frd(ry.bias_tune); frd(ry.t_end);
	ry.drum = vector<instr>(12 * ry.n_oct);
	for (auto& inst : ry.drum) {
		inst.clear(); int sz = 0; frd(sz);
		rep(i, 0, sz) { inst.push_back(channel::load(f)); }
	}

	int sz = 0; frd(sz);
	rep(i, 0, sz) { ry.tracks.push_back(load_track_old(f)); }
	fclose(f); return ry;
}

double loud(channel const& ch, int t, double sp) {
	if (ch.typ_dc == DC_EXP) {
		// 下面的 >= 是有必要的，否则会造成无穷大的出现。
		return exp(-ch.dec * t / sp) * ch.vol *
			(t / sp >= ch.inc ? 1 : t / ch.inc / sp);
	}
	else {
		return pow(max(0.0, (1 - ch.dec * t / sp)), ch.pow) *
			ch.vol * (t / sp >= ch.inc ? 1 : t / ch.inc / sp);
	}
}
double wv_val(int typ, double ph, vector<double> const& user) {
	switch (typ) {
	case WV_SQR: return ph > 0.5 ? 1 : -1;
	case WV_SIN: return sin(2 * PI * ph);
	case WV_NOI: return frnd(-1, 1);
	case WV_SAW: return ph / 2 - 1;
	case WV_TRI: return abs(1 - 2 * ph) * 2 - 1;
	case WV_USR: {
		if (user.size() <= 1) { return 0; }
		double x = clmp<double>(ph * user.size(), 0, user.size() - 1);
		int f = floor(x), c = ceil(x);
		return user[f] * (1 - (x - f)) + user[c] * (x - f);
	}
	}
}
double phase(int tune, int bias_tune, int ph_mtp, int t, double sp) {
	double ph = exp2((tune + bias_tune) / 12.0) * 261.6 * t * ph_mtp / sp;
	return modf(ph, 1);
}

bool play_rhythm(App& app, rhythm const& ry, int& cursp, double vol) {
	double sp = app.wv.sp; 
	auto& wvin = app.wv.wvin;
	rep(i, 0, wvin.size()) {
		for (auto const& tk : ry.tracks) for (auto const& nr : tk.sheet) {
			int s0 = nr.t0 * sp * 60 / 4 / ry.spd;
			int s1 = nr.t1 * sp * 60 / 4 / ry.spd;

			if (cursp >= s0 && cursp < s1) {
				int t = cursp - s0;
				for (auto const& ch : tk.inst) {
					double ph = ch.fix_tune ?
						phase(ch.tune, 0, ch.ph_mtp, t, sp) :
						phase(nr.tune, ry.bias_tune, ch.ph_mtp, t, sp);
					double out = loud(ch, t, sp);
					if (ch.typ_wv != WV_DRM) {
						out *= wv_val(ch.typ_wv, ph, ch.user);
					}
					else {
						double tmp = 0;
						int zero_tune = 12 * (ry.zero_oct + 1) - 1;
						for (auto const& ch : ry.drum[zero_tune - nr.tune]) {
							double ph = ch.fix_tune ?
								phase(ch.tune, 0, ch.ph_mtp, t, sp) :
								phase(0, ry.bias_tune, ch.ph_mtp, t, sp);
							double out = loud(ch, t, sp);
							out *= wv_val(ch.typ_wv, ph, ch.user); tmp += out;
						}
						out *= tmp;
					}
					wvin[i] += out * vol;
				}
			}
		}
		++cursp;
	}
	return cursp > ry.t_end * sp * 60 * 2 / ry.spd;
}
