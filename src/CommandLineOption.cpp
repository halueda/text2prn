/*! @file
  @brief コマンドライン引数
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"CommandLineOption.h"

/* ==================================================================== */
//! コマンドライン引数の解析
/*
  スイッチ("/","-"で始まる)は除外し、
  存在するファイルのみを記憶する。					*/
/* ==================================================================== */
void CommandLineOption::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
    if (bFlag)
	return;

    if (!::PathFileExists(lpszParam))
	return;

    if (::PathIsDirectory(lpszParam))
	return;
	
    files_.push_back(lpszParam);
}
