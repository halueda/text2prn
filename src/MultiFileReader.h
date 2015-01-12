/*! @file
  @brief 複数ファイル入力
  @author 依積晶紀
*/
#pragma once

#include	"MultiReader.h"
#include	"FileInfo.h"

/************************************************************************/
//! 複数のファイル入力ストリームから連続して入力するテキスト入力ストリーム
/************************************************************************/
class MultiFileReader : public MultiReader
{
  private:
    const std::vector<FileInfo>& files_;	//!< ファイル一覧
    size_t current_;				//!< 処理中ファイル

  public:
    MultiFileReader(const std::vector<FileInfo>&, int, int);
    virtual ~MultiFileReader();

    virtual CString get_name() const;
    
  protected:
    virtual TextReader* get_next_reader();
};
