/*! @file
  @brief 印刷設定
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"Property.h"
#include	"Util.h"

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////

const int Property::MAX_SPACE = 50;

//! 指定可能な「行番号を出力する間隔」の一覧
const int Property::output_line_index[] =
{
    0, 1, 5, 10, 20, 50, 100,
};

Property::Property() : lf(NULL)
{
}

/* -------------------------------------------------------------------- */
//! デフォルトの設定値をセットする。
/* -------------------------------------------------------------------- */
void Property::set_default()
{
    char_p_line        = 96;			// 1行の文字数(半角換算)
    line_p_column      = 100;			// 1段の行数
    font_size          = 5;			// フォントサイズ(ポイント)
    column             = 2;			// 段組数
    tab_char           = 8;			// タブ幅
    font_base_type     = FONT_BASE_CHAR_P_LINE;	// 1行の文字数で計算
    wrap_right         = true;			// 右端で折り返す

    begin_region       = 0;			// 初めから印刷
    end_region         = 0;			// 印刷終了位置
    end_region_type    = END_REGION_LAST;	// 最後まで印刷

    view_info          = true;			// 印刷前に情報を表示する
    output_line_count  = 0;			// 行番号を出力しない
    line_count_digit   = 4;			// 行番号の桁数
    output_column_line = false;			// 段組の区切り線をなし
    output_page_count  = false;			// ページ番号を出力しない
    output_title       = false;			// ファイル名を出力しない

    font_type          = _T("日本語");		// フォントの言語
    font_name          = _T("ＭＳ 明朝");	// フォント名
    vertical           = false;			// 横書き
    proportional_font  = false;			// 等幅
    // lf は、後で設定する。

    use_space          = false;			// 余白を使うか
    space_top          = 20;			// 余白 上
    space_bottom       = 20;			// 余白 下
    space_left         = 20;			// 余白 左
    space_right        = 20;			// 余白 右
    line_space         = 1;			// 行間
    column_space       = 8;			// 段落間

    separate_line_num  = 0;			// 複数テキストの区切り行数
    separate_text_type = SEPARATE_COLUMN;	// 複数テキストの区切り方法

    use_detail_header  = false;			// 詳細ヘッダ/フッタを使用する
    header_l           = _T("");		// ヘッダ (左寄せ)
    header_c           = _T("");		// ヘッダ (中央寄せ)
    header_r           = _T("");		// ヘッダ (右寄せ)
    footer_l           = _T("");		// フッタ (左寄せ)
    footer_c           = _T("");		// フッタ (中央寄せ)
    footer_r           = _T("");		// フッタ (右寄せ)
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 行番号の出力間隔が、指定可能な出力間隔一覧での何番目のものなのか取得する。
/*!
  指定の値が、一覧に存在しない場合は、-1を返す。
  
  @return 順番								*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
int Property::get_output_line_index() const
{
    const int size = sizeof(output_line_index)/sizeof(output_line_index[0]);
    for (int i = 0; i < size; i++)
	if (output_line_count == output_line_index[i])
	    return i;
    return -1;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 行番号の出力間隔を設定する
/*!
  出力間隔そのものではなく、指定可能な一覧での何番目か（先頭が０）で指定する。
  
  @param [in] index 行番号の出力間隔（一覧での順番）
  @pre 指定の順番は、一覧の範囲内であること				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Property::set_output_line_index(int index)
{
    ASSERT(index >= 0 && index < sizeof(output_line_index)/sizeof(output_line_index[0]));

    output_line_count = output_line_index[index];
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定値が正しいかチェックする。
/*!
  設定値に異常がある場合は、
  err_mes の改行区切りでエラーメッセージを設定する。

  列挙型の設定値での値異常は区別してエラーメッセージを作成する。
  
  @param err_mes [out] 正しくなかった時のエラーメッセージ
  @retval true 設定値に問題なし。
  @retval false 設定値に何らかの問題あり。
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool Property::check_value(CString& err_mes) const
{
    err_mes.Empty();

    CString illegal;	// 重大なエラー

    check_value(char_p_line,   1, 999, _T("\n文字数（1〜999）"), err_mes);
    check_value(line_p_column, 1, 999, _T("\n行数（1〜999）"), err_mes);
    check_value(font_size,     1, 999, _T("\nフォントサイズ（1〜999）"), err_mes);
    check_value(column,        1,   9, _T("\n段組数（1〜9）"), err_mes);
    check_value(tab_char,      1,  99, _T("\nタブ幅（1〜99）"), err_mes);
    check_value(font_base_type, FONT_BASE_CHAR_P_LINE, FONT_BASE_FONT_SIZE, _T("\n異常：フォントサイズ計算基準"), illegal);

    check_value(begin_region, 0, _T("\n印刷開始行（1〜）"), err_mes);
    // 設定値 = 0 が１行目を表すので、メッセージは「1〜」で正しい

    check_value(end_region_type, END_REGION_LAST, END_REGION_PAGE, _T("\n異常：印刷終了条件"), illegal);

    switch (end_region_type) {
      case END_REGION_LINE_COUNT:
	check_value(end_region, 1, _T("\n印刷行数（1〜）"), err_mes);
	break;
      case END_REGION_LINE_POS:
	check_value(end_region, begin_region + 1, _T("\n印刷終了行（印刷開始行〜）"), err_mes);
	break;
      case END_REGION_PAGE:
	check_value(end_region, 1, _T("\n印刷ページ数（1〜）"), err_mes);
	break;
    }

    if (get_output_line_index() == -1)
	err_mes += _T("\n行番号");
    check_value(line_count_digit, 1, 9, _T("\n行番号桁数（1〜9）"), err_mes);

    if (font_type.IsEmpty() || font_name.IsEmpty()) {
	err_mes += _T("\nフォント名");
    }

    check_value(space_top,    0, MAX_SPACE, FString(_T("\n余白：上（0〜%d）"), MAX_SPACE), err_mes);
    check_value(space_bottom, 0, MAX_SPACE, FString(_T("\n余白：下（0〜%d）"), MAX_SPACE), err_mes);
    check_value(space_left,   0, MAX_SPACE, FString(_T("\n余白：左（0〜%d）"), MAX_SPACE), err_mes);
    check_value(space_right,  0, MAX_SPACE, FString(_T("\n余白：右（0〜%d）"), MAX_SPACE), err_mes);
    check_value(line_space,   0,         4,  _T("\n行間（0〜4）"), err_mes);
    check_value(column_space, 0, MAX_SPACE, FString(_T("\n段落間（0〜%d）"), MAX_SPACE), err_mes);


    check_value(separate_line_num, 0, _T("\n複数テキスト区切り行数（0〜）"), err_mes);
    check_value(separate_text_type, SEPARATE_EMPTY_LINE, SEPARATE_PAGE, _T("\n異常：複数テキスト区切り方法"), illegal);
    
    if (!illegal.IsEmpty()) {
	err_mes += _T('\n');
	err_mes += illegal;
    }

    if (!err_mes.IsEmpty()) {
	err_mes = _T("設定値が正しくありません。\n") + err_mes;
	return false;
    }
    
    return true;
}

/* -------------------------------------------------------------------- */
//! 設定値のロード処理を、各項目ごとのロードメソッドに振り分ける
/*!
  @param [in] IN ロード先
  @param [in] TEXT テキスト設定ロードメソッドのバージョン
  @param [in] REGION 印刷範囲設定ロードメソッドのバージョン
  @param [in] ETC その他設定ロードメソッドのバージョン
  @param [in] FONT フォント設定ロードメソッドのバージョン
  @param [in] SPACE 余白設定ロードメソッドのバージョン
  @param [in] SEPARATOR 複数テキスト区切り設定ロードメソッドのバージョン
  @param [in] HEADER 詳細ヘッダ/フッタ設定ロードメソッドのバージョン
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
#define LOAD_PROPERTY(IN, TEXT, REGION, ETC, FONT, SPACE, SEPARATOR, HEADER) \
		load_text_##TEXT(IN) && \
		load_region_##REGION(IN) && \
		load_etc_##ETC(IN) && \
		load_font_##FONT(IN) && \
		load_space_##SPACE(IN) && \
		load_separator_##SEPARATOR(IN) && \
		load_header_##HEADER(IN) && \
		check_value()

/* -------------------------------------------------------------------- */
//! 設定値をロードする
/*!
  設定ファイルのフォーマットをバージョン管理する以前の設定ファイル用。
  @param [in] in ロード先
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_old(CStdioFile& in)
{
    return LOAD_PROPERTY(in, 0, 0, 0, 0, 0, 0, 0);
}

/* -------------------------------------------------------------------- */
//! 設定値をロードする
/*!
  設定ファイルのバージョン = 0用。
  
  @param [in] in ロード先
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_0(CStdioFile& in)
{
    return LOAD_PROPERTY(in, 1, 0, 1, 0, 0, 0, 0);
}

/* -------------------------------------------------------------------- */
//! 設定値をロードする
/*!
  設定ファイルのバージョン = 1用。
  
  @param [in] in ロード先
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_1(CStdioFile& in)
{
    return LOAD_PROPERTY(in, 1, 0, 2, 0, 0, 0, 0);
}

/* -------------------------------------------------------------------- */
//! 設定値をロードする
/*!
  設定ファイルのバージョン = 2用。
  
  @param [in] in ロード先
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_2(CStdioFile& in)
{
    return LOAD_PROPERTY(in, 1, 0, 2, 1, 1, 0, 0);
}

/* -------------------------------------------------------------------- */
//! 設定値をロードする
/*!
  設定ファイルのバージョン = 3用。
  
  @param [in] in ロード先
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_3(CStdioFile& in)
{
    return LOAD_PROPERTY(in, 1, 0, 2, 2, 1, 0, 0);
}

/* -------------------------------------------------------------------- */
//! 設定値をロードする
/*!
  設定ファイルのバージョン = 4用。
  
  @param [in] in ロード先
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_4(CStdioFile& in)
{
    return LOAD_PROPERTY(in, 1, 0, 2, 2, 1, 1, 0);
}

/* -------------------------------------------------------------------- */
//! 設定値をロードする
/*!
  設定ファイルのバージョン = 5用。
  
  @param [in] in ロード先
  @retval true 正しくロードでき、設定値に問題もない
  @retval false ロードに失敗、もしくは設定値に問題あり
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_5(CStdioFile& in)
{
    return LOAD_PROPERTY(in, 1, 0, 2, 2, 1, 1, 1);
}

#undef LOAD_PROPERTY

/* -------------------------------------------------------------------- */
//! テキスト設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_text_0(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    if (_stscanf(buff, _T("%d,%d,%d,%d,%d,%d"),
		 &char_p_line,
		 &line_p_column,
		 &font_size,
		 &column,
		 &tab_char,
		 &font_base_type) != 6) {
	return false;
    }

    wrap_right = true;
    
    return true;
}

