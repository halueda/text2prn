/*! @file
  @brief デストラクタで解放する配列
  @author 依積晶紀
*/
#pragma

/************************************************************************/
//! デストラクタで解放する配列
/************************************************************************/
class AutoBuff
{
  private:
    //! データ
    void* buff_;

  public:
    /* ================================================================ */
    /*! データサイズを指定して構築する
	@param [in] size データサイズ（byte数）
	@exception std::bad_alloc 領域確保に失敗			*/
    /* ================================================================ */
    AutoBuff(size_t size) : buff_(new char[size])
    {
    }

    ~AutoBuff()
    {
	delete[] buff_;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 領域を開放する。
    /*!
	返される領域は、呼び出し元が責任を持って delete[] する
	@return 今まで管理していた領域					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void* release()
    {
	void* temp = buff_;
	buff_ = NULL;
	return temp;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 領域のポインタを取得する。
    /*!
      キャストして使う。
      @return 配列の先頭アドレス					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void* get()
    {
	return buff_;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 領域のポインタを取得する。
    /*!
      キャストして使う。
      @return 配列の先頭アドレス					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    const void* get() const
    {
	return buff_;
    }
    
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 領域を入れ替える。
    /*! @param [in] other 入れ替えを行う相手				*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void swap(AutoBuff& other)
    {
	std::swap(buff_, other.buff_);
    }
};

