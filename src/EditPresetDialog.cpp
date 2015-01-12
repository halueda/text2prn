/*! @file
  @brief 登録設定編集ダイアログ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"EditPresetDialog.h"
#include	"Property.h"
#include	"PropertyUtil.h"
#include	"CtrlUtil.h"
#include	"ConfirmNameDialog.h"

BEGIN_MESSAGE_MAP(EditPresetDialog, CDialog)
    ON_CBN_SELCHANGE(IDC_PRESET_LIST, OnChangePreset)
    ON_COMMAND(IDC_ADD, OnAdd)
    ON_COMMAND(IDC_DEL, OnDel)
END_MESSAGE_MAP()

/* ==================================================================== */
/*! @param [in] property 設定						*/
/* ==================================================================== */
EditPresetDialog::EditPresetDialog(PreSetPropertyList& property) :
	CDialog(IDD_EDIT_PRESET), property_(property), dirty_(false),
	current_(&property.get())
{
}

/* ==================================================================== */
//! ダイアログの初期化
/* ==================================================================== */
BOOL EditPresetDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    CListCtrl* const ctrl = get_property_ctrl();
    ASSERT(ctrl != NULL);
    ctrl->InsertColumn(0, _T("設定"), LVCFMT_LEFT, 75);
    ctrl->InsertColumn(1, _T("内容"), LVCFMT_LEFT, 150);

    init_ctrl();

    return TRUE;
}

/* ==================================================================== */
//! [閉じる]ボタン
/* ==================================================================== */
void EditPresetDialog::OnCancel()
{
    EndDialog(dirty_ ? IDOK : IDCANCEL);
}

/* ==================================================================== */
//! 登録済み設定コントロールを変更
/* ==================================================================== */
void EditPresetDialog::OnChangePreset()
{
    const size_t index = CtrlUtil::get_combo_index(this, IDC_PRESET_LIST);	// 選択された値
    if (index < 0 || index >= property_.get_preset_size())
        return;

    init_property(property_.get(index));
}

/* ==================================================================== */
//! [追加]ボタン
/* ==================================================================== */
void EditPresetDialog::OnAdd()
{
    CString name;			// 登録名

    GetDlgItemText(IDC_NAME, name);
    name.Trim();

    if (name.IsEmpty()) {
	MessageBox(_T("登録名が入力されていません。"),
		   CAPTION_WARNING, MB_ICONWARNING);
	return;
    }

    CString err_mes;			// 設定値のエラー
    if (!current_->check_value(err_mes)) {
	MessageBox(err_mes, CAPTION_WARNING, MB_ICONWARNING);
	return;
    }

    // 登録名が重複しているかどうかはチェックしない。
    if (property_.is_exist_preset_name(name)) {
	ConfirmNameDialog dialog(name);
	switch (dialog.DoModal()) {
	  case ConfirmNameDialog::SELECT_CANCEL:
	    return;
	  case ConfirmNameDialog::SELECT_APPEND:
	    property_.add_preset(name, *current_);
	    break;
	  case ConfirmNameDialog::SELECT_OVERWRITE:
	    property_.overwrite_preset(name, *current_);
	    break;
	}
    } else {
	property_.add_preset(name, *current_);
    }

    dirty_ = true;
    init_preset();

    SetDlgItemText(IDC_NAME, _T(""));
}

/* ==================================================================== */
//! [削除]ボタン
/* ==================================================================== */
void EditPresetDialog::OnDel()
{
    const size_t index = CtrlUtil::get_combo_index(this, IDC_PRESET_LIST);	// 選択された値
    if (index < 0 || index >= property_.get_preset_size()) {
	MessageBox(_T("削除する設定が選択されていません。"),
		   CAPTION_WARNING, MB_ICONWARNING);
        return;
    }

    if (MessageBox(_T("削除しても良いですか？"),
		   CAPTION_CONFIRM, MB_ICONQUESTION | MB_YESNO) != IDYES) {
	return;
    }

    property_.del_preset(index);

    dirty_ = true;
    init_ctrl();
}

/* -------------------------------------------------------------------- */
//! コントロールの初期化
/* -------------------------------------------------------------------- */
void EditPresetDialog::init_ctrl()
{
    init_property(property_.get());
    init_preset();
}

/* -------------------------------------------------------------------- */
//! 設定値コントロールの初期化
/*!
  @param [in] prop 表示する設定						*/
/* -------------------------------------------------------------------- */
void EditPresetDialog::init_property(const Property& prop)
{
    current_ = &prop;

    // 設定値の説明
    const PropertyUtil::Description desc = PropertyUtil::get_description(*current_);

    CListCtrl* const ctrl = get_property_ctrl();
    ASSERT(ctrl != NULL);
    ctrl->DeleteAllItems();

    PropertyUtil::Description::const_iterator it;
    for (it = desc.begin(); it != desc.end(); ++it) {

	const int index = ctrl->InsertItem(ctrl->GetItemCount(), it->first);
	ctrl->SetItemText(index, 1, it->second);
    }
}

/* -------------------------------------------------------------------- */
//! 登録済み設定コントロールの初期化
/* -------------------------------------------------------------------- */
void EditPresetDialog::init_preset()
{
    PropertyUtil::update_preset(get_preset_ctrl(), property_.get_preset());
}
