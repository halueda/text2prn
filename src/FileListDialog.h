/*! @file
  @brief 複数ファイル選択ダイアログ
  @author 依積晶紀
*/
#pragma once

#include	"FileInfo.h"
#include	"resource.h"

class Property;

/************************************************************************/
//! 複数の印刷対象ファイルを指定するためのダイアログ
/************************************************************************/
class FileListDialog : public CDialog
{
  private:
    /* ---------------------------------------------------------------- */
    //! ファイル一覧リストコントロールの各行に割り当てるデータ
    /*!
      pairのfirst   : 文字コードのID（FileInfo参照）@n
      pairのsecond  : 表示順を入れ替える際に利用する一時的な値。
    */
    typedef std::pair<int, int> RowData;

    //! ファイル一覧リストコントロールの列
    enum {
	COL_FILE_NAME = 0,	//!< ファイル名
	COL_ENCODING = 1,	//!< 文字コード
	COL_PATH = 2,		//!< ファイルパス
    };

  private:
    /* ---------------------------------------------------------------- */
    //! プロパティ
    /*!
      複数ファイルの区切り方法について、設定を行う。
    */
    Property& prop_;

    /* ---------------------------------------------------------------- */
    //! ファイル一覧
    std::vector<FileInfo>& files_;

    /* ---------------------------------------------------------------- */
    //! デフォルトの文字コード
    /*!
      ファイルを追加した場合、その文字コードはコレを使う。
    */
    int default_encoding_;

    /* ---------------------------------------------------------------- */
    //! ファイル一覧リストコントロールの行に割り当てられるデータ
    std::vector<RowData> row_data_list_;

    /* ---------------------------------------------------------------- */
    //! row_data_list_ の未使用部分のインデックス一覧
    std::vector<size_t> nouse_row_data_list_index_;
    
    /* ---------------------------------------------------------------- */
    //! アクセラレータキーテーブル
    HACCEL accel_;


    
  public:
    FileListDialog(std::vector<FileInfo>&, Property&, int);

  protected:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG*);
    virtual void OnOK();

    afx_msg void OnChangeLineCount();
    afx_msg void OnDropFiles(HDROP);
    afx_msg void OnAddFile();
    afx_msg void OnDelFile();
    afx_msg void OnClearFile();
    afx_msg void OnMoveUp();
    afx_msg void OnMoveDown();
    afx_msg void OnSelectAll();
    afx_msg void OnSelectInvert();
    afx_msg void OnChangeEncoding();

    afx_msg void OnActivateList(NMHDR*, LRESULT*);
    
  private:
    /* ---------------------------------------------------------------- */
    //! ファイル一覧のリストコントロールを取得する。
    /*! @return ファイル一覧のリストコントロール			*/
    /* ---------------------------------------------------------------- */
    CListCtrl* get_file_list_ctrl() const
    {
	return reinterpret_cast<CListCtrl*>(GetDlgItem(IDC_FILE_LIST));
    }

    // ファイル一覧リストコントロールへファイルの追加
    void add_file(CListCtrl&, const CString&, int);

    // 新しい行データを作成
    size_t get_new_row_data_index();

    // ファイルの文字コードを変更
    void change_file_encoding(int);
    
    // デフォルト文字コードを変更
    void change_default_encoding();

    // ファイルの移動操作用の位置比較関数
    static int CALLBACK CompareFileListForMove(LPARAM, LPARAM, LPARAM);
    
    DECLARE_MESSAGE_MAP();
};

