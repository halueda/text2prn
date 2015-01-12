/*! @file
  @brief 設定から算出した印刷情報
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"Attribute.h"
#include	"Property.h"
#include	"Throwable.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 印刷に必要な情報を算出する。
/*!
  @param [in] pDC 出力先のDC
  @param [in] property 設定
  @param [in] _title タイトル
  @param [in] path ファイルパス
  @exception Warning 設定値が正しくない
  @attention 印刷処理を行う前に必ずこのメソッドを呼び出しておくこと	*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Attribute::init(const CDC* pDC, const Property& property,
		     LPCTSTR _title, LPCTSTR path)
{
    title = _title;
    init_header(property, path);

    // ===== プリンタの情報を取得 ===== 
    printable_width  = paper_width  = pDC->GetDeviceCaps(HORZRES);
    printable_height = paper_height = pDC->GetDeviceCaps(VERTRES);

    dpi_x = pDC->GetDeviceCaps(LOGPIXELSX);
    dpi_y = pDC->GetDeviceCaps(LOGPIXELSY);

    if (printable_width <= 0 || printable_height <= 0 ||
	dpi_x <= 0 || dpi_y <= 0) {
	
	throw Warning(_T("プリンタの設定が正しくありません。"));
    }
    
    if (property.vertical) {
	init_v(property);
    } else {
	init_h(property);
    }
}

/* -------------------------------------------------------------------- */
//! ヘッダ/フッタを設定する。
/*!
  @param [in] property 設定
  @param [in] path ファイルパス
  @param [in] title タイトル
  @exception Warning 設定値が正しくない					*/
/* -------------------------------------------------------------------- */
void Attribute::init_header(const Property& property, LPCTSTR path)
{
    if (property.use_detail_header) {
	parse_term(property.header_l, lheader, path, title);
	parse_term(property.header_c, cheader, path, title);
	parse_term(property.header_r, rheader, path, title);
	parse_term(property.footer_l, lfooter, path, title);
	parse_term(property.footer_c, cfooter, path, title);
	parse_term(property.footer_r, rfooter, path, title);
    } else {
	if (property.output_title)
	    parse_term(_T("【 &w 】"), cheader, path, title);
	if (property.output_page_count)
	    parse_term(_T("- &p -"), cfooter, path, title);
    }

    output_header = !(lheader.empty() && cheader.empty() && rheader.empty());
    output_footer = !(lfooter.empty() && cfooter.empty() && rfooter.empty());
}

/* -------------------------------------------------------------------- */
//! ヘッダ/フッタを解析する。
/*!
  @param [in] str ヘッダ/フッタの指定文字列
  @param [in] terms 解析結果の格納先
  @param [in] path ファイルパス						*/
/* -------------------------------------------------------------------- */
void Attribute::parse_term(LPCTSTR str, TermList& terms, LPCTSTR path, LPCTSTR title)
{
    LPCTSTR start = str;

    while (*str != _T('\0')) {

	if (*str == _T('&')) {
	    switch (*(++str)) {

	      case _T('w'):
		add_term_string(terms, start, str - 1);
		terms.add(new TermString(title));
		start = ++str;
		break;

	      case _T('f'):
		add_term_string(terms, start, str - 1);
		terms.add(new TermString(path));
		start = ++str;
		break;
		
	      case _T('d'):
		add_term_string(terms, start, str - 1);
		terms.add(new TermString(CTime::GetCurrentTime().Format(_T("%x"))));
		start = ++str;
		break;

	      case _T('t'):
		add_term_string(terms, start, str - 1);
		terms.add(new TermString(CTime::GetCurrentTime().Format(_T("%X"))));
		start = ++str;
		break;
		
	      case _T('p'):
		add_term_string(terms, start, str - 1);
		terms.add(new TermPage());
		start = ++str;
		break;

	      case _T('P'):
		add_term_string(terms, start, str - 1);
		terms.add(new TermTotalPage());
		start = ++str;
		break;
		
	      case _T('&'):
		add_term_string(terms, start, str);
		start = ++str;
		break;

	      default:
		str++;
		break;
	    }		

	} else {
	    str++;
	}
    }
    add_term_string(terms, start, str);
}

