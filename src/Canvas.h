/*! @file
  @brief 印刷イメージの描画
  @author 依積晶紀
*/
#pragma once

class Attribute;
class Property;
struct TermContext;

/************************************************************************/
//! 印刷イメージの描画先
/************************************************************************/
class Canvas
{
  protected:
    CDC& dc_;			//!< 出力先のDC

    CFont font_body_;		//!< 印字フォント
    CFont font_title_;		//!< タイトル、ページ印字フォント（縦書き用）
    CFont* old_font_;		//!< 元のフォントのバックアップ

    const Attribute& attr_;	//!< 印刷情報
    const Property& prop_;	//!< 設定

  public:
    Canvas(CDC&, const Attribute&, const Property&);
    virtual ~Canvas();

    void draw_new_page(int);
    void draw_text(int, int, LPCTSTR);

  private:
    void draw_header(const TermContext&);
    void draw_footer(const TermContext&);
//    void draw_page_number(int);
//    void draw_title();
    void draw_column_line();
};
