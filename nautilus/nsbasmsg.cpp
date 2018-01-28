// Fichier NsBasMsg.cpp : Définition des bases contenant les structures NSMsg du Mail
// RS Janvier 99
//////////////////////////////////////////////////////////////////////////////////////

#include <cstring.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\nsbasmsg.h"

//===========================================================================
// CLASSE NSMessage
//---------------------------------------------------------------------------
//  Classe NSMessageData
//---------------------------------------------------------------------------
NSMessageData::NSMessageData()
{
	// met les champs de donnees a zero

    memset(code,		 0,	MSG_CODE_LEN + 1);
    memset(msg_id, 		 0,	MSG_MSG_ID_LEN + 1);
    memset(content_type, 0,	MSG_CONTENT_TYPE_LEN + 1);
    memset(from,		 0,	MSG_FROM_LEN + 1);
    memset(subject,		 0,	MSG_SUBJECT_LEN + 1);
    memset(date,		 0,	MSG_DATE_LEN + 1);
    memset(heure,		 0,	MSG_HEURE_LEN + 1);
    memset(priority,	 0,	MSG_PRIORITY_LEN + 1);
    memset(delmsg,		 0,	MSG_DELMSG_LEN + 1);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageData::NSMessageData(const NSMessageData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMessageData::NSMessageData(const NSMessageData& rv)
{
   	strcpy(code,   			rv.code);
    strcpy(msg_id,    		rv.msg_id);
    strcpy(content_type,	rv.content_type);
    strcpy(from,			rv.from);
    strcpy(subject,			rv.subject);
    strcpy(date,			rv.date);
    strcpy(heure,			rv.heure);
    strcpy(priority,		rv.priority);
    strcpy(delmsg,			rv.delmsg);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageData::operator=(NSMessageData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSMessageData&
NSMessageData::operator=(NSMessageData src)
{
   	strcpy(code,   			src.code);
    strcpy(msg_id,    		src.msg_id);
    strcpy(content_type,	src.content_type);
    strcpy(from,			src.from);
    strcpy(subject,			src.subject);
    strcpy(date,			src.date);
    strcpy(heure,			src.heure);
    strcpy(priority,		src.priority);
    strcpy(delmsg,			src.delmsg);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageData::operator==(const NSMessageData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSMessageData::operator == ( const NSMessageData& o )
{

	if ((strcmp(code, 			o.code) 		== 0) &&
    	(strcmp(msg_id,			o.msg_id)		== 0) &&
        (strcmp(content_type,	o.content_type) == 0) &&
        (strcmp(from,			o.from)			== 0) &&
        (strcmp(subject,		o.subject)		== 0) &&
        (strcmp(date,			o.date)			== 0) &&
        (strcmp(heure,			o.heure)		== 0) &&
        (strcmp(priority,		o.priority)		== 0) &&
       	(strcmp(delmsg,			o.delmsg)		== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Classe NSMessage
//---------------------------------------------------------------------------

NSMessage::NSMessage(NSContexte* pCtx) : NSFiche(pCtx)
{
	// cree un objet de donnees
    pDonnees = new NSMessageData();
}

NSMessage::~NSMessage()
{
	delete pDonnees;
    pDonnees = 0;
}

// Ouverture et fermeture de la base des messages
//////////////////////////////////////////////////

DBIResult
NSMessage::open()
{
    char tableName[] = "MESSAGES.DB";
    //
    // Appelle la fonction open() de la classe de base pour ouvrir
    // l'index primaire
    //
    lastError = NSFiche::open(tableName, NSF_PERSONNEL);
    return(lastError);
}

// alimenteFiche
/////////////////////////////////////////////////////////////////////////

void
NSMessage::alimenteFiche()
{
	alimenteChamp(pDonnees->code,		  MSG_CODE_FIELD,		  MSG_CODE_LEN);
    alimenteChamp(pDonnees->msg_id,		  MSG_MSG_ID_FIELD,		  MSG_MSG_ID_LEN);
    alimenteChamp(pDonnees->content_type, MSG_CONTENT_TYPE_FIELD, MSG_CONTENT_TYPE_LEN);
	alimenteChamp(pDonnees->from,		  MSG_FROM_FIELD,		  MSG_FROM_LEN);
    alimenteChamp(pDonnees->subject,	  MSG_SUBJECT_FIELD,	  MSG_SUBJECT_LEN);
    alimenteChamp(pDonnees->date,		  MSG_DATE_FIELD,		  MSG_DATE_LEN);
    alimenteChamp(pDonnees->heure,		  MSG_HEURE_FIELD,		  MSG_HEURE_LEN);
    alimenteChamp(pDonnees->priority,	  MSG_PRIORITY_FIELD,	  MSG_PRIORITY_LEN);
    alimenteChamp(pDonnees->delmsg,		  MSG_DELMSG_FIELD,		  MSG_DELMSG_LEN);
}

// videFiche
/////////////////////////////////////////////////////////////////////////

void
NSMessage::videFiche()
{
  	videChamp(pDonnees->code,		  MSG_CODE_FIELD,		  MSG_CODE_LEN);
    videChamp(pDonnees->msg_id,		  MSG_MSG_ID_FIELD,		  MSG_MSG_ID_LEN);
    videChamp(pDonnees->content_type, MSG_CONTENT_TYPE_FIELD, MSG_CONTENT_TYPE_LEN);
	videChamp(pDonnees->from,		  MSG_FROM_FIELD,		  MSG_FROM_LEN);
    videChamp(pDonnees->subject,	  MSG_SUBJECT_FIELD,	  MSG_SUBJECT_LEN);
    videChamp(pDonnees->date,		  MSG_DATE_FIELD,		  MSG_DATE_LEN);
    videChamp(pDonnees->heure,		  MSG_HEURE_FIELD,		  MSG_HEURE_LEN);
    videChamp(pDonnees->priority,	  MSG_PRIORITY_FIELD,	  MSG_PRIORITY_LEN);
    videChamp(pDonnees->delmsg,		  MSG_DELMSG_FIELD,		  MSG_DELMSG_LEN);
}

//---------------------------------------------------------------------------
//  Classe NSMessageInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSMessageInfo::NSMessageInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMessageInfo::NSMessageInfo()
{
	// Crée l'objet de données
	pDonnees = new NSMessageData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageInfo::NSMessageInfo(NSMessage*)
//  Description:	Constructeur à partir d'un NSMessage
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMessageInfo::NSMessageInfo(NSMessage* pMessage)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSMessageData();
	//
	// Copie les valeurs du NSMessage
	//
	*pDonnees = *(pMessage->pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageInfo::~NSMessageInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMessageInfo::~NSMessageInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageInfo::NSMessageInfo(const NSMessageInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMessageInfo::NSMessageInfo(const NSMessageInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSMessageData();
	//
	// Copie les valeurs du NSTempubInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageInfo::operator=(NSMessageInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSMessageInfo&
NSMessageInfo::operator=(NSMessageInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMessageInfo::operator==(NSMessageInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSMessageInfo::operator == ( const NSMessageInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSMessageArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie
/*
NSMessageArray::NSMessageArray(NSMessageArray& rv)
               :NSMessageVector()
{
    if (!(rv.empty()))
        for (NSMessageIter i = rv.begin(); i != rv.end(); i++)
            push_back(new NSMessageInfo(*(*i)));
}

// fonction vider()
void
NSMessageArray::vider()
{
    if (empty())
        return;
    for (NSMessageIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// destructeur
NSMessageArray::~NSMessageArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSMessageArray::operator=(NSMessageArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSMessageArray&
NSMessageArray::operator=(NSMessageArray src)
{
	vider();
    if (!(src.empty()))
        for (NSMessageIter i = src.begin(); i != src.end(); i++)
            push_back(new NSMessageInfo(*(*i)));
	return *this;
}
      */
//===========================================================================
// CLASSE NSMsgPart
//---------------------------------------------------------------------------
//  Classe NSMsgPartData
//---------------------------------------------------------------------------

NSMsgPartData::NSMsgPartData()
{
	// met les champs de donnees a zero

   memset(message,		0,	PART_MESSAGE_LEN + 1);
   memset(code, 		0,	PART_CODE_LEN + 1);
   memset(stype,		0,	PART_STYPE_LEN + 1);
   memset(itype,		0,	PART_ITYPE_LEN + 1);
   memset(encoding,		0,  PART_ENCODING_LEN + 1);
   memset(fichier,		0,	PART_FICHIER_LEN + 1);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartData::NSMsgPartData(const NSMsgPartData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMsgPartData::NSMsgPartData(const NSMsgPartData& rv)
{
   	strcpy(message,   		rv.message);
    strcpy(code,    		rv.code);
    strcpy(stype,			rv.stype);
    strcpy(itype,			rv.itype);
    strcpy(encoding,		rv.encoding);
    strcpy(fichier,			rv.fichier);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartData::operator=(NSMsgPartData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSMsgPartData&
NSMsgPartData::operator=(NSMsgPartData src)
{
   	strcpy(message,   		src.message);
    strcpy(code,    		src.code);
    strcpy(stype,			src.stype);
    strcpy(itype,			src.itype);
    strcpy(encoding,		src.encoding);
    strcpy(fichier,			src.fichier);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartData::operator==(const NSMsgPartData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSMsgPartData::operator == ( const NSMsgPartData& o )
{

	if ((strcmp(message, 		o.message) 		== 0) &&
    	(strcmp(code,			o.code)			== 0) &&
        (strcmp(stype,			o.stype)		== 0) &&
        (strcmp(itype,			o.itype)		== 0) &&
        (strcmp(encoding,		o.encoding)		== 0) &&
       	(strcmp(fichier,		o.fichier)		== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Classe NSMsgPart
//---------------------------------------------------------------------------

NSMsgPart::NSMsgPart(NSContexte* pCtx) : NSFiche(pCtx)
{
	// cree un objet de donnees
    pDonnees = new NSMsgPartData();
}

NSMsgPart::~NSMsgPart()
{
	delete pDonnees;
    pDonnees = 0;
}

// Ouverture et fermeture de la base des templates
//////////////////////////////////////////////////

DBIResult
NSMsgPart::open()
{
    char tableName[] = "MSGPARTS.DB";
    //
    // Appelle la fonction open() de la classe de base pour ouvrir
    // l'index primaire
    //
    lastError = NSFiche::open(tableName, NSF_PERSONNEL);
    return(lastError);
}

// alimenteFiche
/////////////////////////////////////////////////////////////////////////

void
NSMsgPart::alimenteFiche()
{
	alimenteChamp(pDonnees->message,  PART_MESSAGE_FIELD,	PART_MESSAGE_LEN);
    alimenteChamp(pDonnees->code,	  PART_CODE_FIELD,		PART_CODE_LEN);
    alimenteChamp(pDonnees->stype,	  PART_STYPE_FIELD,		PART_STYPE_LEN);
    alimenteChamp(pDonnees->itype,	  PART_ITYPE_FIELD,		PART_ITYPE_LEN);
    alimenteChamp(pDonnees->encoding, PART_ENCODING_FIELD,	PART_ENCODING_LEN);
    alimenteChamp(pDonnees->fichier,  PART_FICHIER_FIELD,	PART_FICHIER_LEN);
}

// videFiche
/////////////////////////////////////////////////////////////////////////

void
NSMsgPart::videFiche()
{
  	videChamp(pDonnees->message,  PART_MESSAGE_FIELD,  PART_MESSAGE_LEN);
    videChamp(pDonnees->code,	  PART_CODE_FIELD,	   PART_CODE_LEN);
    videChamp(pDonnees->stype,	  PART_STYPE_FIELD,	   PART_STYPE_LEN);
    videChamp(pDonnees->itype,	  PART_ITYPE_FIELD,	   PART_ITYPE_LEN);
    videChamp(pDonnees->encoding, PART_ENCODING_FIELD, PART_ENCODING_LEN);
    videChamp(pDonnees->fichier,  PART_FICHIER_FIELD,  PART_FICHIER_LEN);
}

//---------------------------------------------------------------------------
//  Classe NSMsgPartInfo
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartInfo::NSMsgPartInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMsgPartInfo::NSMsgPartInfo()
{
	// Crée l'objet de données
	pDonnees = new NSMsgPartData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartInfo::NSMsgPartInfo(NSMsgPart*)
//  Description:	Constructeur à partir d'un NSMsgPart
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMsgPartInfo::NSMsgPartInfo(NSMsgPart* pMsgPart)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSMsgPartData();
	//
	// Copie les valeurs du NSMsgPart
	//
	*pDonnees = *(pMsgPart->pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartInfo::~NSMsgPartInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMsgPartInfo::~NSMsgPartInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartInfo::NSMsgPartInfo(const NSMsgPartInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMsgPartInfo::NSMsgPartInfo(const NSMsgPartInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSMsgPartData();
	//
	// Copie les valeurs du NSTempubInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartInfo::operator=(NSMsgPartInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSMsgPartInfo&
NSMsgPartInfo::operator=(NSMsgPartInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMsgPartInfo::operator==(NSMsgPartInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSMsgPartInfo::operator == ( const NSMsgPartInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSMsgPartArray ---------------------
//-------------------------------------------------------------------------

// constructeur copie
    /*
NSMsgPartArray::NSMsgPartArray(NSMsgPartArray& rv)
					  : NSMsgPartVector()
{
    if (!(rv.empty()))
        for (NSMsgPartIter i = rv.begin(); i != rv.end(); i++)
            push_back(new NSMsgPartInfo(*(*i)));
}

// fonction vider()
void
NSMsgPartArray::vider()
{
    if (empty())
        return;
    for (NSMsgPartIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// destructeur
NSMsgPartArray::~NSMsgPartArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSMsgPartArray::operator=(NSMsgPartArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSMsgPartArray&
NSMsgPartArray::operator=(NSMsgPartArray src)
{
	vider();
    if (!(src.empty()))
        for (NSMsgPartIter i = src.begin(); i != src.end(); i++)
            push_back(new NSMsgPartInfo(*(*i)));
    return *this;
}                */

// Fin du fichier NsBasMsg.cpp
