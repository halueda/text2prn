/*! @file
  @brief 印刷用のテキスト出力(ページ総数対応)
  @author 依積晶紀
*/
#pragma once

#include	"OutputBuffered.h"
#include	"Canvas.h"

/************************************************************************/
//! プリンタへのテキストの出力(ページ総数対応)
/************************************************************************/
class OutputPrinterBuffered : public OutputBuffered
{
  private:
    const CString title_;	//!< テキストのタイトル
    CDC dc_;			//!< プリンタのデバイスコンテキスト
    bool abort_flag_;		//!< 出力の中止をしたかどうか

  public:
    OutputPrinterBuffered(CDC*, const Property&, LPCTSTR);
    virtual ~OutputPrinterBuffered();

    virtual void end();
    virtual void abort();

  private:
    void start_print();
};
