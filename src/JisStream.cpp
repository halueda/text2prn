/*! @file
  @brief ISO-2022-JP の文字列
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"JisStream.h"

/* ==================================================================== */
//! ISO-2022-JPで記述された文字列を指定して構築する。
/*!
  文字列は NUL終端であること。
  
  指定の文字列の文字コードが妥当かどうかの検証は行わない。
  
  @param [in] str ISO-2022-JPで記述された文字列				*/
/* ==================================================================== */
JisStream::JisStream(const unsigned char* str) : str_(str), category_(ASCII)
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 次の文字へ移動する。
/*!
  次の文字を表すコードまでポインタを進める。
  （文字の種類を表す制御コードは飛ばす）

  文字の種類切り替え用の制御コード一覧\n
  ESC $ B	2byteコード\n
  ESC $ \@	2byteコード\n
  ESC ( J	ASCIIコード\n
  ESC ( B	ASCIIコード\n
  ESC ( I	半角カナ

  上記以外の制御コードは全て ASCII文字として処理する。
  2byteコードの2byte目が存在しない時は、無理やりASCII文字として解釈する。  
  
  @attention falseを返した後では、next()の呼び出しは行わない。
  @retval true 文字がまだある
  @retval false 文字列の終わりに達した					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool JisStream::next()
{
    if (category_ == KANJI) {
	ASSERT(*str_ != '\0');
	str_ += 1;
    }

    while (*str_ == '\x1b') {
	if (*(str_+1) == '$' && (*(str_+2) == '@' || *(str_+2) == 'B')) {
	    str_ += 3;
	    category_ = KANJI;
	    continue;
	} else if (*(str_+1) == '(') {
	    if (*(str_+2) == 'J' || *(str_+2) == 'B') {
		str_ += 3;
		category_ = ASCII;
		continue;
	    } else if (*(str_+2) == 'I') {
		str_ += 3;
		category_ = KANA;
		continue;
	    }
	}
	category_ = ASCII;
	break;
    }

    if (*str_ == '\0')
	return false;

    if (category_ == KANJI && *(str_ + 1) == '\0')
	// 2byteコードの2byte目が存在しない
	category_ = ASCII;

    str_ += 1;
    return true;
}
