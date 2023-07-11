#pragma once
#include "label.h"
#include "button.h"
#include "textbox.h"
#include "dragbar.h"
#include "droplist.h"
#include "checkbox.h"
#include "ctrl_list.h"

// 这里有很多操作涉及到取地址以及传递自己的地址。
// 如果发生移动和复制的话就会乱掉。
// 所以要搞清楚哪些做法会发生复制，哪些不会。
// 为了初始化变量方便，控件成员还是用 ptr 吧。
struct LbTb : CtrlListX {
	struct Tb : Textbox {
		LbTb* cp = NULL;

		Tb() = default;
		Tb(LbTb* cp, int w_tb);
		void Sync() override;
		void OnDone() const override;
	};

	ptr<LbLan> lb;
	ptr<Tb> tb;

	LbTb() = default;
	LbTb(wstring const& id_lb, int w_tb);
	// 这里还是不管 const 这些吧，容易乱。
	// 我也不知道有什么意义。
	virtual void TbSync() {}
	virtual void TbOnDone() {}
};

struct DlBt : CtrlListX {
	struct Dl : DlLan {
		DlBt* cp = NULL;

		Dl() = default;
		Dl(DlBt* cp, int w_dl, function<wstring(int)> const& id_dl, int n_dl);
		void Sync() override;
		void Upload() const override;
	};
	struct Bt : BtLan {
		DlBt* cp = NULL;

		Bt() = default;
		Bt(DlBt* cp, int w_bt, wstring const& id_bt);
		void OnClick() override;
	};

	ptr<Dl> dl;
	ptr<Bt> bt;

	DlBt() = default;
	DlBt(int w_dl, function<wstring(int)> const& id_dl,
		int n_dl, int w_bt, wstring const& id_bt);
	virtual void DlSync() {}
	virtual void DlUpload() {}
	virtual void BtOnClick() {}
};

struct TbBt : CtrlListX {
	struct Tb : Textbox {
		TbBt* cp = NULL;

		Tb() = default;
		Tb(TbBt* cp, int w_tb);
		void Sync() override;
		void OnDone() const override;
	};
	struct Bt : BtLan {
		TbBt* cp = NULL;

		Bt() = default;
		Bt(TbBt* cp, int w_bt, wstring const& id_bt);
		void OnClick() override;
	};

	ptr<Tb> tb;
	ptr<Bt> bt;

	TbBt() = default;
	TbBt(int w_tb, int w_bt, wstring const& id_bt);
	virtual void TbSync() {}
	virtual void TbOnDone() {}
	virtual void BtOnClick() {}
};

struct LbDl : CtrlListX {
	struct Dl : DlLan {
		LbDl* cp = NULL;

		Dl() = default;
		Dl(LbDl* cp, int w_dl, function<wstring(int)> const& id_dl, int n_dl);
		void Sync() override;
		void Upload() const override;
	};

	ptr<LbLan> lb;
	ptr<Dl> dl;

	LbDl() = default;
	LbDl(wstring const& id_lb, int w_dl,
		function<wstring(int)> const& id_dl, int n_dl);
	virtual void DlSync() {}
	virtual void DlUpload() {}
};

struct LbCb : CtrlListX {
	struct Cb : Checkbox {
		LbCb* cp = NULL;

		Cb(LbCb* cp = NULL);
		void Sync() override;
		void Upload() const override;
	};

	ptr<LbLan> lb;
	ptr<Cb> cb;

	LbCb() = default;
	LbCb(wstring const& id_lb);
	virtual void CbSync() {}
	virtual void CbUpload() {}
};

struct DbXTb : CtrlListX {
	struct DbX : DragbarX {
		DbXTb* cp = NULL;

		DbX() = default;
		DbX(DbXTb* cp, int w_db);
		void Sync() override;
		void Upload() const override;
	};
	struct Tb : Textbox {
		DbXTb* cp = NULL;

		Tb() = default;
		Tb(DbXTb* cp, int w_tb);
		void Sync() override;
		void OnDone() const override;
	};

	ptr<DbX> dbx;
	ptr<Tb> tb;

	DbXTb() = default;
	DbXTb(int w_db, int w_tb);
	virtual void DbXSync() {}
	virtual void DbXUpload() {}
	virtual void TbSync() {}
	virtual void TbOnDone() {}
};
