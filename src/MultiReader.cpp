/*! @file
  @brief 複数データ入力
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"MultiReader.h"

/* ==================================================================== */
//! テキスト入力ストリームの区切り方を指定して構築する。
/*!
  区切り記号が必要ない（連続して入力する）場合は、
  type = LINE , num = 0 を指定する。

  type が LINE 以外の時は、num の値を無視する。
  
  @param [in] type 区切り方
  @param [in] num 区切り記号の個数					*/
/* ==================================================================== */
MultiReader::MultiReader(SeparatorType type, size_t num) :
	type_(type),
	num_sep_(type == LINE ? num : 1),
	rest_sep_(0),
	reader_(NULL)
{
}

MultiReader::~MultiReader()
{
    delete reader_;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 次の行へ移動する。
/*!
  get_text()が次の行を返すように、現在位置を1行分進める。

  行の区切りは CR, LF, NUL とする。
  （CR LFは、これで1つの行区切り記号とする）

  行頭にある FF は改段落として、これ１つで１行と扱う。

  テキストデータの終わりに達して次の行が存在しない時は、false を返す。

  テキストデータが空の場合、
  初回の呼び出し時のみ true を返す。\n
  （この時の get_text() は、空文字列を返す）
	
  @retval true 移動は成功した
  @retval false 次の行は存在しない					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
bool MultiReader::next()
{
    if (rest_sep_ > 0) {
	rest_sep_--;
	flag_return_separator = true;
	return true;
    }
    flag_return_separator = false;

    if (reader_ != NULL) {
	if (reader_->next())
	    return true;
	rest_sep_ = num_sep_;
    }

    delete reader_;
    reader_ = get_next_reader();
    if (reader_ == NULL)
	return false;

    return next();
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 現在の行を取得する。
/*!
  next()が呼ばれてから、次にnext()が呼ばれるまで有効。\n
  next()がfalseを返した後（全ての行の処理後）での値は不定。

  @return 現在の行							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
const Text MultiReader::get_text() const
{
    if (flag_return_separator) {
	switch (type_) {
	  case LINE:
	    return Text(_T(""));
	  case COLUMN:
	    return Text(Text::TYPE_CTRL_NEW_COLUMN);
	  case PAGE:
	    return Text(Text::TYPE_CTRL_NEW_PAGE);
	}
	ASSERT(false);
    }

    ASSERT(reader_ != NULL);
    return reader_->get_text();
}
