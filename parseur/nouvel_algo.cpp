#include <string>
#include <stdio.h>

#include "balise.h"
#include "nsdivfct.h"
#include "ParseError.h"

bool
Cbalise::parser_valeurs()
{
  cleanValeurs() ;

  size_t tag_start    = _sValeurs.find ('<') ;
  size_t last_tag_end = _sValeurs.rfind('>') ;

	if ((string::npos == tag_start) && (string::npos == last_tag_end))
	  // il n y a pas de < dans la chaine
		return true ;

  if (string::npos == last_tag_end)
  {
	  // il n y a pas de > dans la chaine
    _iParsingError = EVAL_NOCLOSE ;
    return false ;
  }

  string xmlstring = string(_sValeurs, 0, last_tag_end + 1) ;

  while (string("") != xmlstring)
	{
    tag_start = xmlstring.find('<') ;
    if (string::npos == tag_start)
    {
      // il manque un caractere de balise ouvrante : a verifier ici pourquoi on ne renvoi pas true ?!
      _iParsingError = EVAL_NOOPEN ;
      return false ;
    }

    if (tag_start > 0)
      xmlstring = string(xmlstring, tag_start, xmlstring.length () - tag_start) ;

    size_t attributes_end = xmlstring.find('>') ;
    if (string::npos == attributes_end)
    {
      // il manque un caractere de balise ouvrante ('<' sans '>')
      _iParsingError = EVAL_NOOPEN ;
      return false ;
    }

    // Commentaire
    //
    if ((attributes_end > 3) && (string(xmlstring, 0, 4) == string("<!--")))
    {
      attributes_end = xmlstring.find("-->") ;
      if (string::npos == attributes_end)
      {
        // il manque la balise de fin de commentaire
        _iParsingError = EVAL_CLOSEBALISE ;
        return false ;
      }
      if (attributes_end + 3 < strlen(xmlstring.c_str()))
        xmlstring = string(xmlstring, attributes_end + 3, strlen(xmlstring.c_str()) - attributes_end - 3) ;
      else
        xmlstring = string("") ;
    }
    else
    {
      size_t values_end = 0 ;
      bool   bTest ;

      if ('/' == xmlstring[attributes_end - 1])
      {
        attributes_end-- ;
        bTest = false ;
      }
      else
      {
        bTest = true ;
      }

      size_t label_end = xmlstring.find (' ') ;
      if ((string::npos == label_end) || (label_end > attributes_end))
        label_end = attributes_end ;

      if (1 == label_end)
      {
        // il y a un espace juste apres le debut de balise
        _iParsingError = EVAL_SPACE ;
        return false ;
      }

      string sLabel = string(xmlstring, 1, label_end - 1) ;

      string sAttributes = string("") ;

      if (attributes_end == label_end)
        sAttributes = string("") ;
      else
      {
        sAttributes = string(xmlstring, label_end + 1, attributes_end - label_end - 1) ;
        strip(sAttributes, stripBoth, ' ') ;
      }

      string sValues = string("") ;

      if ('/' == xmlstring[attributes_end])
        sValues = string("") ;
      else
      // c est ici qu il faut rajouter le code de test des balises ouvrantes et fermantes
      {
        int    iCount = 1 ;
        size_t iClose = 0 ;
        size_t pos    = attributes_end ;

        while ((0 != iCount) && (true == bTest))
        {
          size_t iOpen ;

          size_t open1 = xmlstring.find("<" + sLabel + " ", pos) ;
          size_t open2 = xmlstring.find("<" + sLabel + ">", pos) ;
          if      (string::npos == open1)
            iOpen = open2 ;
          else if (string::npos == open2)
            iOpen = open1 ;
          else
            iOpen = min(open1, open2) ;

          iClose = xmlstring.find("</" + sLabel + ">", pos) ;

          if (string::npos == iClose)
          {
            _iParsingError = EVAL_CLOSE ;
            bTest = false ;
          }
          else if ((iClose < iOpen) || (string::npos == iOpen))
          {
            // la premiere balise trouvee est une balise fermante
            iCount-- ;
            pos = iClose + 1 ;
          }
          else
          {
            // la premiere balise trouvee est une balise ouvrante
            // il y a deux possibilites :
                        // - c'est une balise ouvrante-fermante
                        // - c'est une balise ouvrante
            //
            size_t end_tag = xmlstring.find(">" , iOpen + 1) ;
            if (string::npos == end_tag)
            {
              _iParsingError = EVAL_CLOSEBALISE ;
              bTest = false ;
            }
            else
            {
              if ('/' != xmlstring[end_tag-1])
                iCount++ ;
              pos = iOpen + 1 ;
            }
          }
        }

        if (false == bTest)
          return false ;
        else
        {
          values_end = iClose ;
          if (attributes_end > values_end)
          {
            _iParsingError = EVAL_DEF ;
            return false ;
          }
          sValues = string(xmlstring, attributes_end + 1, values_end - attributes_end - 1);
        }
      }
#if !defined _OWLDLL
	  // cerr <<"label : " <<label <<endl;
	  // cerr <<"attributes : " <<attributes <<endl;
	  // cerr <<"values : " <<values <<endl;
#endif

      vect_val.push_back(new CValeur(sLabel, sValues, sAttributes, this)) ;

      if ('/' == xmlstring[attributes_end])
        xmlstring = string (xmlstring, attributes_end + 2, strlen(xmlstring.c_str()) - attributes_end - 2) ;
      else
        xmlstring = string (xmlstring, values_end + strlen(sLabel.c_str()) + 3, strlen(xmlstring.c_str()) - values_end - strlen(sLabel.c_str()) - 3) ;
      strip (xmlstring, stripBoth, ' ') ;
    }
  }

	return true ;
}

