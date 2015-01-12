/*! @file
  @brief メイン画面
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"MainDialog.h"
#include	"resource.h"
#include	"OutputFactory.h"
#include	"Output.h"
#include	"MultiFileReader.h"
#include	"FileReaderUtil.h"
#include	"ClipboardReader.h"
#include	"Throwable.h"
#include	"CtrlUtil.h"
#include	"FileListDialog.h"
#include	"EditPresetDialog.h"
#include	"AttributeDialog.h"
#include	"Util.h"
#include	"PropertyUtil.h"

BEGIN_MESSAGE_MAP(MainDialog, CDialog)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnChangeTab)
    ON_COMMAND_RANGE(IDC_FROM_FILE, IDC_FROM_CLIP, OnChangeReaderType)
    ON_EN_CHANGE(IDC_FILE, OnEditFilePath)
    ON_COMMAND(IDC_REF, OnRefFile)
    ON_COMMAND(IDC_REF_MULTI, OnRefMultiFile)
    ON_WM_DROPFILES()
    ON_MESSAGE(WM_APPLY_PRESET, OnApplyPreset)
    ON_MESSAGE(WM_EDIT_PRESET, OnEditPreset)
    ON_MESSAGE(WM_PREVIEW, OnPreview)
    ON_MESSAGE(WM_PAGE_SETUP, OnPageSetup)
    ON_MESSAGE(WM_USE_HEADER, OnChangeUseHeader)
END_MESSAGE_MAP()

MainDialog::MainDialog() : CDialog(IDD_MAIN), preview_(NULL)
{
    preview_.reset(new PreviewWindow(this));

    tab_pages_[0] = &tab_text_;
    tab_pages_[1] = &tab_font_;
    tab_pages_[2] = &tab_region_;
    tab_pages_[3] = &tab_space_;
    tab_pages_[4] = &tab_header_;
    tab_pages_[5] = &tab_option_;
    tab_pages_[6] = &tab_preview_;
}

/* ==================================================================== */
//! ダイアログの初期化
/*!
  各コントロールの設定を行う。						*/
/* ==================================================================== */
BOOL MainDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ----- アイコンの設定 -----
    const HICON icon = ::AfxGetApp()->LoadIcon(IDI_MAIN);
    ASSERT(icon != NULL);
    SetIcon(icon, TRUE);
    SetIcon(icon, FALSE);

    init_tab_ctrl();

    init_encoding_ctrl();

    init_reader_type_ctrl();

    return TRUE;
}

/* ==================================================================== */
//! [OK]ボタンの処理
/*!
  画面での設定に従い、印刷を行う。

  設定値に不備がある場合は、エラーメッセージを表示し、処理を中断する。	*/
/* ==================================================================== */
void MainDialog::OnOK()
{
    update_property();
    if (!properties_.check_value()) {
	MessageBox(properties_.get_err_mes(), CAPTION_WARNING, MB_ICONWARNING);
	return;
    }
    print(PrinterFactory(), properties_.get().view_info);
}

/* ==================================================================== */
//! [終了]ボタンを押した
/*!
  @li プレビュー画面を破棄する。
  @li 設定値を保存する。
  @li 画面を閉じて、プログラムを終了する				*/
/* ==================================================================== */
void MainDialog::OnCancel()
{
    preview_->destroy();
    
    update_property();
    CDialog::OnCancel();
}

/* ==================================================================== */
//! タブを変更した時の処理
/*!
  タブコントロールの内容を切り替える					*/
/* ==================================================================== */
void MainDialog::OnChangeTab(NMHDR* lpnmhdr, LRESULT* result)
{
    change_tab_page(CtrlUtil::get_tab_index(this, IDC_TAB));
    *result = 0;
}

/* ==================================================================== */
//! 印刷対象選択のラジオボタンを変更した時の処理
/*!
  選択状態にあるラジオボタンにあわせて、
  各コントロールの状態を変更する					*/
/* ==================================================================== */
void MainDialog::OnChangeReaderType(UINT nID)
{
    change_reader_type(nID - IDC_FROM_FILE);
}

/* ==================================================================== */
//! ファイル名のテキストボックスを編集した時の処理
/*!
  [印刷]ボタンの状態を変更する。

  ファイル名の入力がある -> [印刷]ボタン、有効 @n
  ファイル名の入力がない -> [印刷]ボタン、無効				*/
