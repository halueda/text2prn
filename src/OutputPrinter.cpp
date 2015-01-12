/*! @file
  @brief 印刷用のテキスト出力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"OutputPrinter.h"
#include	"Util.h"

/* ==================================================================== */
/*!
  @param [in] pDC 出力先のDC
  @param [in] property 設定
  @param [in] title タイトル
  @exception Warning 設定値が正しくない
  @attention 引数で指定したDCは無効になる。				*/
/* ==================================================================== */
OutputPrinter::OutputPrinter(CDC* pDC, const Property& property,
			     LPCTSTR title) :
	Output(pDC, property, title),
	state_(STATE_INIT),
	title_(FString(_T("%s - テキスト印刷"), get_attribute().title))
{
    dc_.Attach(pDC->Detach());
    canvas_.reset(new Canvas(dc_, get_attribute(), get_property()));
}

OutputPrinter::~OutputPrinter()
{
    // ===== 印刷の停止 =====
    switch (state_) {
      case STATE_INIT:
      case STATE_PRE_START:
      case STATE_END:
	break;
      case STATE_START:
	if (dc_.EndPage() <= 0) {
	    dc_.AbortDoc();
	} else {
	    dc_.EndDoc();
	}
	break;
      case STATE_ABORT:
	dc_.AbortDoc();
	break;
    }
}

/* -------------------------------------------------------------------- */
//! 印刷を開始する
/*!
  @exception PrinterError 印刷に失敗					*/
/* -------------------------------------------------------------------- */
void OutputPrinter::start_print()
{
    int result;
    
    DOCINFO di;
    ::ZeroMemory(&di, sizeof(DOCINFO));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = title_;
    if ((result = dc_.StartDoc(&di)) <= 0)
	error(result);

    state_ = STATE_PRE_START;
    
    new_page();

    state_ = STATE_START;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 次のページへ移行するときの処理
/*!
  @exception PrinterError 印刷に失敗					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPrinter::new_page()
{
    Output::new_page();

    int result;
    if (state_ == STATE_START) {
	if ((result = dc_.EndPage()) <= 0)
	    error(result);
    }

    if ((result = dc_.StartPage()) <= 0)
	error(result);

    canvas_->draw_new_page(get_page());
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//!  出力を終了する時の処理
/*!
  @exception PrinterError 印刷に失敗					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPrinter::end()
{
    int result;

    switch (state_) {
      case STATE_INIT:
      case STATE_PRE_START:
	state_ = STATE_END;
	break;
      case STATE_START:
	state_ = STATE_END;
	if ((result = dc_.EndPage()) <= 0)
	    error(result);
	if ((result = dc_.EndDoc()) <= 0)
	    error(result);
	break;
      case STATE_ABORT:
      case STATE_END:
	break;
    }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力を中断する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPrinter::abort()
{
    switch (state_) {
      case STATE_ABORT:
      case STATE_END:
      case STATE_INIT:
      case STATE_PRE_START:
	break;;
      case STATE_START:
	state_ = STATE_ABORT;
	dc_.AbortDoc();
	break;
    }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! テキストを出力する毎の初期化処理
/*!
  @exception PrinterError 印刷に失敗					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool OutputPrinter::init()
{
    switch (state_) {
      case STATE_ABORT:
      case STATE_END:
	ASSERT(false);
	return false;

      case STATE_INIT:
	start_print();
	break;
    }

    return true;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! テキストを出力する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPrinter::do_print()
{
    canvas_->draw_text(get_col(), get_line(), get_line_buff());
}
