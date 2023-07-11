#pragma once
#include "app.h"
#include "var.h"

struct param {
	wstring nm;
	double val = 0;
	bool del = false;

	param(wstring const& nm = {});
	static param load(FILE* f);
	void save(FILE* f);
};
