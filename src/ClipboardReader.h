/*! @file
  @brief クリップボードからのテキスト入力ストリーム
  @author 依積晶紀
*/
#pragma once

#include	"TextReader.h"

/************************************************************************/
//! クリップボードからのテキスト入力ストリーム
/************************************************************************/
class ClipboardReader : public TextReader
{
  public:
    ClipboardReader();

    virtual CString get_name() const;
};