/* ==================================================================== */
void MainDialog::OnEditFilePath()
{
    TCHAR buff[2];
    if (GetDlgItemText(IDC_FILE, buff, 2) != 0) {
	GetDlgItem(IDOK)->EnableWindow(TRUE);
    } else {
	GetDlgItem(IDOK)->EnableWindow(FALSE);
    }
}

/* ==================================================================== */
//! [参照]ボタンを押した時の処理
/*!
  @li ファイル選択ダイアログを表示。
  @li 選択したファイルが1つなら、パスを [ファイル名]欄に設定、
	「(単一)ファイル」のラジオボタンをチェック
  @li 複数選択したなら、複数選択用のファイル一覧を選択したものに置き換え、
	「複数ファイル」のラジオボタンをチェック。
	文字コードが画面で指定されているものを使う。			*/
/* ==================================================================== */
void MainDialog::OnRefFile()
{
    CFileDialog dialog(TRUE, NULL, NULL,
		       OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
		       _T("全てのファイル(*.*)|*.*||"));

    if (dialog.DoModal() != IDOK)
	return;

    std::vector<FileInfo> buff;
    const int enc = CtrlUtil::get_combo_index(this, IDC_ENCODING);

    POSITION pos = dialog.GetStartPosition();
    while (pos != NULL)
	buff.push_back(FileInfo(dialog.GetNextPathName(pos), enc));

    switch (buff.size()) {
      case 0:
	break;

      case 1:
	// ----- 選択したファイル、1つ ⇒ 「(単一)ファイル」選択 -----
	SetDlgItemText(IDC_FILE, buff[0].path);
	CtrlUtil::set_radio_button(this, IDC_FROM_FILE);
	break;

      default:
	// ----- 選択したファイル、複数 ⇒ 「複数ファイル」選択 -----
	files_.swap(buff);
	// ファイル一覧置き換え（追加ではない）
	CtrlUtil::set_radio_button(this, IDC_FROM_MULTI_FILE);
	break;
    }
}

/* ==================================================================== */
//! [詳細]ボタンを押した時の処理
/*!
  @li ファイル一覧ダイアログを表示する。
  @li ファイル一覧ダイアログで変更をおこなったら、
	「複数ファイル」のラジオボタンをチェック。			*/
/* ==================================================================== */
void MainDialog::OnRefMultiFile()
{
    FileListDialog dialog(files_, properties_.get_for_update(),
			  CtrlUtil::get_combo_index(this, IDC_ENCODING));

    if (dialog.DoModal() != IDOK)
	return;

    CtrlUtil::set_radio_button(this, IDC_FROM_MULTI_FILE);
}

/* ==================================================================== */
//! ファイルをドロップした時の処理
/*!
  ドロップしたファイルが1つなら、パスを [ファイル名]欄に設定、
  「(単一)ファイル」のラジオボタンをチェック。
  
  ファイルが複数なら、複数選択用のファイル一覧を置き換え、
  「複数ファイル」のラジオボタンをチェック。@n
  文字コードが画面で指定されているものを使う。

  ディレクトリは除外する。						*/
/* ==================================================================== */
void MainDialog::OnDropFiles(HDROP hDropInfo)
{
    TCHAR fname[_MAX_PATH];
    std::vector<FileInfo> buff;
    const int encoding = CtrlUtil::get_combo_index(this, IDC_ENCODING);
    const int num = ::DragQueryFile(hDropInfo, -1, NULL, 0);

    for (int i = 0; i < num; i++) {
	::DragQueryFile(hDropInfo, i, fname, _MAX_PATH);

	if (::PathIsDirectory(fname))
	    continue;	// ディレクトリは除外

	buff.push_back(FileInfo(fname, encoding));
    }

    switch (buff.size()) {
      case 0:
	break;

      case 1:
	// ----- ドロップしたファイル、1つ ⇒ 「(単一)ファイル」選択 -----
	SetDlgItemText(IDC_FILE, buff[0].path);
	CtrlUtil::set_radio_button(this, IDC_FROM_FILE);
	break;

      default:
	// ----- ドロップしたファイル、複数 ⇒ 「複数ファイル」選択 -----
	files_.swap(buff);
	// ファイル一覧置き換え（追加ではない）
	CtrlUtil::set_radio_button(this, IDC_FROM_MULTI_FILE);
	break;
    }

    ::DragFinish(hDropInfo);
}

/* ==================================================================== */
//! 登録済み設定の適用を行った時の処理
/*!
  登録済み設定の内容に基づき、コントロールの内容を更新する。
  @param [in] index 適用したい登録済み設定のインデックス		*/
