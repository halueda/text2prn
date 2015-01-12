/*! @file
  @brief 文字種判定
  @author 依積晶紀
*/
#pragma once

//////////////////////////////////////////////////////////////////////

const TCHAR CHAR_NUL = _T('\0');	//!< 文字列の終わり
const TCHAR CHAR_CR = _T('\r');		//!< 改行
const TCHAR CHAR_LF = _T('\n');		//!< 改行
const TCHAR CHAR_TAB = _T('\t');	//!< タブ
const TCHAR CHAR_FF = 0x000C;		//!< 改ページ
const TCHAR CHAR_BOM = _T('\uFEFF');	//!< エンディアン判定文字
const TCHAR CHAR_BOM_BE = _T('\uFFFE');	//!< エンディアン判定文字(BigEndian用)

// VC++の制限？ '\u000C' は使えない。

/************************************************************************/
//! 文字の種類を調べる
/************************************************************************/
class CharType
{
  private:
    static const unsigned char visible_map[];	//!< 文字の種類を判別するためのマップ
    static const unsigned char width_map[];	//!< 文字の種類を判別するためのマップ

  public:
    //! 文字の種類
    enum {
	HALF_SIZE,	//!< 半角文字
	FULL_SIZE,	//!< 全角文字
	TAB,		//!< タブ
	TERMINATE,	//!< 文字列終端
	NOT_PRINTABLE,	//!< 表示不可
    };

  public:
    static int detect(TCHAR);
};

//////////////////////////////////////////////////////////////////////

