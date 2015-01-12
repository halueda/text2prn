/*! @file
  @brief 重複した登録名確認ダイアログ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"ConfirmNameDialog.h"
#include	"resource.h"

BEGIN_MESSAGE_MAP(ConfirmNameDialog, CDialog)
    ON_COMMAND(IDC_APPEND, OnAppend)
    ON_COMMAND(IDC_OVERWRITE, OnOverwrite)
END_MESSAGE_MAP()


/* ==================================================================== */
/*! @param [in] name 登録名
/* ==================================================================== */
ConfirmNameDialog::ConfirmNameDialog(LPCTSTR name) :
	CDialog(IDD_CONFIRM_NAME), name_(name)
{
}

/* ==================================================================== */
//! ダイアログの初期化
/* ==================================================================== */
BOOL ConfirmNameDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetDlgItemText(IDC_NAME, name_);

    reinterpret_cast<CStatic*>(GetDlgItem(IDC_IMAGE))->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_QUESTION));
    
    return TRUE;
}

/* ==================================================================== */
//! [OK]ボタン
/*!
  何もしない								*/
/* ==================================================================== */
void ConfirmNameDialog::OnOK()
{
}

/* ==================================================================== */
//! [キャンセル]ボタン
/*!
  終了コード SELECT_CANCEL でダイアログ終了				*/
/* ==================================================================== */
void ConfirmNameDialog::OnCancel()
{
    EndDialog(SELECT_CANCEL);
}

/* ==================================================================== */
//! [キャンセル]ボタン
/*!
  終了コード SELECT_APPEND でダイアログ終了				*/
/* ==================================================================== */
void ConfirmNameDialog::OnAppend()
{
    EndDialog(SELECT_APPEND);
}

/* ==================================================================== */
//! [キャンセル]ボタン
/*!
  終了コード SELECT_OVERWRITE でダイアログ終了				*/
/* ==================================================================== */
void ConfirmNameDialog::OnOverwrite()
{
    EndDialog(SELECT_OVERWRITE);
}
