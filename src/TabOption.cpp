/*! @file
  @brief その他タブページ
  @author 依積晶紀
*/#include	"stdafx.h"
#include	"TabOption.h"
#include	"PropertyUtil.h"
#include	"CtrlUtil.h"

BEGIN_MESSAGE_MAP(TabOption, TabPage)
    ON_EN_CHANGE(IDC_LINE_DIGIT, OnChangeDigit)
    ON_COMMAND(IDC_PRESET_APPLY, OnApplyPreset)
    ON_COMMAND(IDC_PRESET_EDIT, OnEditPreset)
END_MESSAGE_MAP()

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! タブの初期化
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabOption::init()
{
    ((CEdit*)GetDlgItem(IDC_LINE_DIGIT))->SetLimitText(1);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定値をコントロールに反映する
/*!
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabOption::update_ctrl(const Property& property)
{
    SetDlgItemInt(IDC_LINE_DIGIT, property.line_count_digit);

    CtrlUtil::set_check_box(this, IDC_VIEWINFO, property.view_info);
    CtrlUtil::set_combo_index(this, IDC_VIEWLINE, property.get_output_line_index());
    CtrlUtil::set_check_box(this, IDC_COLUMN_LINE, property.output_column_line);
    CtrlUtil::set_check_box(this, IDC_VIEWPAGE, property.output_page_count);
    CtrlUtil::set_check_box(this, IDC_VIEWTITLE, property.output_title);

    enable_header_option(property.use_detail_header ? FALSE : TRUE);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コントロールの内容を設定に反映する
/*!
  @param property [out]設定						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabOption::update_value(Property& property) const
{
    property.line_count_digit = GetDlgItemInt(IDC_LINE_DIGIT);

    property.view_info = CtrlUtil::get_check_box(this, IDC_VIEWINFO);
    property.set_output_line_index(CtrlUtil::get_combo_index(this, IDC_VIEWLINE));
    property.output_column_line = CtrlUtil::get_check_box(this, IDC_COLUMN_LINE);
    property.output_page_count = CtrlUtil::get_check_box(this, IDC_VIEWPAGE);
    property.output_title = CtrlUtil::get_check_box(this, IDC_VIEWTITLE);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 登録済み設定をコントロールに繁栄する
/*!
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabOption::update_preset(const PreSetPropertyList::PropertyList& property)
{
    PropertyUtil::update_preset(get_preset_ctrl(), property);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ヘッダ/フッタに関係するオプションの有効/無効を変更する。
/*!
  @param [in] state 有効ならTRUE / 無効なら FALSE			*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabOption::enable_header_option(BOOL state)
{
    GetDlgItem(IDC_VIEWPAGE)->EnableWindow(state);
    GetDlgItem(IDC_VIEWTITLE)->EnableWindow(state);
}

/* ==================================================================== */
//! 行番号桁数のテキストボックスを編集した
/* ==================================================================== */
void TabOption::OnChangeDigit()
{
    if (GetDlgItemInt(IDC_LINE_DIGIT) > 0) {
	CComboBox* const combo = get_viewline_ctrl();
	if (combo->GetCurSel() == 0)
	    combo->SetCurSel(1);
    }
}

/* ==================================================================== */
//! [登録済み設定 適用]ボタンを押した
/* ==================================================================== */
void TabOption::OnApplyPreset()
{
    GetParent()->PostMessage(WM_APPLY_PRESET,
			     CtrlUtil::get_combo_index(this, IDC_PRESET));
}

/* ==================================================================== */
//! [登録済み設定 編集]ボタンを押した
/* ==================================================================== */
void TabOption::OnEditPreset()
{
    GetParent()->PostMessage(WM_EDIT_PRESET,
			     CtrlUtil::get_combo_index(this, IDC_PRESET));
}
