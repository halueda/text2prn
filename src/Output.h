/*! @file
  @brief テキストの出力
  @author 依積晶紀
*/
#pragma once

#include	"Attribute.h"

class Property;

/************************************************************************/
//! テキストの出力先
/************************************************************************/
class Output
{
  private:
    Attribute attr_;		//!< 印刷情報
    const Property& prop_;	//!< 設定

    CString line_buff_;		//!< 現在出力中の行の内容

    int current_col_;		//!< 現在出力中の段番号
    int current_line_;		//!< 現在出力中の行の位置
    int current_page_;		//!< 現在出力中のページ番号（1〜）
    int current_char_;		//!< 現在出力中の行の文字数 (半角換算)

    CDC dc_;		//!< 文字列幅計算用（プロポーショナルフォント時）
    CFont font_;	//!< 文字列幅計算用（プロポーショナルフォント時）

  protected:
    Output(CDC*, const Property&, LPCTSTR);

  public:
    virtual ~Output();

    
    void print(int, LPCTSTR);	// テキストの出力
    void new_column();		// 改段落
    virtual void new_page();	// 改ページ

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! テキストの出力が全て完了した時の処理
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    virtual void end() = 0;

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! （エラー等の理由により）テキストの出力を中止した時の処理
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    virtual void abort() = 0;

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 印刷情報を取得する。
    /*! @return 印刷情報						*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    const Attribute& get_attribute() const
    {
	return attr_;
    }
    
  protected:
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    //! 出力先の初期化処理
    /*!
      テキストの出力を行う度に[ print(int, LPCTSTR) 呼び出し毎に] 、
      呼び出される。

      派生クラスでは、テキストの出力先の初期化を行い、
      テキストが出力可能かどうかを返す。

      初回の呼び出し時には、必ず new_page() を呼ぶように実装すること。

      @retval true 出力できる
      @retval false 出力できない					*/
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    virtual bool init() = 0;

    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    //! テキストの出力処理
    /*!
      派生クラスでは、出力先にテキストを出力するよう実装する。

      出力するテキストは、get_line_buff()で取得する。\n
      出力位置は、get_page(), get_col(), get_line() から取得する。	*/
    /* 〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜〜 */
    virtual void do_print() = 0;

    //! 現在出力中のページ番号を取得
    int get_page() const {return current_page_;}
    //! 現在出力中の段を取得
    int get_col() const {return current_col_;}
    //! 現在出力中の行を取得
    int get_line() const {return current_line_;}
    //! 現在出力中の行のテキストを取得
    const CString& get_line_buff() const {return line_buff_;}

    //! 設定を取得
    const Property& get_property() const
    {
	return prop_;
    }
    
  private:
    void print_line_number(int);
    void print_char(TCHAR, int);
    void print_line();
    void check_char_full(TCHAR);
    void check_line_full();
    void check_column_full();
    void flush();
};
