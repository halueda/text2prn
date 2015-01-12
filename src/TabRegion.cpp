/*! @file
  @brief 印刷範囲タブページ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"TabRegion.h"
#include	"Property.h"
#include	"CtrlUtil.h"

BEGIN_MESSAGE_MAP(TabRegion, TabPage)
    ON_EN_CHANGE(IDC_BEGIN, OnChangeStart)
    ON_COMMAND(IDC_BEGIN2, OnClickStartLabel)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_END_LINE, IDC_END_PAGE, OnChangeEnd)
    ON_COMMAND_RANGE(IDC_END_L2, IDC_END_P2, OnClickEndLabel)
END_MESSAGE_MAP()

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! タブの初期化
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabRegion::init()
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定値をコントロールに反映する
/*!
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabRegion::update_ctrl(const Property& property)
{
    // ----- 設定値の表示 -----
    SetDlgItemInt(IDC_BEGIN, property.begin_region + 1);
    change_start_type(property.begin_region_type());

    switch (property.end_region_type) {
      case Property::END_REGION_LAST:		// 終わりまで
	break;
      case Property::END_REGION_LINE_COUNT:	// n行
	SetDlgItemInt(IDC_END_LINE, property.end_region);
	break;
      case Property::END_REGION_LINE_POS:		// n行目まで
	SetDlgItemInt(IDC_TO_LINE, property.end_region);
	break;
      case Property::END_REGION_PAGE:		// nページ
	SetDlgItemInt(IDC_END_PAGE, property.end_region);
	break;
      default:
	NODEFAULT;
    }
    change_end_type(property.end_region_type);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コントロールの内容を設定に反映する
/*!
  @param property [out]設定						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabRegion::update_value(Property& property) const
{
    switch (CtrlUtil::get_radio_button(this, IDC_FIRST, IDC_BEGIN1)) {
      case IDC_FIRST:	// はじめから
	property.begin_region = 0;
	break;
      case IDC_BEGIN1:	// n行目から
	property.begin_region = GetDlgItemInt(IDC_BEGIN) - 1;
	break;
      default:
	NODEFAULT;
    }

    switch (CtrlUtil::get_radio_button(this, IDC_LAST, IDC_END_P1)) {
      case IDC_LAST:	// 終わりまで
	property.end_region_type = Property::END_REGION_LAST;
	property.end_region = 0;
	break;
      case IDC_END_L1:	// n行
	property.end_region_type = Property::END_REGION_LINE_COUNT;
	property.end_region = GetDlgItemInt(IDC_END_LINE);
	break;
      case IDC_TO_L1:	// n行目まで
	property.end_region_type = Property::END_REGION_LINE_POS;
	property.end_region = GetDlgItemInt(IDC_TO_LINE);
	break;
      case IDC_END_P1:	// nページ
	property.end_region_type = Property::END_REGION_PAGE;
	property.end_region = GetDlgItemInt(IDC_END_PAGE);
	break;
      default:
	NODEFAULT;
    }
}

/* ==================================================================== */
//! 開始行のテキストボックスを編集した
/* ==================================================================== */
void TabRegion::OnChangeStart()
{
    change_start_type(Property::BEGIN_REGION_LINE_POS);
    GetDlgItem(IDC_BEGIN)->SetFocus();
}

/* ==================================================================== */
//! 開始行のラベルをクリックした
/* ==================================================================== */
void TabRegion::OnClickStartLabel()
{
    change_start_type(Property::BEGIN_REGION_LINE_POS);
}

/* ==================================================================== */
//! 開始行のテキストボックスを編集した
/* ==================================================================== */
void TabRegion::OnChangeEnd(UINT nID)
{
    change_end_type(nID - IDC_END_LINE + 1);
    GetDlgItem(nID)->SetFocus();
}

/* ==================================================================== */
//! 開始行のラベルをクリックした
/* ==================================================================== */
void TabRegion::OnClickEndLabel(UINT nID)
{
    change_end_type(nID - IDC_END_L2 + 1);
}

/* -------------------------------------------------------------------- */
//! 印刷開始位置の指定方法を変更した(ラジオボタンの変更以外で)
/*!
  @param start_type 印刷開始位置の指定方法				*/
/* -------------------------------------------------------------------- */
void TabRegion::change_start_type(int start_type)
{
    CtrlUtil::set_radio_button(this, IDC_FIRST + start_type);
}

/* -------------------------------------------------------------------- */
//! 印刷終了位置の指定方法を変更した(ラジオボタンの変更以外で)
/*!
  @param end_type 印刷終了位置の指定方法				*/
/* -------------------------------------------------------------------- */
void TabRegion::change_end_type(int end_type)
{
    CtrlUtil::set_radio_button(this, IDC_LAST + end_type);
}
