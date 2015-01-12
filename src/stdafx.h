#pragma once

#pragma warning(disable : 4786)

//! Windows ヘッダーから使用されていない部分を除外します。
#define WIN32_LEAN_AND_MEAN

#define WINVER 0x0500		// Win2K 以降

#include	<afxwin.h>
#include	<afxcmn.h>
#include	<afxdlgs.h>
#include	<afxext.h>
#include 	<shlwapi.h>

#include	<vector>
#include	<utility>
#include	<map>
#include	<string>
#include	<algorithm>

//#define OUTPUT_FILE_PREVIEW
// ↑
// これを定義すると、
// プレビュー時にテキストの内容をファイル(output.txt)に保存する。
// レイアウトの確認用

#pragma comment(lib, "shlwapi.lib")

#ifdef DEBUG
# define NODEFAULT   _ASSERT(0)
#else
# define NODEFAULT   __assume(0)
#endif

#define	CAPTION_ERROR	_T("エラー [テキスト印刷]")
#define	CAPTION_WARNING	_T("警告 [テキスト印刷]")
#define	CAPTION_CONFIRM	_T("確認 [テキスト印刷]")
#define CAPTION_PREVIEW _T("プレビュー [テキスト印刷]")

const UINT WM_APPLY_PRESET = WM_USER + 100;	//!< 登録済み設定の適用
const UINT WM_EDIT_PRESET  = WM_USER + 101;	//!< 登録済み設定の編集開始
const UINT WM_PREVIEW      = WM_USER + 102;	//!< プレビュー開始
const UINT WM_PAGE_SETUP   = WM_USER + 103;	//!< プリンタ設定開始
const UINT WM_USE_HEADER   = WM_USER + 104;	//!< ヘッダ/フッタの使用 変更

#include	"resource.h"