/* -------------------------------------------------------------------- */
//! テキスト設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_text_1(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    int temp1;
    if (_stscanf(buff, _T("%d,%d,%d,%d,%d,%d,%d"),
		 &char_p_line,
		 &line_p_column,
		 &font_size,
		 &column,
		 &tab_char,
		 &font_base_type,
		 &temp1) != 7) {
	return false;
    }

    wrap_right = (temp1 != 0);
    
    return true;
}

/* -------------------------------------------------------------------- */
//! 印刷範囲設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_region_0(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    if (_stscanf(buff, _T("%d,%d,%d"),
		 &begin_region,
		 &end_region,
		 &end_region_type) != 3) {
	return false;
    }

    return true;
}

/* -------------------------------------------------------------------- */
//! その他設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_etc_0(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    int temp1, temp2, temp3, temp4;
    if (_stscanf(buff, _T("%d,%d,%d,%d,%d"),
		 &temp1,
		 &temp2,
		 &line_count_digit,
		 &temp3,
		 &temp4) != 5) {
	return false;
    }

    view_info = (temp1 != 0);
    output_line_count = (temp2 != 0 ? 1 : 0);
    output_column_line = (temp3 != 0);
    output_page_count = (temp4 != 0);
    output_title = false;

    return true;
}

/* -------------------------------------------------------------------- */
//! その他設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_etc_1(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    int temp1, temp2, temp3;
    if (_stscanf(buff, _T("%d,%d,%d,%d,%d"),
		 &temp1,
		 &output_line_count,
		 &line_count_digit,
		 &temp2,
		 &temp3) != 5) {
	return false;
    }

    view_info = (temp1 != 0);
    output_column_line = (temp2 != 0);
    output_page_count = (temp3 != 0);
    output_title = false;

    return true;
}

