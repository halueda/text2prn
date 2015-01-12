/*! @file
  @brief 文字コード
  @author 依積晶紀
*/
#include	"stdafx.h"
#include	"Encoding.h"
#include	"Throwable.h"
#include	"CharType.h"
#include	"AutoBuff.h"
#include	"JisStream.h"

namespace
{

/* -------------------------------------------------------------------- */
//! 2バイト文字を、JISコードからShift-JISコードに変換する
/*!
  @param [in,out] lead 1バイト目
  @param [in,out] trail 2バイト目					*/
/* -------------------------------------------------------------------- */
void convert_jis_to_sjis(unsigned char& lead, unsigned char& trail)
{
    if (lead & 0x01) {
        if (trail <= 0x5f)
            trail += 0x1f;
        else
            trail += 0x20;
    } else {
        trail += 0x7e;
    }

    lead = ((lead - 0x21) >> 1) + 0x81;
    if (lead >= 0xa0 && lead <= 0xdf)
        lead += 0x40;
}

}

/* ==================================================================== */
//! BOMが存在しない場合のエンディアンを指定して構築する。
/*!
  @param [in] little_endian true=Little Endian / false = Big Endian	*/
/* ==================================================================== */
EncodingUTF16::EncodingUTF16(bool little_endian) :
	priority_little_endian_(little_endian)
{
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! UTF-16 LE に変換する。
/*!
  先頭の１文字が BOM なら BOM に従い、
  違うのならコンストラクタで指定したエンディアンに従い、
  エンディアンをLittle Endian に変換して返す。

  変換元の文字列は NUL ターミネートされていない。\n
  NUL ターミネートしてから返す。

  size が奇数の場合、変換元がUTF-16ではないとみなして、エラーとする。

  @param [in,out] buff 変換対象の文字列
  @param [in] size 変換元の文字列のサイズ（バイト数）
  @exception Error 変換元の文字コードがUTF-16ではない
  @attention buff には size + 2 byte 分確保されていること		*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void EncodingUTF16::convert(AutoBuff& buff, int size) const
{
    if (size % 2 != 0) {
	throw Error(_T("入力データがUTF-16ではない。"));
    }
    
    const int len = size/2;	// 文字数
    wchar_t* str = static_cast<wchar_t*>(buff.get());

    str[len] = CHAR_NUL;

    if (str[0] == CHAR_BOM) {
	// 何もしない
    } if (str[0] == CHAR_BOM_BE) {
	convert_endian(str, len);
    } else {
	if (!priority_little_endian_)
	    convert_endian(str, len);
    }
}

/* -------------------------------------------------------------------- */
//! エンディアンを変換する
/*!
  @param [in] str 変換対象の文字列
  @param [in] len 変換する文字列の文字数				*/
/* -------------------------------------------------------------------- */
void EncodingUTF16::convert_endian(wchar_t* str, int len) const
{
    char* buff = reinterpret_cast<char*>(str);
    for (int i = 0; i < len; i++) {
	std::swap(buff[i*2], buff[i*2 + 1]);
    }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! UTF-16 LE に変換する。
/*!
  size が -1 以外なら、変換元の文字列は NUL ターミネートされていない。\n
  （size == -1 は NUL ターミネート されている）\n
  NUL ターミネートしてから返す。

  変換に失敗した場合は、エラーとする。

  @param [in,out] buff 変換対象の文字列
  @param [in] size 変換元の文字列のサイズ（バイト数）
  @exception Error 文字コードの変換に失敗
  @exception std::bad_alloc 変換後の文字列用の領域確保失敗		*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void EncodingMultiByte::convert(AutoBuff& buff, int size) const
{
    char* const str_src = static_cast<char*>(buff.get());
    
    const int size_dest = ::MultiByteToWideChar(get_code_page(),
						0,
						str_src,
						size,
						NULL,
						0);
    if (size_dest == 0)
	throw Error();

    AutoBuff buff_dest((size_dest + 1)*sizeof(wchar_t));
    wchar_t* const str_dest = static_cast<wchar_t*>(buff_dest.get());
    
    ::MultiByteToWideChar(get_code_page(),
			  0,
			  str_src,
			  size,
			  str_dest,
			  size_dest);
    str_dest[size_dest] = CHAR_NUL;

    buff.swap(buff_dest);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! EUC-JPの文字列をUTF-16 LE に変換する。
/*!
  変換元の文字列は NUL ターミネートされていない。\n
  NUL ターミネートしてから返す。

  変換に失敗した場合は、エラーとする。

  一旦 Shift-JISに変換してから UTF-16に変換する。

  @param [in,out] buff 変換対象の文字列
  @param [in] size 変換元の文字列のサイズ（バイト数）
  @exception Error 文字コードの変換に失敗
  @exception std::bad_alloc 変換後の文字列用の領域確保失敗
  @attention buff には size + 1 byte 分確保されていること		*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void EncodingEUC::convert(AutoBuff& buff, int size) const
{
    unsigned char* src = static_cast<unsigned char*>(buff.get());
    unsigned char* dest = src;
    src[size] = '\0';

    unsigned char c;
    while ((c = *(src++)) != '\0') {
	if (c & 0x80) {
	    unsigned char c2 = *(src++);
	    if (c2 == '\0') {
		// 2byte目が無ければ、1byte目だけをそのまま出力
		*(dest++) = c;
		break;
	    }

	    if (c == 0x8e) {	// '\x8e' ではダメ
		// 半角カナ
		*(dest++) = c2;
	    } else {
		// 漢字
		c &= ~0x80;
		c2 &= ~0x80;
		convert_jis_to_sjis(c, c2);

		*(dest++) = c;
		*(dest++) = c2;
	    }
	} else {
	    // ASCII
	    *(dest++) = c;
	}
    }

    *dest = '\0';

    EncodingSJIS::convert(buff, -1);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
//! ISO-2022-JPの文字列をUTF-16 LE に変換する。
/*!
  変換元の文字列は NUL ターミネートされていない。\n
  NUL ターミネートしてから返す。

  変換に失敗した場合は、エラーとする。

  一旦 Shift-JISに変換してから UTF-16に変換する。

  @param [in,out] buff 変換対象の文字列
  @param [in] size 変換元の文字列のサイズ（バイト数）
  @exception Error 文字コードの変換に失敗
  @exception std::bad_alloc 変換後の文字列用の領域確保失敗
  @attention buff には size + 1 byte 分確保されていること		*/
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void EncodingJIS::convert(AutoBuff& buff, int size) const
{
    unsigned char* dest = static_cast<unsigned char*>(buff.get());
    dest[size] = '\0';

    JisStream src(dest);
    while (src.next()) {
	switch (src.get_category()) {

	  case JisStream::ASCII:
	    *(dest++) = src.get_char();
	    break;

	  case JisStream::KANJI:
	  {
	      unsigned char lead = src.get_char();
	      unsigned char trail= src.get_trail_char();
	      convert_jis_to_sjis(lead, trail);
	      *(dest++) = lead;
	      *(dest++) = trail;
	  }
	    break;

	  case JisStream::KANA:
	    *(dest++) = src.get_char() + 0x80;
	    break;
	}
    }

    *dest = '\0';
    
    EncodingSJIS::convert(buff, -1);
}


