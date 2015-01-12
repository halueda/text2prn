/*! @file
  @brief 印刷用のテキスト出力(ページ総数対応)
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"OutputPrinterBuffered.h"
#include	"PreviewWindow.h"
#include	"Printer.h"

/* ==================================================================== */
/*!
  @param [in] pDC 出力先のDC
  @param [in] property 設定
  @param [in] title タイトル
  @exception Warning 設定値が正しくない					*/
/* ==================================================================== */
OutputPrinterBuffered::OutputPrinterBuffered(CDC* pDC,
					     const Property& property,
					     LPCTSTR title) :
	OutputBuffered(pDC, property, title), abort_flag_(false)
{
    dc_.Attach(pDC->Detach());
}

OutputPrinterBuffered::~OutputPrinterBuffered()
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力を終了する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPrinterBuffered::end()
{
    OutputBuffered::end();

    if (!abort_flag_)
	start_print();
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力を中断する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPrinterBuffered::abort()
{
    abort_flag_ = true;
}

/* -------------------------------------------------------------------- */
//! 印刷を開始する
/*!
  @exception PrinterError 印刷に失敗					*/
/* -------------------------------------------------------------------- */
void OutputPrinterBuffered::start_print()
{
    Canvas canvas(dc_, get_attribute(), get_property());

    Printer p;
    p.init(&dc_, &canvas);
    try {
	p.start(title_);

	const size_t total_page = data_->get_page_count();
	for (size_t page = 0; page < total_page; page++) {

	    // 表示するテキストの範囲
	    const std::pair<size_t, size_t> region = data_->get_index(page);
	    if (region.first >= region.second)
		continue;
	    p.new_page(page + 1);
	    
	    for (size_t i = region.first; i < region.second; i++) {
		const PreviewData::Text& text = data_->get_text(i);
		p.output(text.col, text.line, text.str);
	    }	    
	}
	
    } catch (...) {
	p.abort();
    }
    p.end();
}
