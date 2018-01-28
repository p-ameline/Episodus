#include "balise.h"

#define LABEL_PMED_MESSAGE "pmed_message"
#define ATTRIBUT_PMED_MESSAGE_SENDERID "senderid"

 #define LABEL_PMED_PATIENT "pmed_patient"
 #define ATTRIBUT_PMED_PATIENT_PATIENTID "patientid"

 #define LABEL_PMED_ADMINDATA "pmed_admindata"
 #define ATTRIBUT_PMED_ADMINDATA_SEXE "sexe"

 #define LABEL_PMED_HEALTHDATA "pmed_healthdata"

 #define LABEL_PMED_PROBLEM "pmed_problem"
 #define ATTRIBUT_PMED_PROBLEM_PROBLEMID "problemid"

 #define LABEL_PROBLEM_EVENT "problem_event"
 #define ATTRIBUT_PROBLEM_EVENT_TYPE "type"
 #define ATTRIBUT_PROBLEM_EVENT_RELATED_PB_ID "related_pb_id"
 #define ATTRIBUT_PROBLEM_EVENT_TITLE "title"
 #define ATTRIBUT_PROBLEM_EVENT_IMPORTANT "important"

 #define LABEL_PMED_CONTACT "pmed_contact"
 #define ATTRIBUT_PMED_CONTACT_CONTACTID "contactid"
 #define ATTRIBUT_PMED_CONTACT_TYPE_PRINCIPE "type_principe"

 #define LABEL_CONTACT_TYPE "contact_type"
 #define ATTRIBUT_CONTACT_TYPE_CONDITIONS "conditions"
 #define ATTRIBUT_CONTACT_TYPE_RENDEZ_VOUS "rendez_vous"

 #define LABEL_PMED_CONTACTELEMENT "pmed_contactelement"
 #define ATTRIBUT_PMED_CONTACTELEMENT_CONTACTELEMENTID "contactelementid"
 #define ATTRIBUT_PMED_CONTACTELEMENT_PROBLEMID "problemid"

 #define LABEL_PMED_CONTACTCODE "pmed_contactcode"
 #define ATTRIBUT_PMED_CONTACTCODE_POSITION "position"
 #define ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION "classification"
 #define ATTRIBUT_PMED_CONTACTCODE_CODE "code"
 #define ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS "classification_bis"
 #define ATTRIBUT_PMED_CONTACTCODE_CODE_BIS "code_bis"

