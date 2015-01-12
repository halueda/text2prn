/*! @file
  @brief ファイル入力補助
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"FileReaderUtil.h"
#include	"Encoding.h"
#include	"FileInfo.h"

//! ファイル入力補助
namespace FileReaderUtil
{

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 指定文字コード用の「ファイルからのテキスト入力ストリーム」を取得する。
/*!
  @param [in] path ファイルパス
  @param [in] enc 文字コードのID (FileInfo参照)
  @return ファイルからのテキスト入力ストリーム
  @exception Error ファイル操作で失敗 / 文字コードの指定間違い
  @exception std::bad_alloc ファイルの内容をコピーするための領域確保失敗 */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
FileReader* get_reader(LPCTSTR path, int enc)
{
    switch (enc) {
      case FileInfo::SJIS:
	return new FileReader(path, EncodingSJIS());

      case FileInfo::UTF16:
	return new FileReader(path, EncodingUTF16(true));

      case FileInfo::UTF16BE:
	return new FileReader(path, EncodingUTF16(false));

      case FileInfo::UTF8:
	return new FileReader(path, EncodingUTF8());

      case FileInfo::JIS:
	return new FileReader(path, EncodingJIS());

      case FileInfo::EUC:
	return new FileReader(path, EncodingEUC());
    }

    ASSERT(false);
    return NULL;
}

}
