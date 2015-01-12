/*! @file
  @brief 余白タブページ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"TabSpace.h"
#include	"Property.h"
#include	"CtrlUtil.h"

BEGIN_MESSAGE_MAP(TabSpace, TabPage)
    ON_COMMAND(IDC_USE_SPACE, OnUseSpace)
END_MESSAGE_MAP()

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! タブの初期化
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabSpace::init()
{
    init_ctrl(IDC_TOP,    IDC_STOP);
    init_ctrl(IDC_BOTTOM, IDC_SBOTTOM);
    init_ctrl(IDC_LEFT,   IDC_SLEFT);
    init_ctrl(IDC_RIGHT,  IDC_SRIGHT);

    CtrlUtil::set_check_box(this, IDC_USE_SPACE, false);

    init_ctrl(IDC_COLUMN_SPACE,  IDC_SCOLUMN, TRUE);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定値をコントロールに反映する
/*!
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabSpace::update_ctrl(const Property& property)
{
    set_space_value(IDC_STOP,    property.space_top);
    set_space_value(IDC_SBOTTOM, property.space_bottom);
    set_space_value(IDC_SLEFT,   property.space_left);
    set_space_value(IDC_SRIGHT,  property.space_right);

    if (property.use_space != CtrlUtil::get_check_box(this, IDC_USE_SPACE))
	GetDlgItem(IDC_USE_SPACE)->SendMessage(BM_CLICK);

    CtrlUtil::set_combo_index(this, IDC_LINE_SPACE, property.line_space);
    set_space_value(IDC_SCOLUMN,  property.column_space);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コントロールの内容を設定に反映する
/*!
  @param property [out]設定						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabSpace::update_value(Property& property) const
{
    property.space_top    = CtrlUtil::get_spin_pos(this, IDC_STOP);
    property.space_bottom = CtrlUtil::get_spin_pos(this, IDC_SBOTTOM);
    property.space_left   = CtrlUtil::get_spin_pos(this, IDC_SLEFT);
    property.space_right  = CtrlUtil::get_spin_pos(this, IDC_SRIGHT);

    property.use_space = CtrlUtil::get_check_box(this, IDC_USE_SPACE);

    property.line_space = CtrlUtil::get_combo_index(this, IDC_LINE_SPACE);
    property.column_space = CtrlUtil::get_spin_pos(this, IDC_SCOLUMN);
}

/* -------------------------------------------------------------------- */
//! コントロールの初期化
/*!
  @param [in] edit_id エディットコントロールのID
  @param [in] spin_id スピンコントロールのID
  @param [in] state コントロールの有効(TRUE)/無効(FALSE)		*/
/* -------------------------------------------------------------------- */
void TabSpace::init_ctrl(int edit_id, int spin_id, BOOL state)
{
    CEdit* const edit = (CEdit*)GetDlgItem(edit_id);
    edit->SetLimitText(2);
    edit->EnableWindow(state);

    CSpinButtonCtrl* const spin = (CSpinButtonCtrl*)GetDlgItem(spin_id);
    spin->SetRange32(0, 50);
    spin->EnableWindow(state);
}

/* ==================================================================== */
//! [余白を使う]チェックボタンを押した
/* ==================================================================== */
void TabSpace::OnUseSpace()
{
    const BOOL state = CtrlUtil::get_check_box(this, IDC_USE_SPACE) ? TRUE : FALSE;

    for (int ID = IDC_TOP; ID <= IDC_SRIGHT; ID++)
	GetDlgItem(ID)->EnableWindow(state);
}

/* -------------------------------------------------------------------- */
//! 余白の値を設定する
/*!
  @param ID 余白を表すスピンコントロールのID
  @param value 値							*/
/* ----====------------------------------------------------------------ */
void TabSpace::set_space_value(UINT ID, int value)
{
    if (value > Property::MAX_SPACE)
	value = Property::MAX_SPACE;

    CtrlUtil::set_spin_pos(this, ID, value);
}
