/*! @file
  @brief 文字コード選択ダイアログ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"SelectEncodingDialog.h"
#include	"resource.h"
#include	"FileInfo.h"
#include	"CtrlUtil.h"

/* ==================================================================== */
/*!
  @param [in] encoding 初期表示時の文字コード				*/
/* ==================================================================== */
SelectEncodingDialog::SelectEncodingDialog(int encoding) :
	CDialog(IDD_ENCODING), encoding_(encoding)
{
}

/* ==================================================================== */
//! ダイアログの初期化
/*!
  @li 文字コードのコンボボックスの初期化				*/
/* ==================================================================== */
BOOL SelectEncodingDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    CComboBox* const ctrl = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_ENCODING));
    ASSERT(ctrl != NULL);
    FileInfo::set_combo_ctrl(ctrl, encoding_);

    return TRUE;
}

/* ==================================================================== */
//! [OK]ボタンの処理
/*!
  @li 選択されている文字コードの記憶
  @li 画面を閉じる							*/
/* ==================================================================== */
void SelectEncodingDialog::OnOK()
{
    encoding_ = CtrlUtil::get_combo_index(this, IDC_ENCODING);

    CDialog::OnOK();
}