/* -------------------------------------------------------------------- */
//! ヘッダ/フッタに固定文字列を追加する。
/*!
  @param [in] terms 解析済みのヘッダ/フッタ
  @param [in] first 追加する固定文字列（開始位置）
  @param [in] last 追加する固定文字列（終了位置の次）			*/
/* -------------------------------------------------------------------- */
void Attribute::add_term_string(TermList& terms, LPCTSTR first, LPCTSTR last)
{
    ASSERT(first <= last);
    if (first < last)
	terms.add(new TermString(first, last));
}

/* -------------------------------------------------------------------- */
//! 印刷に必要な情報を計算する(横書き用)。
/*!
  @param [in] property 設定
  @exception Warning 設定値が正しくない					*/
/* -------------------------------------------------------------------- */
void Attribute::init_h(const Property& property)
{
    // ===== 余白の設定 =====
    if (property.use_space) {
	offset_x = mm2dot(property.space_left, dpi_x);
	offset_y = mm2dot(property.space_top, dpi_y);
	printable_width -= (offset_x + mm2dot(property.space_right, dpi_x));
	printable_height -= (offset_y + mm2dot(property.space_bottom, dpi_y));

	if (printable_width <= 0 || printable_height <= 0)
	    throw Warning(_T("余白が大き過ぎます。"));
	
    } else {
	offset_x = offset_y = 0;
    }
    
    // ===== 印刷に必要なデータを作成 =====
    ASSERT(property.column > 0);

    column_space = mm2dot(property.column_space, dpi_x);
    column_width = (printable_width + column_space)/property.column - column_space;
    // width = column*column_width + (column - 1)*space の解

    if (column_width <= 0)
	throw Warning(_T("段組数が多過ぎます。"));

    switch (property.font_base_type) {
      case Property::FONT_BASE_CHAR_P_LINE:
	calc_from_char_h(property);
	break;
      case Property::FONT_BASE_LINE_P_COL:
	calc_from_line_h(property);
	break;
      case Property::FONT_BASE_FONT_SIZE:
	calc_from_font_h(property);
	break;
      default:
	NODEFAULT;
    }
    if (output_header) {
	offset_y += (font_y + line_space);
	printable_height -= (font_y + line_space);
    }
    if (output_footer) {
	printable_height -= (font_y + line_space);
    }

    // ===== 行番号による補正 =====
    if (property.output_line_count) {
	if (printable_char - (property.line_count_digit + 1) <= 0)
	    throw Warning(_T("行番号の桁数が多過ぎます。"));
	// 行番号の後ろに区切り記号「:」を入れるので、
	// 行番号の桁数に+1する。

	line_format.Format(_T("%%%dd:"), property.line_count_digit);
    }
    
    // ===== 終了行 =====
    switch (property.end_region_type) {
      case Property::END_REGION_LINE_POS:	// n行目まで
	end_line = property.end_region;
	break;
      case Property::END_REGION_LINE_COUNT:	// n行
	end_line = property.begin_region + property.end_region;
	break;
      case Property::END_REGION_LAST:		// 終わりまで
      case Property::END_REGION_PAGE:		// nページ
	end_line = 0;
	break;
      default:
	NODEFAULT;
    }

    // ===== 印刷用フォントを作成 =====
    if (property.lf == NULL)
	throw Warning(_T("印刷用フォントが正しく指定されていません。"));

    property.lf->lfHeight = -font_y;
    property.lf->lfWidth  =  0;
//    property.lf->lfWidth  =  font_x;
    // 横サイズは自動計算させる。
    property.lf->lfEscapement = 0;

    font_size = static_cast<double>(font_y * SCREEN_DPI) / dpi_y;
}

/* -------------------------------------------------------------------- */
//! 1行の文字数から他の値を計算する(横書き用)
/*!
  @param [in] property 設定
  @exception Warning 値に問題あり					*/