/* ==================================================================== */
LRESULT MainDialog::OnApplyPreset(WPARAM index, LPARAM)
{
    if (index < 0 || index >= properties_.get_preset_size()) {
	MessageBox(_T("適用する設定が選択されていません。"),
		   CAPTION_WARNING, MB_ICONWARNING);
	return 0;
    }

    properties_.set_property_from_preset(index);

    for (int i = 0; i < TAB_SIZE; i++)
	tab_pages_[i]->update_ctrl(properties_.get());

    return 0;
}

/* ==================================================================== */
//! 登録済み設定の編集を行った時の処理
/*!
  登録済み設定編集ダイアログを表示する。

  編集が行われた場合は、画面の内容を更新する。				*/
/* ==================================================================== */
LRESULT MainDialog::OnEditPreset(WPARAM, LPARAM)
{
    update_property();

    EditPresetDialog dialog(properties_);

    if (dialog.DoModal() == IDOK)
	tab_option_.update_preset(properties_.get_preset());

    return 0;
}

/* ==================================================================== */
//! プレビューを行った時の処理
/*!
  画面での設定に従い、プレビュー画面を表示する。

  設定値に不備がある場合は、エラーメッセージを表示し、処理を中断する。	*/
/* ==================================================================== */
LRESULT MainDialog::OnPreview(WPARAM, LPARAM)
{
    update_property();
    if (!properties_.check_value()) {
	MessageBox(properties_.get_err_mes(), CAPTION_WARNING, MB_ICONWARNING);
	return 0;
    }

    print(PreviewFactory(*preview_), false);

    return 0;
}

/* ==================================================================== */
//! プリンタ設定を行った時の処理
/*!
  プリンタ設定ダイアログを表示し、
  ダイアログで設定した内容を画面に反映させる。
  （設定する項目は、余白）						*/
/* ==================================================================== */
LRESULT MainDialog::OnPageSetup(WPARAM, LPARAM)
{
    update_property();

    CPageSetupDialog dialog(PSD_MARGINS | PSD_MINMARGINS);
    PropertyUtil::init_page_setup_dialog(dialog, properties_.get());

    if (dialog.DoModal() != IDOK)
	return 0;

    AfxGetApp()->SelectPrinter(dialog.m_psd.hDevNames, dialog.m_psd.hDevMode);
    if (properties_.get().use_space) {
	Property& p = properties_.get_for_update();
	p.space_left   = dialog.m_psd.rtMargin.left   / 100;
	p.space_top    = dialog.m_psd.rtMargin.top    / 100;
	p.space_right  = dialog.m_psd.rtMargin.right  / 100;
	p.space_bottom = dialog.m_psd.rtMargin.bottom / 100;
	tab_space_.update_ctrl(p);
    }

    return 0;
}

/* ==================================================================== */
//! ヘッダ/フッタの詳細設定を使用するかどうかを変更した時の処理
/*!
  「その他」タブにある「ページ数表示」「ファイル名表示」の無効/有効を変更する

  詳細設定が有効なら、無効にする。
  
  @param [in] state 詳細設定を使用するならTRUE。使用しないならFALSE	*/
/* ==================================================================== */
LRESULT MainDialog::OnChangeUseHeader(WPARAM state, LPARAM)
{
    if (tab_option_.GetSafeHwnd() != NULL) {
	tab_option_.enable_header_option(state ? FALSE : TRUE);
    }

    return 0;
}

/* -------------------------------------------------------------------- */
//! テキストを出力（印刷もしくはプレビュー）する。
/*!
  問題がある場合は、
  
  @param [in] factory 出力先作成オブジェクト
  @param [in] view_info 出力情報の詳細表示を行うかどうか。true:表示する
  @note	プレビュー時は、設定に関係なく印刷情報は表示したくないので、
	引数で渡すようにしてある。					*/