/* -------------------------------------------------------------------- */
//! その他設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_etc_2(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    int temp1, temp2, temp3, temp4;
    if (_stscanf(buff, _T("%d,%d,%d,%d,%d,%d"),
		 &temp1,
		 &output_line_count,
		 &line_count_digit,
		 &temp2,
		 &temp3,
		 &temp4) != 6) {
	return false;
    }

    view_info = (temp1 != 0);
    output_column_line = (temp2 != 0);
    output_page_count = (temp3 != 0);
    output_title = (temp4 != 0);

    return true;
}

/* -------------------------------------------------------------------- */
//! フォント設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_font_0(CStdioFile& in)
{
    if (!in.ReadString(font_type))
	return false;

    if (!in.ReadString(font_name))
	return false;

    font_type.Trim();
    font_name.Trim();
    vertical = false;
    proportional_font = false;

    return true;
}

/* -------------------------------------------------------------------- */
//! フォント設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_font_1(CStdioFile& in)
{
    if (!in.ReadString(font_type))
	return false;

    if (!in.ReadString(font_name))
	return false;

    font_type.Trim();
    font_name.Trim();

    CString buff;
    if (!in.ReadString(buff))
	return false;
    int temp1;
    if (_stscanf(buff, _T("%d"),
		 &temp1) != 1) {
	return false;
    }

    vertical = (temp1 != 0);
    proportional_font = false;

    return true;
}

