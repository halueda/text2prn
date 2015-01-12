/*! @file
  @brief タブページ
  @author 依積晶紀
*/#pragma once

class Property;

/************************************************************************/
//! タブページの親クラス
/************************************************************************/
class TabPage : public CDialog
{
  protected:
    // OKの対策。
    // コレをしておかないと、コントロール上で[Return]キー入力があると
    // このタブの中身だけ閉じてしまう。
    virtual void OnOK()
    {
    }

    // CANCELの対策。
    // コレをしておかないと、コントロール上で[ESC]キー入力があると
    // このタブの中身だけ閉じてしまう。
    virtual void OnCancel()
    {
	GetParent()->PostMessage(WM_COMMAND, IDCANCEL);
    }
    
  public:
    virtual void init() = 0;
    virtual void update_ctrl(const Property&) = 0;
    virtual void update_value(Property&) const = 0;
};
