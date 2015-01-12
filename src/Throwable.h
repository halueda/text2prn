/*! @file
  @brief 例外用クラス
  @author 依積晶紀
*/
#pragma once

/************************************************************************/
//! エラー等の通知クラス
/************************************************************************/
class Exception
{
  private:
    CString message_;	//!< メッセージ

  public:
    // メッセージを指定して構築
    Exception(LPCTSTR);

    // メッセージを指定せずに構築
    Exception();

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! メッセージを取得する。
    /*! 
	@return メッセージ						*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    const CString& get_message() const
    {
	return message_;
    }
};

/************************************************************************/
//! 警告
/************************************************************************/
class Warning : public Exception
{
  public:
    Warning(LPCTSTR _mes) : Exception(_mes) {}
    Warning() {}
};

/************************************************************************/
//! エラー
/************************************************************************/
class Error : public Exception
{
  public:
    Error(LPCTSTR _mes) : Exception(_mes) {}
    Error() {}
};

/************************************************************************/
//! 印刷エラー
/************************************************************************/
class PrinterError : public Error
{
  public:
    PrinterError(int reason) : Error(message(reason)) {}

  private:
    static LPCTSTR message(int);
};

/************************************************************************/
//! 出力終了通知
/************************************************************************/
class NotifyEndOutput
{
};

/************************************************************************/
//! 出力終了通知 (行の折り返しをしない時用)
/************************************************************************/
class NotifyEndLine
{
};