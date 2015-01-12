/*! @file
  @brief ファイル情報
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"FileInfo.h"

const LPCTSTR FileInfo::encoding_name[] = {
    _T("Shift JIS"),
    _T("Unicode"),
    _T("Unicode (big endian)"),
    _T("UTF-8"),
    _T("ISO-2022 JP"),
    _T("EUC JP"),
};

/* ==================================================================== */
/*!
  @param [in] _path ファイルのパス
  @param [in] _encoding 文字コード					*/
/* ==================================================================== */
FileInfo::FileInfo(LPCTSTR _path, int _encoding) :
	path(_path), encoding(_encoding)
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 対応している文字コードの個数を取得
/*!
  @return 対応している文字コードの個数					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
int FileInfo::encoding_num()
{
    return sizeof(encoding_name) / sizeof(encoding_name[0]);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コンボボックスに対応している文字コードの一覧を設定する
/*!
  @param [in] ctrl コンボボックスコントロール
  @param [in] init_code 初期状態で選択されている文字コード		*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void FileInfo::set_combo_ctrl(CComboBox* ctrl, int init_code)
{
    ASSERT(ctrl != NULL);

    ctrl->ResetContent();

    const size_t size = encoding_num();
    for (size_t i = 0; i < size; i++) {
	ctrl->AddString(encoding_name[i]);
    }

    ctrl->SetCurSel(init_code);
}
