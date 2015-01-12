/*! @file
  @brief 文字種判定
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"CharType.h"

/* -------------------------------------------------------------------- */
//! 文字の種類判別用テーブル
/* -------------------------------------------------------------------- */
const unsigned char CharType::visible_map[] = {
#include "visible.map"
};

const unsigned char CharType::width_map[] = {
#include "width.map"
};

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 文字の種類を調べる。
/*!
  マルチバイト文字もあるので、文字列を渡し、先頭文字の種類を返す。
  @param c 文字
  @retval HALF_SIZE 半角文字
  @retval FULL_SIZE 全角文字
  @retval TAB タブ
  @retval TERMINATE 文字列終端
  @retval NOT_PRINTABLE 表示不可文字					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
int CharType::detect(TCHAR c)
{
    if (c == CHAR_NUL)
	return TERMINATE;
    if (c == CHAR_TAB)
	return TAB;

    if (!visible_map[c/8] & (1 << (c % 8)))
	return NOT_PRINTABLE;

    return width_map[c/8] & (1 << (c % 8)) ? FULL_SIZE : HALF_SIZE;
}

//////////////////////////////////////////////////////////////////////

#if 0

#include	<unicode/unistr.h>
#include	<unicode/ucnv.h>
#include	<unicode/uchar.h>
#pragma comment(lib, "icuuc.lib")

// code_map[]を作成するプログラム
int main(int argc, char* argv[])
{
    unsigned char visible_map[65536/8] = {0};
    unsigned char width_map[65536/8] = {0};
    
    for (int i = 0; i <= 0xffff; i++) {
	wchar_t c = i;
	if (u_isprint(c)) {
	    visible_map[i/8] |= (1 << (i % 8));

	    UEastAsianWidth width =
		    (UEastAsianWidth) u_getIntPropertyValue(c, UCHAR_EAST_ASIAN_WIDTH);

	    switch (width) {
	      case U_EA_WIDE:
	      case U_EA_FULLWIDTH:
	      case U_EA_AMBIGUOUS:
		width_map[i/8] |= (1 << (i % 8));
		break;
	    }
	}
    }

    int x, y;
    for (y = 0; y < 16*2; y++) {
	for (x = 0; x < 8; x++) {
	    printf(" 0x%02x,", code[y*8+x]);
	}
	printf("\n");
    }

    return 0;
}
#endif
