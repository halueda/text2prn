/*! @file
  @brief メイン画面
  @author 依積晶紀
*/
#pragma once

#include	"Property.h"
#include	"FileInfo.h"
#include	"PreviewWindow.h"
#include	"TabText.h"
#include	"TabFont.h"
#include	"TabRegion.h"
#include	"TabSpace.h"
#include	"TabHeader.h"
#include	"TabOption.h"
#include	"TabPreview.h"

class TextReader;
class OutputFactory;

/************************************************************************/
//! メイン画面
/************************************************************************/
class MainDialog : public CDialog
{
  private:
    enum {
	TAB_SIZE = 7,		//!< タブの個数
    };

    //! 印刷対象の種類
    enum {
	READER_TYPE_FILE,	//!< 単一ファイル
	READER_TYPE_MULTI_FILE,	//!< 複数ファイル
	READER_TYPE_CLIPBOARD,	//!< クリップボード
    };

  private:
    //! 登録済みの設定も含めた全ての印刷設定
    PreSetPropertyList properties_;

    //! 複数ファイル印刷用のファイル一覧
    std::vector<FileInfo> files_;

    //! プレビューウィンドウ
    std::auto_ptr<PreviewWindow> preview_;

    TabPage*	tab_pages_[TAB_SIZE];	//!< タブ
    
    TabText	tab_text_;	//!< テキストタブ
    TabFont	tab_font_;	//!< フォントタブ
    TabRegion	tab_region_;	//!< 印刷範囲タブ
    TabSpace	tab_space_;	//!< 余白タブ
    TabHeader	tab_header_;	//!< ヘッダ/フッタ タブ
    TabOption	tab_option_;	//!< その他タブ
    TabPreview	tab_preview_;	//!< プレビュータブ

  public:
    MainDialog();

    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //! 初期表示に設定するファイルを設定する。
    /*!
      画面表示後の呼び出し禁止。
      @param [in] path ファイルのパス					*/
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    void add_init_file(const CString& path)
    {
	ASSERT(GetSafeHwnd() == NULL);
	files_.push_back(FileInfo(path, FileInfo::SJIS));
    }

  protected:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

    afx_msg void OnChangeTab(NMHDR*, LRESULT*);
    afx_msg void OnChangeReaderType(UINT);
    afx_msg void OnEditFilePath();

    afx_msg void OnRefFile();
    afx_msg void OnRefMultiFile();

    afx_msg void OnDropFiles(HDROP);

    afx_msg LRESULT OnApplyPreset(WPARAM, LPARAM);    
    afx_msg LRESULT OnEditPreset(WPARAM, LPARAM);    
    afx_msg LRESULT OnPreview(WPARAM, LPARAM);    
    afx_msg LRESULT OnPageSetup(WPARAM, LPARAM);    
    afx_msg LRESULT OnChangeUseHeader(WPARAM, LPARAM);    
    
  private:
    void init_tab_ctrl();
    void init_encoding_ctrl();
    void init_reader_type_ctrl();
    
    TextReader* get_reader();
    void print(OutputFactory&, bool);

    void change_tab_page(int);
    void change_reader_type(int);

    void update_property();
    
    DECLARE_MESSAGE_MAP();
};
