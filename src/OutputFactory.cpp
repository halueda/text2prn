/*! @file
  @brief テキストの出力先の作成
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"OutputFactory.h"
#include	"OutputPreview.h"
#include	"OutputPrinter.h"
#include	"OutputPrinterBuffered.h"
#include	"text2prn.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力先のインスタンスを作成する。
/*!
  印刷ダイアログを表示し、
  選択した印刷先用のテキスト出力インスタンスを返す。

  ダイアログでキャンセルした場合は、NULLを返す。
      
  戻り値として返されるインスタンスは、
  呼び出し元で delete すること。

  @param [in] prop 設定
  @param [in] title タイトル
  @return 出力先のインスタンス
  @exception Warning 設定値に問題あり					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
Output* PrinterFactory::get_output(const Property& prop, LPCTSTR title)
{
    CPrintDialog dialog(FALSE);
    if (::AfxGetApp()->DoPrintDialog(&dialog) != IDOK)
	return NULL;

    CDC dc;
    dc.Attach(dialog.GetPrinterDC());

    if (Attribute::use_total_page_count(prop))
	return new OutputPrinterBuffered(&dc, prop, title);
    else
	return new OutputPrinter(&dc, prop, title);
}

/* ==================================================================== */
/*!
  @param [in] preview プレビュー画面					*/
/* ==================================================================== */
PreviewFactory::PreviewFactory(PreviewWindow& preview) : preview_(preview)
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 出力先のインスタンスを作成する。
/*!
  プレビュー用のテキスト出力インスタンスを返す。

  戻り値として返されるインスタンスは、
  呼び出し元で delete すること。

  @param [in] prop 設定
  @param [in] title タイトル
  @return 出力先のインスタンス
  @exception Warning 設定値に問題あり					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
Output* PreviewFactory::get_output(const Property& prop, LPCTSTR title)
{
    CMyApp* app = CMyApp::get_instance();
    if (app == NULL)
	return NULL;

    HDC hDC = app->get_default_printer_dc();
    if (hDC == NULL)
	throw Warning(_T("プリンタの設定が取得できません。"));

    CDC dc;		// プリンタの情報さえ取得してしまえば、後は要らない。
    dc.Attach(hDC);

    return new OutputPreview(&dc, prop, title, preview_);
}
