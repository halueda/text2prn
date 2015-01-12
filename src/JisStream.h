/*! @file
  @brief ISO-2022-JP の文字列
  @author 依積晶紀
*/
#pragma once

/************************************************************************/
//! ISO-2022-JP の文字列
/*!
  \par 使用例：

  <pre>
  JisStream str(\"〜\");

  while (str.next()) {
    unsigned char c = str.get_char();    // 現在の文字
    if (str.get_category() == JisStream::KANJI) {
        unsigned short kanji = c*256 + str.get_trail_char();
	// 等
    }
  }
  </pre>
*/
/************************************************************************/
class JisStream
{
  public:
    //! 文字の種類
    enum CharCategory
    {
	ASCII,		//!< ASCII 文字
	KANJI,		//!< 漢字等の２バイト文字
	KANA,		//!< 半角カナ
    };

  private:
    /* ---------------------------------------------------------------- */
    //! 現在の文字の種類
    CharCategory category_;

    /* ---------------------------------------------------------------- */
    //! 対象となる ISO-2022-JP で記述された文字列
    /*!
      現在位置の次の文字を指す
    */
    const unsigned char* str_;

  public:
    JisStream(const unsigned char*);

    // 次の文字へ移動
    bool next();

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 現在の文字を取得する。
    /*!
      次にnext()を呼び出すまで有効。\n
      2byte文字の場合は、その1byte目を返す。
      
      next()を呼ぶ前、および next()がfalseを返した後の値は不定。
      @return 現在の文字						*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    unsigned char get_char() const
    {
	return *(str_ - 1);
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 2byte文字の2byte目を取得する。
    /*!
      次にnext()を呼び出すまで有効。

      現在の文字が2byte文字ではない場合、
      next()を呼ぶ前、および next()がfalseを返した後の値は不定。
      @return 現在の文字の2byte目					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    unsigned char get_trail_char() const
    {
	ASSERT(category_ == KANJI);
	return *str_;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 現在の文字の種類を取得する。
    /*!
      @return 現在の文字の種類						*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    CharCategory get_category() const
    {
	return category_;
    }
};

