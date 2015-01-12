/*! @file
  @brief 複数ファイル選択ダイアログ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"FileListDialog.h"
#include	"CtrlUtil.h"
#include	"resource.h"
#include	"Property.h"
#include	"SelectEncodingDialog.h"

BEGIN_MESSAGE_MAP(FileListDialog, CDialog)
    ON_EN_CHANGE(IDC_LINE_COUNT, OnChangeLineCount)
    ON_COMMAND(ID_ADD_FILE, OnAddFile)
    ON_WM_DROPFILES()
    ON_COMMAND(ID_DEL_FILE, OnDelFile)
    ON_COMMAND(ID_CLEAR_FILE_LIST, OnClearFile)
    ON_COMMAND(ID_MOVE_UP, OnMoveUp)
    ON_COMMAND(ID_MOVE_DOWN, OnMoveDown)
    ON_COMMAND(IDC_UP, OnMoveUp)
    ON_COMMAND(IDC_DOWN, OnMoveDown)
    ON_COMMAND(ID_SELECT_ALL, OnSelectAll)
    ON_COMMAND(ID_SELECT_INVERT, OnSelectInvert)
    ON_COMMAND(ID_ENCODING, OnChangeEncoding)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_FILE_LIST, OnActivateList)
END_MESSAGE_MAP()

/* ==================================================================== */
/*!
  @param [in,out] files ファイル一覧
  @param [in,out] prop プロパティ
  @param [in] encoding デフォルトの文字コード				*/
/* ==================================================================== */
FileListDialog::FileListDialog(std::vector<FileInfo>& files, Property& prop, int encoding) :
	CDialog(IDD_FILE_LIST),
	files_(files), prop_(prop), default_encoding_(encoding)
{
}

/* ==================================================================== */
//! ダイアログの初期化
/*!
  @li アクセラレータキーの設定
  @li ファイル一覧リストコントロールの初期化
  @li 既存のファイル一覧をファイル一覧リストコントロールに設定
  @li 複数ファイルの区切り方法をコントロールに設定			*/
/* ==================================================================== */
BOOL FileListDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    accel_ = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_FILE_LIST));

    // ----- ファイル一覧リストコントロールの設定
    CListCtrl* const list = get_file_list_ctrl();
    ASSERT(list != NULL);
    
    list->InsertColumn(COL_FILE_NAME, _T("名前"), LVCFMT_LEFT, 130);
    list->InsertColumn(COL_ENCODING, _T("文字コード"), LVCFMT_LEFT, 130);
    list->InsertColumn(COL_PATH, _T("ファイルパス"), LVCFMT_LEFT, 200);

//    list->SetExtendedStyle(list->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    
    std::vector<FileInfo>::const_iterator it;
    for (it = files_.begin(); it != files_.end(); ++it) {
	add_file(*list, it->path, it->encoding);
    }

    // ----- 複数ファイルの区切り方法の設定
    SetDlgItemInt(IDC_LINE_COUNT, prop_.separate_line_num);
    CtrlUtil::set_radio_button(this, IDC_NEW_LINE + prop_.separate_text_type);

    return TRUE;
}

/* ==================================================================== */
//! メッセージ処理
/*!
  アクセラレータキーを有効にする					*/
