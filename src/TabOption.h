/*! @file
  @brief その他タブページ
  @author 依積晶紀
*/
#pragma once

#include	"TabPage.h"
#include	"Property.h"

/************************************************************************/
//! その他タブページ
/************************************************************************/
class TabOption : public TabPage
{
  public:
    virtual void init();
    virtual void update_ctrl(const Property&);
    virtual void update_value(Property&) const;

    void update_preset(const PreSetPropertyList::PropertyList&);
    void enable_header_option(BOOL);

  protected:
    afx_msg void OnChangeDigit();
    afx_msg void OnApplyPreset();
    afx_msg void OnEditPreset();

  private:
    //! 登録済み設定を表すコンボボックスコントロールを取得
    CComboBox* get_preset_ctrl() const
    {
	return (CComboBox*)GetDlgItem(IDC_PRESET);
    }

    //! 行番号表示設定を表すコンボボックスコントロールを取得
    CComboBox* get_viewline_ctrl() const
    {
	return (CComboBox*)GetDlgItem(IDC_VIEWLINE);
    }
    
    DECLARE_MESSAGE_MAP();
};
