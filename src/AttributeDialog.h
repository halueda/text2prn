/*! @file
  @brief 詳細表示ダイアログ
  @author 依積晶紀
*/
#pragma once

class Property;
class Attribute;

/************************************************************************/
//! 詳細表示ダイアログ
/*!
  詳細情報（設定値やフォントサイズなど）を表示するダイアログ		*/
/************************************************************************/
class AttributeDialog: public CDialog
{
  private:
    const CString	first_line_;	//!< 1行目の内容
    const Property&	prop_;		//!< 設定
    const Attribute&	attr_;		//!< 印刷情報

  public:
    AttributeDialog(LPCTSTR, const Property&, const Attribute&);

  protected:
    virtual BOOL OnInitDialog();
};