/* ==================================================================== */
BOOL FileListDialog::PreTranslateMessage(MSG* pMsg)
{
    if (accel_ != NULL) {
	// アクセラレータキーを有効にするための処理
        if (::TranslateAccelerator(m_hWnd, accel_, pMsg))
            return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

/* ==================================================================== */
//! [OK]ボタンの処理
/*!
  @li ファイル一覧の作成
  @li 複数ファイルの区切り方法の変更
  @li 画面を閉じる							*/
/* ==================================================================== */
void FileListDialog::OnOK()
{
    // ----- ファイル一覧の作成
    files_.clear();

    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);

    const int size = ctrl->GetItemCount();
    for (int i = 0; i < size; i++) {
	files_.push_back(FileInfo(ctrl->GetItemText(i, COL_PATH),
				  row_data_list_[ctrl->GetItemData(i)].first));
    }
    
    // ----- 複数ファイルの区切り方法、設定変更
    prop_.separate_line_num = GetDlgItemInt(IDC_LINE_COUNT);
    prop_.separate_text_type = CtrlUtil::get_radio_button(this, IDC_NEW_LINE, IDC_NEW_PAGE) - IDC_NEW_LINE;

    CDialog::OnOK();
}

/* ==================================================================== */
//! 複数ファイルを区切る空行の行数を変更した時の処理
/*!
  「空行区切り」ラジオボタンにチェックを付ける				*/
/* ==================================================================== */
void FileListDialog::OnChangeLineCount()
{
    CtrlUtil::set_radio_button(this, IDC_NEW_LINE);

    GetDlgItem(IDC_LINE_COUNT)->SetFocus();
    // ↑これをやらないと、
    // ラジオボタンの方にフォーカスが移ってしまい、
    // 連続して入力できない。
}

/* ==================================================================== */
//! [ファイルの追加」時の処理
/*!
  ファイル選択ダイアログを表示。
  選択されたファイルを、ファイル一覧リストコントロールの末尾に追加する。*/
/* ==================================================================== */
void FileListDialog::OnAddFile()
{
    CFileDialog dialog(TRUE, NULL, NULL,
		       OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
		       _T("テキストファイル(*.txt)|*.TXT|")
		       _T("全てのファイル(*.*)|*.*||"));
    dialog.m_ofn.lpstrTitle = _T("ファイル選択 [Text2Prn]");

    if (dialog.DoModal() == IDOK) {
	CListCtrl* const ctrl = get_file_list_ctrl();
	ASSERT(ctrl != NULL);

	POSITION pos = dialog.GetStartPosition();
	while (pos != NULL)
	    add_file(*ctrl, dialog.GetNextPathName(pos), default_encoding_);
    }
}

/* ==================================================================== */
//! ファイルドロップ時の処理
/*!
  ドロップされたファイルを、ファイル一覧リストコントロールの末尾に追加する。
  ドロップされたディレクトリは無視					*/
/* ==================================================================== */
void FileListDialog::OnDropFiles(HDROP hDropInfo)
{
    TCHAR fname[_MAX_PATH];

    const int num = ::DragQueryFile(hDropInfo, -1, NULL, 0);

    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);
    
    for (int i = 0; i < num; i++) {
	::DragQueryFile(hDropInfo, i, fname, _MAX_PATH);

	if (!::PathIsDirectory(fname))
	    add_file(*ctrl, fname, default_encoding_);
    }

    ::DragFinish(hDropInfo);
}

/* ==================================================================== */
//! [ファイルの削除」時の処理
/*!
  確認ダイアログを表示。@n
  ファイル一覧リストコントロールから選択しているファイルを取り除く。

  ファイルが選択されていない場合は、何もしない。			*/
/* ==================================================================== */
void FileListDialog::OnDelFile()
{
    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);
    
    if (ctrl->GetSelectedCount() <= 0)
	return;
    
    if (MessageBox(_T("選択されているファイルをリストから削除します。"),
		   CAPTION_CONFIRM,
		   MB_ICONQUESTION | MB_OKCANCEL) != IDOK)
	return;

    POSITION pos;
    while ((pos = ctrl->GetFirstSelectedItemPosition()) != NULL) {
	const int index = ctrl->GetNextSelectedItem(pos);

	nouse_row_data_list_index_.push_back(ctrl->GetItemData(index));

	ctrl->DeleteItem(index);
    }
}

/* ==================================================================== */
//! [ファイルのクリア」時の処理
/*!
  確認ダイアログを表示。@n
  ファイル一覧リストコントロールから全てのファイルを取り除く。		*/
/* ==================================================================== */
void FileListDialog::OnClearFile()
{
    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);
    
    if (MessageBox(_T("ファイルリストをクリアします。"),
		   CAPTION_CONFIRM,
		   MB_ICONQUESTION | MB_OKCANCEL) != IDOK)
	return;

    ctrl->DeleteAllItems();

    row_data_list_.clear();
    nouse_row_data_list_index_.clear();
}