bool Cbalise::parser ()
{
	if (this->parser_attributs ())
	{
		// creer sLabel dans la balise ou le remplacer par une reference vers le label correspondant a la balise
			if (this->sLabel == LABEL_PMED_MESSAGE) // pmed_message
			{
				// 1 attribut (senderid)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_senderid;
				nb_senderid = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
					if ((*i)->sLabel == ATTRIBUT_PMED_MESSAGE_SENDERID) // senderid
					{
						this->pMainValues->send_id = (*i)->sValue;
						nb_senderid ++;
					}
					else
						return false; // erreur (attribut incorrect)
				
				if (nb_senderid != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_PMED_PATIENT) // pmed_patient
			{
				// 1 attribut (patientid)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_patientid;
				nb_patientid = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
					if ((*i)->sLabel == ATTRIBUT_PMED_PATIENT_PATIENTID) // patientid
					{
						this->pMainValues->pat_id = (*i)->sValue;
						nb_patientid ++;
					}
					else
						return false; // erreur (attribut incorrect)
				
				if (nb_patientid != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_PMED_ADMINDATA) // pmed_admindata
			{
				// pour l'instant, 1 attribut (sexe)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_sexe;
				nb_sexe = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
					if ((*i)->sLabel == ATTRIBUT_PMED_ADMINDATA_SEXE) // sexe
						nb_sexe ++;
					else
						return false; // erreur (attribut incorrect)
				
				if (nb_sexe != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_PMED_HEALTHDATA) // pmed_healthdata
			{
				// pas d'attribut
				
				if (! this->vect_attr.empty())
					return false; // erreur (trop d'attributs)
				
			}
			else if (this->sLabel == LABEL_PMED_PROBLEM) // pmed_problem
			{
				// 1 attribut (problemid)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_problemid;
				nb_problemid = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
					if ((*i)->sLabel == ATTRIBUT_PMED_PROBLEM_PROBLEMID) // problemid
						nb_problemid ++;
					else
						return false; // erreur (attribut incorrect)
				
				if (nb_problemid != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_PROBLEM_EVENT) // problem_event
			{
				// entre 2 et 4 attributs (type, related_pb_id optionnel, title optionnel, important)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_type;
				int nb_related_pb_id;
				int nb_title;
				int nb_important;
				nb_type = 0;
				nb_related_pb_id = 0;
				nb_title = 0;
				nb_important = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
						if ((*i)->sLabel == ATTRIBUT_PROBLEM_EVENT_TYPE) // type
							nb_type ++;
						else if ((*i)->sLabel == ATTRIBUT_PROBLEM_EVENT_RELATED_PB_ID) // related_pb_id
							nb_related_pb_id ++;
						else if ((*i)->sLabel == ATTRIBUT_PROBLEM_EVENT_TITLE) // title
							nb_title ++;
						else if ((*i)->sLabel == ATTRIBUT_PROBLEM_EVENT_IMPORTANT) // important
							nb_important ++;
						else
							return false; // erreur (attribut incorrect)
				
				if (nb_type != 1|| nb_related_pb_id > 1 || nb_title > 1 || nb_important != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_PMED_CONTACT) // pmed_contact
			{
				// 2 attributs (contactid, type_principe)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_contactid;
				int nb_type_principe;
				nb_contactid = 0;
				nb_type_principe = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
						if ((*i)->sLabel == ATTRIBUT_PMED_CONTACT_CONTACTID) // contactid
							nb_contactid ++;
						else if ((*i)->sLabel == ATTRIBUT_PMED_CONTACT_TYPE_PRINCIPE) // type_principe
							nb_type_principe ++;
						else
							return false; // erreur (attribut incorrect)
				
				if (nb_contactid != 1|| nb_type_principe != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_CONTACT_TYPE) // contact_type
			{
				// 2 attributs (conditions, rendez_vous)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_conditions;
				int nb_rendez_vous;
				nb_conditions = 0;
				nb_rendez_vous = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
						if ((*i)->sLabel == ATTRIBUT_CONTACT_TYPE_CONDITIONS) // conditions
							nb_conditions ++;
						else if ((*i)->sLabel == ATTRIBUT_CONTACT_TYPE_RENDEZ_VOUS) // rendez_vous
							nb_rendez_vous ++;
						else
							return false; // erreur (attribut incorrect)
				
				if (nb_conditions != 1|| nb_rendez_vous != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_PMED_CONTACTELEMENT) // contactelement
			{
				// 2 attributs (contactelementid, problemid)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_contactelementid;
				int nb_problemid;
				nb_contactelementid = 0;
				nb_problemid = 0;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
						if ((*i)->sLabel == ATTRIBUT_PMED_CONTACTELEMENT_CONTACTELEMENTID) // contactelementid
							nb_contactelementid ++;
						else if ((*i)->sLabel == ATTRIBUT_PMED_CONTACTELEMENT_PROBLEMID) // problemid
							nb_problemid ++;
						else
							return false; // erreur (attribut incorrect)
				
				if (nb_contactelementid != 1|| nb_problemid != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else if (this->sLabel == LABEL_PMED_CONTACTCODE) // contactcode
			{
				// 5 attributs (position, classification, code, classification_bis, code_bis)
				
				if (this->vect_attr.empty())
					return false; // erreur (pas d'attribut)
				
				int nb_position;
				int nb_classification;
				int nb_code;
				int nb_classification_bis;
				int nb_code_bis;
				nb_position;
				nb_classification;
				nb_code;
				nb_classification_bis;
				nb_code_bis;
				
				for (Attr_iter i = this->vect_attr.begin(); i != this->vect_attr.end(); i++)
						if ((*i)->sLabel == ATTRIBUT_PMED_CONTACTCODE_POSITION) // position
							nb_position ++;
						else if ((*i)->sLabel == ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION) // classification
							nb_classification ++;
						else if ((*i)->sLabel == ATTRIBUT_PMED_CONTACTCODE_CODE) // code
							nb_code ++;
						else if ((*i)->sLabel == ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS) // classification_bis
							nb_classification_bis ++;
						else if ((*i)->sLabel == ATTRIBUT_PMED_CONTACTCODE_CODE_BIS) // code_bis
							nb_code_bis ++;
						else
							return false; // erreur (attribut incorrect)
				
				if (nb_position != 1|| nb_classification != 1 || nb_code != 1 || nb_classification_bis != 1 || nb_code_bis != 1)
					return false; // erreur (nb attributs incorrect)

			}
			else
				return false; // erreur (label invalide)

		if (this->parser_valeurs ())
		{
			
		}
		else
			return false; // erreur (parser_valeurs a echoue)
	}
	else
		return false; // erreur (parser_attributs a echoue)

	return true;
}

