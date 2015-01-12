/*! @file
  @brief テキスト入力
  @author 依積晶紀
*/
#pragma once

/************************************************************************/
//! テキスト入力ストリーム
/************************************************************************/
struct Text
{
    //! テキストの種類
    enum Type
    {
	TYPE_TEXT,		//!< 通常のテキスト
	TYPE_CTRL_NEW_COLUMN,	//!< 改段落
	TYPE_CTRL_NEW_PAGE,	//!< 改ページ
    };

    /* ---------------------------------------------------------------- */
    //! テキストの種類
    Type type_;

    /* ---------------------------------------------------------------- */
    //! テキストの本文
    /*!
      type_ が TYPE_TEXT 以外の時は NULL
    */
    LPCTSTR str_;

    /* ================================================================ */
    //! 通常のテキストとして構築する。
    /*!
      @param [in] text テキスト本文					*/
    /* ================================================================ */
    Text(LPCTSTR text) : type_(TYPE_TEXT), str_(text)
    {
	ASSERT(text != NULL);
    }

    /* ================================================================ */
    //! 通常のテキスト以外として構築する。
    /*!
      @param [in] type テキストの種類					*/
    /* ================================================================ */
    Text(Type type) : type_(type), str_(NULL)
    {
	ASSERT(type != TYPE_TEXT);
    }
};

/************************************************************************/
//! 行単位でのテキスト入力ストリーム
/*!
  \par 使用例：

  <pre>
  TextReader in;  (実際は、何らかの派生クラス)

  while (in.next()) {
    const Text text = in.get_text();
    if (text.type_ == Text::TYPE_TEXT) {
         (text.str_ に対する処理)
    }
  }
  </pre>
*/
/************************************************************************/
class TextReader
{
  private:
    /* ---------------------------------------------------------------- */
    //! 全テキストデータ
    /*!
      NUL でターミネートされている。\n
      next()を呼び出す度に改行記号が NUL に置き換えられる。

      派生クラスで、領域の確保およびデータの格納を行う。
    */
    LPTSTR buff_;

    /* ---------------------------------------------------------------- */
    //! 現在の行の内容
    /*!
      next()を呼び出す度に更新される。
      next()を呼び出す前は NULL。
    */
    LPCTSTR current_;

    /* ---------------------------------------------------------------- */
    //! 次の行の開始位置
    /*!
      次の行が存在しない（テキストデータの終わりに達した）場合は、
      *pos_ == CHAR_NUL となる。
    */
    LPTSTR pos_;
    
  protected:
    TextReader();

  public:
    virtual ~TextReader();

  public:
    // 次の行へ移動
    virtual bool next();

    // 現在の行を取得
    virtual const Text get_text() const;

    //! データの名称を取得する。
    virtual CString get_name() const = 0;
    CString pre_get() const;
    bool skip_line(size_t);

  protected:
    // テキストデータの初期化
    void init_buff(LPTSTR, LPTSTR = NULL);

  private:
    // コピー禁止
    TextReader(const TextReader&);
    const TextReader& operator=(const TextReader&);
    
};
