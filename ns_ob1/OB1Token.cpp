/*
** Copyright Nautilus, (10/9/2004)
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/

#include "ns_ob1\OB1Token.h"
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Class.h"
#include "ns_ob1\BB1Link.h"
#include "ns_ob1\OB1Node.h"
#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\BB1KB.h"

long OB1Token::_lTokenCount = 0 ;

OB1Token::OB1Token()
{
  _lTokenCount++ ;

  _type              = ADD ;
  _pObject           = NULL ;
  _pAttributesChange = (AVPSet *)   0 ;
  _pLinkChange       = (LinkSet *)  0 ;
  _pWhere_I_am       = NULL ;
  _pLauncherToken    = NULL ;
  _iTokenState       = tokenFree ;
  _interfaceOpened   = false ;
  _hInterfaceHandle  = 0 ;
  _class             = -42 ; /* not assigned */

  incNumero() ;
}

OB1Token::OB1Token(EventType /* type */, BB1Object* pTr)
{
  _lTokenCount++ ;

  _type              = ADD ;
  _pObject           = pTr ;
  _pAttributesChange = (AVPSet *)   0 ;
  _pLinkChange       = (LinkSet *)  0 ;
  _pWhere_I_am       = (OB1Node *)  0 ;
  _pLauncherToken    = (OB1Token *) 0 ;
  _iTokenState       = tokenFree ;
  _interfaceOpened   = false ;
  _hInterfaceHandle  = 0 ;
  _class             = -42 ; /* not assigned */

  incNumero() ;
}

OB1Token::OB1Token(EventType /* type */, BB1Object* pTr, AVPSet* Change, LinkSet*  chan)
{
  _lTokenCount++ ;

  _type              = ADD ;
  _pObject           = pTr ;
  _pLinkChange       = chan ;
  _pAttributesChange = Change ;
  _pWhere_I_am       = (OB1Node *)  0 ;
  _pLauncherToken    = (OB1Token *) 0 ;
  _iTokenState       = tokenFree ;
  _interfaceOpened   = false ;
  _hInterfaceHandle  = 0 ;
  _class             = -42 ; /* not assigned */

  incNumero() ;
}

OB1Token::OB1Token(const OB1Token& tok)
{
  _lTokenCount++ ;

  _type            = tok._type ;
  _pObject         = tok._pObject ;

	if (NULL == tok._pLinkChange)
  	_pLinkChange = (LinkSet*) 0 ;
  else
  	_pLinkChange = new LinkSet(*(tok._pLinkChange)) ;

	if (NULL == tok._pAttributesChange)
  	_pAttributesChange = (AVPSet *) 0 ;
  else
  	_pAttributesChange = new AVPSet(*(tok._pAttributesChange)) ;

  _pWhere_I_am      = tok._pWhere_I_am ;
  _pLauncherToken   = tok._pLauncherToken ;
  _iTokenState      = tok._iTokenState ;
  _interfaceOpened  = tok._interfaceOpened ;
  _hInterfaceHandle = tok._hInterfaceHandle ;
  _class            = tok._class ;

  incNumero() ;
}

OB1Token::~OB1Token()
{
  if (_pWhere_I_am)
    _pWhere_I_am->removeToken(this) ;

  if (_pAttributesChange)
    delete (_pAttributesChange) ;
  if (_pLinkChange)
    delete (_pLinkChange) ;

  _lTokenCount-- ;
}

EventType
OB1Token::getEventType()
{
  return _type ;
}

AVPSet*
OB1Token::getAttributesChange()
{
  return _pAttributesChange ;
}

LinkSet*
OB1Token::getChangeLinks()
{
  return _pLinkChange ;
}

/*
** Return -42 if it not exist
** else return the number of the class
*/

int
OB1Token::Class()
{
  return _class ;
}

void
OB1Token::PutStorageNode(OB1Node* input)
{
	_pWhere_I_am = input ;
	if (_pObject && input && input->Controler())
  {
    std::string clasName = _pObject->Class()->Name() ;
    _class = input->Controler()->getClassName(clasName) ;
  }
}

Cycle
OB1Token::getCreatedCycle()
{
  return _pObject->CycleCreated() ;
}

BB1Object*
OB1Token::getStatusObject()
{
	if (NULL == _pObject)
		return (BB1Object*) 0 ;

	string sObjectKbName = _pObject->KB().Name() ;

  if (string("QuestionKB") == sObjectKbName)
		return getAnswerObject() ;

  if (string("InformationKB") == sObjectKbName)
  	return _pObject ;

	if (string("NautilusEventKB") == sObjectKbName)
  	return _pObject ;

	return (BB1Object*) 0 ;
}

BB1Object*
OB1Token::getAnswerObject()
{
	if (NULL == _pObject)
		return (BB1Object*) 0 ;

	TypedVal* pDispinf = _pObject->PathAttributes() ;
  if (NULL == pDispinf)
  	return (BB1Object*) 0 ;

  return _pObject->BB().searchAnswerInKB(*pDispinf, _pObject->SearchParams()) ;
}

bool
OB1Token::bIsValidInterface()
{
	if (0 == _hInterfaceHandle)
		return false ;

	return ::IsWindow(_hInterfaceHandle) ;
}

OB1Token&
OB1Token::operator=(const OB1Token& src)
{
	if (this == &src)
		return *this ;

  _type    = src._type ;
  _pObject = src._pObject ;

	if (_pLinkChange)
		delete _pLinkChange ;

	if (NULL == src._pLinkChange)
  	_pLinkChange = (LinkSet*) 0 ;
  else
  	_pLinkChange = new LinkSet(*(src._pLinkChange)) ;

	if (_pAttributesChange)
		delete _pAttributesChange ;

	if (NULL == src._pAttributesChange)
  	_pAttributesChange = (AVPSet*) 0 ;
  else
  	_pAttributesChange = new AVPSet(*(src._pAttributesChange)) ;

  _pWhere_I_am      = src._pWhere_I_am ;
  _pLauncherToken   = src._pLauncherToken ;
  _iTokenState      = src._iTokenState ;
  _interfaceOpened  = src._interfaceOpened ;
  _hInterfaceHandle = src._hInterfaceHandle ;
  _class            = src._class ;

  incNumero() ;

	return *this ;
}

