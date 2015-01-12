/*! @file
  @brief 設定値関連
  @author 依積晶紀
*/
#pragma once

#include	"Property.h"
class Attribute;

//! 設定値関連
namespace PropertyUtil
{

void update_preset(CComboBox* const, const PreSetPropertyList::PropertyList&);

//! 設定の、設定名と設定値の説明 一覧
typedef std::vector<std::pair<CString, CString> > Description;
Description get_description(const Property&);
Description get_description(const Property&, const Attribute&, LPCTSTR);

void init_page_setup_dialog(CPageSetupDialog&, const Property&);
}
