/*! @file
  @brief ファイル入力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"FileReader.h"
#include	"Throwable.h"
#include	"CharType.h"
#include	"Encoding.h"
#include	"AutoBuff.h"

/* ==================================================================== */
//! ファイル内のテキストデータで構築する。
/*!
  @param [in] path テキストファイルのパス
  @param [in] enc 文字コード変換用のオブジェクト
  @exception Error ファイル操作で失敗 / 文字コードの指定間違い
  @exception std::bad_alloc ファイルの内容をコピーするための領域確保失敗 */
/* ==================================================================== */
FileReader::FileReader(LPCTSTR path, const Encoding& enc)
{
    ASSERT(path != NULL);

    try {
	CFile file(path, CFile::modeRead);
	title_ = file.GetFilePath();

	const UINT size = get_file_size(file);

	AutoBuff buff(size + 2);
	// 文字列の末尾に NUL を追加するため、余分に確保している。
	// (対応している文字コードでは、NULは UTF-16の2byteが最大)
	
	if (file.Read(buff.get(), size) != size)
	    throw Error(_T("ファイルの読み込み失敗。"));

	enc.convert(buff, size);

	const LPTSTR str = static_cast<LPTSTR>(buff.release());
	init_buff(str, *str == CHAR_BOM ? str + 1 : NULL);

    } catch (CException* e) {
	TCHAR mes[255];
	e->GetErrorMessage(mes, 255);
	e->Delete();

	throw Error(mes);
    }
}

/* -------------------------------------------------------------------- */
//! ファイルサイズを取得する。
/*!
  2Gを超える場合は、エラーとして例外を投げる。

  @param [in] file ファイルサイズを取得したいファイル
  @return ファイルサイズ
  @exception Error ファイルサイズが2G超					*/
/* -------------------------------------------------------------------- */
UINT FileReader::get_file_size(const CFile& file)
{
    const ULONGLONG size = file.GetLength();

    // ↓ここで -2 しているのは、呼び出し元で +2 して処理するところがあるから
    if (size > INT_MAX - 2)
	throw Error(_T("ファイルが大きすぎます。"));

    return static_cast<UINT>(size);

    // ファイルの中身を一気に読み込みたいのに、
    // CFile::Read() のサイズ指定は UINT型。
    // new char[〜] で指定できるサイズも unsigned int 型。
    // 後の処理で呼び出す 文字コード変換の MultiByteToWideChar は int 型。
    // 
    // 2Gを超えるテキストファイルを印刷することは考えられないので、
    // 2G以上はエラーとした。
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! データの名称を取得する。
/*!
  データ名称は、ファイル名
  @return データ名							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
CString FileReader::get_name() const
{
    return title_;
}
