/*! @file
  @brief 重複した登録名確認ダイアログ
  @author 依積晶紀
*/
#pragma once

/************************************************************************/
//! 重複した登録名確認ダイアログ
/************************************************************************/
class ConfirmNameDialog: public CDialog
{
  public:
    //! 選択したボタン
    enum
    {
	SELECT_CANCEL,		//!< キャンセル
	SELECT_APPEND,		//!< 追加
	SELECT_OVERWRITE,	//!< 更新
    };

  private:
    CString name_;		//!< 重複した名前

  public:
    ConfirmNameDialog(LPCTSTR);

  protected:
    virtual BOOL OnInitDialog();
    virtual void OnOK( );
    virtual void OnCancel();

    afx_msg void OnAppend();
    afx_msg void OnOverwrite();

    DECLARE_MESSAGE_MAP();
};
