/*! @file
  @brief 例外用クラス
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"throwable.h"

/* ==================================================================== */
//! メッセージを指定せずに構築する。
/*!
  メッセージの内容は、GetLastError() が返すシステムエラーコードに対応した
  エラーメッセージとする。						*/
/* ==================================================================== */
Exception::Exception()
{
    LPTSTR buff;
    ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		    FORMAT_MESSAGE_FROM_SYSTEM |
		    FORMAT_MESSAGE_IGNORE_INSERTS,
		    NULL,
		    ::GetLastError(),
		    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
		    reinterpret_cast<LPTSTR>(&buff),
		    0,
		    NULL);

    message_ = buff;
    ::LocalFree(buff);
}

/* ==================================================================== */
//! メッセージを指定して構築する。
/*!
  @param [in] message メッセージ					*/
/* ==================================================================== */
Exception::Exception(LPCTSTR message) : message_(message)
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! プリンターエラーのメッセージを返す
/*!
  @param reason エラー原因を表すコード
  @return エラーメッセージ						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
LPCTSTR PrinterError::message(int reason)
{
    switch (reason) {
      case SP_APPABORT:
      case SP_USERABORT:
	return _T("印刷が中止されました。");
      case SP_OUTOFDISK:
	return _T("スプールに必要なディスク領域がありません。");
      case SP_OUTOFMEMORY:
	return _T("スプールに必要なメモリがありません。");
      case SP_ERROR:
      default:
	return _T("プリンターエラーが発生しました。");
    }
}
