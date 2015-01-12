/*! @file
  @brief プレビュータブページ
  @author 依積晶紀
*/
#pragma once

#include	"TabPage.h"

/************************************************************************/
//! プレビュータブページ
/************************************************************************/
class TabPreview : public TabPage
{
  public:
    virtual void init();
    virtual void update_ctrl(const Property&);
    virtual void update_value(Property&) const;

  protected:
    afx_msg void OnPreview();
    afx_msg void OnPageSetup();

    DECLARE_MESSAGE_MAP();
};
