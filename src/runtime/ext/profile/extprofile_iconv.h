/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   | Copyright (c) 1997-2010 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef __EXTPROFILE_ICONV_H__
#define __EXTPROFILE_ICONV_H__

// >>>>>> Generated by idl.php. Do NOT modify. <<<<<<

#include <runtime/ext/ext_iconv.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

inline Variant x_iconv_mime_encode(CStrRef field_name, CStrRef field_value, CVarRef preferences = null_variant) {
  FUNCTION_INJECTION_BUILTIN(iconv_mime_encode);
  return f_iconv_mime_encode(field_name, field_value, preferences);
}

inline Variant x_iconv_mime_decode(CStrRef encoded_string, int mode = 0, CStrRef charset = null_string) {
  FUNCTION_INJECTION_BUILTIN(iconv_mime_decode);
  return f_iconv_mime_decode(encoded_string, mode, charset);
}

inline Variant x_iconv_mime_decode_headers(CStrRef encoded_headers, int mode = 0, CStrRef charset = null_string) {
  FUNCTION_INJECTION_BUILTIN(iconv_mime_decode_headers);
  return f_iconv_mime_decode_headers(encoded_headers, mode, charset);
}

inline Variant x_iconv_get_encoding(CStrRef type = "all") {
  FUNCTION_INJECTION_BUILTIN(iconv_get_encoding);
  return f_iconv_get_encoding(type);
}

inline bool x_iconv_set_encoding(CStrRef type, CStrRef charset) {
  FUNCTION_INJECTION_BUILTIN(iconv_set_encoding);
  return f_iconv_set_encoding(type, charset);
}

inline Variant x_iconv(CStrRef in_charset, CStrRef out_charset, CStrRef str) {
  FUNCTION_INJECTION_BUILTIN(iconv);
  return f_iconv(in_charset, out_charset, str);
}

inline Variant x_iconv_strlen(CStrRef str, CStrRef charset = null_string) {
  FUNCTION_INJECTION_BUILTIN(iconv_strlen);
  return f_iconv_strlen(str, charset);
}

inline Variant x_iconv_strpos(CStrRef haystack, CStrRef needle, int offset = 0, CStrRef charset = null_string) {
  FUNCTION_INJECTION_BUILTIN(iconv_strpos);
  return f_iconv_strpos(haystack, needle, offset, charset);
}

inline Variant x_iconv_strrpos(CStrRef haystack, CStrRef needle, CStrRef charset = null_string) {
  FUNCTION_INJECTION_BUILTIN(iconv_strrpos);
  return f_iconv_strrpos(haystack, needle, charset);
}

inline Variant x_iconv_substr(CStrRef str, int offset, int length = INT_MAX, CStrRef charset = null_string) {
  FUNCTION_INJECTION_BUILTIN(iconv_substr);
  return f_iconv_substr(str, offset, length, charset);
}

inline String x_ob_iconv_handler(CStrRef contents, int status) {
  FUNCTION_INJECTION_BUILTIN(ob_iconv_handler);
  return f_ob_iconv_handler(contents, status);
}


///////////////////////////////////////////////////////////////////////////////
}

#endif // __EXTPROFILE_ICONV_H__
