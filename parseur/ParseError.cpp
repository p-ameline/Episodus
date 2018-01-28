#if defined _OWLDLL
    #include <vector.h>
#else
    #include <stl.h>
#endif

#include <string>
#include "ParseError.h"

// variable globale
//int iParsingError;

string parseErrorMessage(int iError)
{
  switch (iError)
    {

    case EA_NOERROR : return string ("il n'y pas d'erreur constatée"); 
    case EATTR_EMPTY : return string("la chaine des attributs est vide"); 
    case EATTR_NOEQUAL : return string("la chaine des attributs ne comporte pas de signe egal"); 
    case EATTR_EQUAL0 : return string("la chaine des attributs a un signe egal comme premier caractere");
    case EATTR_EQUALLAST : return string("la chaine des attributs a un signe egal mais en derniere position");


    case EATTR_QUOTELAST : return string("la chaine des attributs a un caratere \' mais en derniere position");
    case EATTR_QUOTECLOSE : return string("la chaine des attributs ne possede pas de caractere \' de fermeture");
    case EATTR_QUOTEFIN : return string("la chaine des attributs possede un caractère \' mais c est la fin de chaine");
    case EATTR_QUOTESPACE : return string("la chaine des attributs n a pas de caractere espace derriere le \'");


    case EATTR_DQUOTELAST : return string("la chaine des attributs a un caractere \" mais en derniere position ");
    case EATTR_DQUOTECLOSE : return string("la chaine des attributs ne possede pas de caractere \" de fermeture");
    case EATTR_DQUOTEFIN : return string("la chaine des attributs possede un caractere \" mais c est la fin de chaine");
    case EATTR_DQUOTESPACE : return string("la chaine des attributs n a pas de caractere espace derriere le \"");


    case EVAL_NOCLOSE : return string("la chaine n a pas d indicateur de fin de balise : >");
    case EVAL_NOOPEN : return string("il manque un caractere de balise ouvrante : < dans la chaine des valeurs");
    case EVAL_SPACE : return string("il y a un caractere espace juste apres l ouverture de la balise");
    case EVAL_CLOSE : return string("il n y a pas ou il manque une balise de fermeture");
    case EVAL_CLOSEBALISE : return string("il manque un caractere de fermeture de balise : >");
    case EVAL_DEF : return string("la fin de la chaine des attributs de la balise est mal definie ");

    case EPMED_ADMIN_DATA_ATTR : return string("le(s) attribut(s) de pmed_admindata est (sont) incorrect(s) ");
    case EPMED_ADMIN_DATA_NB : return string("le nombre d attributs de pmed_admindata est incorrect ");
    case EPMED_ADMIN_DATA_VAL : return string("pmed_admin erreur : pas de valeur attendue");
    case EPMED_ADMIN_DATA_SEXE : return string("le champ sexe de pmed_admindata doit prendre la valeur 1 ou 2");

    case EPMED_CONTACT_ATTR : return string("le(s) attributs de pmed_contact est (sont) incorrect(s) ");
    case EPMED_CONTACT_NB : return string("le nombre d attributs de pmed_contact est incorrect ");
    case EPMED_CONTACT_VAL : return string("le champs valeur de pmed_contact est incorrect ");
    case EPMED_CONTACT_NBVAL : return string("le nombre de valeurs de pmed_contact est incorrect ");

    case EPMED_CONTACT_TYPE_ATTR : return string("le(s) attributs de pmed_contact_type est (sont) incorrect(s) ");
    case EPMED_CONTACT_TYPE_NB : return string("le nombre d attributs de pmed_contact_type est incorrect ");
    case EPMED_CONTACT_TYPE_VAL : return string("pmed_contact_type erreur : pas de valeur attendue");
    case EPMED_CONTACT_TYPE_CONDITIONS : return string("le champs conditions de pmed_contact_type doit prendre la valeur C ou V");
    case EPMED_CONTACT_TYPE_RENDEZ_VOUS : return string("le champs rendez_vous de pmed_contact_type doit prendre l une des valeurs O, N, ou U");
    case EPMED_CONTACT_CODE_ATTR : return string("le(s) attributs de pmed_contactcode est (sont) incorrect(s) ");
    case EPMED_CONTACT_CODE_NBATTR : return string("le nombre d attributs de pmed_contactcode est incorrect ");
    case EPMED_CONTACT_CODE_VAL : return string("pmed_contactcode erreur : pas de valeur attendue");
    case PMED_CONTACT_CODE_POSITION : return string("le champs position de pmed_contactcode doit prendre l'une des valeurs M, S, O, 1, A, AC ou AI ");
    case PMED_CONTACT_CODE_CLASSIFICATION : return string("le champs classification de pmed_contactcode doit prendre l'une des valeurs CISP2, SFMG, CIM10 ou CDAM ");
    case PMED_CONTACT_CODE_CLASSIFICATION_BIS : return string("le champs classification_bis de pmed_contactcode doit prendre l'une des valeurs CISP2, SFMG, CIM10 ou CDAM ");

    case EPMED_CONTACT_ELT_ATTR : return string("le(s) attributs de pmed_contact_element est (sont) incorrect(s) "); 
    case EPMED_CONTACT_ELT_NBATTR : return string("le nombre d'attributs de pmed_contact_element est incorrect ");
    case EPMED_CONTACT_ELT_VAL_FALSE : return string("le champ valeur de pmed_contact_element est incorrect ");
    case EPMED_CONTACT_ELT_VAL : return string("le nombre de  valeur(s)  de pmed_contact_element est incorrect ");
    case EPMED_CONTACT_PRINCIPE : return string("le champ type_principe de pmed_contact doit prendre la valeur SOAP ou SFMG ou OTHER");

    case EPMED_HEALTH_DATA_EMPTY : return string("la chaine des attributs de pmed_healtdata est vide ");
    case EPMED_HEALTH_DATA_VAL : return string("le champs valeur de  pmed_healthdata est incorrect "); 

    case EPMED_MESSAGE_EMPTY : return string("la chaine des attributs de pmed_message est vide ");
    case EPMED_MESSAGE_ATTR : return string("le(s) attributs de pmed_message est (sont) incorrect(s) ");
    case EPMED_MESSAGE_NBATTR : return string("le nombre d'attributs de pmed_message est incorrect ");
    case EPMED_MESSAGE_VAL_EMPTY : return string("erreur le champ valeur de pmed_message est vide ");
    case EPMED_MESSAGE_VAL : return string("le champ valeur de pmed_message est incorrect ");
    case EPMED_MESSAGE_NBVAL : return string("le nombre de valeurs de pmed_message est incorrect ");

    case EPMED_PATIENT_ATTR_EMPTY : return string("la chaine des attributs de pmed_patient est vide ");
    case EPMED_PATIENT_ATTR : return string("le(s) attributs de pmed_patient est (sont) incorrect(s) ");
    case EPMED_PATIENT_NBATTR : return string("le nombre d'attributs de pmed_patient est incorrect ");
    case EPMED_PATIENT_VAL : return string("le champ valeur de pmed_patient est incorrect ");
    case EPMED_PATIENT_NBVAL : return string("le nombre de valeurs de pmed_patient est incorrect ");

    case EPMED_PROBLEM_ATTR_EMPTY : return string("la chaine des attributs de pmed_problem est vide ");
    case EPMED_PROBLEM_ATTR : return string("le(s) attributs de pmed_problem est (sont) incorrect(s) ");
    case EPMED_PROBLEM_NBATTR : return string("le nombre d'attributs de pmed_problem est incorrect ");
    case EPMED_PROBLEM_VAL : return string("le champ valeur de pMed_problem est incorrect ");

    case EPMED_PROBLEM_EVENT_ATTR_EMPTY : return string("la chaine des attributs de pmed_problem_event est vide ");
    case EPMED_PROBLEM_EVENT_ATTR : return string("le(s) attributs de pmed_problem_event est (sont) incorrect(s) ");
    case EPMED_PROBLEM_EVENT_NBATTR : return string("le nombre d'attributs de pmed_problem_event est incorrect ");
    case EPMED_PROBLEM_EVENT_VAL : return string("erreur : il ne devrait pas y avoir de champ valeur dans pmed_problem_event ");
    case EPMED_PROBLEM_EVENT_TYPE : return string("le champ type de pmed_problem_event doit prendre l une des valeurs : open, modify ou close ");
    case EPMED_PROBLEM_EVENT_IMPORTANT : return string("le champ important de pmed_problem_event doit prendre la valeur Oui ou Non ");
    case EMESSAGE_EMPTY : return string("le message est vide ");
    case EMESSAGE_BALISE_FIN : return string("le message ne possède pas de balise de fin ");
    case EMESSAGE_BALISE_DEBUT_FERMANTE : return string("le message ne possède pas de balise debut fermante ");
    case EMESSAGE_BALISE_DEBUT : return string("le message ne possède pas de balise de début ");
 
    default : return string("erreur inconnue");
    }
}
