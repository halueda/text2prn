/*! @file
  @brief ファイル情報
  @author 依積晶紀
*/
#pragma once

/************************************************************************/
//! 印刷対象となるファイルの情報
/************************************************************************/
struct FileInfo
{
    
    CString path;	//!< ファイルのパス
    int encoding;	//!< 文字コード

    //! 文字コードのID
    enum {
	SJIS,		//!< Shift JIS
	UTF16,		//!< UTF-16
	UTF16BE,	//!< UTF-16 big endian
	UTF8,		//!< UTF-8
	JIS,		//!< ISO-2022 JP
	EUC,		//!< EUC JP
    };

    //!< 文字コードの名前
    static const LPCTSTR encoding_name[];

    FileInfo(LPCTSTR, int);

    static int encoding_num();
    static void set_combo_ctrl(CComboBox*, int);
};