/* -------------------------------------------------------------------- */
//! フォント設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_font_2(CStdioFile& in)
{
    if (!in.ReadString(font_type))
	return false;

    if (!in.ReadString(font_name))
	return false;

    font_type.Trim();
    font_name.Trim();

    CString buff;
    if (!in.ReadString(buff))
	return false;
    int temp1, temp2;
    if (_stscanf(buff, _T("%d,%d"),
		 &temp1,
		 &temp2) != 2) {
	return false;
    }

    vertical = (temp1 != 0);
    proportional_font = (temp2 != 0);

    return true;
}

/* -------------------------------------------------------------------- */
//! 余白設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_space_0(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    int temp1;
    if (_stscanf(buff, _T("%d,%d,%d,%d,%d"),
		 &temp1,
		 &space_top,
		 &space_bottom,
		 &space_left,
		 &space_right) != 5) {
	return false;
    }

    use_space = (temp1 != 0);
    line_space = 1;
    column_space = 8;

    return true;
}

/* -------------------------------------------------------------------- */
//! 余白設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_space_1(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    int temp1;
    if (_stscanf(buff, _T("%d,%d,%d,%d,%d,%d,%d"),
		 &temp1,
		 &space_top,
		 &space_bottom,
		 &space_left,
		 &space_right,
		 &line_space,
		 &column_space) != 7) {
	return false;
    }

    use_space = (temp1 != 0);

    return true;
}

