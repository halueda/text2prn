/*! @file
  @brief コントロールの操作
  @author 依積晶紀
*/
#pragma once

//! コントロールの操作
namespace CtrlUtil
{
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! チェックボックスの状態を変更
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID チェックボックスのコントロールID
	@param [in] state チェックを付けるならtrue			*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline void set_check_box(CWnd* wnd, UINT ID, bool state)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	reinterpret_cast<CButton*>(wnd->GetDlgItem(ID))->SetCheck(state ? 1 : 0);
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! チェックボックスの状態取得
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID チェックボックスのコントロールID
	@retval true チェックあり
	@retval false  チェックなし					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline bool get_check_box(const CWnd* wnd, UINT ID)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	return reinterpret_cast<CButton*>(wnd->GetDlgItem(ID))->GetCheck() == 1;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! ラジオボタンにチェックを付ける
    /*! 同じグループに属する他のラジオボタンのチェックは外れる。
	@param [in] wnd コントロールのあるウィンドウ
	@param [in] ID ラジオボタンのコントロールID			*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline void set_radio_button(CWnd* wnd, UINT ID)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	wnd->GetDlgItem(ID)->SendMessage(BM_CLICK);    
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! チェックのあるラジオボタンを取得
    /*! 指定の範囲のラジオボタンのいずれにもチェックが無い場合は、
	0を返す。
	@param [in] wnd コントロールのあるウィンドウ
	@param [in] ID_S 最初のラジオボタンのコントロールID
	@param [in] ID_E 最後のラジオボタンのコントロールID
	@return チェックのあるラジオボタンのコントロールID		*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline int get_radio_button(const CWnd* wnd, UINT ID_S, UINT ID_E)
    {
	ASSERT(wnd != NULL);
	ASSERT(ID_S <= ID_E);

	for (UINT ID = ID_S; ID <= ID_E; ++ID) {
	    ASSERT(wnd->GetDlgItem(ID) != NULL);
	    
	    if (reinterpret_cast<CButton*>(wnd->GetDlgItem(ID))->GetCheck() == 1)
		return ID;
	}
	NODEFAULT;
	return 0;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! スピンコントロールの値を変更
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID スピンコントロールのコントロールID
	@param value 値							*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline void set_spin_pos(CWnd* wnd, UINT ID, int value)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	reinterpret_cast<CSpinButtonCtrl*>(wnd->GetDlgItem(ID))->SetPos(value);
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! スピンコントロールの値取得
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID スピンコントロールのコントロールID
	@return 値							*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline int get_spin_pos(const CWnd* wnd, UINT ID)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	return LOWORD(reinterpret_cast<CSpinButtonCtrl*>(wnd->GetDlgItem(ID))->GetPos());
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! コンボボックスの値を指定項目に設定
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID コンボボックスのコントロールID
	@param index 指定項目のインデックス				*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline void set_combo_index(CWnd* wnd, UINT ID, int index)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	reinterpret_cast<CComboBox*>(wnd->GetDlgItem(ID))->SetCurSel(index);
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! コンボボックスで選択されている項目のインデックス取得
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID コンボボックスコントロールID
	@return 選択されている項目のインデックス			*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline int get_combo_index(const CWnd* wnd, UINT ID)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	return reinterpret_cast<CComboBox*>(wnd->GetDlgItem(ID))->GetCurSel();
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! タブコントロールで指定のタブを選択する
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID タブコントロールのコントロールID
	@param index 指定タブのインデックス				*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline void set_tab_index(CWnd* wnd, UINT ID, int index)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	reinterpret_cast<CTabCtrl*>(wnd->GetDlgItem(ID))->SetCurSel(index);
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! タブコントロールで選択されているタブのインデックス取得
    /*! @param [in] wnd コントロールのあるウィンドウ
	@param [in] ID タブコントロールのコントロールID
	@return 選択されているタブのインデックス			*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    inline int get_tab_index(const CWnd* wnd, UINT ID)
    {
	ASSERT(wnd != NULL && wnd->GetDlgItem(ID) != NULL);
	return reinterpret_cast<CTabCtrl*>(wnd->GetDlgItem(ID))->GetCurSel();
    }
}
