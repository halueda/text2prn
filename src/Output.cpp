/*! @file
  @brief テキストの出力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"Output.h"
#include	"Property.h"
#include	"CharType.h"
#include	"Throwable.h"

/* ==================================================================== */
/*!
  @param [in] pDC 出力先のDC
  @param [in] property 設定
  @param [in] title タイトル
  @exception Warning 設定値が正しくない					*/
/* ==================================================================== */
Output::Output(CDC* pDC, const Property& property, LPCTSTR title) :
	prop_(property), current_page_(0), current_char_(0),
	current_col_(0), current_line_(0)
{

    attr_.init(pDC, prop_, ::PathFindFileName(title), title);

    if (prop_.proportional_font) {
	font_.CreateFontIndirect(prop_.lf);
	dc_.CreateCompatibleDC(pDC);
	dc_.SelectObject(font_);
    }
}

Output::~Output()
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 1行出力する。
/*!
  @param [in] no_line 行番号
  @param [in] str 出力する文字列
  @exception Error 出力に失敗
  @exception NotifyEndOutput 印刷範囲が終わった				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Output::print(int no_line, LPCTSTR str)
{
    if (!init())
	return;

    int char_num = 0;	// 出力した文字数(半角換算) [タブ位置計算用]

    ASSERT(line_buff_.IsEmpty() && current_char_ == 0);

    // 行番号
    print_line_number(no_line);

    try {
	int c;
	while ((c = CharType::detect(*str)) != CharType::TERMINATE) {
	    switch (c) {
	      case CharType::HALF_SIZE:	// 半角文字
		print_char(*(str++), 1);
		char_num++;
		break;

	      case CharType::FULL_SIZE:	// 全角文字
		print_char(*(str++), 2);
		char_num += 2;
		break;

	      case CharType::TAB:	// タブ
		str++;

		if (prop_.proportional_font) {
		    print_char(_T(' '), 1);
		} else {
		    do {
			print_char(_T(' '), 1);
			char_num++;
		    } while (char_num % prop_.tab_char != 0);
		}

		break;

	      case CharType::NOT_PRINTABLE:	// 表示不可
		
		print_char(prop_.proportional_font ? *str : _T('・'), 1);
		str++;
		char_num++;
		break;

	      default:
		NODEFAULT;
	    }
	}
    } catch (NotifyEndLine&) {
    }

    flush();

    if (no_line == attr_.end_line)
	// 今の行でおしまい。
	throw NotifyEndOutput();
    // 行で終了判定をしていない場合は、end_lineが0なので、
    // 一致することは無い。
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 改段組する
/*
  @exception Error 出力に失敗
  @exception NotifyEndOutput 印刷範囲が終わった				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Output::new_column()
{
    if (!init())
	return;

    current_col_++;
    current_line_ = 0;

    check_column_full();
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 改ページする
/*
  @exception Error 出力に失敗
  @exception NotifyEndOutput 印刷範囲が終わった				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void Output::new_page()
{
    if (!init())
	return;
    
    if (prop_.end_region_type == Property::END_REGION_PAGE &&
	current_page_ == prop_.end_region) {

	throw NotifyEndOutput();
    }

    current_page_++;
    current_col_ = 0;
    current_line_ = 0;
}

/* -------------------------------------------------------------------- */
//! 文字を１つ出力する。
/*!
  必要に応じて、改行、改段落、改ページを行う。
  @param [in] c 文字
  @param [in] count 半角文字換算した文字数
  @exception Error 出力に失敗
  @exception NotifyEndOutput 印刷範囲が終わった				*/
/* -------------------------------------------------------------------- */
void Output::print_char(TCHAR c, int count)
{
    check_char_full(c);

    line_buff_ += c;
    current_char_ += count;
}

/* -------------------------------------------------------------------- */
//! 行番号を出力する。
/*!
  @param [in] line 行番号						*/
/* -------------------------------------------------------------------- */
void Output::print_line_number(int line)
{
    if (prop_.output_line_count == 0)
	return;
    
    ASSERT(!attr_.line_format.IsEmpty());

    ASSERT(prop_.proportional_font ||
	   current_char_ + prop_.line_count_digit + 1 < attr_.printable_char);
    
    // 1行目の行番号は必ず出力
    if (line == 1 || line % prop_.output_line_count == 0) {
	CString line_num;
	line_num.Format(attr_.line_format, line);
	line_num = line_num.Right(prop_.line_count_digit + 1);

	for (int i = 0; i < prop_.line_count_digit + 1; i++)
	    line_buff_ += line_num[i];
    } else {
	for (int i = 0; i < prop_.line_count_digit + 1; i++)
	    line_buff_ += _T(' ');
    }
    current_char_  += prop_.line_count_digit + 1;
}


/* -------------------------------------------------------------------- */
//! バッファの内容を出力する。
/* -------------------------------------------------------------------- */
void Output::print_line()
{
    ASSERT(current_col_ < prop_.column);
    ASSERT(current_line_ < attr_.printable_line);

    do_print();

    current_line_++;
}

/* -------------------------------------------------------------------- */
//! バッファの内容を出力する。
/* -------------------------------------------------------------------- */
void Output::flush()
{
    check_line_full();

    print_line();

    line_buff_.Empty();
    current_char_ = 0;
}

/* -------------------------------------------------------------------- */
//! 1行分の最大文字を出力したか調べ、達していたら改行する。
/*!
  @param [in] next 次の文字
  @exception NotifyEndLine 折り返さない設定で最大文字出力した		*/
/* -------------------------------------------------------------------- */
void Output::check_char_full(TCHAR next)
{
    if (prop_.proportional_font) {
	// プロポーショナルフォント指定時。
	// 文字幅を調べて判定する。
	if (dc_.GetOutputTextExtent(line_buff_ + next).cx < attr_.column_width/* - attr_.font_x*/)
	    // もともと、文字がはみ出しても大丈夫なように、
	    // 1文字分余分に取るようにしている。
	    return;
    } else {
	// 等幅フォント指定時。
	if (current_char_ < attr_.printable_char)
	    return;
    }

    if (!prop_.wrap_right)
	throw NotifyEndLine();
    
    check_line_full();

    print_line();

    // 元の1行が2行以上で出力する際の（２行目以降の）行頭の空白文字数
    int head_space = prop_.proportional_font ? 0 : current_char_ - attr_.printable_char;
    // [等幅フォント時]
    // 全角文字の後ろ半分が、はみ出ることがある。
    // はみ出した分、次の行の先頭に空白をいれる。
    //
    // [プロポーショナルフォント時]
    // もともと行末は揃わないので、はみ出した分は気にしない。
    
    if (prop_.output_line_count > 0) {
	head_space += prop_.line_count_digit + 1;
	// 先頭に行番号の分、空白を入れる。
    }

    line_buff_.Empty();
    for (int i = 0; i < head_space; i++) {
	line_buff_ += _T(' ');
    }
    current_char_ = head_space;
}

/* -------------------------------------------------------------------- */
//! 1段分の最大行数を出力したか調べ、達していたら改段する。
/* -------------------------------------------------------------------- */
void Output::check_line_full()
{
    if (current_line_ < attr_.printable_line)
	return;

    ASSERT(current_line_ == attr_.printable_line);

    new_column();
}

/* -------------------------------------------------------------------- */
//! 出力した段組数を調べ、指定の段組に達していたら改ページする。
/* -------------------------------------------------------------------- */
void Output::check_column_full()
{
    if (current_col_ < prop_.column)
	return;

    ASSERT(current_col_ == prop_.column);

    new_page();
}
