/*! @file
  @brief プレビュー画面
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"PreviewWindow.h"
#include	"Canvas.h"
#include	"Util.h"
#include	"Throwable.h"
#include	"resource.h"

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(PreviewWindow, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_GETMINMAXINFO()
    ON_WM_ERASEBKGND()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
END_MESSAGE_MAP()

/* ==================================================================== */
/*!
  @param [in] parent 親ウィンドウ					*/
/* ==================================================================== */
PreviewWindow::PreviewWindow(CWnd* parent) :
	parent_(parent), page_(0), zoom_(false)
{
}

/* ==================================================================== */
//! ウィンドウ作成時の処理
/* ==================================================================== */
int PreviewWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    const int result = CFrameWnd::OnCreate(lpCreateStruct);
    if (result != 0)
	return result;

    // 操作ボタン
    if (!bar_.Create(this, IDD_PREVIEW_BAR, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	return -1;
    ShowControlBar(&bar_, TRUE, FALSE);

    return 0;
}

/* ==================================================================== */
//! ウィンドウの大きさ指定
/* ==================================================================== */
void PreviewWindow::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
    // ウィンドウの最小サイズ
    lpMMI->ptMinTrackSize.x = 450;
    lpMMI->ptMinTrackSize.y = 200;

    CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

/* ==================================================================== */
//! 背景描画
/* ==================================================================== */
BOOL PreviewWindow::OnEraseBkgnd(CDC* pDC)
{
    CRect client_rect;	// 画面の大きさ
    GetClientRect(client_rect);
    ASSERT(client_rect.top == 0);

    // 操作ボタンのところは背景の描画をしない(ちらつき予防)
    client_rect.top = get_bar_height();
    pDC->FillSolidRect(client_rect, RGB(127, 127, 127));

    change_scale(pDC);

    // ----- 用紙の描画 -----
    CPen blue_pen;
    blue_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    pDC->SelectObject(&blue_pen);
    pDC->SelectStockObject(WHITE_BRUSH);
    pDC->Rectangle(0, 0, attr_.paper_width, attr_.paper_height);

    return FALSE;
}

/* ==================================================================== */
//! 描画
/* ==================================================================== */
void PreviewWindow::OnPaint()
{
    try {
	
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	// 表示するテキストの範囲
	const std::pair<size_t, size_t> region = data_->get_index(page_);
	if (region.first >= region.second)
	    return;
	
	change_scale(&dc);

	Canvas canvas(dc, attr_, property_);

	canvas.draw_new_page(page_ + 1);

	for (size_t i = region.first; i < region.second; i++) {
	    const PreviewData::Text& text = data_->get_text(i);
	    canvas.draw_text(text.col, text.line, text.str);
	}

    } catch (Exception& e) {
	static bool flag = false;
	if (!flag) {
	    flag = true;
	    MessageBox(e.get_message(), CAPTION_ERROR, MB_ICONWARNING);
	}
	close();
    }
}

/* ==================================================================== */
//! 横スクロール
/* ==================================================================== */
void PreviewWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar*)
{
    switch (nSBCode) {
      case SB_LEFT:
	SetScrollPos(SB_HORZ, 0);
	break;

      case SB_RIGHT:
	SetScrollPos(SB_HORZ, attr_.paper_width);
	break;

      case SB_LINELEFT:
	SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) - attr_.font_x);
	break;

      case SB_LINERIGHT:
	SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + attr_.font_x);
	break;

      case SB_PAGELEFT:
      {
	  SCROLLINFO sinfo;
	  sinfo.cbSize = sizeof(sinfo);
	  GetScrollInfo(SB_HORZ, &sinfo, SIF_PAGE);
	  SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) - sinfo.nPage);
      }
	break;
	
      case SB_PAGERIGHT:
      {
	  SCROLLINFO sinfo;
	  sinfo.cbSize = sizeof(sinfo);
	  GetScrollInfo(SB_HORZ, &sinfo, SIF_PAGE);
	  SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + sinfo.nPage);
      }
	break;
	

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
	SetScrollPos(SB_HORZ, nPos);
	break;

      default:
	return;
    }

    const int pos = GetScrollPos(SB_HORZ)*ZOOM_DPI/attr_.dpi_x;
    const int diff = pos - scroll_pos_.x;
    if (diff == 0)
	return;

    scroll_pos_.x = pos;

    CRect rect;
    GetClientRect(rect);
    rect.top = get_bar_height();
    ScrollWindow(-diff, 0, rect, rect);
    
    if (diff < 0) {
	rect.right = rect.left - diff;
    } else {
	rect.left = rect.right - diff;
    }
    InvalidateRect(rect);


//    redraw();
}

