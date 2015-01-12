/*! @file
  @brief 余白タブページ
  @author 依積晶紀
*/
#pragma once

#include	"TabPage.h"

/************************************************************************/
//! 余白タブページ
/************************************************************************/
class TabSpace : public TabPage
{
  protected:
    afx_msg void OnUseSpace();

  public:
    virtual void init();
    virtual void update_ctrl(const Property&);
    virtual void update_value(Property&) const;

  private:
    void init_ctrl(int, int, BOOL = FALSE);
    void set_space_value(UINT, int);
    
    DECLARE_MESSAGE_MAP();
};
