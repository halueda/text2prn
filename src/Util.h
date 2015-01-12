/*! @file
  @brief その他
  @author 依積晶紀
*/
#pragma once

CString FString(LPCTSTR, ...);
void trim_if_empty(CString&);

//! ファイル入出力関連
namespace FileIO
{
CString read_string(CStdioFile&);
void write_string(CStdioFile&, LPCTSTR);
int read_int(CStdioFile&);
void write_int(CStdioFile&, unsigned int);
}

//! ファイル名関連
namespace FilePath
{
CString get_path_same_app_dir(LPCTSTR);
LPCTSTR get_name(LPCTSTR);
}
