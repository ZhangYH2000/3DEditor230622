#pragma once
#include "utils.h"
#include "proc.h"

typedef vector<proc> Program;
struct Var;
typedef function<ptr<Var>(vector<ptr<Var>>&)> Function;
struct Var {
	// 其实还不是很确定 typ 有没有用，这里会有些乱掉。
	wstring typ = L"null";
	// null, num, str, fun, vec, user, dic
	double num = 0;
	wstring str;
	Program procs;
	vector<wstring> pars;
	vector<ptr<Var>> vec;
	map<wstring, ptr<Var>> dic;
	Function fun;

	Var() = default;
	Var(double num);
	Var(wstring const &str);
	Var(vector<ptr<Var>> const& vec);
	Var(Program const& ps, vector<wstring>const& pars);
	Var(decltype(fun) const& fun);
};

struct vec2;
struct vec3;
vec2 tv2(Var const &v);
vec3 tv3(Var const &v);

typedef map<wstring, ptr<Var>> Scope;
ptr<Var> findvar(Scope& sc, wstring nm);
// 下面不用常值引用是为了减少不必要的复制。
// 但是说实话我不知道有没有必要。
bool default_fun(Scope &sc, expr const& ex, Var& out);

ptr<Var> eval_call(Scope& sc, expr const& ex);
ptr<Var> eval_access(Scope& sc, expr const& ex);
ptr<Var> Eval(Scope& sc, expr const& ex);

// 有一个风险是一个函数会不会在执行过程中把自己抹去。这里规避掉了。
ptr<Var> Execute(bool& ret, Scope& sc, Program const& ps);
ptr<Var> Execute(Scope& sc, Program const& ps);
Program Compile(wstring const& code);