/* -------------------------------------------------------------------- */
void MainDialog::print(OutputFactory& factory, bool view_info)
{
    try {
	// ===== 入力元の準備 =====
	std::auto_ptr<TextReader> in(get_reader());
	if (in.get() == NULL)
	    return;

	// ----- 開始行までスキップ
	// ※ 設定が「初めから」の時は、begin_region = 0 なので、
	//    開始位置の指定方法で条件分けする必要なし。
	if (!in->skip_line(properties_.get().begin_region)) {
	    throw Warning(_T("開始行までスキップしたら、データが無くなりました。"));
	}
    
	// ===== 出力先の準備 =====
	std::auto_ptr<Output> out(factory.get_output(properties_.get(),
						     in->get_name()));
	if (out.get() == NULL)
	    return;

	try {
	    if (view_info) {
		// ===== 印刷情報の表示 =====
		AttributeDialog dialog(in->pre_get(),
				       properties_.get(),
				       out->get_attribute());
		if (dialog.DoModal() != IDOK)
		    throw Warning(_T("印刷を中止しました。"));
	    }

	    CWaitCursor wait;
	    
	    // ===== 印字 =====

	    int line_number = properties_.get().begin_region;

	    while (in->next()) {
		const Text text = in->get_text();
		switch (text.type_) {

		  case Text::TYPE_TEXT:			// 通常テキスト
		    out->print(++line_number, text.str_);
		    break;

		  case Text::TYPE_CTRL_NEW_COLUMN:	// 改段落
		    out->new_column();
		    break;

		  case Text::TYPE_CTRL_NEW_PAGE:	// 改ページ
		    out->new_page();
		    break;
		}
	    }

	} catch (NotifyEndOutput&) {
	    // 印刷範囲終了の通知
	    // 正常終了なので、何もしない。
	} catch (...) {
	    out->abort();
	    throw;
	}

	out->end();
	
    } catch (Warning& e) {
	MessageBox(e.get_message(), CAPTION_WARNING, MB_ICONWARNING | MB_OK);
    } catch (Error& e) {
	MessageBox(e.get_message(), CAPTION_ERROR, MB_ICONERROR | MB_OK);
    } catch (std::exception& e) {
//@@@	MessageBox(e.what(), CAPTION_ERROR, MB_ICONERROR | MB_OK);
    }
}

/* -------------------------------------------------------------------- */
//! テキスト入力ストリームを取得する。
/*!
  画面上の指定に合わせて、
  ファイルからの入力ストリーム、クリップボードからの入力ストリームを
  作成して返す。

  @return ファイル等からテキストを取得するための入力ストリームオブジェクト
  @throws Warning 画面上の設定に不備
  @throws Error ファイル操作 or クリップボード操作で失敗
  @throws std::bad_alloc メモリ不足					*/
/* -------------------------------------------------------------------- */
TextReader* MainDialog::get_reader()
{
    switch (CtrlUtil::get_radio_button(this, IDC_FROM_FILE, IDC_FROM_CLIP)) {
      case IDC_FROM_FILE:
      {
	  CString fname;
	  GetDlgItemText(IDC_FILE, fname);
	  fname.Trim();
	  if (fname.IsEmpty())
	      // [プレビュー]ボタンは、有効/無効の制御をしていないので、
	      // この条件判断は必要。
	      throw Warning(_T("ファイルが指定されていません。"));
	  

	  return FileReaderUtil::get_reader(fname, CtrlUtil::get_combo_index(this, IDC_ENCODING));
      }

      case IDC_FROM_MULTI_FILE:
	if (files_.empty())
	    // [プレビュー]ボタンは、有効/無効の制御をしていないので、
	    // この条件判断は必要。
	    throw Warning(_T("ファイルが指定されていません。"));

	return new MultiFileReader(files_, properties_.get().separate_text_type,
				   properties_.get().separate_line_num);

      case IDC_FROM_CLIP:
	if (!::IsClipboardFormatAvailable(CF_UNICODETEXT))
	    throw Warning(_T("クリップボードにデータがありません"));
	return new ClipboardReader();
    }

    ASSERT(false);
    return NULL;
}


/* -------------------------------------------------------------------- */
//! 表示するタブページを切り替える
/*!
  @param [in] page 表示するタブのインデックス				*/
/* -------------------------------------------------------------------- */
void MainDialog::change_tab_page(int page)
{
    for (int i = 0; i < TAB_SIZE; i++)
	tab_pages_[i]->ShowWindow(i == page ? SW_SHOW : SW_HIDE);
}

/* -------------------------------------------------------------------- */
//! 印刷対象の種類を変更したことによるコントロールの状態変更処理
/*!
  [ファイル名]欄は、「(単一)ファイル」選択時のみ有効。

  [詳細]ボタンには、ファイルの個数も表示。
  ただし、「複数ファイル」選択時以外は、ファイルの個数は「？」とする。

  「(単一)ファイル」選択時、[ファイル名]欄が空欄なら[印刷]ボタンは無効。@n
  「複数ファイル」選択時、ファイル数が0個なら[印刷]ボタンは無効。
  
  @param [in] type 現在選択されている印刷対象の種類			*/
