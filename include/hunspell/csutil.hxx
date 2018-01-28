#ifndef __CSUTILHXX__
#define __CSUTILHXX__

#include "hunvisapi.h"

// First some base level utility routines

#include <string>
#include <vector>
#include <string.h>
#include "w_char.hxx"
#include "htypes.hxx"

#ifdef MOZILLA_CLIENT
#include "nscore.h" // for mozalloc headers
#endif

// casing
#define NOCAP   0
#define INITCAP 1
#define ALLCAP  2
#define HUHCAP  3
#define HUHINITCAP  4

// default encoding and keystring
#define SPELL_ENCODING  "ISO8859-1"
#define SPELL_KEYSTRING "qwertyuiop|asdfghjkl|zxcvbnm" 

// default morphological fields
#define MORPH_STEM        "st:"
#define MORPH_ALLOMORPH   "al:"
#define MORPH_POS         "po:"
#define MORPH_DERI_PFX    "dp:"
#define MORPH_INFL_PFX    "ip:"
#define MORPH_TERM_PFX    "tp:"
#define MORPH_DERI_SFX    "ds:"
#define MORPH_INFL_SFX    "is:"
#define MORPH_TERM_SFX    "ts:"
#define MORPH_SURF_PFX    "sp:"
#define MORPH_FREQ        "fr:"
#define MORPH_PHON        "ph:"
#define MORPH_HYPH        "hy:"
#define MORPH_PART        "pa:"
#define MORPH_FLAG        "fl:"
#define MORPH_HENTRY      "_H:"
#define MORPH_TAG_LEN     strlen(MORPH_STEM)

#define MSEP_FLD ' '
#define MSEP_REC '\n'
#define MSEP_ALT '\v'

// default flags
#define DEFAULTFLAGS   65510
#define FORBIDDENWORD  65510
#define ONLYUPCASEFLAG 65511

// fopen or optional _wfopen to fix long pathname problem of WIN32
FILE* LIBHUNSPELL_DLL_EXPORTED myfopen(const char * path, const char * mode);

// convert UTF-16 characters to UTF-8
char* LIBHUNSPELL_DLL_EXPORTED u16_u8(char * dest, int size, const w_char * src, int srclen);
// convert UTF-16 characters to UTF-8
std::string& LIBHUNSPELL_DLL_EXPORTED u16_u8(std::string& dest, const std::vector<w_char>& src);

// convert UTF-8 characters to UTF-16
int LIBHUNSPELL_DLL_EXPORTED u8_u16(w_char * dest, int size, const char * src);
// convert UTF-8 characters to UTF-16
int LIBHUNSPELL_DLL_EXPORTED u8_u16(std::vector<w_char>& dest, const std::string& src);

// sort 2-byte vector
void LIBHUNSPELL_DLL_EXPORTED flag_qsort(unsigned short flags[], int begin, int end);

// binary search in 2-byte vector
int LIBHUNSPELL_DLL_EXPORTED flag_bsearch(unsigned short flags[], unsigned short flag, int right);

// remove end of line char(s)
void LIBHUNSPELL_DLL_EXPORTED mychomp(char * s);

// duplicate string
char* LIBHUNSPELL_DLL_EXPORTED mystrdup(const char * s);

// strcat for limited length destination string
char* LIBHUNSPELL_DLL_EXPORTED mystrcat(char * dest, const char * st, int max);

// duplicate reverse of string
char* LIBHUNSPELL_DLL_EXPORTED myrevstrdup(const char * s);

// parse into tokens with char delimiter
char* LIBHUNSPELL_DLL_EXPORTED mystrsep(char ** sptr, const char delim);
// parse into tokens with char delimiter
char* LIBHUNSPELL_DLL_EXPORTED mystrsep2(char ** sptr, const char delim);

// replace pat by rep in word and return word
char* LIBHUNSPELL_DLL_EXPORTED mystrrep(char * word, const char *pat, const char *rep);
std::string& LIBHUNSPELL_DLL_EXPORTED mystrrep(std::string& str, const std::string& search, const std::string& replace);

