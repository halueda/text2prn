/*! @file
  @brief 設定から算出した印刷情報
  @author 依積晶紀
*/
#pragma once

#include	"TermList.h"

class Property;

/************************************************************************/
//! 設定から算出した印刷情報
/************************************************************************/
class Attribute
{
  private:
    enum {
	SCREEN_DPI = 72,	//!< 画面の解像度
    };
  public:
    int paper_width;		//!< 用紙の大きさ(横)
    int paper_height;		//!< 用紙の大きさ(縦)
    
    int printable_width;	//!< 印刷可能な大きさ(横)
    int printable_height;	//!< 印刷可能な大きさ(縦)
    int dpi_x;			//!< プリンタの解像度(横)
    int dpi_y;			//!< プリンタの解像度(縦)
    int offset_x;		//!< 印刷開始位置（左）
    int offset_y;		//!< 印刷開始位置（上）

    int column_width;		//!< 段組1つに印刷可能な大きさ
    int column_space;		//!< 段組の間の空白

    int font_x;			//!< 印字するフォントの大きさ(横)
    int font_y;			//!< 印字するフォントの大きさ(縦)
    double font_size;		//!< フォントサイズ（ポイント換算）

    int line_space;		//!< 行間 (文字の高さの1/4)

    int printable_char;		//!< 1行で印刷可能な文字数
    int printable_line;		//!< 1段で印刷可能な行数

    int end_line;		//!< 終了行

    CString line_format;	//!< 行番号を変換するためのフォーマット

    bool output_header;		//!< ヘッダを表示するかどうか
    bool output_footer;		//!< フッタを表示するかどうか

    CString title;		//!< タイトル

    TermList lheader;		//!< 左寄せヘッダ
    TermList cheader;		//!< 中央寄せヘッダ
    TermList rheader;		//!< 右寄せヘッダ
    TermList lfooter;		//!< 左寄せフッタ
    TermList cfooter;		//!< 中央寄せフッタ
    TermList rfooter;		//!< 右寄せフッタ

    int total_page;		//!< ページ総数
    
  public:
    void init(const CDC*, const Property&, LPCTSTR, LPCTSTR);
    static bool use_total_page_count(const Property&);

  private:
    void init_header(const Property&, LPCTSTR);
    static void parse_term(LPCTSTR, TermList&, LPCTSTR, LPCTSTR);
    static void add_term_string(TermList&, LPCTSTR, LPCTSTR);
    
    
    void init_h(const Property&);
    void calc_from_char_h(const Property&);
    void calc_from_line_h(const Property&);
    void calc_from_font_h(const Property&);

    void init_v(const Property&);
    void calc_from_char_v(const Property&);
    void calc_from_line_v(const Property&);
    void calc_from_font_v(const Property&);
    void calc_column_height_v(const Property&);
    
    /* ---------------------------------------------------------------- */
    //! mmをdotに変換
    /*! @param [in] mm mmで表した長さ
	@param [in] dpi dpi
	@return dotで表した長さ						*/
    /* ---------------------------------------------------------------- */
    static int mm2dot(int mm, int dpi)
    {
	return dpi*mm*10/254;
    }
};
