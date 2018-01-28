#ifndef __NSDOCHIS_H#define __NSDOCHIS_H

#include "partage\ns_vector.h"
#include "nsdn\nsdocum.h"

// #include "nsdn\nsdocnoy.h"
// #include "nsbb\nspatpat.h"

//document + sa patpatho

class NSDocumentHisto : public NSDocumentInfo{
	public:

		// pointeur pDocNoy de stockage des documents (ne pas faire de new)		NSNoyauDocument* pDocNoy ;
		NSPatPathoArray* pPatPathoArray ;
		char             dateDoc[DOC_CREATION_LEN + 1] ;

		//constructeurs  Destructeur		NSDocumentHisto(NSContexte* pSuper) ;
		NSDocumentHisto(NSDocumentInfo* pNSDocument, bool bNoDocnoy /* fictif : pour distinguer les ctors */) ;
		NSDocumentHisto(NSDocumentInfo* pNSDocumentInfo, NSNoyauDocument* pDoc = 0) ;
		~NSDocumentHisto() ;

		//copie		NSDocumentHisto(NSDocumentHisto& rv) ;
    NSDocumentHisto& operator=(NSDocumentHisto src) ;

		// Surcharges des opérateurs    int operator == ( const NSDocumentHisto& o ) ;

		char* GetDateDoc() ;    void 	TrouveDateDoc() ;
    void 	ResetDateDoc() ;

    bool  checkPptConsistency() ;
};

//// Définition de NSDocumenHistotArray (Array de NSDocumentHisto)
//

typedef vector<NSDocumentHisto*> NSDocumentHistoArray ;typedef NSDocumentHistoArray::iterator DocumentIter ;
//Reverse_iter on  NSDocumentHistoArraytypedef NSDocumentHistoArray::reverse_iterator DocumentReverseIter ;
class NSDocHistoArray : public NSDocumentHistoArray{
public :

	// Constructeurs	NSDocHistoArray() : NSDocumentHistoArray() {}
	NSDocHistoArray(NSDocHistoArray& rv) ;

	// Destructeur	virtual ~NSDocHistoArray() ;
  void vider() ;

	DocumentIter TrouveDocHisto(const string codeDocBrut) ;};

DocumentIter
NSDocHistoArray::TrouveDocHisto(const string codeDocBrut)
{
	if (true == empty())
		return NULL ;

	DocumentIter iterNode = begin() ;
	for (; iterNode != end(); iterNode++)
	{
		string codeDoc = (*iterNode)->getID() ;
		string codeDocMeta = (*iterNode)->sCodeDocMeta ;
		if ((codeDocBrut == codeDoc) || (codeDocBrut == codeDocMeta))
			return iterNode ;
	}
	return end() ;
}

#endif
