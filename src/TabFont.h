/*! @file
  @brief フォントタブページ
  @author 依積晶紀
*/
#pragma once

#include	"TabPage.h"

/************************************************************************/
//! フォントタブページ
/************************************************************************/
class TabFont : public TabPage
{
  private:
    //! インストール済みのフォント情報
    std::vector<LOGFONT> lf_pool;

    //! フォント名とフォント情報
    typedef std::pair<CString, int> FontInfo;

    //! 言語毎に分類したフォントの情報
    typedef std::multimap<CString, FontInfo> FontInfoList;

    //! インストール済みのフォント一覧
    FontInfoList font_info;

    //! フォント一覧取得時にコールバックに渡すオプション
    struct EnumFontOption
    {
	TabFont* target;	//!< フォント情報の格納先
	bool vertical;		//!< 縦書きフォントが対象かどうか
	bool proportional;	//!< プロポーショナルフォントが対象かどうか
    };
    
  protected:
    afx_msg void OnChangeFontType();
    afx_msg void OnChangeTextDirection();
    afx_msg void OnChangeFontWidth();

  public:
    virtual void init();
    virtual void update_ctrl(const Property&);
    virtual void update_value(Property&) const;

  private:
    void set_font_ctrl(bool, bool, LPCTSTR, LPCTSTR);
    
    void enum_font(bool, bool);
    void set_font_info(LPCTSTR, LPCTSTR, LOGFONT&);
    static int CALLBACK enum_font_callback(ENUMLOGFONTEX*, NEWTEXTMETRICEX*, int, LPARAM);

    //! フォントの種類（言語）を表すコンボボックスコントロールを取得
    CComboBox* get_font_type_ctrl() const
    {
	return (CComboBox*)GetDlgItem(IDC_FONT_TYPE);
    }

    //! フォント名を表すコンボボックスコントロールを取得
    CComboBox* get_font_name_ctrl() const
    {
	return (CComboBox*)GetDlgItem(IDC_FONT_NAME);
    }
    

    DECLARE_MESSAGE_MAP();
};