/* ==================================================================== */
//! 縦スクロール
/* ==================================================================== */
void PreviewWindow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar*)
{
    switch (nSBCode) {
      case SB_TOP:
	SetScrollPos(SB_VERT, 0);
	break;

      case SB_BOTTOM:
	SetScrollPos(SB_VERT, attr_.paper_height);
	break;

      case SB_LINEUP:
	SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - attr_.font_y);
	break;

      case SB_LINEDOWN:
	SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + attr_.font_y);
	break;

      case SB_PAGEDOWN:
      {
	  SCROLLINFO sinfo;
	  sinfo.cbSize = sizeof(sinfo);
	  GetScrollInfo(SB_VERT, &sinfo, SIF_PAGE);
	  SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + sinfo.nPage);
      }
	break;

      case SB_PAGEUP:
      {
	  SCROLLINFO sinfo;
	  sinfo.cbSize = sizeof(sinfo);
	  GetScrollInfo(SB_VERT, &sinfo, SIF_PAGE);
	  SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - sinfo.nPage);
      }
	break;

      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
	SetScrollPos(SB_VERT, nPos);
	break;

      default:
	return;
    }

    const int pos = GetScrollPos(SB_VERT)*ZOOM_DPI/attr_.dpi_y;
    const int diff = pos - scroll_pos_.y;
    if (diff == 0)
	return;

    scroll_pos_.y = pos;

    CRect rect;
    GetClientRect(rect);
    rect.top = get_bar_height();
    ScrollWindow(0, -diff, rect, rect);
    
    if (diff < 0) {
	rect.bottom = rect.top - diff;
    } else {
	rect.top = rect.bottom - diff;
    }
    InvalidateRect(rect);
    
//    redraw();
}

/* ==================================================================== */
//! 画面サイズ変更
/* ==================================================================== */
void PreviewWindow::OnSize(UINT, int cx, int cy)
{
    CRect rect;	// 画面の大きさ
    bar_.GetClientRect(rect);
    rect.right = cx;
    bar_.MoveWindow(rect);
    
    if (zoom_)
	adjust_scroll_bar();
}


