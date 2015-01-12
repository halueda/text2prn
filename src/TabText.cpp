/*! @file
  @brief テキストタブページ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"TabText.h"
#include	"Property.h"
#include	"CtrlUtil.h"

BEGIN_MESSAGE_MAP(TabText, TabPage)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_CHAR, IDC_FONT, OnChangeSize)
    ON_COMMAND_RANGE(IDC_CHAR2, IDC_FONT2, OnClickLabel)
END_MESSAGE_MAP()

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! タブの初期化
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabText::init()
{
    ((CEdit*)GetDlgItem(IDC_CHAR))->SetLimitText(3);
    ((CEdit*)GetDlgItem(IDC_LINE))->SetLimitText(3);
    ((CEdit*)GetDlgItem(IDC_FONT))->SetLimitText(3);
    ((CEdit*)GetDlgItem(IDC_COLUMN))->SetLimitText(1);
    ((CEdit*)GetDlgItem(IDC_TABNUM))->SetLimitText(2);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定値をコントロールに反映する
/*!
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabText::update_ctrl(const Property& property)
{
    // ----- 設定値の表示 -----
    SetDlgItemInt(IDC_CHAR, property.char_p_line);
    SetDlgItemInt(IDC_LINE, property.line_p_column);
    SetDlgItemInt(IDC_FONT, property.font_size);
    SetDlgItemInt(IDC_COLUMN, property.column);
    SetDlgItemInt(IDC_TABNUM, property.tab_char);

    change_font_base_type(property.font_base_type);

    CtrlUtil::set_check_box(this, IDC_WRAP_RIGHT, property.wrap_right);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コントロールの内容を設定に反映する
/*!
  @param property [out]設定						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabText::update_value(Property& property) const
{
    property.char_p_line = GetDlgItemInt(IDC_CHAR);
    property.line_p_column = GetDlgItemInt(IDC_LINE);
    property.font_size = GetDlgItemInt(IDC_FONT);
    property.column = GetDlgItemInt(IDC_COLUMN);
    property.tab_char = GetDlgItemInt(IDC_TABNUM);

    switch (CtrlUtil::get_radio_button(this, IDC_CHAR1, IDC_FONT1)) {
      case IDC_CHAR1:
	property.font_base_type = Property::FONT_BASE_CHAR_P_LINE;
	break;
      case IDC_LINE1:
	property.font_base_type = Property::FONT_BASE_LINE_P_COL;
	break;
      case IDC_FONT1:
	property.font_base_type = Property::FONT_BASE_FONT_SIZE;
	break;
      default:
	NODEFAULT;
    }

    property.wrap_right = CtrlUtil::get_check_box(this, IDC_WRAP_RIGHT);
}

/* ==================================================================== */
//! 文字数/行数/フォントのテキストボックスを編集した
/* ==================================================================== */
void TabText::OnChangeSize(UINT nID)
{
    change_font_base_type(nID - IDC_CHAR);
    GetDlgItem(nID)->SetFocus();
}

/* ==================================================================== */
//! 文字数/行数/フォントのラベルをクリックした
/* ==================================================================== */
void TabText::OnClickLabel(UINT nID)
{
    change_font_base_type(nID - IDC_CHAR2);
}

/* -------------------------------------------------------------------- */
//! フォントサイズ計算の基準を変更した(ラジオボタンの変更以外で)
/*!
  @param font_base_type フォントサイズ計算の基準			*/
/* -------------------------------------------------------------------- */
void TabText::change_font_base_type(int font_base_type)
{
    CtrlUtil::set_radio_button(this, IDC_CHAR1 + font_base_type);
}
