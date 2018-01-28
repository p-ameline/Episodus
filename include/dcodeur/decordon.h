#ifndef __DECODER_H#define __DECODER_H

#include "dcodeur\nsdkd.h"

class _NSDKDCLASSE decodeMediAdmin : public decodageBase
{
    public:

        string voieAdmin;
        string lieuAdmin;

        // Méthodes
        //
        decodeMediAdmin(NSContexte* pCtx, string sLangue);
        decodeMediAdmin(decodageBase* pBase);

        void decode(int colonne);
};

class _NSDKDCLASSE decodeMediDose : public decodageBase
{
    public:

        string dose;

        // Méthodes
        //
        decodeMediDose(NSContexte* pCtx, string sLangue);
        decodeMediDose(decodageBase* pBase);

        void decode(int colonne);
};

//-----------------------------------------------------------------
// classe decodeOrdonnance
//
// Classe de décodage des ordonnance
//-----------------------------------------------------------------
class _NSDKDCLASSE decodeOrdonnance : public decodageBase
{
    public:

        decodeOrdonnance(NSContexte* pCtx, string sLangue) : decodageBase(pCtx, sLangue) {}
        decodeOrdonnance(decodageBase* pBase)   : decodageBase(pBase) {}
};

class _NSDKDCLASSE decodeMedicament : public decodageBase
{
    public:

        decodeMediAdmin* pAdministration;
        gereNum*         pDose;
        string           sUniteAnthropo;
        string           sMedicament;

        decodeMedicament(NSContexte* pCtx, string sLangue);
        decodeMedicament(decodageBase* pBase);

        ~decodeMedicament();

        void setMedicament(string sMedic) { sMedicament = sMedic; }

        void decode(int colonne);
        void donnePhrase();
};

#endif  // __DECODER_H

