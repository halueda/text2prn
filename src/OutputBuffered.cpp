/*! @file
  @brief 出力内容をメモリ上に蓄えるテキスト出力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"OutputBuffered.h"

/* ==================================================================== */
/*!
  @param [in] pDC 出力先のDC
  @param [in] property 設定
  @param [in] title タイトル
  @exception Warning 設定値が正しくない					*/
/* ==================================================================== */
OutputBuffered::OutputBuffered(CDC* pDC, const Property& property,
			     LPCTSTR title) :
	Output(pDC, property, title), data_(new PreviewData())
{
    new_page();
}

OutputBuffered::~OutputBuffered()
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力を終了する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputBuffered::end()
{
    data_->markNewPage();
    const_cast<Attribute&>(get_attribute()).total_page = data_->get_page_count();
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 次のページへ移行するときの処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputBuffered::new_page()
{
    Output::new_page();
    data_->markNewPage();
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! テキストを出力する毎の初期化処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool OutputBuffered::init()
{
    return true;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! テキストを出力する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputBuffered::do_print()
{
    data_->addText(get_col(), get_line(), get_line_buff());
}
