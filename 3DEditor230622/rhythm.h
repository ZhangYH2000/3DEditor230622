#pragma once
#include "utils.h"

int constexpr WV_SQR = 0;
int constexpr WV_SIN = 1;
int constexpr WV_TRI = 2;
int constexpr WV_SAW = 3;
int constexpr WV_NOI = 4;
int constexpr WV_USR = 5;
int constexpr WV_DRM = 6;

int constexpr DC_EXP = 0;
int constexpr DC_POW = 1;

struct nrect {
	int tune = 0;
	double t0 = 0, t1 = 0;
};
struct channel {
	int typ_wv = 0;
	double vol = 0;
	int typ_dc = 0;
	double dec = 0;
	double pow = 0;
	double inc = 0;
	bool fix_tune = false;
	int tune = 0;
	int ph_mtp = 0;
	vector<double> user;

	void save(FILE* f) const;
	static channel load(FILE* f);
};
typedef vector<channel> instr;
struct track {
	wstring nm;
	instr inst;
	vector<nrect> sheet;

	void save(FILE* f) const;
	static track load(FILE* f);
};
// 旧版的存档方式有 bug。
track load_track_old(FILE* f);

struct rhythm {
	int n_oct = 0;
	int zero_oct = 0;
	int spd = 0;
	double t_end = 0;
	int bias_tune = 0;
	vector<instr> drum;
	vector<track> tracks;

	rhythm() = default;

	void save(wstring const& nm) const;
	static rhythm load(wstring const& nm);
};
rhythm load_rhythm_old(wstring const& nm);

double loud(channel const& ch, int t, double sp);
double wv_val(int typ, double ph, vector<double> const& user);
double phase(int tune, int bias_tune, int ph_mtp, int t, double sp);

struct App;
bool play_rhythm(App& app, rhythm const& ry, int& cursp, double vol);
