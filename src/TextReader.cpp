/*! @file
  @brief テキスト入力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"TextReader.h"
#include	"CharType.h"

TextReader::TextReader() : buff_(NULL), current_(NULL), pos_(NULL)
{
}

TextReader::~TextReader()
{
    delete[] buff_;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 次の行へ移動する。
/*!
  get_text()が次の行を返すように、現在位置を1行分進める。

  行の区切りは CR, LF, NUL とする。
  （CR LFは、これで1つの行区切り記号とする）

  行頭にある FF は改段落として、これ１つで１行と扱う。

  テキストデータの終わりに達して次の行が存在しない時は、false を返す。

  テキストデータが空の場合、
  初回の呼び出し時のみ true を返す。\n
  （この時の get_text() は、空文字列を返す）
	
  @retval true 移動は成功した
  @retval false 次の行は存在しない					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool TextReader::next()
{
    ASSERT(buff_ != NULL);
    ASSERT(pos_ != NULL);
    
    if (*pos_ == CHAR_NUL) {
	if (current_ == NULL) {
	    // テキストデータがが空の時の初回呼び出し。
	    // 空文字列を表す。
	    current_ = pos_;
	    return true;
	}

	return false;
    }

    current_ = pos_;
    if (*pos_ == CHAR_FF) {
	switch (*(++pos_)) {
	  case CHAR_CR:
	    if (*(++pos_) == CHAR_LF)
		++pos_;
	    break;
	  case CHAR_LF:
	    ++pos_;
	    break;
	}
	return true;
    }

    while (*pos_ != CHAR_NUL) {
	if (*pos_ == CHAR_CR) {
	    *(pos_++) = CHAR_NUL;
	    if (*pos_ == CHAR_LF)
		// CR LF で1つの改行
		++pos_;
	    break;
	} else if (*pos_ == CHAR_LF) {
	    *(pos_++) = CHAR_NUL;
	    // 改行を LF CR で表すことは無いと仮定
	    break;
	} else {
	    ++pos_;
	}
    }

    return true;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 現在の行を取得する。
/*!
  next()が呼ばれてから、次にnext()が呼ばれるまで有効。\n
  next()がfalseを返した後（全ての行の処理後）での値は不定。

  @return 現在の行							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
const Text TextReader::get_text() const
{
    ASSERT(current_ != NULL);

    if (*current_ == CHAR_FF)
	return Text(Text::TYPE_CTRL_NEW_COLUMN);
    
    return Text(current_);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 次の表示可能な行を取得する。
/*!
  行の取得位置は変更しない。

  @return 次の行							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
CString TextReader::pre_get() const
{
    if (pos_ == NULL)
	return CString();

    LPCTSTR start = pos_;
    while (!_istprint(*start))
	start++;

    LPCTSTR end = start;
    while (_istprint(*end))
	end++;

    return CString(start, end - start);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 行を読み飛ばす
/*!
  @param [in] num 読み飛ばす行数
  @retval true 成功
  @retval false 途中でデータの終わりに達した				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool TextReader::skip_line(size_t num)
{
    for (size_t i = 0; i < num; i++) {
	if (!next())
	    return false;
    }

    return true;
}


/* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
//! テキストデータを初期化する。
/*!
  既にテキストデータが設定済みの場合、古いテキストデータは破棄する。

  @param [in] buff テキストデータが格納されているバッファ
  @param [in] pos テキストデータの先頭位置 (NULLなら buffと同じ)	*/
/* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
void TextReader::init_buff(LPTSTR buff, LPTSTR pos /* = NULL*/)
{
    ASSERT(buff != NULL);

    if (buff_ != buff)
	delete[] buff_;
    buff_ = buff;

    pos_ = (pos == NULL ? buff_ : pos);
    current_ = NULL;
}

