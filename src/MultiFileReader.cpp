/*! @file
  @brief 複数ファイル入力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"MultiFileReader.h"
#include	"FileReaderUtil.h"
#include	"Util.h"

/* ==================================================================== */
/*!
  @param [in] files ファイル一覧
  @param [in] type 複数ファイルの区切り方法
  @param [in] num 複数ファイルを空行で区切る場合の行数			*/
/* ==================================================================== */
MultiFileReader::MultiFileReader(const std::vector<FileInfo>& files,
				 int type, int num) :
	MultiReader(static_cast<MultiReader::SeparatorType>(type), num), files_(files), current_(0)
{
}

MultiFileReader::~MultiFileReader()
{
}

/* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
//! 次のテキスト入力ストリームを取得する
/*!
  全て処理済みの場合は、NULLを返す。

  @return 次に入力対象となるテキスト入力ストリーム
  @exception Error ファイル操作で失敗 / 文字コードの指定間違い
  @exception std::bad_alloc ファイルの内容をコピーするための領域確保失敗 */
/* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
TextReader* MultiFileReader::get_next_reader()
{
    if (current_ >= files_.size())
	return NULL;

    const FileInfo& info = files_[current_++];

    return FileReaderUtil::get_reader(info.path, info.encoding);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! データの名称を取得する。
/*!
  データ名称は「対象としているファイルの総数」
  @return データ名							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
CString MultiFileReader::get_name() const
{
    return FString(_T("%d 個のファイル"), files_.size());
}
