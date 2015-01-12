/*! @file
  @brief プレビュー画面
  @author 依積晶紀
*/
#pragma once

class PreviewData;

#include	"Attribute.h"
#include	"Property.h"
#include	"PreviewData.h"


/************************************************************************/
//! プレビュー画面
/************************************************************************/
class PreviewWindow : public CFrameWnd
{
  private:
    /****************************************************************/
    //! プレビュー画面の操作ボタン
    /****************************************************************/
    class PreviewBar : public CDialogBar
    {
      protected:
	afx_msg void OnNextPage();
	afx_msg void OnPrevPage();
	afx_msg void OnZoom();
	afx_msg void OnCloseWindow();
    
      public:
	// ページ番号の変更
	void set_page(size_t, size_t);

      private:
	/* ---------------------------------------------------- */
	//! プレビュー画面を取得する
	/*! @return プレビュー画面				*/
	/* ---------------------------------------------------- */
	PreviewWindow* get_preview_window()
	{
	    return reinterpret_cast<PreviewWindow*>(GetParent());
	}
    
	DECLARE_MESSAGE_MAP();
    };

    CWnd* parent_;		//!< 親ウィンドウ
    PreviewBar bar_;		//!< 操作ボタン

    std::auto_ptr<PreviewData> data_;		//!< プレビューデータ
    Attribute attr_;		//!< 印刷情報
    Property property_;		//!< 設定

    int page_;			//!< 表示中のページ（0〜）
    bool zoom_;			//!< 拡大表示中かどうか

    enum {
	ZOOM_DPI = 128,		//!< 拡大表示時の画面のDPI
    };

    CPoint scroll_pos_;		//!< スクロールバーの現在設定位置

  public:
    PreviewWindow(CWnd*);

  protected:
    virtual void PostNcDestroy();
    afx_msg int OnCreate(LPCREATESTRUCT);
    afx_msg void OnPaint();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR*);
    afx_msg BOOL OnEraseBkgnd(CDC*);

    afx_msg void OnHScroll(UINT, UINT, CScrollBar*);
    afx_msg void OnVScroll(UINT, UINT, CScrollBar*);
    afx_msg void OnSize(UINT, int, int);
    
  public:
    void show(PreviewData*, const Attribute&, const Property&);
    void close();
    void destroy();

  private:
    /* ---------------------------------------------------------------- */
    //! 前のページへ移動
    /* ---------------------------------------------------------------- */
    void prev_page()
    {
	set_page(page_ - 1);
    }

    /* ---------------------------------------------------------------- */
    //! 次のページへ移動
    /* ---------------------------------------------------------------- */
    void next_page()
    {
	set_page(page_ + 1);
    }

    /* ---------------------------------------------------------------- */
    //! 表示モードを変更
    /*!
      全体表示と拡大表示をトグルで変更する				*/
    /* ---------------------------------------------------------------- */
    void change_zoom_mode()
    {
	zoom_ ? show_full() : show_zoom();
    }
    
    /* ---------------------------------------------------------------- */
    //! ウィンドウが生成されているか
    /*!
      @retval true 生成済み
      @retval false 未作成						*/
    /* ---------------------------------------------------------------- */
    bool is_valid() {return ::IsWindow(m_hWnd) != FALSE; }

    /* ---------------------------------------------------------------- */
    //! 操作ボタンの高さを取得
    /*!
      @return 操作ボタン部分の高さ					*/
    /* ---------------------------------------------------------------- */
    int get_bar_height() const
    {
	CRect rect;
	bar_.GetWindowRect(rect);
	return rect.Height();
    }

    /* ---------------------------------------------------------------- */
    //! 画面の再描画を行う
    /* ---------------------------------------------------------------- */
    void redraw()
    {
	CRect rect;	// 画面の大きさ
	GetClientRect(rect);
	rect.top = get_bar_height();
	InvalidateRect(rect);
    }

    void change_scale(CDC*);
    void adjust_scroll_bar();

    void set_page(size_t);
    void show_full();
    void show_zoom();

    DECLARE_MESSAGE_MAP();
};
