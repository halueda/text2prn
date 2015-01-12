/*! @file
  @brief テキストタブページ
  @author 依積晶紀
*/
#pragma once

#include	"TabPage.h"

/************************************************************************/
//! テキストタブページ
/************************************************************************/
class TabText : public TabPage
{
  protected:
    afx_msg void OnChangeSize(UINT);
    afx_msg void OnClickLabel(UINT);

  public:
    virtual void init();
    virtual void update_ctrl(const Property&);
    virtual void update_value(Property&) const;

  private:
    void change_font_base_type(int);

    DECLARE_MESSAGE_MAP();
};
