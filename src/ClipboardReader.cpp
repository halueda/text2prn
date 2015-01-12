/*! @file
  @brief クリップボードからのテキスト入力ストリーム
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"ClipboardReader.h"
#include	"ClipboardUtil.h"

/* ==================================================================== */
//! クリップボード内のテキスト形式のデータで構築する。
/*!
  @throws Error クリップボード操作で失敗
  @throws std::bad_alloc クリップボードの内容をコピーするための領域確保失敗 */
/* ==================================================================== */
ClipboardReader::ClipboardReader()
{
    ClipboardData clip(CF_UNICODETEXT);

    const DWORD size = clip.get_size();	// クリップボードのデータサイズ

    char* const buff = new char[size];
    ::CopyMemory(buff, clip.get_data(), size);

    init_buff(reinterpret_cast<LPTSTR>(buff));
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! データの名称を取得する。
/*!
  データ名称は「クリップボード」固定値
  @return データ名							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
CString ClipboardReader::get_name() const
{
    return _T("クリップボード");
}

