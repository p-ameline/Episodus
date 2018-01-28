//---------------------------------------------------------------------------
//    NTLlistBox.H
//    NAUTILUS Septembre 2003
//
//    Gestion des "choses"
//---------------------------------------------------------------------------
#ifndef _NT_LISTBOX_H
# define _NT_LISTBOX_H

# include <owl\listbox.h>
#include  <string>
#include <vector>
# include "nsbb\nsutidlg.h"
# include "nsbb\nspatpat.h"
# include "nautilus\nssuper.h"
#include "nsbb\NTFunctor.h"

class GListBox : public OWL::TListBox
{
 public:

	GListBox(TWindow *parent, int resId) : OWL::TListBox(parent, resId) {}
  GListBox(TWindow *parent, int Id, int x, int y, int w, int h) : OWL::TListBox(parent, Id, x, y, w, h) {}

  void PrintArray(char *input[]) ;

  virtual void    EvKeyDown(uint key, uint repeatCount, uint flags) ;

	DECLARE_RESPONSE_TABLE(GListBox) ;
};

typedef void (*func_void)(); // Declarationd 'un pointeur sur fonction ex:  void foo();

/*
** Class permettant d'afficher une liste de label
** et de gerer une liste associé dans uen ListBox
*/
template <class type, class Contain>
class NTTList  :  public  GListBox
{
  protected:
    std::vector<type*>        *_associated_data ;  // type de donnée associé
    std::vector<std::string*> *_labels ;           // a afficher
    int _index ;                                   // Index courrant de la liste
    bool bOwnsAssociatedData ;

  protected:
  	MemFunctor<Contain>* event_enter ; // Fonction a appeler dans le cas d'un enter
		MemFunctor<Contain>* KeyUpEvent ;
		MemFunctor<Contain>* KeyDownEvent ;
		MemFunctor<Contain>* BeforeChange ;
		MemFunctor<Contain>* MouseClick ;

  public:
  	/*
    ** Constructeur : on passe la fenetre et l'id du  fichier de RC
    */
    NTTList(TWindow *parent, int resId)  :  GListBox(parent, resId)
    {
    	event_enter = KeyUpEvent = KeyDownEvent = BeforeChange = MouseClick = NULL ;
      _associated_data = new std::vector<type* >() ;
      _labels          = new std::vector< std::string* >() ;
      _index = -1 ;
      bOwnsAssociatedData = true ;
    }

    NTTList(TWindow *parent, int Id, int x, int y, int w, int h)  :  GListBox(parent, Id, x, y, w, h)
    {
    	event_enter = KeyUpEvent = KeyDownEvent = BeforeChange = MouseClick = NULL ;
      _associated_data = new std::vector<type* >() ;
      _labels          = new std::vector< std::string* >() ;
      _index = -1 ;
      bOwnsAssociatedData = true ;
    }

		int Index() { return _index ; }

		type* getDataAt(size_t index)
		{
    	if ((_associated_data != NULL) && (index < _associated_data->size()))
      	return (*_associated_data)[index] ;
      return NULL ;
		}

		int Size() { return _associated_data->size() ; }

		/*
		** Fonction permettant de changer les elements associé
		*/
		void SetAssociatedData(std::vector<type *> *data, bool bOwnsData)
    {
    	DeleteAssociatedData() ;
    	_associated_data    = data ;
      bOwnsAssociatedData = bOwnsData ;
    }

		/*
    ** Vide la liste contenant les labels
    */
		void AddLabel(char* temp) { _labels->push_back(new std::string(temp)) ; }

    void ClearLabels()
		{
    	if ((NULL == _labels) || (true == _labels->empty()))
      	return ;

      std::vector<std::string* >::iterator iters = _labels->begin() ;
      for ( ; _labels->begin() != iters ; )
      {
      	if (NULL != *iters)
        	delete *iters ;
      	_labels->erase(iters) ;
      }
    }

