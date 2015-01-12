/*! @file
  @brief フォントタブページ
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"TabFont.h"
#include	"Property.h"
#include	"CtrlUtil.h"

BEGIN_MESSAGE_MAP(TabFont, TabPage)
    ON_CBN_SELCHANGE(IDC_FONT_TYPE, OnChangeFontType)
    ON_COMMAND(IDC_VERTICAL, OnChangeTextDirection)
    ON_COMMAND(IDC_PROPORTIONAL, OnChangeFontWidth)
END_MESSAGE_MAP()

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! タブの初期化
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabFont::init()
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! 設定値をコントロールに反映する
/*!
  @param property 設定							*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabFont::update_ctrl(const Property& property)
{
    CtrlUtil::set_check_box(this, IDC_VERTICAL, property.vertical);
    CtrlUtil::set_check_box(this, IDC_PROPORTIONAL, property.proportional_font);
    set_font_ctrl(property.vertical, property.proportional_font,
		  property.font_type, property.font_name);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! コントロールの内容を設定値に反映する
/*!
  @param property [out]設定						*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void TabFont::update_value(Property& property) const
{
    property.vertical          = CtrlUtil::get_check_box(this, IDC_VERTICAL);
    property.proportional_font = CtrlUtil::get_check_box(this, IDC_PROPORTIONAL);
    
    get_font_type_ctrl()->GetWindowText(property.font_type);

    const CComboBox* const combo_name = get_font_name_ctrl();
    combo_name->GetWindowText(property.font_name);

    const int index = combo_name->GetCurSel();
    property.lf = const_cast<LOGFONT*>(&lf_pool[combo_name->GetItemData(index)]);
}

/* ==================================================================== */
//! 言語コンボボックスを変更した
/* ==================================================================== */
void TabFont::OnChangeFontType()
{
    CString type;	// 言語
    get_font_type_ctrl()->GetWindowText(type);

    std::pair<FontInfoList::iterator, FontInfoList::iterator> p = font_info.equal_range((LPCTSTR)type);

    CComboBox* const combo_name = get_font_name_ctrl();
    combo_name->ResetContent();

    for ( ;p.first != p.second; ++(p.first)) {
	const int index = combo_name->AddString(p.first->second.first);

	const int x = p.first->second.second;
	ASSERT(x >= 0 && x < static_cast<int>(lf_pool.size()));

	combo_name->SetItemData(index, x);
    }

    combo_name->SetCurSel(0);
}

/* ==================================================================== */
//! 縦書きチェックボックスを変更した
/* ==================================================================== */
void TabFont::OnChangeTextDirection()
{
    const bool vertical = CtrlUtil::get_check_box(this, IDC_VERTICAL);
    const bool proportional = CtrlUtil::get_check_box(this, IDC_PROPORTIONAL);

    CString font_type;
    get_font_type_ctrl()->GetWindowText(font_type);

    CString font_name;
    get_font_name_ctrl()->GetWindowText(font_name);

    // フォント名が @ で始まるかどうかで、縦書きと横書きのフォント名を変換する。
    // （「フォント名」コンボボックスの選択位置を再設定する）
    if (vertical) {
	if (font_name[0] != _T('@'))
	    font_name = _T('@') + font_name;
    } else {
	if (font_name[0] == _T('@'))
	    font_name = font_name.Mid(1);
    }

    set_font_ctrl(vertical, proportional, font_type, font_name);
}

/* ==================================================================== */
//! プロポーショナルフォントチェックボックスを変更した
/* ==================================================================== */
void TabFont::OnChangeFontWidth()
{
    const bool vertical = CtrlUtil::get_check_box(this, IDC_VERTICAL);
    const bool proportional = CtrlUtil::get_check_box(this, IDC_PROPORTIONAL);

    CString font_type;
    get_font_type_ctrl()->GetWindowText(font_type);

    CString font_name;
    get_font_name_ctrl()->GetWindowText(font_name);

    // 等幅とプロポーショナルのフォント名の変換にルールはないので、
    // 変換できない。

    set_font_ctrl(vertical, proportional, font_type, font_name);
}

