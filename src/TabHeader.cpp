/*! @file
  @brief ヘッダ/フッタ タブページ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"TabHeader.h"
#include	"Property.h"
#include	"CtrlUtil.h"
#include	"Util.h"

BEGIN_MESSAGE_MAP(TabHeader, TabPage)
    ON_COMMAND(IDC_USE_HEADER, OnUseHeader)
END_MESSAGE_MAP()

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! タブの初期化
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabHeader::init()
{
    CtrlUtil::set_check_box(this, IDC_USE_HEADER, false);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定値をコントロールに反映する
/*!
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabHeader::update_ctrl(const Property& property)
{
    if (property.use_detail_header != CtrlUtil::get_check_box(this, IDC_USE_HEADER))
	GetDlgItem(IDC_USE_HEADER)->SendMessage(BM_CLICK);

    SetDlgItemText(IDC_HEAD_L, property.header_l);
    SetDlgItemText(IDC_HEAD_C, property.header_c);
    SetDlgItemText(IDC_HEAD_R, property.header_r);
    SetDlgItemText(IDC_FOOT_L, property.footer_l);
    SetDlgItemText(IDC_FOOT_C, property.footer_c);
    SetDlgItemText(IDC_FOOT_R, property.footer_r);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コントロールの内容を設定に反映する
/*!
  @param property [out]設定						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabHeader::update_value(Property& property) const
{
    property.use_detail_header = CtrlUtil::get_check_box(this, IDC_USE_HEADER);
    
    GetDlgItemText(IDC_HEAD_L, property.header_l);
    GetDlgItemText(IDC_HEAD_C, property.header_c);
    GetDlgItemText(IDC_HEAD_R, property.header_r);
    GetDlgItemText(IDC_FOOT_L, property.footer_l);
    GetDlgItemText(IDC_FOOT_C, property.footer_c);
    GetDlgItemText(IDC_FOOT_R, property.footer_r);

    trim_if_empty(property.header_l);
    trim_if_empty(property.header_c);
    trim_if_empty(property.header_r);
    trim_if_empty(property.footer_l);
    trim_if_empty(property.footer_c);
    trim_if_empty(property.footer_r);
}

/* ==================================================================== */
//! ヘッダ/フッタを使うかどうかのチェックボックスを編集した
/* ==================================================================== */
void TabHeader::OnUseHeader()
{
    const BOOL state = CtrlUtil::get_check_box(this, IDC_USE_HEADER) ? TRUE : FALSE;

    for (int ID = IDC_HEAD_L; ID <= IDC_FOOT_R; ID++)
	GetDlgItem(ID)->EnableWindow(state);

    GetParent()->PostMessage(WM_USE_HEADER, state);
}