/*
bool
Cbalise::parser_valeurs()
{
  string copie;
  string tampon;
  string BaliseEnCours;
  string entete;
  string fin;
  string attribut;

  int counter1 = 0;
  int counter2 = 0;
  int counter_char = 0;
  int counter_fermeture = 0;
  int counter_ouverture = 0;
  int pos_fermeture = 0;
  int pos_ouverture = 0;
  int reperage = 0;
  int value_debut;
  int value_fin;

  bool reponse = true;
  bool boucle1 = true;
  bool boucle2 = true;
  bool test = true;

  copie = sValeurs;
  strip (copie, stripBoth, ' ');

  // on teste si la string est vide
  if (copie == "")
    return true;

  // on teste si le premier caractere de la string est different de <
  if (copie[0] != '<')
    return false;

  // le caractere juste apres la balise ouvrante est un espace d ou erreur de nomencalture
  if (copie[1] == ' ')
    return false;

  while (boucle1 == true) // tant que copie est different de la chaine vide
    {
      // la boucle1 est la boucle qui indique qu on parse toute la string
      // ceci veut dire tant que copie != ""

      copie = string(copie, 1, strlen(copie.c_str()));
      counter1 = copie.find('>');

      if (counter1 == string::npos)
	{
	  // il n y a pas dans la string de fermeture de balise

	  boucle1 = false;
	  boucle2 = false;
	  reponse = false;
	}
      else
	{
	  // il y a bien un indicateur de fermeture de balise

	  counter2 = copie.find(' ');

	  if  (counter2 == string::npos)
	    {
	      // il n y a plus de caractere espace dans la chaine
	      // ce n est pas une mauvaise nomenclature mais cela signifi
	      // qu il n y a plus d attribut dans la chaine
	      // exemple : <entete1><entete2></entete2></entete1>

	      entete = string(copie, 0, (counter1 - 1));
	      entete = '<' + entete;
	      fin = "</" + entete + '>';

	      copie = string(copie, (counter1 + 1), strlen(copie.c_str()));
	      strip (copie, stripBoth, ' ');
	      sAttributs = "";

	      counter_ouverture = 1;
	      boucle2 = true;
	    }
	  else if (counter2 < counter1)
	    {
	      // c est a peu pres le meme cas qu au dessus
	      // il y a un espace mais apres la fin de fermeture de balise
	      // ex <entete1><entete2 id=123></entete2></entete1>

	      entete = string(copie, 0, (counter1 - 1));
	      entete = '<' + entete;
	      fin = "</" + entete + '>';

	      copie = string(copie, (counter1 + 1), strlen(copie.c_str()));
	      strip (copie, stripBoth, ' ');
	      sAttributs = "";

	      counter_ouverture = 1;
	      boucle2 = true;
	    }
	  else if (counter2 > counter1)
	    {
	      // c est le cas ou dans la balise de niveau 1
	      // il y a un attribut
	      // ex <entete1 machin=truc><entete2></entete2></entete1>

	      entete = string(copie, 0, (counter2 - 1));
	      entete = '<' + entete;
	      fin = "</" + entete + '>';

	      // la mise en place des attributs

	      attribut = string(copie, counter2, (counter1 - 1));
	      strip (attribut, ' ');
	      sAttributs = attribut;

	      copie = string(copie, (counter1 + 1), strlen(copie.c_str()));
	      strip (copie, stripBoth, ' ');

	      counter_ouverture = 1;
	      boucle2 = true;
	    }
	} // fin du else il y a bien un indicateur de fin de balise

      // on est dans la boucle si la string est differente de la chaine vide

      if (boucle1 == true)
	{
	  // on ne test ici que si on a une balise ouvrante valide

	  tampon = copie;
	  while (boucle2 == true)
	  {
	    pos_fermeture = tampon.find (fin);

	    if (pos_fermeture == string::npos)
	      {
		// il n y a pas de balise de fermeture dans la string

		boucle1 = false; // on ferme la boucle1 car le message est faux
		boucle2 = false; // de meme la boucle2 de compteur est fausse
		reponse = false;
	      }
	    else
	      {
		// il y a au moins une balise de fermeture dans la string

		pos_ouverture = tampon.find (entete);
		if (pos_ouverture == string::npos)
		  {
		    // il n y a plus dans la string des occurences du
		    // style rencontre <entete...
		    // et alors ? rien !!
		    // ceci est du a un changement d algo !!!
		  }
		else
		  {
		    // on est dans le cas ou il y a
		    // une balise ouvrante et une fermante

		    if (pos_fermeture < pos_ouverture)
		      {
			// on est dans le cas de desincrementation

			counter_ouverture--;
			if (counter_ouverture == 0)
			  {
			    // on est dans le cas ou la recherche est terminee

			    sValeurs = string (copie, counter_ouverture, counter_fermeture);
			    vect_val.push_back (new CValeur(entete, sValeurs, sAttributs));
			    boucle2 = false;
			  }
			else
		      	  {
			    // il reste encore des balises ouvrantes

			    counter_char += (pos_fermeture + 1) + strlen(fin.c_str());
                            tampon = string(tampon, (pos_fermeture + strlen(fin.c_str())), strlen(tampon.c_str()));
			  }
		      }
		    else
		      {
			// on est dans l incrementation
			// il faut verifier que l on a pas un / de fermeture

			// reperer la taille de la chaine de balise ouvrante
			// a extraire ainsi que le counter_char

			reperage = 0;
			reperage = tampon.find('>', pos_ouverture);

			if (reperage == string::npos)
			  {
			    // il n y a pas de caractere de fermeture

			    boucle2 = false;
			    boucle1 = false;
			    reponse = false;
			  }
			else if (tampon[reperage - 1] == '/')
			  {
			    // on est dans le cas de la balise
			    // ouvrante et fermante
			    // il reste a gerer ce cas
			  }
			else
			  {
			    // il y a un caratere de fermeture de balise
			    // la longueur de la chaine est reperage + 1

			    counter_ouverture++;
			    reperage += 1;

			    // la chaine a retirer est :
			    // la chaine avant la balise + la balise

			    reperage += pos_ouverture;

			    counter_char += reperage;
			    tampon = string(tampon, reperage, strlen(tampon.c_str()));
			  }
		      }
		  }
	      }
	  }
		 // on est encore dans la boucle while (boucle2 == 0))
		 //		 }

		 } // fin de la boucle while (boucle2 == 0)
	  strip (copie, stripBoth, ' ');
	  if (copie == "")
	    {
	      // on a termine le parsing de la string

	      boucle1 == false;
	    }
    }
}
*/