// append s to ends of every lines in text
void LIBHUNSPELL_DLL_EXPORTED strlinecat(char * lines, const char * s);

// append s to ends of every lines in text
std::string& LIBHUNSPELL_DLL_EXPORTED strlinecat(std::string& str, const std::string& apd);

// tokenize into lines with new line
int LIBHUNSPELL_DLL_EXPORTED line_tok(const char * text, char *** lines, char breakchar);

// tokenize into lines with new line and uniq in place
char* LIBHUNSPELL_DLL_EXPORTED line_uniq(char * text, char breakchar);
char* LIBHUNSPELL_DLL_EXPORTED line_uniq_app(char ** text, char breakchar);

// change oldchar to newchar in place
char* LIBHUNSPELL_DLL_EXPORTED tr(char * text, char oldc, char newc);

// reverse word
int LIBHUNSPELL_DLL_EXPORTED reverseword(char *);
// reverse word
std::string& LIBHUNSPELL_DLL_EXPORTED reverseword(std::string& word);

// reverse word
int LIBHUNSPELL_DLL_EXPORTED reverseword_utf(char *);
// reverse word
std::string& LIBHUNSPELL_DLL_EXPORTED reverseword_utf(std::string&);

// remove duplicates
int LIBHUNSPELL_DLL_EXPORTED uniqlist(char ** list, int n);

// free character array list
void LIBHUNSPELL_DLL_EXPORTED freelist(char *** list, int n);

// character encoding information
struct cs_info {
  unsigned char ccase;
  unsigned char clower;
  unsigned char cupper;
};

int LIBHUNSPELL_DLL_EXPORTED initialize_utf_tbl();
void LIBHUNSPELL_DLL_EXPORTED free_utf_tbl();
unsigned short LIBHUNSPELL_DLL_EXPORTED unicodetoupper(unsigned short c, int langnum);
unsigned short LIBHUNSPELL_DLL_EXPORTED unicodetolower(unsigned short c, int langnum);
int LIBHUNSPELL_DLL_EXPORTED unicodeisalpha(unsigned short c);

struct cs_info* LIBHUNSPELL_DLL_EXPORTED get_current_cs(const char * es);

// get language identifiers of language codes
int LIBHUNSPELL_DLL_EXPORTED get_lang_num(const char * lang);

// get characters of the given 8bit encoding with lower- and uppercase forms
char* LIBHUNSPELL_DLL_EXPORTED get_casechars(const char * enc);

// convert null terminated string to all caps using encoding
void LIBHUNSPELL_DLL_EXPORTED enmkallcap(char * d, const char * p, const char * encoding);

// convert null terminated string to all little using encoding
void LIBHUNSPELL_DLL_EXPORTED enmkallsmall(char * d, const char * p, const char * encoding);

// convert null terminated string to have initial capital using encoding
void LIBHUNSPELL_DLL_EXPORTED enmkinitcap(char * d, const char * p, const char * encoding);

// convert null terminated string to all caps
void LIBHUNSPELL_DLL_EXPORTED mkallcap(char * p, const struct cs_info * csconv);
// convert std::string to all caps
std::string& LIBHUNSPELL_DLL_EXPORTED mkallcap(std::string &s, const struct cs_info * csconv);

// convert null terminated string to all little
void LIBHUNSPELL_DLL_EXPORTED mkallsmall(char * p, const struct cs_info * csconv);
// convert null terminated string to all little
std::string& LIBHUNSPELL_DLL_EXPORTED mkallsmall(std::string &s, const struct cs_info * csconv);

// convert null terminated string to have initial capital
void LIBHUNSPELL_DLL_EXPORTED mkinitcap(char * p, const struct cs_info * csconv);

// convert first nc characters of UTF-8 string to little
void LIBHUNSPELL_DLL_EXPORTED mkallsmall_utf(w_char * u, int nc, int langnum);
// convert first nc characters of UTF-8 string to little
std::vector<w_char>& LIBHUNSPELL_DLL_EXPORTED mkallsmall_utf(std::vector<w_char>& u, int nc, int langnum);

