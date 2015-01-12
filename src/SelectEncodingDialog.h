/*! @file
  @brief 文字コード選択ダイアログ
  @author 依積晶紀
*/
#pragma once

/************************************************************************/
//! 文字コード選択ダイアログ
/************************************************************************/
class SelectEncodingDialog : public CDialog
{
  private:
    /* ---------------------------------------------------------------- */
    //! 文字コードのID
    /*!
      @see FileInfo
    */
    int encoding_;

  public:
    SelectEncodingDialog(int encoding = 0);

  protected:
    virtual BOOL OnInitDialog();
    virtual void OnOK();

  public:
    
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 選択した文字コードを取得する。
    /*! @return 文字コード						*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    int get_selected_encoding() const
    {
	return encoding_;
    }
};
