/*! @file
  @brief 設定値関連
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"PropertyUtil.h"
#include	"Attribute.h"
#include	"Util.h"
#include	"text2prn.h"

namespace
{

//! 出力文字数の説明を作成
CString make_type_description(const Property& p)
{
    switch (p.font_base_type) {
      case Property::FONT_BASE_CHAR_P_LINE:
	return FString(_T("%d 文字/行"), p.char_p_line);

      case Property::FONT_BASE_LINE_P_COL:
	return FString(_T("%d 行/段"), p.line_p_column);

      case Property::FONT_BASE_FONT_SIZE:
	return FString(_T("%d ポイント"), p.font_size);

      default:
	NODEFAULT;
	return _T("");
    }
}
    
//! 出力フォントの説明を作成
CString make_font_description(const Property& p)
{
    return FString(_T("%s (%s)"), p.font_name, p.font_type);
}
    
//! 出力範囲の説明を作成
CString make_region_description(const Property& p)
{
    CString buff;

    if (p.begin_region == 0)
	buff = _T("初めから、");
    else
	buff.Format(_T("%d 行目から、"), p.begin_region);

    switch (p.end_region_type) {
      case Property::END_REGION_LAST:
	buff += _T("終わりまで");
	break;
      case Property::END_REGION_LINE_COUNT:
	buff += FString(_T("%d 行分"), p.end_region);
	break;
      case Property::END_REGION_LINE_POS:
	buff += FString(_T("%d 行目まで"), p.end_region);
	break;
      case Property::END_REGION_PAGE:
	buff += FString(_T("%d ページ分"), p.end_region);
	break;

      default:
	NODEFAULT;
    }

    return buff;
}

//! 余白の説明を作成
CString make_space_description(const Property& p)
{
    if (p.use_space) {
	return FString(_T("%d, %d, %d, %d"),
		       p.space_top, p.space_bottom,
		       p.space_left, p.space_right);
    } else {
	return _T("なし");
    }
}

//! 行間の説明を作成
CString make_line_space_description(const Property& p)
{
    static const LPCTSTR mes[] = {
	_T("なし"), _T("1/4行分"), _T("1/2行分"), _T("3/4行分"), _T("1行分"),
    };
    return mes[p.line_space];
}

//! 行番号出力の説明を作成
CString make_line_no_description(const Property& p)
{
    switch (p.output_line_count) {
      case 0:
	return _T("なし");
      case 1:
	return FString(_T("%d 桁"), p.line_count_digit);
      default:
	return FString(_T("%d 行毎、%d 桁"), p.output_line_count, p.line_count_digit);
    }
}

//! あり/なしの説明を作成
CString make_onoff_description(bool state)
{
    return state ? CString(_T("あり")) : CString(_T("なし"));
}

}


namespace PropertyUtil
{

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コンボボックスに登録済み設定の名前を設定する。
/*!
  @param [in] combo コンボボックス
  @param [in] property 登録済み設定					*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void update_preset(CComboBox* const combo,
		   const PreSetPropertyList::PropertyList& property)
{
    combo->ResetContent();

    PreSetPropertyList::PropertyList::const_iterator it;
    for (it = property.begin(); it != property.end(); ++it) {
	combo->AddString(it->first);
    }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定の説明を取得する。
/*!
  登録済み設定編集画面用
  @param [in] p 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
Description get_description(const Property& p)
{
    Description desc;

    desc.push_back(std::make_pair(CString(p.vertical ? _T("縦書き") : _T("横書き")), CString()));
    desc.push_back(std::make_pair(CString(_T("テキスト")), make_type_description(p)));
    desc.push_back(std::make_pair(CString(_T("段組")), FString(_T("%d 段/ページ"), p.column)));
    desc.push_back(std::make_pair(CString(_T("タブ")), FString(_T("%d 文字毎"), p.tab_char)));
    desc.push_back(std::make_pair(CString(_T("折り返し")),
				  make_onoff_description(p.wrap_right)));

    desc.push_back(std::make_pair(CString(_T("フォント")), make_font_description(p)));
    
    desc.push_back(std::make_pair(CString(_T("印刷範囲")), make_region_description(p)));

    desc.push_back(std::make_pair(CString(_T("余白")), make_space_description(p)));
    desc.push_back(std::make_pair(CString(_T("行間")), make_line_space_description(p)));
    desc.push_back(std::make_pair(CString(_T("段落間")), FString(_T("%d mm"), p.column_space)));
    
    desc.push_back(std::make_pair(CString(_T("情報表示")), make_onoff_description(p.view_info)));
    desc.push_back(std::make_pair(CString(_T("行番号")), make_line_no_description(p)));
    desc.push_back(std::make_pair(CString(_T("区切り線")),
				  make_onoff_description(p.output_column_line)));
    if (p.use_detail_header) {
	desc.push_back(std::make_pair(CString(_T("ヘッダ(左)")),
				      p.header_l));
	desc.push_back(std::make_pair(CString(_T("ヘッダ(中)")),
				      p.header_c));
	desc.push_back(std::make_pair(CString(_T("ヘッダ(右)")),
				      p.header_r));
	desc.push_back(std::make_pair(CString(_T("フッタ(左)")),
				      p.footer_l));
	desc.push_back(std::make_pair(CString(_T("フッタ(中)")),
				      p.footer_c));
	desc.push_back(std::make_pair(CString(_T("フッタ(右)")),
				      p.footer_r));
    } else {
	desc.push_back(std::make_pair(CString(_T("ページ番号")),
				      make_onoff_description(p.output_page_count)));
	desc.push_back(std::make_pair(CString(_T("ファイル名")),
				      make_onoff_description(p.output_title)));
    }
    
    return desc;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定の説明を取得する。
/*!
  印刷情報表示用
  @param [in] p 設定
  @param [in] info 印刷情報
  @param [in] first_line テキストの最初の1行				*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
Description get_description(const Property& p, const Attribute& info,
			    LPCTSTR first_line)
{
    Description desc;

    desc.push_back(std::make_pair(CString(p.vertical ? _T("縦書き") : _T("横書き")), CString()));

    desc.push_back(std::make_pair(CString(_T("1行目の内容")), CString(first_line)));

    desc.push_back(std::make_pair(CString(_T("フォント")),
				  p.font_base_type == Property::FONT_BASE_FONT_SIZE ?
				  FString(_T("%d ポイント"), p.font_size) :
				  FString(_T("%.1f ポイント相当"), info.font_size)));
    desc.push_back(std::make_pair(CString(_T("文字数")), FString(_T("%d 文字/行"), info.printable_char)));
    desc.push_back(std::make_pair(CString(_T("行数")), FString(_T("%d 行/段"), info.printable_line)));
    desc.push_back(std::make_pair(CString(_T("段組数")), FString(_T("%d 段/ページ"), p.column)));
    desc.push_back(std::make_pair(CString(_T("タブ幅")), FString(_T("%d 文字毎"), p.tab_char)));
    desc.push_back(std::make_pair(CString(_T("折り返し")),
				  make_onoff_description(p.wrap_right)));

    desc.push_back(std::make_pair(CString(_T("フォント")), make_font_description(p)));

    desc.push_back(std::make_pair(CString(_T("印刷範囲")), make_region_description(p)));

    desc.push_back(std::make_pair(CString(_T("余白")), make_space_description(p)));
    desc.push_back(std::make_pair(CString(_T("行間")), make_line_space_description(p)));
    desc.push_back(std::make_pair(CString(_T("段落間")), FString(_T("%d mm"), p.column_space)));

    desc.push_back(std::make_pair(CString(_T("行番号")), make_line_no_description(p)));
    desc.push_back(std::make_pair(CString(_T("区切り線")),
				  make_onoff_description(p.output_column_line)));
    if (p.use_detail_header) {
	desc.push_back(std::make_pair(CString(_T("ヘッダ(左)")),
				      p.header_l));
	desc.push_back(std::make_pair(CString(_T("ヘッダ(中)")),
				      p.header_c));
	desc.push_back(std::make_pair(CString(_T("ヘッダ(右)")),
				      p.header_r));
	desc.push_back(std::make_pair(CString(_T("フッタ(左)")),
				      p.footer_l));
	desc.push_back(std::make_pair(CString(_T("フッタ(中)")),
				      p.footer_c));
	desc.push_back(std::make_pair(CString(_T("フッタ(右)")),
				      p.footer_r));
    } else {
	desc.push_back(std::make_pair(CString(_T("ページ番号")),
				      make_onoff_description(p.output_page_count)));
	desc.push_back(std::make_pair(CString(_T("ファイル名")),
				      make_onoff_description(p.output_title)));
    }
    return desc;
}

/* -------------------------------------------------------------------- */
//! ページ設定ダイアログを設定値で初期化する。
/*!
  @param [in] dialog ページ設定ダイアログ
  @param [in] p 設定値							*/
/* -------------------------------------------------------------------- */
void init_page_setup_dialog(CPageSetupDialog& dialog, const Property& p)
{
    dialog.m_psd.Flags &= ~PSD_INTHOUSANDTHSOFINCHES;
    dialog.m_psd.Flags |= PSD_INHUNDREDTHSOFMILLIMETERS;

    dialog.m_psd.hDevNames = CMyApp::get_instance()->get_printer_name();
    dialog.m_psd.hDevMode = CMyApp::get_instance()->get_printer_mode();

    dialog.m_psd.rtMinMargin = CRect(0, 0, 0, 0);
    
    if (p.use_space) {
	dialog.m_psd.rtMargin = CRect(p.space_left   * 100,
				      p.space_top    * 100,
				      p.space_right  * 100,
				      p.space_bottom * 100);
    } else {
	dialog.m_psd.rtMargin = CRect(0, 0, 0, 0);
	dialog.m_psd.Flags |= PSD_DISABLEMARGINS;
    }
}

}
