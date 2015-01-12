/*! @file
  @brief 印刷設定
  @author 依積晶紀
*/
#pragma once

//////////////////////////////////////////////////////////////////////

/************************************************************************/
//! ユーザが指定する印刷設定
/************************************************************************/
class Property
{
  public:
    typedef bool (Property::*Loader)(CStdioFile&);

  public:
    int char_p_line;		//!< 1行の文字数(半角換算)
    int line_p_column;		//!< 1段の行数
    int font_size;		//!< フォントサイズ(ポイント)
    int column;			//!< 段組数
    int tab_char;		//!< タブ幅
    bool wrap_right;		//!< 右端で折り返す

    int font_base_type;		//!< フォントサイズ計算の基準

    //! フォントサイズ計算の基準
    enum {
	FONT_BASE_CHAR_P_LINE,	//!< 1行の文字数を基準にフォントサイズ計算
	FONT_BASE_LINE_P_COL,	//!< 1段の行数を基準にフォントサイズ計算
	FONT_BASE_FONT_SIZE,	//!< フォントサイズから文字数他を計算
    };

    int begin_region;		//!< 印刷開始行 (0なら最初から)

    //!印刷開始位置の指定方法
    enum {
	BEGIN_REGION_FIRST,	//!< 最初から
	BEGIN_REGION_LINE_POS,	//!< n行目から
    };

    //! 印刷開始位置の指定方法
    int begin_region_type() const
    {
	return begin_region <= 0 ? BEGIN_REGION_FIRST : BEGIN_REGION_LINE_POS;
    }
    
    int end_region;		//!< 印刷終了位置
    int end_region_type;	//!< 印刷終了位置の指定方法

    //! 印刷終了位置の指定方法
    enum {
	END_REGION_LAST,	//!< 終わりまで
	END_REGION_LINE_COUNT,	//!< n行
	END_REGION_LINE_POS,	//!< n行目まで
	END_REGION_PAGE,	//!< nページ
    };

    bool view_info;		// trueなら印刷前に情報を表示する
    int output_line_count;	//!< 行番号を出力する間隔（0なら非表示）
    int line_count_digit;	//!< 行番号の桁数
    bool output_column_line;	//!< trueなら段組の間に区切り線を出力
    bool output_page_count;	//!< trueならページ数を出力する
    bool output_title;		//!< trueならファイル名を出力する

    CString font_type;		//!< フォントの言語
    CString font_name;		//!< フォント名
    LOGFONT* lf;		//!< フォント情報

    bool vertical;		//!< 縦書きならtrue
    bool proportional_font;	//!< プロポーショナルフォントを使うならtrue

    bool use_space;		//!< 余白を使用するか
    int space_top;		//!< 余白(上) (単位：mm)
    int space_bottom;		//!< 余白(下) (単位：mm)
    int space_left;		//!< 余白(左) (単位：mm)
    int space_right;		//!< 余白(右) (単位：mm)
    static const int MAX_SPACE;	//!< 余白の最大値
    int line_space;		//!< 行間 (単位：1/4行)
    int column_space;		//!< 段落間 (単位：mm)

    int separate_line_num;	//!< 複数テキストの区切りに使用する空行の数
    int separate_text_type;	//!< 複数テキストの区切り方法

    //! 複数テキストの区切り方法
    enum {
	SEPARATE_EMPTY_LINE,	//!< 空行で区切る
	SEPARATE_COLUMN,	//!< 改段落
	SEPARATE_PAGE,		//!< 改ページ
    };

    bool use_detail_header;	//!< 詳細ヘッダ/フッタを使用する。
    CString header_l;		//!< ヘッダ (左寄せ)
    CString header_c;		//!< ヘッダ (中央寄せ)
    CString header_r;		//!< ヘッダ (右寄せ)
    CString footer_l;		//!< フッタ (左寄せ)
    CString footer_c;		//!< フッタ (中央寄せ)
    CString footer_r;		//!< フッタ (右寄せ)

  private:
    static const int output_line_index[];
    
  public:
    Property();

    // 設定値の妥当性チェック
    bool check_value(CString&) const;

    // 行番号出力間隔（output_line_index[]の添え字）のアクセッサ
    int get_output_line_index() const;
    void set_output_line_index(int);

  private:
    // 設定ファイルのロード
    bool load_old(CStdioFile&);
    bool load_0(CStdioFile&);
    bool load_1(CStdioFile&);
    bool load_2(CStdioFile&);
    bool load_3(CStdioFile&);
    bool load_4(CStdioFile&);
    bool load_5(CStdioFile&);

    bool load_text_0(CStdioFile&);
    bool load_text_1(CStdioFile&);
    bool load_region_0(CStdioFile&);
    bool load_etc_0(CStdioFile&);
    bool load_etc_1(CStdioFile&);
    bool load_etc_2(CStdioFile&);
    bool load_font_0(CStdioFile&);
    bool load_font_1(CStdioFile&);
    bool load_font_2(CStdioFile&);
    bool load_space_0(CStdioFile&);
    bool load_space_1(CStdioFile&);
    bool load_separator_0(CStdioFile&);
    bool load_separator_1(CStdioFile&);
    bool load_header_0(CStdioFile&);
    bool load_header_1(CStdioFile&);

    
    // 設定ファイルのセーブ
    void save(CStdioFile&) const;

    void save_text(CStdioFile&) const;
    void save_region(CStdioFile&) const;
    void save_etc(CStdioFile&) const;
    void save_font(CStdioFile&) const;
    void save_space(CStdioFile&) const;
    void save_separator(CStdioFile&) const;
    void save_header(CStdioFile&) const;
    
