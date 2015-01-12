/*! @file
  @brief ファイル入力
  @author 依積晶紀
*/
#pragma once

#include	"TextReader.h"

class Encoding;

/************************************************************************/
//! ファイルからのテキスト入力ストリーム
/************************************************************************/
class FileReader : public TextReader
{
  private:
    //!< ファイル名
    CString title_;
    
  public:
    FileReader(LPCTSTR, const Encoding&);

    virtual CString get_name() const;
    
  private:
    // ファイルサイズ取得
    static UINT get_file_size(const CFile&);
};
