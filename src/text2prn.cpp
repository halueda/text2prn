/*! @file
  @brief メイン
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"text2prn.h"
#include	"CommandLineOption.h"
#include	"MainDialog.h"

#include	"FileListDialog.h"
#include	"Property.h"
#include	"FileInfo.h"

/* ==================================================================== */
//! プログラム起動時の処理
/* ==================================================================== */
BOOL CMyApp::InitInstance()
{
    std::locale::global(std::locale("japanese"));
    
    // ----- コマンド引数の解析 -----
    CommandLineOption option;
    ParseCommandLine(option);

    // ----- メインダイアログの作成 & 表示 -----
    MainDialog dialog;

    std::vector<CString>::const_iterator it;
    for (it = option.get_files().begin(); it != option.get_files().end(); ++it)
	dialog.add_init_file(*it);

    dialog.DoModal();

    return FALSE;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! デフォルトプリンタのDCを取得する。
/*!
  デフォルトプリンタとは、@n
  ツール上でプリンタの設定が行われている場合は、そのプリンタ。@n
  行われていない場合は、「通常使うプリンタ」

  プリンタの設定が取得できない時は、NULLを返す。
  
  @return デフォルトプリンタのDC					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
HDC CMyApp::get_default_printer_dc()
{
    if (m_hDevMode == NULL || m_hDevNames == NULL) {
	// ページ設定を行っていない場合。
	// デフォルトプリンタの設定を取得する。
	CPageSetupDialog dialog(PSD_RETURNDEFAULT);
	if (dialog.DoModal() != IDOK)
	    return NULL;

	SelectPrinter(dialog.m_psd.hDevNames, dialog.m_psd.hDevMode);

	return dialog.CreatePrinterDC();

    } else {

	// ページ設定を行った場合。
	CPageSetupDialog dialog(PSD_MARGINS | PSD_MINMARGINS);
	dialog.m_psd.hDevMode = m_hDevMode;
	dialog.m_psd.hDevNames = m_hDevNames;

	return dialog.CreatePrinterDC();
    }
}

CMyApp theApp;
