/*! @file
  @brief 印刷範囲タブページ
  @author 依積晶紀
*/
#pragma once

#include	"TabPage.h"

/************************************************************************/
//! 印刷範囲タブページ
/************************************************************************/
class TabRegion : public TabPage
{
  protected:
    afx_msg void OnChangeStart();
    afx_msg void OnClickStartLabel();
    afx_msg void OnChangeEnd(UINT);
    afx_msg void OnClickEndLabel(UINT);

  public:
    virtual void init();
    virtual void update_ctrl(const Property&);
    virtual void update_value(Property&) const;

  private:
    void change_start_type(int);
    void change_end_type(int);

    DECLARE_MESSAGE_MAP();
};
