/*! @file
  @brief 印刷用のテキスト出力
  @author 依積晶紀
*/
#pragma once

#include	"Output.h"
#include	"Canvas.h"
#include	"Throwable.h"

/************************************************************************/
//! プリンタへのテキストの出力
/************************************************************************/
class OutputPrinter : public Output
{
    const CString title_;	//!< テキストのタイトル
    int state_;			//!< 印刷の状態
    CDC dc_;			//!< プリンタのデバイスコンテキスト
    std::auto_ptr<Canvas> canvas_;	//!< 出力補助

    //! 印刷の状態
    enum {
	STATE_INIT,		//!< 初期状態
	STATE_PRE_START,	//!< 印刷準備中
	STATE_START,		//!< 印刷中
	STATE_ABORT,		//!< 印刷中断
	STATE_END,		//!< 印刷完了
    };

  public:
    OutputPrinter(CDC*, const Property&, LPCTSTR);
    virtual ~OutputPrinter();

    virtual void new_page();
    virtual void end();
    virtual void abort();

  protected:
    virtual bool init();
    virtual void do_print();
    
  private:
    void start_print();

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