/* -------------------------------------------------------------------- */
//! フォント情報をコントロールに設定する
/*!
  @param vertical 縦書きかどうか
  @param proportional プロポーショナルフォントかどうか
  @param font_type 言語
  @param font_name フォント名						*/
/* -------------------------------------------------------------------- */
void TabFont::set_font_ctrl(bool vertical, bool proportional,
			    LPCTSTR font_type, LPCTSTR font_name)
{
    enum_font(vertical, proportional);

    // 言語の選択
    {
	CComboBox* const combo_type = get_font_type_ctrl();
	const int index = combo_type->SelectString(-1, font_type);
	if (index == CB_ERR)
	    combo_type->SetCurSel(0);
	OnChangeFontType();
    }

    // フォントの選択
    {
	CComboBox* const combo_name = get_font_name_ctrl();
	const int index = combo_name->SelectString(-1, font_name);
	if (index == CB_ERR)
	    combo_name->SetCurSel(0);
    }
}

/* -------------------------------------------------------------------- */
//! フォントの一覧を取得
/*!
  @param vertical     true = 縦書きフォント   / false = 横書きフォント
  @param proportional true = プロポーショナル / false = 等幅		*/
/* -------------------------------------------------------------------- */
void TabFont::enum_font(bool vertical, bool proportional)
{
    lf_pool.clear();
    font_info.clear();
    
    // フォントの列挙
    CClientDC dc(this);

    LOGFONT lf;
    ::ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfCharSet = DEFAULT_CHARSET;

    EnumFontOption option;
    option.target = this;
    option.vertical = vertical;
    option.proportional = proportional;

    ::EnumFontFamiliesEx(dc.GetSafeHdc(), &lf,
			 (FONTENUMPROC)(enum_font_callback),
			 reinterpret_cast<LPARAM>(&option),
			 0);
}

/* ==================================================================== */
//! フォント名を列挙するコールバック
/* ==================================================================== */
int CALLBACK TabFont::enum_font_callback(ENUMLOGFONTEX* lpelfe,
					 NEWTEXTMETRICEX* lpntme,
					 int FontType,
					 LPARAM lParam)
{
    const EnumFontOption* const option = reinterpret_cast<EnumFontOption*>(lParam);
    ASSERT(option != NULL);
    
    // truetypeのみ
    if ((FontType & TRUETYPE_FONTTYPE) == 0)
	return TRUE;

    // シンボルフォント除外
    if (lpelfe->elfLogFont.lfCharSet == SYMBOL_CHARSET)
	return TRUE;
    
    // 等幅フォントかどうか
    if (((lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH) == 0) != option->proportional)
	return TRUE;


    // 縦書き用フォント(名前が@で始まる)かどうか
    if ((lpelfe->elfFullName[0] == _T('@')) != option->vertical)
	return TRUE;

    ASSERT(option->target != NULL);
    option->target->set_font_info((LPCTSTR)lpelfe->elfScript,
				  (LPCTSTR)lpelfe->elfFullName,
				  lpelfe->elfLogFont);
    return TRUE;
}

/* -------------------------------------------------------------------- */
//! フォント情報をセットする
/*!
  @param type 言語
  @param name フォント名
  @param lf フォント情報						*/
/* -------------------------------------------------------------------- */
void TabFont::set_font_info(LPCTSTR type, LPCTSTR name, LOGFONT& lf)
{
    lf_pool.push_back(lf);

    // 初回登場時のみ、言語名をコンボボックスに追加
    if (font_info.count(type) == 0)
	get_font_type_ctrl()->AddString(type);

    font_info.insert(FontInfoList::value_type(type, FontInfo(name, lf_pool.size() - 1)));
    
}