/* -------------------------------------------------------------------- */
//! 複数テキストの区切り設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue					*/
/* -------------------------------------------------------------------- */
bool Property::load_separator_0(CStdioFile& in)
{
    separate_line_num = 0;
    separate_text_type = SEPARATE_COLUMN;

    return true;
}

/* -------------------------------------------------------------------- */
//! 複数テキストの区切り設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_separator_1(CStdioFile& in)
{
    CString buff;

    if (!in.ReadString(buff))
	return false;

    if (_stscanf(buff, _T("%d,%d"),
		 &separate_line_num,
		 &separate_text_type) != 2) {
	return false;
    }

    return true;
}

/* -------------------------------------------------------------------- */
//! 詳細ヘッダ/フッタ設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_header_0(CStdioFile& in)
{
    use_detail_header  = false;			// 詳細ヘッダ/フッタを使用する
    header_l           = _T("");		// ヘッダ (左寄せ)
    header_c           = _T("");		// ヘッダ (中央寄せ)
    header_r           = _T("");		// ヘッダ (右寄せ)
    footer_l           = _T("");		// フッタ (左寄せ)
    footer_c           = _T("");		// フッタ (中央寄せ)
    footer_r           = _T("");		// フッタ (右寄せ)

    return true;
}

/* -------------------------------------------------------------------- */
//! 詳細ヘッダ/フッタ設定をロードする
/*!
  @param [in] in ロード先
  @return 正しくロードできればtrue
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
bool Property::load_header_1(CStdioFile& in)
{
    CString buff;
    if (!in.ReadString(buff))
	return false;

    int temp1;
    if (_stscanf(buff, _T("%d"), &temp1) != 1)
	return false;

    use_detail_header = (temp1 != 0);

    if (!in.ReadString(header_l))
	return false;
    if (!in.ReadString(header_c))
	return false;
    if (!in.ReadString(header_r))
	return false;
    if (!in.ReadString(footer_l))
	return false;
    if (!in.ReadString(footer_c))
	return false;
    if (!in.ReadString(footer_r))
	return false;

    trim_if_empty(header_l);
    trim_if_empty(header_c);
    trim_if_empty(header_r);
    trim_if_empty(footer_l);
    trim_if_empty(footer_c);
    trim_if_empty(footer_r);

    return true;
}

/* -------------------------------------------------------------------- */
//! セーブする
/*!
  @param [out] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save(CStdioFile& out) const
{
    save_text(out);
    save_region(out);
    save_etc(out);
    save_font(out);
    save_space(out);
    save_separator(out);
    save_header(out);
}

/* -------------------------------------------------------------------- */
//! テキスト設定をセーブする
/*!
  @param [in] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save_text(CStdioFile& out) const
{
    FileIO::write_string(out, FString(_T("%d,%d,%d,%d,%d,%d,%d"),
				      char_p_line,
				      line_p_column,
				      font_size,
				      column,
				      tab_char,
				      font_base_type,
				      wrap_right ? 1 : 0));
}

/* -------------------------------------------------------------------- */
//! 印刷範囲設定をセーブする
/*!
  @param [in] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save_region(CStdioFile& out) const
{
    FileIO::write_string(out, FString(_T("%d,%d,%d"),
				      begin_region,
				      end_region,
				      end_region_type));
}

/* -------------------------------------------------------------------- */
//! その他設定をセーブする
/*!
  @param [in] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save_etc(CStdioFile& out) const
{
    FileIO::write_string(out, FString(_T("%d,%d,%d,%d,%d,%d"),
				      view_info ? 1 : 0,
				      output_line_count,
				      line_count_digit,
				      output_column_line,
				      output_page_count ? 1 : 0,
				      output_title ? 1 : 0));
}

/* -------------------------------------------------------------------- */
//! フォント設定をセーブする
/*!
  @param [in] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save_font(CStdioFile& out) const
{
    FileIO::write_string(out, font_type);
    FileIO::write_string(out, font_name);

    FileIO::write_string(out, FString(_T("%d,%d"),
				      vertical ? 1 : 0,
				      proportional_font ? 1 : 0));
}

/* -------------------------------------------------------------------- */
//! 余白設定をセーブする
/*!
  @param [in] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save_space(CStdioFile& out) const
{
    FileIO::write_string(out, FString(_T("%d,%d,%d,%d,%d,%d,%d"),
				      use_space ? 1 : 0,
				      space_top,
				      space_bottom,
				      space_left,
				      space_right,
				      line_space,
				      column_space));
}

/* -------------------------------------------------------------------- */
//! 複数テキストの区切り設定をセーブする
/*!
  @param [in] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save_separator(CStdioFile& out) const
{
    FileIO::write_string(out, FString(_T("%d,%d"),
				      separate_line_num,
				      separate_text_type));
}

/* -------------------------------------------------------------------- */
//! 詳細ヘッダ/フッタ設定をセーブする
/*!
  @param [in] out セーブ先
  @exception CFileException* セーブ失敗					*/