/* ==================================================================== */
//! ウィンドウ破棄時の処理
/* ==================================================================== */
void PreviewWindow::PostNcDestroy()
{
    // 親クラスで delete this されている。
    // このクラスは new PreviewWindow()では作られないので、無効化する。
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ウィンドウを（必要なら）作成し、表示する
/*!
  @param [in] data プレビューで表示するデータ
  @param [in] attr 印刷情報
  @param [in] prop 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void PreviewWindow::show(PreviewData* data,
			 const Attribute& attr, const Property& prop)
{
    data_.reset(data);
    attr_ = attr;
    property_ = prop;
    
    if (!is_valid()) {
	Create(NULL, CAPTION_PREVIEW,
	       WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
	       rectDefault, parent_);
    }

    set_page(0);
    show_full();
    ShowWindow(SW_SHOW);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ウィンドウを閉じる
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void PreviewWindow::close()
{
    if (is_valid())
	PostMessage(WM_CLOSE);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ウィンドウを破棄する
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void PreviewWindow::destroy()
{
    if (is_valid())
	DestroyWindow();
}

/* -------------------------------------------------------------------- */
//! 表示しているページを変更する
/*!
  @param [in] p ページ番号（0〜）					*/
/* -------------------------------------------------------------------- */
void PreviewWindow::set_page(size_t p)
{
    // 総ページ数
    const size_t total_page = data_->get_page_count();

    // 範囲チェック
    if (p < 0)
	p = 0;
    else if (p >= total_page)
	p = total_page - 1;

    page_ = p;
    
    bar_.set_page(page_ + 1, total_page);

    redraw();
}

/* -------------------------------------------------------------------- */
//! 座標系をプリンタの大きさに合わせる
/*!
  @param [in] pDC ウィンドウのDC					*/
/* -------------------------------------------------------------------- */
void PreviewWindow::change_scale(CDC* pDC)
{
    const int offset = get_bar_height();

    pDC->SetMapMode(MM_ANISOTROPIC);
    
    if (zoom_) {

	pDC->SetWindowExt(attr_.dpi_x, attr_.dpi_y);
	pDC->SetViewportExt(ZOOM_DPI, ZOOM_DPI);
	pDC->SetViewportOrg(0, offset);
	pDC->SetWindowOrg(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
    } else {

	CRect client_rect;
	GetClientRect(client_rect);
	// 画面の大きさ
	const int client_width = client_rect.Width();
	const int client_height = client_rect.Height() - offset;
    
	// 用紙の大きさ
	const int paper_width = attr_.paper_width;
	const int paper_height = attr_.paper_height;

    // 画面一杯に表示するための拡大率の比
	const double x_rate = static_cast<double>(client_width)*(attr_.dpi_y*paper_height);
	const double y_rate = static_cast<double>(client_height)*(attr_.dpi_x*paper_width);

	if (x_rate > y_rate) {
	    // 縦一杯
	    const int draw_width = static_cast<int>(y_rate/(attr_.dpi_y*paper_height) + 0.5);
	    const int x_offset = (client_width - draw_width)/2;
	    ASSERT(x_offset >= 0);

	    pDC->SetWindowExt(paper_height, paper_height);
	    pDC->SetViewportExt(client_height, client_height);
	    pDC->SetViewportOrg(x_offset, offset);
	
	} else {
	    // 横一杯
	    const int draw_height = static_cast<int>(x_rate/(attr_.dpi_x*paper_width) + 0.5);
	    const int y_offset = (client_height - draw_height)/2 + offset;
	    ASSERT(y_offset >= offset);

	    pDC->SetWindowExt(paper_width, paper_width);
	    pDC->SetViewportExt(client_width, client_width);
	    pDC->SetViewportOrg(0, y_offset);
	}
	pDC->SetWindowOrg(0, 0);
    }
}

/* -------------------------------------------------------------------- */
//! 全体表示に切り替える。
/* -------------------------------------------------------------------- */
void PreviewWindow::show_full()
{
    zoom_ = false;
    bar_.GetDlgItem(IDC_ZOOM)->SetWindowText(_T("拡大(&Z)"));
    ShowScrollBar(SB_BOTH, FALSE);
}

/* -------------------------------------------------------------------- */
//! 拡大表示に切り替える。
/* -------------------------------------------------------------------- */
void PreviewWindow::show_zoom()
{
    zoom_ = true;
    bar_.GetDlgItem(IDC_ZOOM)->SetWindowText(_T("全体(&F)"));
    ShowScrollBar(SB_BOTH, TRUE);
    adjust_scroll_bar();
}

/* -------------------------------------------------------------------- */
//! スクロールバーの設定を行う
/* -------------------------------------------------------------------- */
void PreviewWindow::adjust_scroll_bar()
{
    ASSERT(zoom_);

    const int offset = get_bar_height();

    CRect client_rect;
    GetClientRect(client_rect);
    // 画面の大きさ
    const int client_width = client_rect.Width();
    const int client_height = client_rect.Height() - offset;

    // 用紙の大きさ
    const int paper_width = attr_.paper_width;
    const int paper_height = attr_.paper_height;

    SCROLLINFO sinfo;
    sinfo.cbSize = sizeof(sinfo);
    sinfo.fMask = SIF_RANGE | SIF_PAGE;
    sinfo.nMin = 0;
    sinfo.nMax = paper_height;
    sinfo.nPage = client_height * attr_.dpi_y/ZOOM_DPI;

    SetScrollInfo(SB_VERT, &sinfo);

    sinfo.nMin = 0;
    sinfo.nMax = paper_width;
    sinfo.nPage = client_width * attr_.dpi_x/ZOOM_DPI;

    SetScrollInfo(SB_HORZ, &sinfo);

    scroll_pos_.x = GetScrollPos(SB_HORZ)*ZOOM_DPI/attr_.dpi_x;
    scroll_pos_.y = GetScrollPos(SB_VERT)*ZOOM_DPI/attr_.dpi_y;
}


//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(PreviewWindow::PreviewBar, CDialogBar)
    ON_COMMAND(IDC_NEXTPAGE, OnNextPage)
    ON_COMMAND(IDC_PREVPAGE, OnPrevPage)
    ON_COMMAND(IDC_ZOOM, OnZoom)
    ON_COMMAND(IDC_CLOSE, OnCloseWindow)
END_MESSAGE_MAP()

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 表示しているページを設定する
/*!
  @param [in] current 現在表示しているページ（1〜）
  @param [in] total 総ページ数						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void PreviewWindow::PreviewBar::set_page(size_t current, size_t total)
{
    GetDlgItem(IDC_NEXTPAGE)->EnableWindow(current >= total ? FALSE : TRUE);
    GetDlgItem(IDC_PREVPAGE)->EnableWindow(current <= 1 ? FALSE : TRUE);
    SetDlgItemText(IDC_PAGE, FString(_T("%d/%d"), current, total));
}

/* ==================================================================== */
//! [次ページ]ボタン
/* ==================================================================== */
void PreviewWindow::PreviewBar::OnNextPage()
{
    get_preview_window()->next_page();
}

/* ==================================================================== */
//! [前ページ]ボタン
/* ==================================================================== */
void PreviewWindow::PreviewBar::OnPrevPage()
{
    get_preview_window()->prev_page();
}

/* ==================================================================== */
//! [拡大/全体]ボタン
/* ==================================================================== */
void PreviewWindow::PreviewBar::OnZoom()
{
    get_preview_window()->change_zoom_mode();
}

/* ==================================================================== */
//! [閉じる]ボタン
/* ==================================================================== */
void PreviewWindow::PreviewBar::OnCloseWindow()
{
    get_preview_window()->close();
}

//////////////////////////////////////////////////////////////////////
