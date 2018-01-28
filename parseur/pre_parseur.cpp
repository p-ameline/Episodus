// -----------------------------------------------------------------------------
// pre prasing de la dtd
// -----------------------------------------------------------------------------

#include <string>
#include <stdio.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nsdivfct.h" // pour les fonctions strip ...
#endif

#include "ParseError.h"

int checkBalise(string*, size_t, size_t) ;


// -----------------------------------------------------------------------------
// pre_parseur la string sera modifie remplacant plusieurs carateres espaces se
// suivant par un seul , ainsi que tabulations et retour a la ligne
// -----------------------------------------------------------------------------
bool
pre_parseur(string* pointeur)
{
  string dtd, buffer, la_chaine ;
//  int tag_start, count, answer ;
  int answer ;
  bool test = true ;

  dtd = *pointeur ;
  size_t count = 0 ;

  size_t tag_start = dtd.find('<') ;
  if (tag_start == string::npos)
  {
    // il n'y a pas de caractere de balise ouvrante < dans la chaine
#if !defined FOR_WINDOWS
    cerr << "erreur 9" << endl ;
#endif
    return false ;
  }

  strip(dtd, stripBoth, ' ') ;

  size_t iTaille = strlen(dtd.c_str()) ;
  while ((count != iTaille) && (test == true))
  {
    if (dtd[count] == '<')
    {
      answer = checkBalise(&dtd, count, iTaille) ;
      if (answer == -1)
      {
        // le contenu de la balise est incorrect si answer = -1 sinon elle renvoi la position du caractere de fin de balise
        test = false ;
#if !defined FOR_WINDOWS
        cerr << "erreur 10" << endl ;
#endif
      }
      else
      {
        count = answer ;
      }
    }
    else
    {
      // le caractere pointe n est pas un <
      // a voir ici si on doit implement des erreurs de semantiques
      count++ ;
    }
  }

  if (test == false)
  {
    // il y a eu une erreur dans l'ecriture d'une balise
#if !defined FOR_WINDOWS
    cerr << "erreur 11" << endl ;
#endif
    return false ;
  }
  else
    return true ;
}