/* -------------------------------------------------------------------- */
void Attribute::calc_from_char_h(const Property& property)
{
    printable_char = property.char_p_line;
    
    font_x = column_width/(printable_char + 1);
    // 端数の繰り上げが蓄積するとはみ出すので四捨五入はしない。
    // 全角文字の後ろ半分がはみ出す事もあるので、
    // 1半角文字分余分に確保しておく

    font_y = (font_x*dpi_y/dpi_x)*2;
    // 文字の縦横比は2:1なので横の2倍
    
    line_space = (font_x*dpi_y/dpi_x)*property.line_space/2;
    // 演算誤差を蓄積させないためにfont_y*property.line_space/4とはしない。

    if (font_y <= 0)
	throw Warning(_T("文字数もしくは段組数が多過ぎます。"));

    ASSERT(line_space + font_y > 0);

    printable_line = (printable_height + line_space) / (font_y + line_space);
    // 最後の行は行間のスペースが要らないので、
    // その分、印刷可能域に加えて計算する。

    if (output_footer) {
	printable_line--;
	if (printable_line < 1)
	    throw Warning(_T("1行も表示できません。"));
    }

    if (output_header) {
	printable_line--;
	if (printable_line < 1)
	    throw Warning(_T("1行も表示できません。"));
    }
    
    // 1行も表示できない(はみ出した)場合は、はみ出した状態で続行
    // (ページ番号/ファイル名を表示する時はエラー)
    if (printable_line <= 0)
	printable_line = 1;
}

/* -------------------------------------------------------------------- */
//! 1段の行数から他の値を計算する(横書き用)
/*!
  @param [in] property 設定
  @exception Warning 値に問題あり					*/
/* -------------------------------------------------------------------- */
void Attribute::calc_from_line_h(const Property& property)
{
    printable_line = property.line_p_column;

    const int real_printable_line = printable_line +
	    (output_footer ? 1 : 0) +
	    (output_header ? 1 : 0);

    font_y = printable_height*4/((4 + property.line_space)*real_printable_line - property.line_space);
    // 端数の繰り上げが蓄積するとはみ出すので四捨五入はしない。
    // height = line*font_y + (line - 1)*(font_y*space/4) の解

    font_x = font_y*dpi_x/(dpi_y*2);
    // 文字の縦横比は2:1なので縦の半分

    line_space = font_y*property.line_space/4;
    // 行間は文字高の1/4が基準。

    if (font_y <= 0)
	throw Warning(_T("行数もしくは段組数が多過ぎます。"));

    printable_char = column_width*dpi_y*2/(font_y*dpi_x) - 1;
    // 演算誤差を蓄積させないためにfont_xで割らない。
    // 全角文字の後ろ半分がはみ出す事もあるので、
    // その分を減らしておく

    // 1文字も表示できない時はエラー
    if (printable_char <= 0)
	throw Warning(_T("行数もしくは段組数が多過ぎます。"));
}

/* -------------------------------------------------------------------- */
//! フォントサイズから他の値を計算する(横書き用)
/*!
  @param [in] property 設定
  @exception Warning 値に問題あり					*/
/* -------------------------------------------------------------------- */
void Attribute::calc_from_font_h(const Property& property)
{
    font_y = property.font_size * dpi_y/SCREEN_DPI;
    font_x = property.font_size * dpi_x/(SCREEN_DPI*2);

    printable_char = column_width*SCREEN_DPI*2/(property.font_size * dpi_x) - 1;
    // 演算誤差を蓄積させないためにfont_xで割らない。

    if (printable_char <= 0)
	throw Warning(_T("フォントが大き過ぎ、もしくは段組数が多過ぎます。"));

    line_space = property.line_space*property.font_size * dpi_y/(SCREEN_DPI*4);
    // 演算誤差を蓄積させないためにfont_yを使わない

    printable_line = (printable_height*4 + property.line_space*font_y)/((4 + property.line_space)*font_y);
    // height = line*font_y + (line - 1)*(font_y*space/4) の解

    if (output_footer) {
	printable_line--;
	if (printable_line < 1)
	    throw Warning(_T("1行も表示できません。"));
    }

    if (output_header) {
	printable_line--;
	if (printable_line < 1)
	    throw Warning(_T("1行も表示できません。"));
    }
    
    // 1行も表示できない(はみ出した)場合は、はみ出した状態で続行
    // (ページ番号/ファイル名を表示する時はエラー)
    if (printable_line <= 0)
	printable_line = 1;
}

/* -------------------------------------------------------------------- */
//! 印刷に必要な情報を計算する(縦書き用)。
/*!
  @param [in] property 設定
  @exception Warning 設定値が正しくない					*/
