/*! @file
  @brief 出力内容をメモリ上に蓄えるテキスト出力
  @author 依積晶紀
*/
#pragma once

#include	"Output.h"
#include	"PreviewData.h"

/************************************************************************/
//! 出力内容をメモリ上に蓄えるテキスト出力
/************************************************************************/
class OutputBuffered : public Output
{
  protected:
    std::auto_ptr<PreviewData> data_;	//!< 出力データ

  public:
    OutputBuffered(CDC*, const Property&, LPCTSTR);
    virtual ~OutputBuffered();

    virtual void new_page();
    virtual void end() = 0;
//    virtual void abort();

  protected:
    virtual bool init();
    virtual void do_print();
};
