/*! @file
  @brief フッタ/ヘッダの内容
  @author 依積晶紀
*/
#pragma once

#include	"Term.h"

/************************************************************************/
//! フッタ/ヘッダ用記述
/************************************************************************/
class TermList
{
  private:
    std::vector<Term*> terms_;
    
  public:
    TermList()
    {
    }

    ~TermList()
    {
	std::vector<Term*>::iterator it;
	for (it = terms_.begin(); it != terms_.end(); ++it)
	    delete *it;
    }

    TermList(const TermList& other)
    {
	std::vector<Term*>::const_iterator it;
	for (it = other.terms_.begin();
	     it != other.terms_.end();
	     ++it) {

	    terms_.push_back((*it)->clone());
	}	
    }

    const TermList& operator=(const TermList& rhs)
    {
	if (this != &rhs) {
	    swap(TermList(rhs));
	}
	return *this;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! ヘッダ/フッタに項目を追加する。
    /*!
	@param [in] term 追加する項目					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void add(Term* term)
    {
	terms_.push_back(term);
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! ヘッダ/フッタの内容を空にする。
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    bool empty() const
    {
	return terms_.empty();
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! ヘッダ/フッタに出力する文字列を取得する
    /*!
	@param [in] context ヘッダ/フッタに必要となる情報
	@return ヘッダ/フッタに出力する文字列				*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    CString get_term(const TermContext& context) const
    {
	CString str;
	std::vector<Term*>::const_iterator it;
	for (it = terms_.begin(); it != terms_.end(); ++it)
	    str += (*it)->get_term(context);
	return str;
    }

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! ページ総数を指定しているかどうか
    /*!
	@retval true ページ総数 あり
	@retval false ページ総数 なし					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    bool use_total_page_count() const
    {
	std::vector<Term*>::const_iterator it;
	for (it = terms_.begin(); it != terms_.end(); ++it) {
	    if (typeid(**it) == typeid(TermTotalPage))
		return true;
	}
	return false;
    }
    
  private:
    /* ---------------------------------------------------------------- */
    //! データの入れ替え
    /* ---------------------------------------------------------------- */
    void swap(TermList& other)
    {
	terms_.swap(other.terms_);
    }
};

