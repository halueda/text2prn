/*! @file
  @brief ヘッダ/フッタ タブページ
  @author 依積晶紀
*/
#pragma once

#include	"TabPage.h"

/************************************************************************/
//! ヘッダ/フッタ タブページ
/************************************************************************/
class TabHeader : public TabPage
{
  protected:
    afx_msg void OnUseHeader();

  public:
    virtual void init();
    virtual void update_ctrl(const Property&);
    virtual void update_value(Property&) const;

  private:
    DECLARE_MESSAGE_MAP();
};