    void ClearAssociatedData()
		{
    	if (false == bOwnsAssociatedData)
      	return ;

    	if ((NULL == _associated_data) || (true == _associated_data->empty()))
      	return ;

      std::vector<type *>::iterator iterd = _associated_data->begin() ;
      for ( ; _associated_data->begin() != iterd ; )
      {
      	if (NULL != *iterd)
        	delete *iterd ;
      	_associated_data->erase(iterd) ;
      }
    }

		void Clear()
		{
    	ClearLabels() ;
      ClearAssociatedData() ;
  	}

		/*
		** Constructeur : on passe la fenetre et l'id du  fichier de RC
    ** La liste d'element associé doit etre initlialisé
    */
		NTTList(TWindow *parent, int resId, std::vector<type* >* list_assoc, bool bOwnsData):GListBox(parent, resId)
		{
    	event_enter = KeyUpEvent = KeyDownEvent = BeforeChange = MouseClick = NULL ;
    	_associated_data = list_assoc ;
      _labels  = new std::vector< std::string* >() ;
      _index = -1 ;
      bOwnsAssociatedData = bOwnsData ;
    }

		/*
		** Attention ! LE destructeur detruit la liste des labels
		** mais pas la lsite associé
		*/
		~NTTList()
		{
    	// if (event_enter != NULL) delete(event_enter);
      if (event_enter != NULL)
      	delete(event_enter) ;
      if (KeyUpEvent != NULL)
      	delete(KeyUpEvent) ;
      if (KeyDownEvent != NULL)
      	delete(KeyDownEvent) ;
      if (BeforeChange != NULL)
      	delete(BeforeChange) ;
      if (MouseClick != NULL)
      	delete(MouseClick) ;
    	//  if (BeforeChange != NULL) delete(BeforeChange);
      DeleteAssociatedData() ;
      DeleteLabels() ;
		}

    void DeleteAssociatedData()
		{
    	if ((false == bOwnsAssociatedData) || (NULL == _associated_data))
      	return ;
      ClearAssociatedData() ;
      delete _associated_data ;
      _associated_data = 0 ;
    }

    void DeleteLabels()
		{
    	if (NULL == _labels)
      	return ;
      ClearLabels() ;
      delete _labels ;
      _labels = 0 ;
    }

		/*
    ** Fonction servant a maintenir une coherence dans la liste
    ** lors d'un tri
    */
		bool swap(int i, int j)
		{
    	std::string *temp_lab   = (*_labels)[i] ;
      type        *temp_assoc = (*_associated_data)[i] ;
      (*_labels)[i]           = (*_labels)[j] ;
      (*_associated_data)[i]  = (*_associated_data)[j] ;
      (*_labels)[j]           = temp_lab ;
      (*_associated_data)[j]  = temp_assoc ;
      return true ;
		}

		void addItem(const char* lab, type *ad)
		{
    	_associated_data->push_back(ad) ;
      _labels->push_back(new std::string(lab)) ;
      PrintListAsNow() ;
		}

		void EvKeyDown(uint key, uint /* repeatCount */, uint /* flags */)
		{
			//   if (NULL != BeforeChange)
			//     (*BeforeChange)();

			switch(key)
      {
      	case VK_DELETE :
        	if (_labels->size() > 0)
          {
            int index = GetSelIndex() ;
            erase(index) ;
            if (index >= (int)_labels->size())
              SetSelIndex(--index) ;
            else
              SetSelIndex(index) ;

             _index = index ;
          }
          break ;
        case VK_UP:
        {
        	int index = GetSelIndex() ;
          index-- ;
          _index = index ;
          SetSelIndex(index) ;
          if (KeyUpEvent != NULL)
          	(*KeyUpEvent)() ;
        }
        break ;
        case VK_DOWN:
        {
          int index = GetSelIndex() ;
          index++ ;
          _index = index ;
          SetSelIndex(index) ;
          if (KeyDownEvent != NULL)
          	(*KeyDownEvent)() ;
        }
        break ;
        case VK_RETURN:
          if ( NULL == event_enter)
            (*event_enter)() ;
        	break ;
        default:
        	break ;
      }

      _index =  GetSelIndex() ;
		}

