/*! @file
  @brief 印刷イメージの描画
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"Canvas.h"
#include	"Attribute.h"
#include	"Property.h"
#include	"Util.h"
#include	"Throwable.h"

/* ==================================================================== */
/*!
  @param [in] dc 出力先のDC
  @param [in] attr 印刷情報
  @param [in] prop 設定
  @exception Warning フォント作成失敗					*/
/* ==================================================================== */
Canvas::Canvas(CDC& dc, const Attribute& attr, const Property& prop) :
	dc_(dc), old_font_(NULL), attr_(attr), prop_(prop)
{
    if (!font_body_.CreateFontIndirect(prop_.lf))
	throw Warning(_T("印刷用フォントが作成できませんでした。"));

    if (prop_.vertical) {    
	LOGFONT lf = *(prop_.lf);
	lf.lfEscapement = 0;
	if (lf.lfFaceName[0] == _T('@')) {
	    _tcsncpy_s(lf.lfFaceName, prop_.lf->lfFaceName + 1, LF_FACESIZE - 1);
	    lf.lfFaceName[LF_FACESIZE - 1] = _T('\0');
	}
	if (!font_title_.CreateFontIndirect(&lf))
	    throw Warning(_T("印刷用フォントが作成できませんでした。"));
    }
    
    old_font_ = dc_.SelectObject(&font_body_);
}

Canvas::~Canvas()
{
    if (old_font_ != NULL) {
	dc_.SelectObject(old_font_);
	old_font_ = NULL;
    }
}

/* -------------------------------------------------------------------- */
//! 段組の区切り線を印刷する。
/* -------------------------------------------------------------------- */
void Canvas::draw_column_line()
{
    CPen pen(PS_DASH, 1, RGB(0, 0, 0));
    CPen* old_pen = dc_.SelectObject(&pen);

    if (prop_.vertical) {
	for (int i = 1; i < prop_.column; i++) {
	    const int y = attr_.offset_y + i*(attr_.printable_height + attr_.column_space)/prop_.column - attr_.column_space/2;

	    dc_.MoveTo(attr_.offset_x - attr_.printable_width, y);
	    dc_.LineTo(attr_.offset_x, y);
	}
    } else {
	for (int i = 1; i < prop_.column; i++) {
	    const int x = attr_.offset_x + i*(attr_.printable_width + attr_.column_space)/prop_.column - attr_.column_space/2;

	    dc_.MoveTo(x, attr_.offset_y);
	    dc_.LineTo(x, attr_.offset_y + attr_.printable_height);
	}
    }

    dc_.SelectObject(old_pen);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 新しいページを出力する。
/*!
  設定により、段組の区切り線、ページ番号、タイトルを出力する。
  @param [in] page_no ページ番号					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Canvas::draw_new_page(int page_no)
{
    // 段組の区切り線
    if (prop_.output_column_line)
	draw_column_line();

    TermContext context;
    context.page = page_no;
    context.total_page = attr_.total_page;
    
    // ヘッダ
    if (attr_.output_header)
	draw_header(context);

    // フッタ
    if (attr_.output_footer)
	draw_footer(context);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ヘッダを出力する
/*!
  @param [in] context コンテキスト					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Canvas::draw_header(const TermContext& context)
{
    if (prop_.vertical) {

	CRect rect(attr_.offset_x - attr_.printable_width,
		   attr_.offset_y - 2*attr_.font_y,
		   attr_.offset_x,
		   attr_.offset_y - attr_.font_y);
	
	CFont* main_font = dc_.SelectObject(&font_title_);

	CString term;
	term = attr_.lheader.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_LEFT | DT_NOPREFIX);

	term = attr_.cheader.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_CENTER | DT_NOPREFIX);

	term = attr_.rheader.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_RIGHT | DT_NOPREFIX);

	dc_.SelectObject(main_font);

    } else {
	CRect rect(attr_.offset_x,
		   attr_.offset_y - attr_.font_y - attr_.line_space,
		   attr_.offset_x + attr_.printable_width,
		   attr_.offset_y - attr_.line_space);

	CString term;
	term = attr_.lheader.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_LEFT | DT_NOPREFIX);

	term = attr_.cheader.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_CENTER | DT_NOPREFIX);

	term = attr_.rheader.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_RIGHT | DT_NOPREFIX);
    }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! フッタを出力する
/*!
  @param [in] context コンテキスト					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Canvas::draw_footer(const TermContext& context)
{
    if (prop_.vertical) {

	CRect rect(attr_.offset_x - attr_.printable_width,
		   attr_.offset_y + attr_.printable_height + attr_.font_y,
		   attr_.offset_x,
		   attr_.offset_y + attr_.printable_height + 2*attr_.font_y);
	
	CFont* main_font = dc_.SelectObject(&font_title_);

	CString term;
	term = attr_.lfooter.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_LEFT | DT_NOPREFIX);

	term = attr_.cfooter.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_CENTER | DT_NOPREFIX);

	term = attr_.rfooter.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_RIGHT | DT_NOPREFIX);

	dc_.SelectObject(main_font);

    } else {
	CRect rect(attr_.offset_x,
		   attr_.offset_y + attr_.printable_height + attr_.line_space,
		   attr_.offset_x + attr_.printable_width,
		   attr_.offset_y + attr_.printable_height + attr_.line_space + attr_.font_y);
	
	CString term;
	term = attr_.lfooter.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_LEFT | DT_NOPREFIX);

	term = attr_.cfooter.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_CENTER | DT_NOPREFIX);

	term = attr_.rfooter.get_term(context);
	if (!term.IsEmpty())
	    dc_.DrawText(term, rect, DT_SINGLELINE | DT_RIGHT | DT_NOPREFIX);
    }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! テキストを出力する
/*!
  @param [in] column 段
  @param [in] line 段の中での行
  @param [in] str 出力するテキスト					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Canvas::draw_text(int column, int line, LPCTSTR str)
{
    if (prop_.vertical) {
	const int x = attr_.offset_x - line*(attr_.font_y + attr_.line_space);
	const int y = attr_.offset_y + column*(attr_.printable_height + attr_.column_space)/prop_.column;

	dc_.TextOut(x, y, str);
    } else {
	const int x = attr_.offset_x + column*(attr_.printable_width + attr_.column_space)/prop_.column;
	const int y = attr_.offset_y + line*(attr_.font_y + attr_.line_space);

	dc_.TextOut(x, y, str);
    }
}