int
checkBalise(string* dtd, size_t counter, size_t iTaille)
{
  // cette fonction verifie si la semantique a l'interieur d'une balise est correct
  // elle renvoie -1 si incorrect et la position du caractere de fin de balise > si correct
//  int baliseEnd, doubleQuote, Quote, counter_pos, tagStart, tagEnd, lgString ;
  int tagEnd, lgString ;

  int comment ;
  string buffer, concat ;
  bool boucle, answerBoucle ;

  size_t tagStart        = counter + 1 ;
  boucle          = true ;
  answerBoucle    = false ;

  if (((*dtd)[counter + 1] == ' ') || ((*dtd)[counter + 1] == '\t') ||
      ((*dtd)[counter + 1] == '\r') || ((*dtd)[counter + 1] == '\n'))
  {
    // il n'y a pas d'espace juste derriere le caractere d'ouverture de balise
    return -1 ;
  }

  bool bComment = false ;
  if ((tagStart + 3 < iTaille) && (string(*dtd, tagStart, 3) == "!--"))
  {
    bComment    = true ;
    comment     = tagStart ;

    size_t baliseEnd = dtd->find('>', counter) ;
    if (baliseEnd == string::npos)
    {
      boucle          = false ;
      answerBoucle    = false ;
    }
    else
    {
      size_t commentEnd = dtd->find("--", comment + 3) ;
      if (commentEnd == string::npos)
      {
        // commentaire non refermé
        boucle = false ;
        answerBoucle = false ;
      }
      else if (commentEnd == baliseEnd - 2)
      {
        // commentaire valide
        counter = baliseEnd ;
        boucle = false ;
        answerBoucle = true ;
      }
      else
      {
        // commentaire invalide
        boucle = false ;
        answerBoucle = false ;
      }
    }
  }

  bool bOuvert = false ;
  char cOpenChar ;

  answerBoucle = false ;

  while (boucle == true)
  {
    if (!bOuvert)
    {
      if ((*dtd)[counter] == '>')
      {
        boucle          = false ;
        answerBoucle    = true ;
      }
      else if (((*dtd)[counter] == '\'') || ((*dtd)[counter] == '\"'))
      {
        bOuvert     = true ;
        cOpenChar   = (*dtd)[counter] ;
      }
    }
    else if ((*dtd)[counter] == cOpenChar)
      bOuvert = false ;

    if (boucle)
    {
      counter++ ;
      if (counter >= iTaille)
        boucle = false ;
    }
  }

  // si on a bien detecter une balise de la forme < > regarder si l interieur de la balise est valide
  if (answerBoucle == false)
  {
    return -1 ;
#if !defined FOR_WINDOWS
    cerr << "erreur 6" << endl ;
#endif
  }
  // cas des balises non commentaires
  else if (!bComment)
  {
    // tagStart correspond au premier caractere a l interieur de la balise
    tagEnd = counter - 1 ;
    // tagEnd correspond a l element precedent la fin de balise : >

    lgString = tagEnd - tagStart + 1 ;
    if (lgString < 1)
    {
      // la chaine est réduite à l'element vide d'où erreur
      return -1 ;
    }
    else
    {
      buffer = string(*dtd, tagStart, lgString) ;
      strip(buffer, stripBoth, ' ') ;

      size_t space = buffer.find(' ') ; // space = trouve_espace (buffer,counter)
      if (space == string::npos)
      {
        // c est le cas d une balise sans attribut
        return counter ;
      }
      else
      {
        // c est le cas d une balise avec attributs
        size_t equal = buffer.find('=') ;
        if (equal == string::npos) // attention risque ici d avoir <label /> voir dans le livre !!
        {
          // erreur on a une balise comportant un espace mais sans attribut
          return -1 ;
#if !defined FOR_WINDOWS
          cerr << "erreur 7" << endl ;
#endif
        }
        else
        {
          if (equal < space)
          {
            // erreur le = espace est situe avant le premier espace
            return -1 ;
#if !defined FOR_WINDOWS
            cerr << "erreur 8" << endl ;
#endif
          }
          else
          {
            // cas d'une balise correcte avec attribut
            return counter ;
          }
        }
      }
    }
  }
  else
  {
    // cas des commentaires : sont nécessairement valides ici
    return counter ;
  }
}

/*
int
trouve_space(string* chaine, size_t counter)
{
  string la_modif ;
  string reponse ;
  string tempon ;

  int lengh ;
  int space ;
  int i ;
  int mange = counter ;
  char caractere ;
  bool tabulation = false ;
  bool quote = false ;
  bool double_quote = false ;

  la_modif = *chaine ;

  lengh = strlen(la_modif.c_str()) ;

  for (i = 0 ; i < lengh ; i++)
  {
    caractere = la_modif[i] ;

    if ((caractere == ' ') || (caractere == '\n') || (caractere == '\t') || (caractere == '\r'))
	  {
	    if ((quote == false) && (double_quote == false))
	    {
	      if (space == false)
        {
          space = true ;
          tempon = tempon + ' ' ;
        }
	      else
        {
          space-- ;
        }
	    }
	    else
	    {
	      tempon = tempon + ' ' ;
	    }
	  }
    else if (caractere == '\'')
	  {
	    if (quote == true)
	    {
	      quote = false ;
	      tempon = tempon + caractere ;
	    }
	    else
	    {
	      tempon = tempon + caractere ;
	      quote = true ;
	    }
    }
    else if (caractere == '\"')
    {
      if (double_quote == true)
      {
        double_quote = false ;
        tempon = tempon + caractere ;
      }
      else
      {
        double_quote = true ;
        tempon = tempon + caractere ;
      }
    }
    else
    {
      tempon = tempon + caractere ;
    }
  }
  *chaine = tempon ;
  return space ;
}

*/
