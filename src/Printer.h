/*! @file
  @brief 印刷
  @author 依積晶紀
*/
#pragma once

#include	"Throwable.h"
#include	"Canvas.h"

/************************************************************************/
//! 印刷
/************************************************************************/
class Printer
{
// OutputPrinter から印刷用の処理を分離し、こっちに持ってこれないか検討する。
    
  private:
    int state_;		//!< 印刷の状態
    CDC* pDC_;		//!< プリンタのデバイスコンテキスト
    Canvas* canvas_;	//!< 出力補助

    //! 印刷の状態
    enum {
	STATE_INIT,		//!< 初期状態
	STATE_PRE_START,	//!< 印刷準備中
	STATE_START,		//!< 印刷中
	STATE_ABORT,		//!< 印刷中断
	STATE_END,		//!< 印刷完了
    };

  public:
    Printer();

  public:
    void init(CDC*, Canvas*);
    void start(LPCTSTR);
    void end();
    void abort();
    void new_page(int);

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! テキストを出力する
    /*! @param [in] column 段
	@param [in] line 段の中での行
	@param [in] str 出力するテキスト				*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void output(int column, int line, LPCTSTR str)
    {
	canvas_->draw_text(column, line, str);
    }

  private:
    void start_print(LPCTSTR);

    /* ---------------------------------------------------------------- */
    //! エラー発生を記憶し、例外を投げる
    /*! @param reason エラーの原因
	@exception PrintError エラー					*/
    /* ---------------------------------------------------------------- */
    void error(int reason)
    {
	state_ = STATE_ABORT;
	throw PrinterError(reason);
    }    
};