/* -------------------------------------------------------------------- */
void Attribute::init_v(const Property& property)
{
    // ===== 余白の設定 =====
    if (property.use_space) {
	offset_x = mm2dot(property.space_right, dpi_x);
	offset_y = mm2dot(property.space_top, dpi_y);
	printable_width -= (offset_x + mm2dot(property.space_left, dpi_x));
	printable_height -= (offset_y + mm2dot(property.space_bottom, dpi_y));

	if (printable_width <= 0 || printable_height <= 0)
	    throw Warning(_T("余白が大き過ぎます。"));
	
    } else {
	offset_x = offset_y = 0;
    }
    offset_x = paper_width - offset_x;
    
    // ===== 印刷に必要なデータを作成 =====
    ASSERT(property.column > 0);

    column_space = mm2dot(property.column_space, dpi_y);

    switch (property.font_base_type) {
      case Property::FONT_BASE_CHAR_P_LINE:
	calc_from_char_v(property);
	break;
      case Property::FONT_BASE_LINE_P_COL:
	calc_from_line_v(property);
	break;
      case Property::FONT_BASE_FONT_SIZE:
	calc_from_font_v(property);
	break;
      default:
	NODEFAULT;
    }

    // ===== 行番号による補正 =====
    if (property.output_line_count) {
	if (printable_char - (property.line_count_digit + 1) <= 0)
	    throw Warning(_T("行番号の桁数が多過ぎます。"));
	// 行番号の後ろに区切り記号「:」を入れるので、
	// 行番号の桁数に+1する。

	line_format.Format(_T("%%%dd:"), property.line_count_digit);
    }
    
    // ===== 終了行 =====
    switch (property.end_region_type) {
      case Property::END_REGION_LINE_POS:	// n行目まで
	end_line = property.end_region;
	break;
      case Property::END_REGION_LINE_COUNT:	// n行
	end_line = property.begin_region + property.end_region;
	break;
      case Property::END_REGION_LAST:		// 終わりまで
      case Property::END_REGION_PAGE:		// nページ
	end_line = 0;
	break;
      default:
	NODEFAULT;
    }

    // ===== 印刷用フォントを作成 =====
    if (property.lf == NULL)
	throw Warning(_T("印刷用フォントが正しく指定されていません。"));

    property.lf->lfHeight = -font_y;
    property.lf->lfWidth  =  0;
//    property.lf->lfWidth  =  font_x;
    // 横サイズは自動計算させる。
    property.lf->lfEscapement = 2700;

    font_size = static_cast<double>(font_y * SCREEN_DPI) / dpi_x;
}

/* -------------------------------------------------------------------- */
//! 1行の文字数から他の値を計算する(縦書き用)
/*!
  @param [in] property 設定
  @exception Warning 値に問題あり					*/
/* -------------------------------------------------------------------- */
void Attribute::calc_from_char_v(const Property& property)
{
    printable_char = property.char_p_line;

    const int N = (output_footer ? 1 : 0) +
	    (output_header ? 1 : 0);

    font_x = (printable_height - (property.column - 1)*column_space)/(4*N + (printable_char + 1)*property.column);
    // N*(2x+2x)+x*(char+1)*column + (column - 1)*space = heightの解
    // ↑         ↑                  ↑
    // ↑        本文                段組の隙間
    // タイトル、タイトルと本文の隙間、ページ、ページと本文の隙間
    // タイトル、ページと本文の隙間は常に1行分

    calc_column_height_v(property);
    
    // 再計算
    font_x = column_width/(printable_char + 1);
    // 端数の繰り上げが蓄積するとはみ出すので四捨五入はしない。
    // 全角文字の後ろ半分がはみ出す事もあるので、
    // 1半角文字分余分に確保しておく

    font_y = (font_x*dpi_x/dpi_y)*2;
    // 文字の縦横比は2:1なので横の2倍
    
    line_space = (font_x*dpi_x/dpi_y)*property.line_space/2;
    // 演算誤差を蓄積させないためにfont_y*property.line_space/4とはしない。

    if (font_y <= 0)
	throw Warning(_T("文字数もしくは段組数が多過ぎます。"));

    ASSERT(line_space + font_y > 0);

    printable_line = (printable_width + line_space) / (font_y + line_space);
    // 最後の行は行間のスペースが要らないので、
    
    // 1行も表示できない(はみ出した)場合は、はみ出した状態で続行
    if (printable_line <= 0)
	printable_line = 1;
}

