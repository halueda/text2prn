/*! @file
  @brief プレビュー用のテキスト出力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"OutputPreview.h"
#include	"PreviewWindow.h"

/* ==================================================================== */
/*!
  @param [in] pDC 出力先のDC
  @param [in] property 設定
  @param [in] title タイトル
  @param [in] win プレビュー画面
  @exception Warning 設定値が正しくない					*/
/* ==================================================================== */
OutputPreview::OutputPreview(CDC* pDC, const Property& property,
			     LPCTSTR title, PreviewWindow& win) :
	OutputBuffered(pDC, property, title), win_(win)
{
}

OutputPreview::~OutputPreview()
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力を終了する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPreview::end()
{
    OutputBuffered::end();
    win_.show(data_.release(), get_attribute(), get_property());
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力を中断する時の処理
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void OutputPreview::abort()
{
    win_.close();
}
