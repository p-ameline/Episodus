// -----------------------------------------------------------------------------
// balise.h
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
// $Author: pameline $
// $Date: 2013/11/11 23:15:51 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#ifndef __BALISE_H__
# define __BALISE_H__

# ifndef FOR_WINDOWS
// #  include <stl.h>
#  define FILESLASH "/"
#  define NEWLINE   "\n\n"
#  include "ParseError.h"
# else
#  include <vector.h>
#  define FILESLASH "\\"
#  define NEWLINE   "\r\n"
#  include "nsbb/nsarcParseError.h"
# endif

# include <string>

# include "attribut.h"
# include "valeur.h"

# define _MESSAGE_NOVERBOSE

class Cconteneur
{
  public:

    string send_id ;
    string pat_id ;
} ;

#ifndef __linux__
class _NSBBCLASSE Cbalise
#else
class Cbalise
#endif
{
  private:

    static long lObjectCount ;

  protected:

    string     _sAttributs ;
    string     _sValeurs ;

#ifndef FOR_WINDOWS
    parseError _iParsingError ;
#else
    nsarcParseError _iParsingError ;
#endif

  public:

    Cbalise *parent ;
    string databaseid ;

    Attribut_array vect_attr ;
    Valeur_array vect_val ;

    Cbalise(string attributs, string valeurs, Cbalise *parentBalise = NULL) ;
    Cbalise(string attributs, string valeurs, Cconteneur* pMV, Cbalise *parentBalise = NULL) ;
    Cbalise(const Cbalise& rv) ;
    Cbalise& operator=(const Cbalise& src) ;

    virtual ~Cbalise() { lObjectCount-- ; }  // Important to keep it being virtual

    virtual bool parser() = 0 ;
    virtual bool verif() = 0 ;
    virtual bool compresser() = 0 ;
    virtual bool traiter() = 0 ;

    bool    parser_attributs() ;
    string  getAttributeValue(string& sCopy, size_t iStartChar) ;

    bool    parser_valeurs() ;

    void    addAttribute(string sAttribut, string sText) ;

    bool    getBooleanAttribute(string sAttribut, bool* bDefaultAndFound) ;
    void    setBooleanAttribute(string sAttribut, bool bValue, bool bAddIfNotThere = false) ;

    CAttributs* getAttributeForValue(string sAttribut) ;
    CAttributs* getNextAttributeForValue(string sAttribut, CAttributs* pPrevious) ;

    string  getStringAttribute(string sAttribut) ;
    void    setStringAttribute(string sAttribut, string sText, bool bAddIfNotThere = false) ;
    string  getUrlStringAttribute(string sAttribut) ;
    void    setUrlStringAttribute(string sAttribut, string sText, bool bAddIfNotThere = false) ;

    int     getIntAttribute(string sAttribut) ;
    void    setIntAttribute(string sAttribut, int iValue, bool bAddIfNotThere = false) ;

    string  getValeurs()      { return texteCourant(_sValeurs) ; }
    string  getRawValeurs()   { return _sValeurs ; }
    string  getAttributes()   { return _sAttributs ; }
#ifndef FOR_WINDOWS
    parseError      getParsingError() { return _iParsingError ; }
#else
    nsarcParseError getParsingError() { return _iParsingError ; }
#endif

    void    cleanValeurs() ;
    void    eraseValeur(string sValueLabel) ;
    void    eraseAttribute(string sAttribut) ;

    string  toString(int iLevel, string sBalise) ;
    void    inscritFichier(ofstream* pOutFile, int iLevel, string sBalise) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    Cconteneur* pMainValues ;
} ;

class Cpmed_message : public Cbalise
{
  public:

    int connfd ;

    Cpmed_message(string attributs, string values, int cfd) ;
    ~Cpmed_message() ;

    bool parser() ;
    bool verif() ;
    bool compresser() ;
    bool traiter() ;
} ;

class Cpmed_patient : public Cbalise
{
  public:

    bool bMVowner ;

    Cpmed_patient(string attributs, string values, Cconteneur* pMV) ;
    Cpmed_patient(const Cpmed_patient& rv) ;
    ~Cpmed_patient() ;

    bool parser() ;
    bool verif() ;
    bool compresser() ;
    bool traiter() ;

    Cpmed_patient& operator=(const Cpmed_patient& src) ;
} ;

class Cpmed_admindata : public Cbalise
{
  public:

    Cpmed_admindata(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_admindata(const Cpmed_admindata& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() ;
    bool traiter() ;

    Cpmed_admindata& operator=(const Cpmed_admindata& src) ;
} ;

class Cpmed_healthdata : public Cbalise
{
  public:

    Cpmed_healthdata(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_healthdata(const Cpmed_healthdata& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() ;
    bool traiter() ;

    Cpmed_healthdata& operator=(const Cpmed_healthdata& src) ;
} ;

class Cpmed_problem : public Cbalise
{
  public:

    Cpmed_problem(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_problem(const Cpmed_problem& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() { return true ; }
    bool traiter() ;

    Cpmed_problem& operator=(const Cpmed_problem& src) ;
} ;

class Cpmed_problem_event : public Cbalise
{
  public:

    Cpmed_problem_event(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_problem_event(const Cpmed_problem_event& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() { return true ; }
    bool traiter() ;

    Cpmed_problem_event& operator=(const Cpmed_problem_event& src) ;
} ;

class Cpmed_contact : public Cbalise
{
  public:

    Cpmed_contact(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_contact(const Cpmed_contact& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() ;
    bool traiter() ;

    Cpmed_contact& operator=(const Cpmed_contact& src) ;
} ;

class Cpmed_contact_type : public Cbalise
{
  public:

    Cpmed_contact_type(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_contact_type(const Cpmed_contact_type& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() { return true ; }
    bool traiter() ;

    Cpmed_contact_type& operator=(const Cpmed_contact_type& src) ;
} ;

class Cpmed_contactelement : public Cbalise
{
  public:

    Cpmed_contactelement(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_contactelement(const Cpmed_contactelement& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() ;
    bool traiter() ;

    Cpmed_contactelement& operator=(const Cpmed_contactelement& src) ;
} ;

class Cpmed_contactcode : public Cbalise
{
  public:

    Cpmed_contactcode(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise) ;
    Cpmed_contactcode(const Cpmed_contactcode& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() { return true ; }
    bool traiter() ;

    Cpmed_contactcode& operator=(const Cpmed_contactcode& src) ;
} ;

class Cpmed_contacttree : public Cbalise
{
  public:

    Cpmed_contacttree(string attributs, string values, Cconteneur *pMV, Cbalise *parentBalise) ;
    Cpmed_contacttree(const Cpmed_contacttree& rv) ;

    bool parser() ;
    bool verif() ;
    bool compresser() { return true ; }
    bool traiter() ;

    Cpmed_contacttree& operator=(const Cpmed_contacttree& src) ;
} ;

class Cpmed_contactnode : public Cbalise
{
  public:

    // constructors
    Cpmed_contactnode(string attributs, string values, Cconteneur *pMV, Cbalise *parentBalise) ;
    Cpmed_contactnode(const Cpmed_contactnode& rv) ;

    // functions
    bool parser() ;
    bool verif() ;
    bool compresser() { return true ; }
    bool traiter() ;

    // operators
    Cpmed_contactnode& operator=(const Cpmed_contactnode& src) ;

    // variables
    string _sContactTreeID ;
} ;

#endif  // !__BALISE_H__