    // デフォルト値
    void set_default();

    //! 設定値が正しいかチェックする。
    bool check_value() const {return check_value(CString());}

    /* ---------------------------------------------------------------- */
    //! 値が指定範囲内（下限のみ）かチェックする。
    /*!
	値が範囲外なら、末尾にエラーメッセージを追加する。

	@param [in] value 値
	@param [in] min_value 最小値
	@param [in] mes 正しくなかった時のエラーメッセージ
	@param [out] err_mes エラーメッセージ出力先			*/
    /* ---------------------------------------------------------------- */
    static void check_value(int value, int min_value,
			    LPCTSTR mes, CString& err_mes)
    {
	return check_value(value, min_value, INT_MAX, mes, err_mes);
    }

    /* ---------------------------------------------------------------- */
    //! 値が指定範囲内かチェックする。
    /*!
	値が範囲外なら、末尾にエラーメッセージを追加する。

	@param max_value 最大値
	@param [in] value 値
	@param [in] min_value 最小値
	@param [in] mes 正しくなかった時のエラーメッセージ
	@param [out] err_mes エラーメッセージ出力先			*/
    /* ---------------------------------------------------------------- */
    static void check_value(int value, int min_value, int max_value,
			    LPCTSTR mes, CString& err_mes)
    {
	if (value < min_value || value > max_value)
	    err_mes += mes;
    }
    
    friend class PreSetPropertyList;
};


//////////////////////////////////////////////////////////////////////

/************************************************************************/
//! 登録済み印刷設定の一覧
/************************************************************************/
class PreSetPropertyList
{
  public:
    /* ---------------------------------------------------------------- */
    //! 印刷設定の一覧を保持するデータの型
    /*!
      {設定名称,印刷設定}のペアの配列
    */
    typedef std::vector<std::pair<CString, Property> > PropertyList;

  private:
    Property current_;		//!< 現在の印刷設定
    PropertyList list_;		//!< 登録済み設定一覧

    mutable CString err_mes_;	//!< 設定値に異常があった場合のエラーメッセージ

  public:
    PreSetPropertyList();
    ~PreSetPropertyList();
    
  public:
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 現在の設定を取得
    /*! @return 印刷設定					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    const Property& get() const
    {
	return current_;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 更新の為に現在の設定を取得
    /*! @return 印刷設定					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    Property& get_for_update()
    {
	return current_;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 現在の設定値が正しいかチェックする。
    /*!
      問題があった場合は、get_err_mes()で詳細が取得できる。

      @retval true 現在の設定値に問題なし
      @retval false 設定値に問題あり				*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    bool check_value() const
    {
	return current_.check_value(err_mes_);
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! エラーメッセージを取得する。
    /*!
      直近の check_value()呼び出しでのエラーメッセージ。	*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    const CString& get_err_mes() const
    {
	return err_mes_;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 登録済み設定の個数を取得する。
    /*! @return 登録済み設定の個数				*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    size_t get_preset_size() const
    {
	return list_.size();
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 登録済み設定を取得する。
    /*!
      @param [in] index インデックス
      @return 印刷設定
      @pre indexは、0以上、登録済み設定の個数未満であること	*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    const Property& get(size_t index) const
    {
	ASSERT(index >= 0 && index < list_.size());
	return list_[index].second;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 更新の為の登録済み設定を取得する。
    /*!
      @param [in] index インデックス
      @return 印刷設定
      @pre indexは、0以上、登録済み設定の個数未満であること	*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    Property& get_for_update(size_t index)
    {
	ASSERT(index >= 0 && index < list_.size());
	return list_[index].second;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 登録済み設定の登録名称を取得する。
    /*!
      @param [in] index インデックス
      @return 登録名称
      @pre indexは、0以上、登録済み設定の個数未満であること	*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    const CString& get_preset_name(size_t index) const
    {
	ASSERT(index >= 0 && index < list_.size());
	return list_[index].first;
    }
    
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 登録済み設定を現在の設定に適用する。
    /*!
      @param [in] index 登録済み設定のインデックス
      @pre indexは、0以上、登録済み設定の個数未満であること	*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void set_property_from_preset(size_t index)
    {
	ASSERT(index >= 0 && index < list_.size());

	current_ = list_[index].second;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 登録済み設定を追加する
    /*!
      登録名称等の重複はチェックしない。
      
      @param name 登録名称
      @param property 設定					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void add_preset(const CString& name, const Property& property)
    {
	list_.push_back(PropertyList::value_type(name, property));
    }

    void overwrite_preset(const CString&, const Property&);

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 登録名の重複チェック
    /*!
      @param [in] name 登録名
      @retval true 重複あり
      @retval false 重複なし					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    bool is_exist_preset_name(const CString& name) const
    {
	return find_preset(name) != NULL;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 登録済み設定を削除する
    /*!
      現在の設定値を、削除した登録済み設定と同じ内容になるようにする。
      
      @param [in] index インデックス
      @pre indexは、0以上、登録済み設定の個数未満であること	*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void del_preset(size_t index)
    {
	ASSERT(index >= 0 && index < list_.size());

	set_property_from_preset(index);

	list_.erase(list_.begin() + index);
    }

    //!< 全設定取得
    const PropertyList& get_preset() const
    {
	return list_;
    }
    
  private:
    void load();
    void save() const;

    void load_old();
    void load_x(CStdioFile&, Property::Loader);

    void set_default();

    const Property* find_preset(const CString&) const;
    
    static CString get_fname();
    static CString get_old_fname();
};

//////////////////////////////////////////////////////////////////////
