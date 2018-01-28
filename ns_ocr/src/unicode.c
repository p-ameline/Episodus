/*
This is a Optical-Character-Recognition program
Copyright (C) 2000  Joerg Schulenburg

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 Joerg.Schulenburg@physik.uni-magdeburg.de */
 
#include "unicode.h"
#include <stdio.h>

int warn=1; /* if 1 a message is generated if compodition is not defined */

/* Arguments: the character (main), and the modifier (accent, etc). See the
      function if you want to know the modifiers. 
   Description: This function intends to be a small helper, to avoid having
      to write switches in functions. It's therefore mainly to accents, and
      specially for the most usual ones. It supports the basic greek 
      characters too, which is actually not very helpful.
   Returns: the unicode character corresponding to the composed character. */
wchar_t compose(wchar_t main, wchar_t modifier) {
/* supported by now: part of ISO8859-1, basic greek characters */
  if( main == UNKNOWN || main == PICTURE ) return main;
  switch (modifier) {
    case UNICODE_NULL:
    case SPACE:
	return      (wchar_t)main;

    case APOSTROPHE: /* do NOT USE this. It's here for compatibility only. 
			    Use ACUTE_ACCENT instead. */
      fprintf( stderr, "COMPOSE: got APOSTROPHE instead of ACUTE_ACCENT");
    case ACUTE_ACCENT: /* acute/cedilla */
      switch (main) {
	case 'a':	    return LATIN_SMALL_LETTER_A_WITH_ACUTE;
	case 'A':	    return LATIN_CAPITAL_LETTER_A_WITH_ACUTE;
	case 'e':	    return LATIN_SMALL_LETTER_E_WITH_ACUTE;
	case 'E':	    return LATIN_CAPITAL_LETTER_E_WITH_ACUTE;
	case 'i':	    return LATIN_SMALL_LETTER_I_WITH_ACUTE;
	case 'I':	    return LATIN_CAPITAL_LETTER_I_WITH_ACUTE;
	case 'o':	    return LATIN_SMALL_LETTER_O_WITH_ACUTE;
	case 'O':	    return LATIN_CAPITAL_LETTER_O_WITH_ACUTE;
	case 'u':	    return LATIN_SMALL_LETTER_U_WITH_ACUTE;
	case 'U':	    return LATIN_CAPITAL_LETTER_U_WITH_ACUTE;
	case 'y':	    return LATIN_SMALL_LETTER_Y_WITH_ACUTE;
 	case 'Y':	    return LATIN_CAPITAL_LETTER_Y_WITH_ACUTE;
	default:
          if(warn)fprintf( stderr, " COMPOSE: ACUTE_ACCENT+%04x not defined\n",(int)main);
      }
      break;

    case CEDILLA:
      switch (main) {
	case 'c':	    return LATIN_SMALL_LETTER_C_WITH_CEDILLA;
	case 'C':	    return LATIN_CAPITAL_LETTER_C_WITH_CEDILLA;
	default:
          if(warn)fprintf( stderr, " COMPOSE: CEDILLA+%04x not defined\n",(int)main);
      }
      break;

    case TILDE:
      switch (main) {
	case 'a':	    return LATIN_SMALL_LETTER_A_WITH_TILDE;
	case 'A':	    return LATIN_CAPITAL_LETTER_A_WITH_TILDE;
	case 'n':	    return LATIN_SMALL_LETTER_N_WITH_TILDE;
	case 'N':	    return LATIN_CAPITAL_LETTER_N_WITH_TILDE;
	case 'o':	    return LATIN_SMALL_LETTER_O_WITH_TILDE;
	case 'O':	    return LATIN_CAPITAL_LETTER_O_WITH_TILDE;
	default:
          if(warn)fprintf( stderr, " COMPOSE: TILDE+%04x not defined\n",(int)main);
      }
      break;
    case GRAVE_ACCENT:
      switch (main) {
	case 'a':	    return LATIN_SMALL_LETTER_A_WITH_GRAVE;
	case 'A':	    return LATIN_CAPITAL_LETTER_A_WITH_GRAVE;
      	case 'e':	    return LATIN_SMALL_LETTER_E_WITH_GRAVE;
	case 'E':	    return LATIN_CAPITAL_LETTER_E_WITH_GRAVE;
	case 'i':	    return LATIN_SMALL_LETTER_I_WITH_GRAVE;
	case 'I':	    return LATIN_CAPITAL_LETTER_I_WITH_GRAVE;
	case 'o':	    return LATIN_SMALL_LETTER_O_WITH_GRAVE;
	case 'O':	    return LATIN_CAPITAL_LETTER_O_WITH_GRAVE;
	case 'u':	    return LATIN_SMALL_LETTER_U_WITH_GRAVE;
	case 'U':	    return LATIN_CAPITAL_LETTER_U_WITH_GRAVE;
	default:
          if(warn)fprintf( stderr, " COMPOSE: GRAVE_ACCENT+%04x not defined\n",(int)main);
      }
      break;
    case QUOTATION_MARK: /* do NOT USE this. It's here for compatibility only. 
			    Use DIAERESIS instead. */
      fprintf( stderr, "COMPOSE: got APOSTROPHE instead of ACUTE_ACCENT");
    case DIAERESIS:
      switch (main) {
	case 'a':	    return LATIN_SMALL_LETTER_A_WITH_DIAERESIS;
	case 'A':	    return LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS;
	case 'e':	    return LATIN_SMALL_LETTER_E_WITH_DIAERESIS;
	case 'E':	    return LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS;
	case 'i':	    return LATIN_SMALL_LETTER_I_WITH_DIAERESIS;
	case 'I':	    return LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS;
	case 'o':	    return LATIN_SMALL_LETTER_O_WITH_DIAERESIS;
	case 'O':	    return LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS;
	case 'u':	    return LATIN_SMALL_LETTER_U_WITH_DIAERESIS;
	case 'U':	    return LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS;
	case 'y':	    return LATIN_SMALL_LETTER_Y_WITH_DIAERESIS;
	case 'Y':	    return LATIN_CAPITAL_LETTER_Y_WITH_DIAERESIS;
	default:
          if(warn)fprintf( stderr, " COMPOSE: DIAERESIS+%04x not defined\n",(int)main);
      }
      break;
    case CIRCUMFLEX_ACCENT:
      switch (main) {
	case 'a':	    return LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX;
	case 'A':	    return LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX;
	case 'e':	    return LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX;
	case 'E':	    return LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX;
	case 'i':	    return LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX;
	case 'I':	    return LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX;
	case 'o':	    return LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX;
	case 'O':	    return LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX;
	case 'u':	    return LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX;
	case 'U':	    return LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX;
	default:
          if(warn)fprintf( stderr, " COMPOSE: CIRCUMFLEX_ACCENT+%04x not defined\n",(int)main);
      }
      break;
    case RING_ABOVE:
      switch (main) {
	case 'a':	    return LATIN_SMALL_LETTER_A_WITH_RING_ABOVE;
	case 'A':	    return LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE;
	default:
          if(warn)fprintf( stderr, " COMPOSE: RING_ABOVE+%04x not defined\n",(int)main);
      }
      break;
    case 'e': /* e ligatures: ae, oe. */
    case 'E':
      switch (main) {
	case 'a':	    return LATIN_SMALL_LETTER_AE;
	case 'A':	    return LATIN_CAPITAL_LETTER_AE;
	case 'o':	    return LATIN_SMALL_LIGATURE_OE;
	case 'O':	    return LATIN_CAPITAL_LIGATURE_OE;
	default:
          if(warn)fprintf( stderr, " COMPOSE: %04x+e/E not defined\n",(int)main);
      }
      break;
    case 'g': /* greek */
      switch (main) {
        /* missing 0x37A-0x390 */
        /* weird cases: Q -> theta (it resembles a little, doesn't it?)
			V -> psi   (what can I do?) */
	case 'A':   return GREEK_CAPITAL_LETTER_ALPHA;
	case 'B':   return GREEK_CAPITAL_LETTER_BETA;
	case 'G':   return GREEK_CAPITAL_LETTER_GAMMA;
	case 'D':   return GREEK_CAPITAL_LETTER_DELTA;
	case 'E':   return GREEK_CAPITAL_LETTER_EPSILON;
	case 'Z':   return GREEK_CAPITAL_LETTER_ZETA;
	case 'H':   return GREEK_CAPITAL_LETTER_ETA;
	case 'Q':   return GREEK_CAPITAL_LETTER_THETA;
	case 'I':   return GREEK_CAPITAL_LETTER_IOTA;
	case 'K':   return GREEK_CAPITAL_LETTER_KAPPA;
	case 'L':   return GREEK_CAPITAL_LETTER_LAMDA;
	case 'M':   return GREEK_CAPITAL_LETTER_MU;
	case 'N':   return GREEK_CAPITAL_LETTER_NU;
	case 'X':   return GREEK_CAPITAL_LETTER_XI;
	case 'O':   return GREEK_CAPITAL_LETTER_OMICRON;
	case 'P':   return GREEK_CAPITAL_LETTER_PI;
	case 'R':   return GREEK_CAPITAL_LETTER_RHO;
	case 'S':   return GREEK_CAPITAL_LETTER_SIGMA;
	case 'T':   return GREEK_CAPITAL_LETTER_TAU;
	case 'Y':   return GREEK_CAPITAL_LETTER_UPSILON;
	case 'F':   return GREEK_CAPITAL_LETTER_PHI;
	case 'C':   return GREEK_CAPITAL_LETTER_CHI;
	case 'V':   return GREEK_CAPITAL_LETTER_PSI;
	case 'W':   return GREEK_CAPITAL_LETTER_OMEGA;
/*
	case '':   return GREEK_CAPITAL_LETTER_IOTA_WITH_DIALYTIKA;
	case '':   return GREEK_CAPITAL_LETTER_UPSILON_WITH_DIALYTIKA;
	case '':   return GREEK_SMALL_LETTER_ALPHA_WITH_TONOS;
	case '':   return GREEK_SMALL_LETTER_EPSILON_WITH_TONOS;
	case '':   return GREEK_SMALL_LETTER_ETA_WITH_TONOS;
	case '':   return GREEK_SMALL_LETTER_IOTA_WITH_TONOS;
	case '':   return GREEK_SMALL_LETTER_UPSILON_WITH_DIALYTIKA_AND_TONOS;
*/
	case 'a':   return GREEK_SMALL_LETTER_ALPHA;
	case 'b':   return GREEK_SMALL_LETTER_BETA;
	case 'g':   return GREEK_SMALL_LETTER_GAMMA;
	case 'd':   return GREEK_SMALL_LETTER_DELTA;
	case 'e':   return GREEK_SMALL_LETTER_EPSILON;
	case 'z':   return GREEK_SMALL_LETTER_ZETA;
	case 'h':   return GREEK_SMALL_LETTER_ETA;
	case 'q':   return GREEK_SMALL_LETTER_THETA;
	case 'i':   return GREEK_SMALL_LETTER_IOTA;
	case 'k':   return GREEK_SMALL_LETTER_KAPPA;
	case 'l':   return GREEK_SMALL_LETTER_LAMDA;
	case 'm':   return GREEK_SMALL_LETTER_MU;
	case 'n':   return GREEK_SMALL_LETTER_NU;
	case 'x':   return GREEK_SMALL_LETTER_XI;
	case 'o':   return GREEK_SMALL_LETTER_OMICRON;
	case 'p':   return GREEK_SMALL_LETTER_PI;
	case 'r':   return GREEK_SMALL_LETTER_RHO;
	case '&':   return GREEK_SMALL_LETTER_FINAL_SIGMA;
	case 's':   return GREEK_SMALL_LETTER_SIGMA;
	case 't':   return GREEK_SMALL_LETTER_TAU;
	case 'y':   return GREEK_SMALL_LETTER_UPSILON;
	case 'f':   return GREEK_SMALL_LETTER_PHI;
	case 'c':   return GREEK_SMALL_LETTER_CHI;
	case 'v':   return GREEK_SMALL_LETTER_PSI;
	case 'w':   return GREEK_SMALL_LETTER_OMEGA;
/*
	case '':   return GREEK_SMALL_LETTER_IOTA_WITH_DIALYTIKA;
	case '':   return GREEK_SMALL_LETTER_UPSILON_WITH_DIALYTIKA;
	case '':   return GREEK_SMALL_LETTER_OMICRON_WITH_TONOS;
	case '':   return GREEK_SMALL_LETTER_UPSILON_WITH_TONOS;
	case '':   return GREEK_SMALL_LETTER_OMEGA_WITH_TONOS;
	case '':   return GREEK_BETA_SYMBOL;
	case '':   return GREEK_THETA_SYMBOL;
	case '':   return GREEK_UPSILON_WITH_HOOK_SYMBOL;
	case '':   return GREEK_UPSILON_WITH_ACUTE_AND_HOOK_SYMBOL;
	case '':   return GREEK_UPSILON_WITH_DIAERESIS_AND_HOOK_SYMBOL;
	case '':   return GREEK_PHI_SYMBOL;
	case '':   return GREEK_PI_SYMBOL;
*/
	default:
          if(warn)fprintf( stderr, " COMPOSE: GREEK %04x not defined\n",(int)main);
      }
      break;   
    default:
      fprintf( stderr, " COMPOSE: modifier %04x not defined\n",(int)modifier);
  }
  return (wchar_t)main;
}