// convert first nc characters of UTF-8 string to capital
void LIBHUNSPELL_DLL_EXPORTED mkallcap_utf(w_char * u, int nc, int langnum);
// convert first nc characters of UTF-8 string to capital
std::vector<w_char>& LIBHUNSPELL_DLL_EXPORTED mkallcap_utf(std::vector<w_char>& u, int nc, int langnum);

// get type of capitalization
int LIBHUNSPELL_DLL_EXPORTED get_captype(char * q, int nl, cs_info *);

// get type of capitalization (UTF-8)
int LIBHUNSPELL_DLL_EXPORTED  get_captype_utf8(w_char * q, int nl, int langnum);

// strip all ignored characters in the string
void LIBHUNSPELL_DLL_EXPORTED remove_ignored_chars_utf(char * word, unsigned short ignored_chars[], int ignored_len);
// strip all ignored characters in the string
std::string& LIBHUNSPELL_DLL_EXPORTED remove_ignored_chars_utf(std::string& word, unsigned short ignored_chars[], int ignored_len);

// strip all ignored characters in the string
void LIBHUNSPELL_DLL_EXPORTED remove_ignored_chars(char * word, char * ignored_chars);
// strip all ignored characters in the string
std::string& LIBHUNSPELL_DLL_EXPORTED remove_ignored_chars(std::string& word, const std::string& ignored_chars);

int LIBHUNSPELL_DLL_EXPORTED parse_string(char * line, char ** out, int ln);

int LIBHUNSPELL_DLL_EXPORTED parse_array(char * line, char ** out, unsigned short ** out_utf16,
    int * out_utf16_len, int utf8, int ln);

int LIBHUNSPELL_DLL_EXPORTED fieldlen(const char * r);

char* LIBHUNSPELL_DLL_EXPORTED copy_field(char * dest, const char * morph, const char * var);
bool LIBHUNSPELL_DLL_EXPORTED copy_field(std::string& dest, const std::string& morph, const std::string& var);

int LIBHUNSPELL_DLL_EXPORTED morphcmp(const char * s, const char * t);

int LIBHUNSPELL_DLL_EXPORTED get_sfxcount(const char * morph);

// conversion function for protected memory
void LIBHUNSPELL_DLL_EXPORTED store_pointer(char * dest, char * source);

// conversion function for protected memory
char* LIBHUNSPELL_DLL_EXPORTED get_stored_pointer(const char * s);

// hash entry macros
inline char* LIBHUNSPELL_DLL_EXPORTED HENTRY_DATA(struct hentry *h)
{
    char *ret;
    if (!h->var)
        ret = NULL;
    else if (h->var & H_OPT_ALIASM)
        ret = get_stored_pointer(HENTRY_WORD(h) + h->blen + 1);
    else
        ret = HENTRY_WORD(h) + h->blen + 1;
    return ret;
}

inline const char* LIBHUNSPELL_DLL_EXPORTED HENTRY_DATA(const struct hentry *h)
{
    const char *ret;
    if (!h->var)
        ret = NULL;
    else if (h->var & H_OPT_ALIASM)
        ret = get_stored_pointer(HENTRY_WORD(h) + h->blen + 1);
    else
        ret = HENTRY_WORD(h) + h->blen + 1;
    return ret;
}

// NULL-free version for warning-free OOo build
inline const char* LIBHUNSPELL_DLL_EXPORTED HENTRY_DATA2(const struct hentry *h)
{
    const char *ret;
    if (!h->var)
        ret = "";
    else if (h->var & H_OPT_ALIASM)
        ret = get_stored_pointer(HENTRY_WORD(h) + h->blen + 1);
    else
        ret = HENTRY_WORD(h) + h->blen + 1;
    return ret;
}

inline char* LIBHUNSPELL_DLL_EXPORTED HENTRY_FIND(struct hentry *h, const char *p)
{
    return (HENTRY_DATA(h) ? strstr(HENTRY_DATA(h), p) : NULL);
}

#define w_char_eq(a,b) (((a).l == (b).l) && ((a).h == (b).h))

#endif
