/*! @file
  @brief 詳細表示ダイアログ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"AttributeDialog.h"
#include	"Property.h"
#include	"PropertyUtil.h"
#include	"Attribute.h"
#include	"resource.h"

/* ==================================================================== */
/*! @param [in] line 1行目の内容（出力する最初の行）
    @param [in] prop 設定
    @param [in] attr 印刷情報						*/
/* ==================================================================== */
AttributeDialog::AttributeDialog(LPCTSTR line,
				 const Property& prop,
				 const Attribute& attr) :
	CDialog(IDD_INFO), first_line_(line),
	prop_(prop), attr_(attr)
{
}

/* ==================================================================== */
//! ダイアログの初期化
/*
  表示する情報をリストコントロールに羅列する				*/
/* ==================================================================== */
BOOL AttributeDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 設定値の説明
    const PropertyUtil::Description desc = PropertyUtil::get_description(prop_, attr_, first_line_);
    
    CListCtrl* const list = (CListCtrl*)GetDlgItem(IDC_ATTRIBUTE_LIST);
    list->InsertColumn(0, _T("設定"), LVCFMT_LEFT, 80);
    list->InsertColumn(1, _T("内容"), LVCFMT_LEFT, 180);

    PropertyUtil::Description::const_iterator it;
    for (it = desc.begin(); it != desc.end(); ++it) {

	const int index = list->InsertItem(list->GetItemCount(), it->first);
	list->SetItemText(index, 1, it->second);
    }

    return TRUE;
}