/* -------------------------------------------------------------------- */
void MainDialog::change_reader_type(int type)
{
    switch (type) {
      case READER_TYPE_FILE:
	GetDlgItem(IDC_FILE)->EnableWindow(TRUE);
	{
	    TCHAR buff[2];
	    GetDlgItem(IDOK)->EnableWindow(GetDlgItemText(IDC_FILE, buff, 2) ? TRUE : FALSE);
	    SetDlgItemText(IDC_REF_MULTI, _T("詳細 （？）"));
	}
	break;
	
      case READER_TYPE_MULTI_FILE:
	GetDlgItem(IDC_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(files_.empty() ? FALSE : TRUE);
	SetDlgItemText(IDC_REF_MULTI, FString(_T("詳細 （%d）"), files_.size()));
	break;

      case READER_TYPE_CLIPBOARD:
	GetDlgItem(IDC_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	// クリップボードにデータがあるかどうかはチェックしない。
	SetDlgItemText(IDC_REF_MULTI, _T("詳細 （？）"));
	break;

      default:
	NODEFAULT;
    }
}

/* -------------------------------------------------------------------- */
//! コントロールの内容から設定値を更新する。
/* -------------------------------------------------------------------- */
void MainDialog::update_property()
{
    for (int i = 0; i < TAB_SIZE; i++) {
	tab_pages_[i]->update_value(properties_.get_for_update());
    }
}

/* -------------------------------------------------------------------- */
//! タブコントロールの初期化
/*!
  @li タブコントロールの設定
  @li 各タブ内のコントロール設定
  @li 「テキスト」タブを選択						*/
/* -------------------------------------------------------------------- */
void MainDialog::init_tab_ctrl()
{
    CTabCtrl* const tab = (CTabCtrl*)GetDlgItem(IDC_TAB);
    ASSERT(tab != NULL);
    tab->InsertItem(0, _T("テキスト"));
    tab->InsertItem(1, _T("フォント"));
    tab->InsertItem(2, _T("印刷範囲"));
    tab->InsertItem(3, _T("余白"));
    tab->InsertItem(4, _T("ヘッダ/フッタ"));
    tab->InsertItem(5, _T("その他"));
    tab->InsertItem(6, _T("プレビュー"));

    CRect rect;		// タブコントロール内の大きさ
    tab->GetWindowRect(&rect);
    ScreenToClient(&rect);
    tab->AdjustRect(FALSE, &rect);

    // タブにダイアログを張り付け
    for (int i = 0; i < TAB_SIZE; i++) {
	tab_pages_[i]->Create(IDD_TEXT + i, this);
	tab_pages_[i]->MoveWindow(rect);
	tab_pages_[i]->init();
	tab_pages_[i]->update_ctrl(properties_.get());
    }
    tab_option_.update_preset(properties_.get_preset());

    // 「テキスト」タブ選択
    tab->SetCurSel(0);
    change_tab_page(0);
}

/* -------------------------------------------------------------------- */
//! 文字コード選択コントロールの初期化
/*!
  対応している文字コードを、
  文字コード選択コンボボックスの選択項目として登録する。		*/
/* -------------------------------------------------------------------- */
void MainDialog::init_encoding_ctrl()
{
    CComboBox* const ctrl = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_ENCODING));
    ASSERT(ctrl != NULL);
    FileInfo::set_combo_ctrl(ctrl, FileInfo::SJIS);
}

/* -------------------------------------------------------------------- */
//! 印刷対象を選択するコントロールの初期化
/*!
  印刷対象を選択するラジオボタンを以下の条件によってチェックする。
  
  ファイルの指定がなければ、「クリップボード」にチェック

  ファイルが1つだけ指定されているなら、「（単一）ファイル」にチェック。
  そのパスを表示する。

  ファイルが2つ以上指定されているなら、「複数ファイル」にチェック。	*/
/* -------------------------------------------------------------------- */
void MainDialog::init_reader_type_ctrl()
{
    switch (files_.size()) {
      case 0:
	CtrlUtil::set_radio_button(this, IDC_FROM_CLIP);
	break;

      case 1:
	SetDlgItemText(IDC_FILE, files_[0].path);
	files_.clear();
	// ↑files_は、本来複数ファイル用のメンバなので、クリアしておく。
	CtrlUtil::set_radio_button(this, IDC_FROM_FILE);
	break;

      default:
	CtrlUtil::set_radio_button(this, IDC_FROM_MULTI_FILE);
	break;
    }
}