#define UNDEFINED			"X"

/* Arguments: character in Unicode format, type of format to convert to.
   Returns: a string containing the Unicode character converted to the chosen
    format. This string is statically allocated and should not be freed. */
const unsigned char *decode(wchar_t c, FORMAT type) {
  static unsigned char d;
  switch (type) {
    case ISO8859_1:
      if ( c <= 0xFF ) { /* UNICODE == ISO8859-1 */
        d = (unsigned char)c;
        return &d;
      }
      switch (c) { /* not found in list, but perhaps we can describe it */
	/* todo: add greek. GREEK_SMALL_LETTER_ALPHA = alpha */
	
	/* general puctuation */
	case HYPHEN:
	  return (const unsigned char *)"-";
	case FIGURE_DASH:
	case EN_DASH:
	  return (const unsigned char *)"--";
	case EM_DASH:
	  return (const unsigned char *)"---";
	case LEFT_SINGLE_QUOTATION_MARK:
	  return (const unsigned char *)"`";
	case RIGHT_SINGLE_QUOTATION_MARK:
	  return (const unsigned char *)"'";
	case SINGLE_LOW_9_QUOTATION_MARK:
	  return (const unsigned char *)",";
	case SINGLE_HIGH_REVERSED_9_QUOTATION_MARK:
	  return (const unsigned char *)UNDEFINED;
	case LEFT_DOUBLE_QUOTATION_MARK:
	  return (const unsigned char *)"``";
	case RIGHT_DOUBLE_QUOTATION_MARK:
	  return (const unsigned char *)"''";
	case DOUBLE_LOW_9_QUOTATION_MARK:
	  return (const unsigned char *)",,";
	case DOUBLE_HIGH_REVERSED_9_QUOTATION_MARK:
	  return (const unsigned char *)UNDEFINED;
	case DAGGER:
	  return (const unsigned char *)"+";
	case DOUBLE_DAGGER:
	  return (const unsigned char *)"*";
	case BULLET:
	  return (const unsigned char *)"*";
	case TRIANGULAR_BULLET:
	  return (const unsigned char *)"*";
	case HYPHENATION_POINT:
	  return (const unsigned char *)"-";
	case HORIZONTAL_ELLIPSIS:
	  return (const unsigned char *)"...";
	case PER_MILLE_SIGN:
	  return (const unsigned char *)"%%"; /* awk! */
	case SINGLE_LEFT_POINTING_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)"<";
	case SINGLE_RIGHT_POINTING_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)">";
	
	/* ligatures */
	case LATIN_SMALL_LIGATURE_FF:
	  return (const unsigned char *)"ff";
	case LATIN_SMALL_LIGATURE_FI:
	  return (const unsigned char *)"fi";
	case LATIN_SMALL_LIGATURE_FL:
	  return (const unsigned char *)"fl";
	case LATIN_SMALL_LIGATURE_FFI:
	  return (const unsigned char *)"ffi";
	case LATIN_SMALL_LIGATURE_FFL:
	  return (const unsigned char *)"ffl";
	case LATIN_SMALL_LIGATURE_LONG_S_T:
	case LATIN_SMALL_LIGATURE_ST:
	  return (const unsigned char *)"st";
	
	/* extra */
	case UNKNOWN:
	  return (const unsigned char *)"_";
	case PICTURE:
	  return (const unsigned char *)"(PICTURE)";
	
	
	default:
	  return (const unsigned char *)UNDEFINED;
      }
      break;
    case TeX:
      if ( c >= SPACE && c <= TILDE ) { /* ASCII */
	switch (c) {
	  case '$':
	      return (const unsigned char *)"\\$";
	  case '&':
	      return (const unsigned char *)"\\&";
	  case '%':
	      return (const unsigned char *)"\\%";
	  case '#':
	      return (const unsigned char *)"\\#";
	  case '_':
	      return (const unsigned char *)"\\_";
	  case '{':
	      return (const unsigned char *)"\\{";
	  case '}':
	      return (const unsigned char *)"\\}";
	  case '\\':
	      return (const unsigned char *)"$\\backslash$";
	  case '~':
	      return (const unsigned char *)"\\~{}";
	  case '^':
	      return (const unsigned char *)"\\^{}";
	  default:
	      d = (unsigned char)c;
	      return (const unsigned char *)&d;
	}
      }
      switch (c) {
	/* ISO8859_1 */
	case NO_BREAK_SPACE:
	  return (const unsigned char *)"~";
	case INVERTED_EXCLAMATION_MARK:
	  return (const unsigned char *)"!'";
	case CENT_SIGN:
	  return (const unsigned char *)UNDEFINED;
	case POUND_SIGN:
	  return (const unsigned char *)"\\pounds";
	case CURRENCY_SIGN:
	  return (const unsigned char *)UNDEFINED;
	case YEN_SIGN:
	  return (const unsigned char *)UNDEFINED;
	case BROKEN_BAR:
	  return (const unsigned char *)UNDEFINED;
	case SECTION_SIGN:
	  return (const unsigned char *)"\\S";
	case DIAERESIS:
	  return (const unsigned char *)"\"";
	case COPYRIGHT_SIGN:
	  return (const unsigned char *)"\\copyright";
	case FEMININE_ORDINAL_INDICATOR:
	  return (const unsigned char *)"$^{\\underbar{a}}$";
	case LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)"\\flqq{}";
	case NOT_SIGN:
	  return (const unsigned char *)"$\\lnot$";
	case SOFT_HYPHEN:
	  return (const unsigned char *)"\\-";
	case REGISTERED_SIGN:
	  return (const unsigned char *)UNDEFINED;
	case MACRON:
	  return (const unsigned char *)UNDEFINED;
	case DEGREE_SIGN:
	  return (const unsigned char *)"$^{o}$";
	case PLUS_MINUS_SIGN:
	  return (const unsigned char *)"$\\pm$";
	case SUPERSCRIPT_TWO:
	  return (const unsigned char *)"$^{2}$";
	case SUPERSCRIPT_THREE:
	  return (const unsigned char *)"$^{3}$";
	case ACUTE_ACCENT:
	  return (const unsigned char *)"\\( \\prime \\)";
	case MICRO_SIGN:
	  return (const unsigned char *)"$\\mu$";
	case PILCROW_SIGN:
	  return (const unsigned char *)"\\P";
	case MIDDLE_DOT:
	  return (const unsigned char *)"$\\cdot$";
	case CEDILLA:
	  return (const unsigned char *)"\\,";
	case SUPERSCRIPT_ONE:
	  return (const unsigned char *)"$^{1}$";
	case MASCULINE_ORDINAL_INDICATOR:
	  return (const unsigned char *)"$^{\\underbar{o}}$";
	case RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)"\\frqq{}";
	case VULGAR_FRACTION_ONE_QUARTER:	 /* these fractions are not good*/
	  return (const unsigned char *)"\\( 1\\over 4 \\)";
	case VULGAR_FRACTION_ONE_HALF:
	  return (const unsigned char *)"\\( 1\\over 2 \\)";
	case VULGAR_FRACTION_THREE_QUARTERS:
	  return (const unsigned char *)"\\( 3\\over 4 \\)";
	case INVERTED_QUESTION_MARK:
	  return (const unsigned char *)"?'";
	case LATIN_CAPITAL_LETTER_A_WITH_GRAVE:
	  return (const unsigned char *)"\\`A";
	case LATIN_CAPITAL_LETTER_A_WITH_ACUTE:
	  return (const unsigned char *)"\\'A";
	case LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^A";
	case LATIN_CAPITAL_LETTER_A_WITH_TILDE:
	  return (const unsigned char *)"\\~A";
	case LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"A";
	case LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE:
	  return (const unsigned char *)"\\AA";
	case LATIN_CAPITAL_LETTER_AE:
	  return (const unsigned char *)"\\AE";
	case LATIN_CAPITAL_LETTER_C_WITH_CEDILLA:
	  return (const unsigned char *)"\\C";
	case LATIN_CAPITAL_LETTER_E_WITH_GRAVE:
	  return (const unsigned char *)"\\`E";
	case LATIN_CAPITAL_LETTER_E_WITH_ACUTE:
	  return (const unsigned char *)"\\'E";
	case LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^E";
	case LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"E";
	case LATIN_CAPITAL_LETTER_I_WITH_GRAVE:
	  return (const unsigned char *)"\\`I";
	case LATIN_CAPITAL_LETTER_I_WITH_ACUTE:
	  return (const unsigned char *)"\\'I";
	case LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^I";
	case LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"I";
	case LATIN_CAPITAL_LETTER_ETH:
	  return (const unsigned char *)UNDEFINED;
	case LATIN_CAPITAL_LETTER_N_WITH_TILDE:
	  return (const unsigned char *)"\\~N";
	case LATIN_CAPITAL_LETTER_O_WITH_GRAVE:
	  return (const unsigned char *)"\\`O";
	case LATIN_CAPITAL_LETTER_O_WITH_ACUTE:
	  return (const unsigned char *)"\\'O";
	case LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^O";
	case LATIN_CAPITAL_LETTER_O_WITH_TILDE:
	  return (const unsigned char *)"\\~O";
	case LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"O";
	case MULTIPLICATION_SIGN:
	  return (const unsigned char *)"$\\times$";
	case LATIN_CAPITAL_LETTER_O_WITH_STROKE:
	  return (const unsigned char *)"\\O";
	case LATIN_CAPITAL_LETTER_U_WITH_GRAVE:
	  return (const unsigned char *)"\\`U";
	case LATIN_CAPITAL_LETTER_U_WITH_ACUTE:
	  return (const unsigned char *)"\\'U";
	case LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^U";
	case LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"U";
	case LATIN_CAPITAL_LETTER_Y_WITH_ACUTE:
	  return (const unsigned char *)"\\'Y";
	case LATIN_CAPITAL_LETTER_THORN:
	  return (const unsigned char *)UNDEFINED;
	case LATIN_SMALL_LETTER_SHARP_S:
	  return (const unsigned char *)"\\ss";
	case LATIN_SMALL_LETTER_A_WITH_GRAVE:
	  return (const unsigned char *)"\\`a";
	case LATIN_SMALL_LETTER_A_WITH_ACUTE:
	  return (const unsigned char *)"\\'a";
	case LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^a";
	case LATIN_SMALL_LETTER_A_WITH_TILDE:
	  return (const unsigned char *)"\\~a";
	case LATIN_SMALL_LETTER_A_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"a";
	case LATIN_SMALL_LETTER_A_WITH_RING_ABOVE:
	  return (const unsigned char *)"\\aa";
	case LATIN_SMALL_LETTER_AE:
	  return (const unsigned char *)"\\ae";
	case LATIN_SMALL_LETTER_C_WITH_CEDILLA:
	  return (const unsigned char *)"\\c";
	case LATIN_SMALL_LETTER_E_WITH_GRAVE:
	  return (const unsigned char *)"\\`e";
	case LATIN_SMALL_LETTER_E_WITH_ACUTE:
	  return (const unsigned char *)"\\'e";
	case LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^e";
	case LATIN_SMALL_LETTER_E_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"e";
	case LATIN_SMALL_LETTER_I_WITH_GRAVE:
	  return (const unsigned char *)"\\`i";
	case LATIN_SMALL_LETTER_I_WITH_ACUTE:
	  return (const unsigned char *)"\\'i";
	case LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^i";
	case LATIN_SMALL_LETTER_I_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"i";
	case LATIN_SMALL_LETTER_ETH:
	  return (const unsigned char *)UNDEFINED;
	case LATIN_SMALL_LETTER_N_WITH_TILDE:
	  return (const unsigned char *)"\\~n";
	case LATIN_SMALL_LETTER_O_WITH_GRAVE:
	  return (const unsigned char *)"\\`o";
	case LATIN_SMALL_LETTER_O_WITH_ACUTE:
	  return (const unsigned char *)"\\'o";
	case LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^o";
	case LATIN_SMALL_LETTER_O_WITH_TILDE:
	  return (const unsigned char *)"\\~o";
	case LATIN_SMALL_LETTER_O_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"o";
	case DIVISION_SIGN:
	  return (const unsigned char *)"$\\div$";
	case LATIN_SMALL_LETTER_O_WITH_STROKE:
	  return (const unsigned char *)"\\o";
	case LATIN_SMALL_LETTER_U_WITH_GRAVE:
	  return (const unsigned char *)"\\`u";
	case LATIN_SMALL_LETTER_U_WITH_ACUTE:
	  return (const unsigned char *)"\\'u";
	case LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"\\^u";
	case LATIN_SMALL_LETTER_U_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"u";
	case LATIN_SMALL_LETTER_Y_WITH_ACUTE:
	  return (const unsigned char *)"\\'y";
	case LATIN_SMALL_LETTER_THORN:
	  return (const unsigned char *)UNDEFINED;
	case LATIN_SMALL_LETTER_Y_WITH_DIAERESIS:
	  return (const unsigned char *)"\\\"y";

	/* greek */
	  /* some (punctuation, accents, accented capital) greek letters missing*/
	case GREEK_CAPITAL_LETTER_ALPHA:
	  return (const unsigned char *)"A";
	case GREEK_CAPITAL_LETTER_BETA:
	  return (const unsigned char *)"B";
	case GREEK_CAPITAL_LETTER_GAMMA:
	  return (const unsigned char *)"\\( \\Gamma \\)";
	case GREEK_CAPITAL_LETTER_DELTA:
	  return (const unsigned char *)"\\( \\Delta \\)";
	case GREEK_CAPITAL_LETTER_EPSILON:
	  return (const unsigned char *)"E";
	case GREEK_CAPITAL_LETTER_ZETA:
	  return (const unsigned char *)"Z";
	case GREEK_CAPITAL_LETTER_ETA:
	  return (const unsigned char *)"H";
	case GREEK_CAPITAL_LETTER_THETA:
	  return (const unsigned char *)"\\( \\Theta \\)";
	case GREEK_CAPITAL_LETTER_IOTA:
	  return (const unsigned char *)"I";
	case GREEK_CAPITAL_LETTER_KAPPA:
	  return (const unsigned char *)"K";
	case GREEK_CAPITAL_LETTER_LAMDA:
	  return (const unsigned char *)"\\( \\Lambda \\)";
	case GREEK_CAPITAL_LETTER_MU:
	  return (const unsigned char *)"M";
	case GREEK_CAPITAL_LETTER_NU:
	  return (const unsigned char *)"N";
	case GREEK_CAPITAL_LETTER_XI:
	  return (const unsigned char *)"\\( \\Xi \\)";
	case GREEK_CAPITAL_LETTER_OMICRON:
	  return (const unsigned char *)"O";
	case GREEK_CAPITAL_LETTER_PI:
	  return (const unsigned char *)"\\( \\Pi \\)";
	case GREEK_CAPITAL_LETTER_RHO:
	  return (const unsigned char *)"P";
	case GREEK_CAPITAL_LETTER_SIGMA:
	  return (const unsigned char *)"\\( \\Sigma \\)";
	case GREEK_CAPITAL_LETTER_TAU:
	  return (const unsigned char *)"T";
	case GREEK_CAPITAL_LETTER_UPSILON:
	  return (const unsigned char *)"\\( \\Upsilon \\)";
	case GREEK_CAPITAL_LETTER_PHI:
	  return (const unsigned char *)"\\( \\Phi \\)";
	case GREEK_CAPITAL_LETTER_CHI:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_CAPITAL_LETTER_PSI:
	  return (const unsigned char *)"\\( \\Psi \\)";
	case GREEK_CAPITAL_LETTER_OMEGA:
	  return (const unsigned char *)"\\( \\Omega \\)";
	case GREEK_CAPITAL_LETTER_IOTA_WITH_DIALYTIKA:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_CAPITAL_LETTER_UPSILON_WITH_DIALYTIKA:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_ALPHA_WITH_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_EPSILON_WITH_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_ETA_WITH_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_IOTA_WITH_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_UPSILON_WITH_DIALYTIKA_AND_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_ALPHA:
	  return (const unsigned char *)"\\( \\alpha \\)";
	case GREEK_SMALL_LETTER_BETA:
	  return (const unsigned char *)"\\( \\beta \\)";
	case GREEK_SMALL_LETTER_GAMMA:
	  return (const unsigned char *)"\\( \\gamma \\)";
	case GREEK_SMALL_LETTER_DELTA:
	  return (const unsigned char *)"\\( \\delta \\)";
	case GREEK_SMALL_LETTER_EPSILON:
	  return (const unsigned char *)"\\( \\epsilon \\)";
	case GREEK_SMALL_LETTER_ZETA:
	  return (const unsigned char *)"\\( \\zeta \\)";
	case GREEK_SMALL_LETTER_ETA:
	  return (const unsigned char *)"\\( \\eta \\)";
	case GREEK_SMALL_LETTER_THETA:
	  return (const unsigned char *)"\\( \\theta \\)";
	case GREEK_SMALL_LETTER_IOTA:
	  return (const unsigned char *)"\\( \\iota \\)";
	case GREEK_SMALL_LETTER_KAPPA:
	  return (const unsigned char *)"\\( \\kappa \\)";
	case GREEK_SMALL_LETTER_LAMDA:
	  return (const unsigned char *)"\\( \\lambda \\)";
	case GREEK_SMALL_LETTER_MU:
	  return (const unsigned char *)"\\( \\mu \\)";
	case GREEK_SMALL_LETTER_NU:
	  return (const unsigned char *)"\\( \\nu \\)";
	case GREEK_SMALL_LETTER_XI:
	  return (const unsigned char *)"\\( \\xi \\)";
	case GREEK_SMALL_LETTER_OMICRON:
	  return (const unsigned char *)"\\( \\omicron \\)";
	case GREEK_SMALL_LETTER_PI:
	  return (const unsigned char *)"\\( \\pi \\)";
	case GREEK_SMALL_LETTER_RHO:
	  return (const unsigned char *)"\\( \\rho \\)";
	case GREEK_SMALL_LETTER_FINAL_SIGMA:
	  return (const unsigned char *)"\\( \\varsigma \\)";
	case GREEK_SMALL_LETTER_SIGMA:
	  return (const unsigned char *)"\\( \\sigma \\)";
	case GREEK_SMALL_LETTER_TAU:
	  return (const unsigned char *)"\\( \\tau \\)";
	case GREEK_SMALL_LETTER_UPSILON:
	  return (const unsigned char *)"\\( \\upsilon \\)";
	case GREEK_SMALL_LETTER_PHI:
	  return (const unsigned char *)"\\( \\varphi \\)";
	case GREEK_SMALL_LETTER_CHI:
	  return (const unsigned char *)"\\( \\chi \\)";
	case GREEK_SMALL_LETTER_PSI:
	  return (const unsigned char *)"\\( \\psi \\)";
	case GREEK_SMALL_LETTER_OMEGA:
	  return (const unsigned char *)"\\( \\omega \\)";
	case GREEK_SMALL_LETTER_IOTA_WITH_DIALYTIKA:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_UPSILON_WITH_DIALYTIKA:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_OMICRON_WITH_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_UPSILON_WITH_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_SMALL_LETTER_OMEGA_WITH_TONOS:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_BETA_SYMBOL:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_THETA_SYMBOL:
	  return (const unsigned char *)"\\( \\vartheta \\)";
	case GREEK_UPSILON_WITH_HOOK_SYMBOL:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_UPSILON_WITH_ACUTE_AND_HOOK_SYMBOL:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_UPSILON_WITH_DIAERESIS_AND_HOOK_SYMBOL:
	  return (const unsigned char *)UNDEFINED;
	case GREEK_PHI_SYMBOL:
	  return (const unsigned char *)"\\( \\phi \\)";
	case GREEK_PI_SYMBOL:
	  return (const unsigned char *)"\\( \\varpi \\)";
	  /* and some greek letters missing*/

	/* punctuation (partial) */
	case HYPHEN:
	  return (const unsigned char *)"-";
	case NON_BREAKING_HYPHEN:
	  return (const unsigned char *)UNDEFINED;
	case FIGURE_DASH:
	case EN_DASH:
	  return (const unsigned char *)"--";
	case EM_DASH:
	  return (const unsigned char *)"---";
	case HORIZONTAL_BAR:
	  return (const unsigned char *)UNDEFINED;
	case LEFT_SINGLE_QUOTATION_MARK:
	  return (const unsigned char *)"`";
	case RIGHT_SINGLE_QUOTATION_MARK:
	  return (const unsigned char *)"'";
	case SINGLE_LOW_9_QUOTATION_MARK:
	  return (const unsigned char *)"\\glq{}";
	case SINGLE_HIGH_REVERSED_9_QUOTATION_MARK:
	  return (const unsigned char *)UNDEFINED;
	case LEFT_DOUBLE_QUOTATION_MARK:
	  return (const unsigned char *)"``";
	case RIGHT_DOUBLE_QUOTATION_MARK:
	  return (const unsigned char *)"''";
	case DOUBLE_LOW_9_QUOTATION_MARK:
	  return (const unsigned char *)"\\glqq{}";
	case DOUBLE_HIGH_REVERSED_9_QUOTATION_MARK:
	  return (const unsigned char *)UNDEFINED;
	case DAGGER:
	  return (const unsigned char *)"\\dag";
	case DOUBLE_DAGGER:
	  return (const unsigned char *)"\\ddag";
	case BULLET:
	  return (const unsigned char *)"$\\bullet$";
	case TRIANGULAR_BULLET:
	  return (const unsigned char *)"$\\blacktriangleright";
	case HYPHENATION_POINT:
	  return (const unsigned char *)"\\-";
	case HORIZONTAL_ELLIPSIS:
	  return (const unsigned char *)"\\ldots";
	case PER_MILLE_SIGN:
	  return (const unsigned char *)UNDEFINED;
	case SINGLE_LEFT_POINTING_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)"\\flq{}";
	case SINGLE_RIGHT_POINTING_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)"\\frq{}";
	/* ligatures */
	case LATIN_SMALL_LIGATURE_FF:
	  return (const unsigned char *)"ff";
	case LATIN_SMALL_LIGATURE_FI:
	  return (const unsigned char *)"fi";
	case LATIN_SMALL_LIGATURE_FL:
	  return (const unsigned char *)"fl";
	case LATIN_SMALL_LIGATURE_FFI:
	  return (const unsigned char *)"ffi";
	case LATIN_SMALL_LIGATURE_FFL:
	  return (const unsigned char *)"ffl";
	case LATIN_SMALL_LIGATURE_LONG_S_T:
	case LATIN_SMALL_LIGATURE_ST:
	  return (const unsigned char *)"st";
	/* reserved */
	case UNKNOWN:
	  return (const unsigned char *)"\\_";
	case PICTURE:
	  return (const unsigned char *)"(PICTURE)";
	default:
	  return (const unsigned char *)UNDEFINED;
	}
    case HTML:
      if ( c >= SPACE && c <= TILDE ) { /* ASCII */
        d = (unsigned char)c;
        return &d;
      }
      switch (c) {
	case FORM_FEED:
	case CARRIAGE_RETURN:
	  return (const unsigned char *)"<br>";
	case NO_BREAK_SPACE:
	  return (const unsigned char *)"<nobr>";
	case INVERTED_EXCLAMATION_MARK:
	  return (const unsigned char *)"&iexcl;";
	case CENT_SIGN:
	  return (const unsigned char *)"&cent;";
	case POUND_SIGN:
	  return (const unsigned char *)"&pound;";
	case CURRENCY_SIGN:
	  return (const unsigned char *)"&curren;";
	case YEN_SIGN:
	  return (const unsigned char *)"&yen;";
	case BROKEN_BAR:
	  return (const unsigned char *)"&brvbar;";
	case SECTION_SIGN:
	  return (const unsigned char *)"&sect;";
	case DIAERESIS:
	  return (const unsigned char *)"&uml;";
	case COPYRIGHT_SIGN:
	  return (const unsigned char *)"&copy;";
	case FEMININE_ORDINAL_INDICATOR:
	  return (const unsigned char *)"&ordfem;";
	case LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)"&laquo;";
	case NOT_SIGN:
	  return (const unsigned char *)"&not;";
	case SOFT_HYPHEN:
	  return (const unsigned char *)"&shy;";
	case REGISTERED_SIGN:
	  return (const unsigned char *)"&reg;";
	case MACRON:
	  return (const unsigned char *)"&macr;";
	case DEGREE_SIGN:
	  return (const unsigned char *)"&deg;";
	case PLUS_MINUS_SIGN:
	  return (const unsigned char *)"&plusmn;";
	case SUPERSCRIPT_TWO:
	  return (const unsigned char *)"&sup2;";
	case SUPERSCRIPT_THREE:
	  return (const unsigned char *)"&sup3;";
	case ACUTE_ACCENT:
	  return (const unsigned char *)"&acute;";
	case MICRO_SIGN:
	  return (const unsigned char *)"&micro;";
	case PILCROW_SIGN:
	  return (const unsigned char *)"&para;";
	case MIDDLE_DOT:
	  return (const unsigned char *)"&middot;";
	case CEDILLA:
	  return (const unsigned char *)"&cedil;";
	case SUPERSCRIPT_ONE:
	  return (const unsigned char *)"&sup1;";
	case MASCULINE_ORDINAL_INDICATOR:
	  return (const unsigned char *)"&ordm;";
	case RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK:
	  return (const unsigned char *)"&raquo;";
	case VULGAR_FRACTION_ONE_QUARTER:
	  return (const unsigned char *)"&frac14;";
	case VULGAR_FRACTION_ONE_HALF:
	  return (const unsigned char *)"&frac12;";
	case VULGAR_FRACTION_THREE_QUARTERS:
	  return (const unsigned char *)"&frac34;";
	case INVERTED_QUESTION_MARK:
	  return (const unsigned char *)"&iquest;";	  
	case LATIN_CAPITAL_LETTER_A_WITH_GRAVE:
	  return (const unsigned char *)"&Agrave;";
	case LATIN_CAPITAL_LETTER_A_WITH_ACUTE:
	  return (const unsigned char *)"&Aacute;";
	case LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&Acirc;";
	case LATIN_CAPITAL_LETTER_A_WITH_TILDE:
	  return (const unsigned char *)"&Atilde;";
	case LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS:
	  return (const unsigned char *)"&Auml;";
	case LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE:
	  return (const unsigned char *)"&Aring;";
	case LATIN_CAPITAL_LETTER_AE:
	  return (const unsigned char *)"&AElig;";
	case LATIN_CAPITAL_LETTER_C_WITH_CEDILLA:
	  return (const unsigned char *)"&Ccedil;";
	case LATIN_CAPITAL_LETTER_E_WITH_GRAVE:
	  return (const unsigned char *)"&Egrave;";
	case LATIN_CAPITAL_LETTER_E_WITH_ACUTE:
	  return (const unsigned char *)"&Eacute;";
	case LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&Ecirc;";
	case LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS:
	  return (const unsigned char *)"&Euml;";
	case LATIN_CAPITAL_LETTER_I_WITH_GRAVE:
	  return (const unsigned char *)"&Igrave;";
	case LATIN_CAPITAL_LETTER_I_WITH_ACUTE:
	  return (const unsigned char *)"&Iacute;";
	case LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&Icirc;";
	case LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS:
	  return (const unsigned char *)"&Iuml;";
	case LATIN_CAPITAL_LETTER_ETH:
	  return (const unsigned char *)"&ETH;";
	case LATIN_CAPITAL_LETTER_N_WITH_TILDE:
	  return (const unsigned char *)"&Ntilde;";
	case LATIN_CAPITAL_LETTER_O_WITH_GRAVE:
	  return (const unsigned char *)"&Ograve;";
	case LATIN_CAPITAL_LETTER_O_WITH_ACUTE:
	  return (const unsigned char *)"&Oacute;";
	case LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&Ocirc;";
	case LATIN_CAPITAL_LETTER_O_WITH_TILDE:
	  return (const unsigned char *)"&Otilde;";
	case LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS:
	  return (const unsigned char *)"&Ouml;";
	case MULTIPLICATION_SIGN:
	  return (const unsigned char *)"&times";
	case LATIN_CAPITAL_LETTER_O_WITH_STROKE:
	  return (const unsigned char *)"&Oslash;";
	case LATIN_CAPITAL_LETTER_U_WITH_GRAVE:
	  return (const unsigned char *)"&Ugrave;";
	case LATIN_CAPITAL_LETTER_U_WITH_ACUTE:
	  return (const unsigned char *)"&Uacute;";
	case LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&Ucirc;";
	case LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS:
	  return (const unsigned char *)"&Uuml;";
	case LATIN_CAPITAL_LETTER_Y_WITH_ACUTE:
	  return (const unsigned char *)"&Yacute;";
	case LATIN_CAPITAL_LETTER_THORN:
	  return (const unsigned char *)"&THORN;";
	case LATIN_SMALL_LETTER_SHARP_S:
	  return (const unsigned char *)"&szlig;";
	case LATIN_SMALL_LETTER_A_WITH_GRAVE:
	  return (const unsigned char *)"&agrave;";
	case LATIN_SMALL_LETTER_A_WITH_ACUTE:
	  return (const unsigned char *)"&acute;";
	case LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&acirc;";
	case LATIN_SMALL_LETTER_A_WITH_TILDE:
	  return (const unsigned char *)"&atilde;";
	case LATIN_SMALL_LETTER_A_WITH_DIAERESIS:
	  return (const unsigned char *)"&auml;";
	case LATIN_SMALL_LETTER_A_WITH_RING_ABOVE:
	  return (const unsigned char *)"&aring;";
	case LATIN_SMALL_LETTER_AE:
	  return (const unsigned char *)"&aelig;";
	case LATIN_SMALL_LETTER_C_WITH_CEDILLA:
	  return (const unsigned char *)"&ccedil;";
	case LATIN_SMALL_LETTER_E_WITH_GRAVE:
	  return (const unsigned char *)"&egrave;";
	case LATIN_SMALL_LETTER_E_WITH_ACUTE:
	  return (const unsigned char *)"&eacute;";
	case LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&ecirc;";
	case LATIN_SMALL_LETTER_E_WITH_DIAERESIS:
	  return (const unsigned char *)"&euml;";
	case LATIN_SMALL_LETTER_I_WITH_GRAVE:
	  return (const unsigned char *)"&igrave;";
	case LATIN_SMALL_LETTER_I_WITH_ACUTE:
	  return (const unsigned char *)"&iacute;";
	case LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&icirc;";
	case LATIN_SMALL_LETTER_I_WITH_DIAERESIS:
	  return (const unsigned char *)"&iuml;";
	case LATIN_SMALL_LETTER_ETH:
	  return (const unsigned char *)"&eth;";
	case LATIN_SMALL_LETTER_N_WITH_TILDE:
	  return (const unsigned char *)"&ntilde;";
	case LATIN_SMALL_LETTER_O_WITH_GRAVE:
	  return (const unsigned char *)"&ograve;";
	case LATIN_SMALL_LETTER_O_WITH_ACUTE:
	  return (const unsigned char *)"&oacute;";
	case LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&ocirc;";
	case LATIN_SMALL_LETTER_O_WITH_TILDE:
	  return (const unsigned char *)"&otilde;";
	case LATIN_SMALL_LETTER_O_WITH_DIAERESIS:
	  return (const unsigned char *)"&ouml;";
	case DIVISION_SIGN:
	  return (const unsigned char *)"&divide;";
	case LATIN_SMALL_LETTER_O_WITH_STROKE:
	  return (const unsigned char *)"&oslash;";
	case LATIN_SMALL_LETTER_U_WITH_GRAVE:
	  return (const unsigned char *)"&ugrave;";
	case LATIN_SMALL_LETTER_U_WITH_ACUTE:
	  return (const unsigned char *)"&uacute;";
	case LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX:
	  return (const unsigned char *)"&ucirc;";
	case LATIN_SMALL_LETTER_U_WITH_DIAERESIS:
	  return (const unsigned char *)"&uuml;";
	case LATIN_SMALL_LETTER_Y_WITH_ACUTE:
	  return (const unsigned char *)"&yacute;";
	case LATIN_SMALL_LETTER_THORN:
	  return (const unsigned char *)"&thorn;";
	case LATIN_SMALL_LETTER_Y_WITH_DIAERESIS:
	  return (const unsigned char *)"&yuml;";
	default:
	  return (const unsigned char *)UNDEFINED;
      }
      break;
    case SGML:
      switch (c) {
	default:
	  return (const unsigned char *)UNDEFINED;
      }
      break;
    default:
	return (const unsigned char *)NULL;
  }
}