/* -------------------------------------------------------------------- */
void Property::save_header(CStdioFile& out) const
{
    FileIO::write_string(out, FString(_T("%d"),
				      use_detail_header ? 1 : 0));
    FileIO::write_string(out, header_l);
    FileIO::write_string(out, header_c);
    FileIO::write_string(out, header_r);
    FileIO::write_string(out, footer_l);
    FileIO::write_string(out, footer_c);
    FileIO::write_string(out, footer_r);
}

//////////////////////////////////////////////////////////////////////

#define PROPERTY_VERSION 5	//!< 設定ファイルのバージョン

/* ==================================================================== */
//! 設定ファイルをロードして構築する
/* ==================================================================== */
PreSetPropertyList::PreSetPropertyList()
{
    load();
}

/* ==================================================================== */
//! 設定ファイルにセーブしてから破棄する
/* ==================================================================== */
PreSetPropertyList::~PreSetPropertyList()
{
    save();
}

/* -------------------------------------------------------------------- */
//! 設定ファイルから設定値をロードする。
/*!
  ロード失敗 / 設定ファイルの内容が正しく無い時は、デフォルト値をセットする。\n
  （エラーメッセージ等は表示しない）					*/
/* -------------------------------------------------------------------- */
void PreSetPropertyList::load()
{
    list_.clear();

    CStdioFile in;

    TRY {
	if (!in.Open(get_fname(), CFile::modeRead | CFile::typeText)) {
	    load_old();
	} else {
	    switch(FileIO::read_int(in)) {
	      case 0:
		load_x(in, &Property::load_0);
		break;

	      case 1:
		load_x(in, &Property::load_1);
		break;

	      case 2:
		load_x(in, &Property::load_2);
		break;

	      case 3:
		load_x(in, &Property::load_3);
		break;

	      case 4:
		load_x(in, &Property::load_4);
		break;

	      case 5:
		load_x(in, &Property::load_5);
		break;
		
	      default:
		set_default();
		break;
	    }
	}
    } CATCH (CFileException, e) {
	set_default();
    } END_CATCH;
}

/* -------------------------------------------------------------------- */
//! 古い設定ファイルから設定値をロードする。
/*!
  古い設定ファイルには、登録済み設定の情報を持たないが、
  デフォルト値を登録済み設定として登録する（登録名称「初期値」）
  
  設定ファイルの内容が正しく無い時は、デフォルト値をセットする。

  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
void PreSetPropertyList::load_old()
{
    const CString fname = get_old_fname();

    if (!current_.load_old(CStdioFile(fname, CFile::modeRead | CFile::typeText))) {
	current_.set_default();
    }

    Property p;
    p.set_default();
    add_preset(_T("初期値"), p);

    // 古い設定ファイルはファイル名が違うので、削除する。
    CFile::Remove(fname);
}

/* -------------------------------------------------------------------- */
//! 設定ファイルから設定値をロードする。
/*!
  設定ファイルの内容が正しくない時は、デフォルト値をセットする。

  登録済み設定に関する内容が正しくない場合は、正しいものだけ登録する。

  @param [in] in 設定ファイルのロード元
  @param [in] load 設定ファイルのバージョンに対応したロードメソッド
  @exception CFileException* ロード失敗					*/
