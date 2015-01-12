/*! @file
  @brief プレビュー用のテキスト出力
  @author 依積晶紀
*/
#pragma once

#include	"OutputBuffered.h"
#include	"PreviewData.h"

class PreviewWindow;

/************************************************************************/
//! プレビュー用のテキスト出力
/************************************************************************/
class OutputPreview : public OutputBuffered
{
  private:
    PreviewWindow& win_;		//!< プレビュー画面

  public:
    OutputPreview(CDC*, const Property&, LPCTSTR, PreviewWindow&);
    virtual ~OutputPreview();

    virtual void end();
    virtual void abort();
};