    /*
    ** Insere une fonction de reponse pour enter
    */
    void setEnterResponse(MemFunctor<Contain>* temp)
    {
      event_enter = temp ;
    }

    /*
    ** Insere une fonction de reponse quand on touche la touche du bas.
    */
    void setKeyUpResponse(MemFunctor<Contain>* temp)
    {
      KeyUpEvent = temp ;
    }

    /*
    **  Insere une fonctionde reponse quand on tape sur touche key dow
    */
    void setKeyDownResponse(MemFunctor<Contain>* temp)
    {
      KeyDownEvent = temp ;
    }

    /*
    ** Fonction se déclenchant avant un changement
    */
    void setBeforeChangeResponse(MemFunctor<Contain>* temp)
    {
      BeforeChange = temp ;
    }

    /*
    ** Fonction appele lors du changement
    */
    void setMouseClickReponse(MemFunctor<Contain>* temp)
    {
      MouseClick = temp ;
    }

    /*
    ** Sert a maintenir une coherence entre les deux listes
    ** quand on efface un element
    */
    bool erase(int num)
    {
     	std::vector<std::string* >::iterator iters = _labels->begin() ;
      std::vector<type *>::iterator        iterd =  _associated_data->begin() ;
      for (int i = 0; i < num; i++)
      {
      	iters++ ;
        iterd++ ;
      }
      delete(*iters) ;
      *iters = NULL ;
      _labels->erase(iters) ;
      delete(*iterd) ;
      *iterd = NULL ;
      _associated_data->erase(iterd) ;
      PrintListAsNow() ;
      return true ;
    }

    /*
    ** Trie la liste en fonction des labels et d'une fonction de comparaison de chaine passé en parametres
    */
    void sortLabel(bool (*comp)(std::string* , std::string* ))
    {
    	register int size =_labels->size() ;
      if (size > 1)
      {
      	for (register int i = size-1; i >=1; i--)
        	for (register int j = 1; j <=i; j++)
          	if (comp((*_labels)[j-1] , (*_labels)[j]) == false )
            	swap(j-1, j) ;
      }
      PrintListAsNow() ;
    }

    /*
    ** Trie la liste en fonction des donne et d'une fonction de comparaison d'element passé en parametres
    */
    void sortElement(bool (*comp)(type* , type* ))
    {
    	register int size =_labels->size();
      if (size > 1)
      {
      	for (register int i = size-1; i >=1; i--)
        	for (register int j = 1; j <=i; j++)
          	if (comp((*_associated_data)[j-1] , (*_associated_data)[j]) == false )
            	swap(j-1, j) ;
      }
      PrintListAsNow() ;
    }

    /*
		** Remplace un label existant
    */
    void ChangeLabel(std::string& str, int index)
    {
    	delete ( (*_labels)[index] ) ;
      (*_labels)[index] = new std::string(str) ;
    }

    /*
    ** change les element en place i par le label temp et l'element temp2
    ** renvoie 0 si succes et 1 sinon
    */
    int ChangeElement(int index, char* temp, type* temp2)
    {
    	if ((*_labels)[index] == NULL)
      	return 1 ;

      delete ( (*_labels)[index] ) ;
      (*_labels)[index] = new std::string(temp) ;

      if ((*_associated_data)[index] == NULL)
      	return 1 ;
      delete ((*_associated_data)[index]) ;
      (*_associated_data)[index] = temp2 ;
      return 0 ;
    }

    /*
    ** Trouve le nom d'un fichier
    ** Renvoie le numero d'index si trouve
    ** Renvoie -1 sinon
    */
    int findNumeroOfLAbel(char* temp)
    {
    	register int i ;
      for (i = 0; i < _labels->size(); i++)
        if ( strcmp((*_labels)[index], temp) == 0)
          return i ;
      return -1 ;
    }

    /*
    ** Efface la liste et imprime la liste des labels
    */
    void PrintListAsNow()
    {
    	ClearList() ;
      for (register int i =0; i < (int)_labels->size() ; i++)
      	AddString((*_labels)[i]->c_str()) ;
    }
};

#endif

