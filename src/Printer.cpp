/*! @file
  @brief 印刷
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"Printer.h"

Printer::Printer() :
	state_(STATE_INIT), pDC_(NULL), canvas_(NULL)
{
    
}


/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 初期化
/*!
  @param [in] pDC プリンタのデバイスコンテキスト
  @param [in] canvas 出力補助						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Printer::init(CDC* pDC, Canvas* canvas)
{
    pDC_ = pDC;
    canvas_ = canvas;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 印刷を開始する
/*!
  @param [in] title タイトル
  @exception PrinterError 印刷に失敗					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Printer::start(LPCTSTR title)
{
    switch (state_) {
      case STATE_ABORT:
      case STATE_END:
	ASSERT(false);
	return;

      case STATE_INIT:
	start_print(title);
	break;
    }
}

/* -------------------------------------------------------------------- */
//! 印刷を開始する
/*!
  @param [in] title タイトル
  @exception PrinterError 印刷に失敗					*/
/* -------------------------------------------------------------------- */
void Printer::start_print(LPCTSTR title)
{
    int result;
    
    DOCINFO di;
    ::ZeroMemory(&di, sizeof(DOCINFO));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = title;
    if ((result = pDC_->StartDoc(&di)) <= 0)
	error(result);

    state_ = STATE_PRE_START;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//!  出力を終了する時の処理
/*!
  @exception PrinterError 印刷に失敗					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Printer::end()
{
    // ===== 印刷の停止 =====
    switch (state_) {
      case STATE_INIT:
      case STATE_PRE_START:
      case STATE_END:
	break;
      case STATE_START:
	if (pDC_->EndPage() <= 0) {
	    pDC_->AbortDoc();
	} else {
	    pDC_->EndDoc();
	}
	break;
      case STATE_ABORT:
	pDC_->AbortDoc();
	break;
    }
    state_ = STATE_END;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力を中断する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Printer::abort()
{
    switch (state_) {
      case STATE_ABORT:
      case STATE_END:
      case STATE_INIT:
      case STATE_PRE_START:
	break;;
      case STATE_START:
	state_ = STATE_ABORT;
	pDC_->AbortDoc();
	break;
    }
}


/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 次のページへ移行するときの処理
/*!
  @param [in] page_no ページ番号
  @exception PrinterError 印刷に失敗					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Printer::new_page(int page_no)
{
    int result;
    if (state_ == STATE_START) {
	if ((result = pDC_->EndPage()) <= 0)
	    error(result);
    } else if (state_ == STATE_PRE_START) {
	state_ = STATE_START;
    }

    if ((result = pDC_->StartPage()) <= 0)
	error(result);

    canvas_->draw_new_page(page_no);
}
