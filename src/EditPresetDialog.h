/*! @file
  @brief 登録設定編集ダイアログ
  @author 依積晶紀
*/
#pragma once

class PreSetPropertyList;
class Property;

/************************************************************************/
//! 登録設定編集ダイアログ
/************************************************************************/
class EditPresetDialog : public CDialog
{
  private:
    PreSetPropertyList& property_;	//!< 設定
    const Property* current_;		//!< 追加、削除対象の設定

    bool dirty_;			//!< 変更したらtrue

  public:
    EditPresetDialog(PreSetPropertyList&);

  protected:
    virtual BOOL OnInitDialog();
    virtual void OnCancel();

    afx_msg void OnAdd();
    afx_msg void OnDel();
    afx_msg void OnChangePreset();

    virtual void OnOK() {}	// リターンキーで終了しないように
    
  private:
    void init_ctrl();
    void init_property(const Property&);
    void init_preset();

    //! 登録済み設定を表すコンボボックスコントロールを取得
    CComboBox* get_preset_ctrl()
    {
	return (CComboBox*)GetDlgItem(IDC_PRESET_LIST);
    }

    //! 設定値を表示するリストコントロールを取得
    CListCtrl* get_property_ctrl()
    {
	return (CListCtrl*)GetDlgItem(IDC_PROPERTY);
    }

    DECLARE_MESSAGE_MAP();
};