/* -------------------------------------------------------------------- */
void PreSetPropertyList::load_x(CStdioFile& in, Property::Loader load)
{
    if (!(current_.*load)(in)) {
	set_default();
	return;
    }

    // 登録済み設定のロード
    const int size = FileIO::read_int(in);
    for (int i = 0; i < size; i++) {
	CString name;
	if (!in.ReadString(name))
	    break;
	name.Trim();
	if (name.IsEmpty())
	    break;

	Property p;
	if (!(p.*load)(in))
	    break;

	add_preset(name, p);
    }
}

/* -------------------------------------------------------------------- */
//! 設定値を設定ファイルへセーブする。
/*!
  設定値が正しく無いときはセーブしない。				*/
/* -------------------------------------------------------------------- */
void PreSetPropertyList::save() const
{
    TRY {
	CStdioFile out(get_fname(), CFile::modeWrite | CFile::modeCreate | CFile::typeText);

	FileIO::write_int(out, PROPERTY_VERSION);
	current_.save(out);

	// 登録済み設定のセーブ
	
	FileIO::write_int(out, list_.size());

	PropertyList::const_iterator it;
	for (it = list_.begin(); it != list_.end(); ++it) {
	    FileIO::write_string(out, it->first);
	    it->second.save(out);
	}
	
    } CATCH (CFileException, e) {
    } END_CATCH;
}

/* -------------------------------------------------------------------- */
//! デフォルトの設定値をセットする。
/*!
  登録済み設定には、デフォルト値を登録する（登録名称「初期値」）	*/
/* -------------------------------------------------------------------- */
void PreSetPropertyList::set_default()
{
    current_.set_default();

    list_.clear();
    add_preset(_T("初期値"), current_);
}

/* -------------------------------------------------------------------- */
//! 設定ファイルのファイル名を取得。
/*!
  実行ファイルと同じディレクトリにある「TEXT2PRN.DAT」とする。
  @return ファイル名							*/
/* -------------------------------------------------------------------- */
CString PreSetPropertyList::get_fname()
{
    return FilePath::get_path_same_app_dir(_T("TEXT2PRN.DAT"));
}

/* -------------------------------------------------------------------- */
//! 昔の設定ファイルのファイル名を取得。
/*!
  実行ファイルと同じディレクトリにある「TEXT2PRN.CNF」とする。
  @return ファイル名							*/
/* -------------------------------------------------------------------- */
CString PreSetPropertyList::get_old_fname()
{
    return FilePath::get_path_same_app_dir(_T("TEXT2PRN.CNF"));
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 登録済み設定を追加する。
/*!
  登録名称が重複した場合は、上書きする。
      
  @param name 登録名称
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void PreSetPropertyList::overwrite_preset(const CString& name, const Property& property)
{
    const Property* target = find_preset(name);
    if (target == NULL) {
	add_preset(name, property);
    } else {
	*const_cast<Property*>(target) = property;
    }
}


/* -------------------------------------------------------------------- */
//! 登録名の重複チェック
/*!
  @param [in] name 登録名
  @retval true 重複あり
  @retval false 重複なし						*/
/* -------------------------------------------------------------------- */
const Property* PreSetPropertyList::find_preset(const CString& name) const
{
    struct EQ
    {
	const CString& name_;
	EQ(const CString& name) : name_(name)
	{
	}
	bool operator()(const PropertyList::value_type& value) const
	{
	    return value.first == name_;
	}
    };

    PropertyList::const_iterator it = std::find_if(list_.begin(), list_.end(), EQ(name));

    return it == list_.end() ? NULL : &(it->second);
}


//////////////////////////////////////////////////////////////////////