/* ==================================================================== */
//! [ファイルを上へ移動」時の処理
/*!
  ファイル一覧リストコントロールで選択状態になっているファイルの表示順を、
  一つ上へ移動する。							*/
/* ==================================================================== */
void FileListDialog::OnMoveUp()
{
    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);

    if (ctrl->GetSelectedCount() <= 0)
	return;
    
    const int size = ctrl->GetItemCount();

    int pos = -size;
    
    int index;
    for (index = 0; index < size; index++) {
	if (ctrl->GetItemState(index, LVIS_SELECTED))
	    break;
	
	row_data_list_[ctrl->GetItemData(index)].second = pos++;
    }

    int after = size;
    pos = 0;

    if (index - 1 >= 0)
	row_data_list_[ctrl->GetItemData(index - 1)].second = after++;

    for ( ;index < size; index++) {
	if (ctrl->GetItemState(index, LVIS_SELECTED))
	    row_data_list_[ctrl->GetItemData(index)].second = pos++;
	else
	    row_data_list_[ctrl->GetItemData(index)].second = after++;
    }

    ctrl->SortItems(CompareFileListForMove, reinterpret_cast<DWORD_PTR>(this));
}

/* ==================================================================== */
//! [ファイルを下へ移動」時の処理
/*!
  ファイル一覧リストコントロールで選択状態になっているファイルの表示順を、
  一つ下へ移動する。							*/
/* ==================================================================== */
void FileListDialog::OnMoveDown()
{
    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);

    if (ctrl->GetSelectedCount() <= 0)
	return;
    
    const int size = ctrl->GetItemCount();

    int pos = size;
    
    int index;
    for (index = size - 1; index >= 0; index--) {
	if (ctrl->GetItemState(index, LVIS_SELECTED))
	    break;
	row_data_list_[ctrl->GetItemData(index)].second = pos--;
    }

    int after = -size;
    pos = 0;

    if (index + 1 < size)
	row_data_list_[ctrl->GetItemData(index + 1)].second = after--;

    for ( ;index >= 0; index--) {
	if (ctrl->GetItemState(index, LVIS_SELECTED))
	    row_data_list_[ctrl->GetItemData(index)].second = pos--;
	else
	    row_data_list_[ctrl->GetItemData(index)].second = after--;
    }

    ctrl->SortItems(CompareFileListForMove, reinterpret_cast<DWORD_PTR>(this));
}

/* ==================================================================== */
//! [すべて選択」時の処理
/*!
  ファイル一覧リストコントロールの全てのファイルを選択状態にする。	*/
/* ==================================================================== */
void FileListDialog::OnSelectAll()
{
    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);

    const int size = ctrl->GetItemCount();
    for (int index = 0; index < size; index++)
	ctrl->SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
}

/* ==================================================================== */
//! [選択の切り替え」時の処理
/*!
  ファイル一覧リストコントロールで、項目の選択状態を反転する。		*/
/* ==================================================================== */
void FileListDialog::OnSelectInvert()
{
    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);

    const int size = ctrl->GetItemCount();
    for (int index = 0; index < size; index++) {
	if (ctrl->GetItemState(index, LVIS_SELECTED)) {
	    ctrl->SetItemState(index, 0, LVIS_SELECTED);
	} else {
	    ctrl->SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
	}
    }
}

/* ==================================================================== */
//! [文字コードの選択」時の処理
/*!
  ファイル一覧リストコントロールで、@n
  項目が選択されていたら、選択されているファイルの文字コードを変更。@n
  選択項目がなければ、デフォルトの文字コードを変更			*/
/* ==================================================================== */
void FileListDialog::OnChangeEncoding()
{
    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);

    POSITION pos = ctrl->GetFirstSelectedItemPosition();
    if (pos == NULL) {
	MessageBox(_T("デフォルトの文字コードを変更します。"));
	change_default_encoding();
    } else {
	const int index = ctrl->GetNextSelectedItem(pos);
	change_file_encoding(row_data_list_[ctrl->GetItemData(index)].first);
    }
}

