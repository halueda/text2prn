/*! @file
  @brief その他
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"Util.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 指定フォーマットに基づいた文字列を返す。
/*!
  printf 同様のフォーマットが使用可能。
  
  @param format フォーマット
  @return フォーマット済み文字列					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
CString FString(LPCTSTR format, ...)
{
    CString str;

    va_list args;
    va_start(args, format);

    str.FormatV(format, args);

    va_end(args);

    return str;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 全てが空白文字なら、空文字にする。
/*!
  @param [in,out] str 文字列						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void trim_if_empty(CString& str)
{
    CString temp(str);
    temp.Trim();
    if (temp.IsEmpty())
	str.Empty();
}

//////////////////////////////////////////////////////////////////////

namespace FileIO
{

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ファイルから1行分の文字列を読み込む
/*!
  @param [in] in 入力もとのファイル
  @return 文字列
  @exception CFileException* 読み込み失敗/ファイルの終りに達した	*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
CString read_string(CStdioFile& in)
{
    CString str;
    if (!in.ReadString(str))
	AfxThrowFileException(CFileException::endOfFile);
    return str;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ファイルへ文字列を書き込む
/*!
  出力後は改行する。
  @param [in] out ファイル
  @param [in] str 文字列
  @exception CFileException* 書き込み失敗				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void write_string(CStdioFile& out, LPCTSTR str)
{
    out.WriteString(str);
    out.WriteString(_T("\n"));
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ファイルから１行読み込み、それを数値に変換してから返す。
/*!
  ファイルから読み込んだデータが数値として解釈できない場合の
  動作は不定。
  
  @param [in] in ファイル
  @return 値
  @exception CFileException* 読み込み失敗/ファイルの終りに達した	*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
int read_int(CStdioFile& in)
{
    return _ttoi(read_string(in));
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ファイルへ数値を書き込む
/*!
  出力後は改行する。

  @param [in] out ファイル
  @param [in] value 値
  @exception CFileException* 書き込み失敗				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void write_int(CStdioFile& out, unsigned int value)
{
    write_string(out, FString(_T("%u"), value));
}

}

//////////////////////////////////////////////////////////////////////

namespace FilePath
{

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! アプリケーションと同じディレクトリにあるファイルのパスを取得
/*!
  @param [in] name ファイル名
  @return ファイルパス							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
CString get_path_same_app_dir(LPCTSTR name)
{
    TCHAR path[MAX_PATH];
    ::GetModuleFileName(NULL, path, MAX_PATH);

    ::PathRemoveFileSpec(path);
    ::PathAppend(path, name);
    
    return path;
}
    
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ファイル名（パスを除く）を取得する。
/*!
  @param [in] path ファイルパス
  @return ファイル名							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
LPCTSTR get_name(LPCTSTR path)
{
    return ::PathFindFileName(path);
}

}