/* -------------------------------------------------------------------- */
//! 1段の行数から他の値を計算する(縦書き用)
/*!
  @param [in] property 設定
  @exception Warning 値に問題あり					*/
/* -------------------------------------------------------------------- */
void Attribute::calc_from_line_v(const Property& property)
{
    printable_line = property.line_p_column;

    font_y = printable_width*4/((4 + property.line_space)*printable_line - property.line_space);
    // 端数の繰り上げが蓄積するとはみ出すので四捨五入はしない。
    // width = line*font_y + (line - 1)*(font_y*space/4) の解

    if (font_y <= 0)
	throw Warning(_T("行数もしくは段組数が多過ぎます。"));
    
    font_x = font_y*dpi_y/(dpi_x*2);
    // 文字の縦横比は2:1なので縦の半分

    calc_column_height_v(property);
    
    line_space = font_y*property.line_space/4;
    // 行間は文字高の1/4が基準。

    printable_char = column_width*dpi_x*2/(font_y*dpi_y) - 1;
    // 演算誤差を蓄積させないためにfont_xで割らない。
    // 全角文字の後ろ半分がはみ出す事もあるので、
    // その分を減らしておく

    // 1文字も表示できない時はエラー
    if (printable_char <= 0)
	throw Warning(_T("行数もしくは段組数が多過ぎます。"));
}

/* -------------------------------------------------------------------- */
//! フォントサイズから他の値を計算する(縦書き用)
/*!
  @param [in] property 設定
  @exception Warning 値に問題あり					*/
/* -------------------------------------------------------------------- */
void Attribute::calc_from_font_v(const Property& property)
{
    font_y = property.font_size * dpi_x/SCREEN_DPI;
    font_x = property.font_size * dpi_y/(SCREEN_DPI*2);

    calc_column_height_v(property);
   
    printable_char = column_width*SCREEN_DPI*2/(property.font_size * dpi_y) - 1;
    // 演算誤差を蓄積させないためにfont_xで割らない。

    if (printable_char <= 0)
	throw Warning(_T("フォントが大き過ぎ、もしくは段組数が多過ぎます。"));

    line_space = property.line_space*property.font_size * dpi_x/(SCREEN_DPI*4);
    // 演算誤差を蓄積させないためにfont_yを使わない

    printable_line = (printable_width*4 + property.line_space*font_y)/((4 + property.line_space)*font_y);
    // width = line*font_y + (line - 1)*(font_y*space/4) の解

    // 1行も表示できない(はみ出した)場合は、はみ出した状態で続行
    // (ページ番号/ファイル名を表示する時はエラー)
    if (printable_line <= 0)
	printable_line = 1;
}

/* -------------------------------------------------------------------- */
//! 段組の高さを計算する(縦書き用)
/*!
  @param [in] property 設定
  @exception Warning 値に問題あり					*/
/* -------------------------------------------------------------------- */
void Attribute::calc_column_height_v(const Property& property)
{
    if (output_header) {
	offset_y += (font_x*4);
	printable_height -= (font_x*4);
    }
    if (output_footer) {
	printable_height -= (font_x*4);
    }
    column_width = (printable_height + column_space)/property.column - column_space;
    // height = column*column_height + (column - 1)*space の解

    if (column_width <= 0)
	throw Warning(_T("段組数が多過ぎます。"));
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ヘッダ/フッタにページ総数を指定しているかどうか
/*!
  @param [in] property 設定
  @retval true ページ総数 あり
  @retval false ページ総数 なし						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool Attribute::use_total_page_count(const Property& property)
{
    if (!property.use_detail_header)
	return false;

    LPCTSTR head[] = {
	property.header_l,
	property.header_c,
	property.header_r,
	property.footer_l,
	property.footer_c,
	property.footer_r,
    };

    for (int i = 0; i < 6; i++) {
	TermList list;
	parse_term(head[i], list, _T(""), _T(""));

	if (list.use_total_page_count())
	    return true;
    }
    return false;
}