/* ==================================================================== */
//! ファイル一覧リストコントロールの項目をダブルクリック
/*!
  選択されている項目の文字コードを変更					*/
/* ==================================================================== */
void FileListDialog::OnActivateList(NMHDR*, LRESULT* result)
{
    OnChangeEncoding();
    *result = 0;
}

/* -------------------------------------------------------------------- */
//! ファイル一覧リストコントロールの末尾にファイル情報を追加する。
/*!
  @param [in] ctrl ファイル一覧リストコントロール
  @param [in] path ファイルのパス
  @param [in] enc 文字コード						*/
/* -------------------------------------------------------------------- */
void FileListDialog::add_file(CListCtrl& ctrl, const CString& path, int enc)
{
    const int index = ctrl.InsertItem(ctrl.GetItemCount(), ::PathFindFileName(path));

    ctrl.SetItemText(index, COL_ENCODING, FileInfo::encoding_name[enc]);
    ctrl.SetItemText(index, COL_PATH, path);

    const size_t row_index = get_new_row_data_index();
    ctrl.SetItemData(index, row_index);

    row_data_list_[row_index].first = enc;
}

/* -------------------------------------------------------------------- */
//! ファイル一覧リストコントロールの行に割り当てるデータを新規作成する
/*!
  @return 行データのインデックス					*/
/* -------------------------------------------------------------------- */
size_t FileListDialog::get_new_row_data_index()
{
    size_t result;
    if (nouse_row_data_list_index_.empty()) {
	// 未使用部分がなければ、末尾に追加
	result = row_data_list_.size();
	row_data_list_.push_back(RowData());
    } else {
	// 未使用部分があれば、それを再利用
	result = nouse_row_data_list_index_.back();
	nouse_row_data_list_index_.pop_back();
    }

    return result;
}

/* -------------------------------------------------------------------- */
//! ファイルの文字コードを変更する。
/*!
  文字コード選択ダイアログを表示する。@n
  ファイル一覧リストコントロールで選択状態になったいる
  ファイルの文字コードを、ダイアログで選択したものに変更する。

  @param [in] encoding_start 初期表示の文字コード			*/
/* -------------------------------------------------------------------- */
void FileListDialog::change_file_encoding(int encoding_start)
{
    SelectEncodingDialog dialog(encoding_start);
    if (dialog.DoModal() != IDOK)
	return;

    const int encoding = dialog.get_selected_encoding();

    CListCtrl* const ctrl = get_file_list_ctrl();
    ASSERT(ctrl != NULL);
    
    POSITION pos = ctrl->GetFirstSelectedItemPosition();
    while (pos != NULL) {
	const int index = ctrl->GetNextSelectedItem(pos);
	row_data_list_[ctrl->GetItemData(index)].first = encoding;
	ctrl->SetItemText(index, COL_ENCODING, FileInfo::encoding_name[encoding]);
    }
}

/* -------------------------------------------------------------------- */
//! デフォルト文字コードを変更する。
/*!
  文字コード選択ダイアログを表示する。@n
  デフォルト文字コードを、ダイアログで選択したものに変更する。		*/
/* -------------------------------------------------------------------- */
void FileListDialog::change_default_encoding()
{
    SelectEncodingDialog dialog(default_encoding_);
    if (dialog.DoModal() != IDOK)
	return;

    default_encoding_ = dialog.get_selected_encoding();
}

/* -------------------------------------------------------------------- */
//! ファイルの表示順移動用の比較関数
/*!
  行に割り当てたデータに新しい表示順が設定されているので、それに従う。	*/
/* -------------------------------------------------------------------- */
int CALLBACK FileListDialog::CompareFileListForMove(LPARAM lParam1,
						    LPARAM lParam2,
						    LPARAM lParamSort)
{
    const FileListDialog* obj = reinterpret_cast<FileListDialog*>(lParamSort);
    const int data1 = lParam1;
    const int data2 = lParam2;
    return obj->row_data_list_[data1].second - obj->row_data_list_[data2].second;
}

