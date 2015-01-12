/*! @file
  @brief 文字コード
  @author 依積晶紀
*/
#pragma once

class AutoBuff;

/************************************************************************/
//! 文字コード の基底クラス
/************************************************************************/
class Encoding
{
  public:
    virtual ~Encoding()
    {
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! UTF-16 LE に変換する。
    /*!
      変換した結果は、buff に入れて返す。
      
      引数 buff が表す文字列は NUL ターミネートされていない。\n
      （文字列の長さは、引数 size で指定する）\n
      この関数が返す文字列は、NUL ターミネートされている。

      @param [in,out] buff 変換対象の文字列
      @param [in] size 変換元の文字列のサイズ（バイト数）
      @exception Error 文字コードの変換に失敗
      @exception std::bad_alloc 変換後の文字列用の領域確保失敗		*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    virtual void convert(AutoBuff& buff, int size) const = 0;
};

/************************************************************************/
//! 文字コード UTF-16
/*!
  Big Endian と Little Endian の両方を扱う				*/
/************************************************************************/
class EncodingUTF16 : public Encoding
{
  private:
    /* ---------------------------------------------------------------- */
    //! BOMが存在しない時のエンディアン
    /*!
      true  : Little Endian \n
      false : Big Endian

      BOMが存在するなら、そちらを優先する。
    */
    bool priority_little_endian_;

  public:
    EncodingUTF16(bool);

    // UTF-16 LE に変換
    virtual void convert(AutoBuff&, int) const;

  private:
    // エンディアンの変換
    void convert_endian(wchar_t*, int) const;
};

/************************************************************************/
//! マルチバイトの文字コード
/************************************************************************/
class EncodingMultiByte : public Encoding
{
  public:
    // UTF-16 LE に変換
    virtual void convert(AutoBuff&, int) const;

  protected:
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    //! 変換元の文字列のコードページを取得する。
    /*!
      @return コードページ						*/
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    virtual int get_code_page() const = 0;
};

/************************************************************************/
//! 文字コード UTF-8
/************************************************************************/
class EncodingUTF8 : public EncodingMultiByte
{
  protected:
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    //! 変換元の文字列のコードページを取得する。
    /*!
      @return コードページ						*/
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    int get_code_page() const {return CP_UTF8;}
};

/************************************************************************/
//! 文字コード Shift-JIS
/************************************************************************/
class EncodingSJIS : public EncodingMultiByte
{
  protected:
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    //! 変換元の文字列のコードページを取得する。
    /*!
      @return コードページ						*/
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    int get_code_page() const {return CP_ACP;}
};

/************************************************************************/
//! 文字コード EUC-JP
/*!
  一旦 Shift-JIS に変換してから UTF-16 にするため、EncodingSJISから派生	*/
/************************************************************************/
class EncodingEUC : public EncodingSJIS
{
  public:
    // UTF-16 LE に変換
    virtual void convert(AutoBuff&, int) const;
};

/************************************************************************/
//! 文字コード ISO-2022-JP
/*!
  一旦 Shift-JIS に変換してから UTF-16 にするため、EncodingSJISから派生	*/
/************************************************************************/
class EncodingJIS : public EncodingSJIS
{
  public:
    // UTF-16 LE に変換
    virtual void convert(AutoBuff&, int) const;
};
