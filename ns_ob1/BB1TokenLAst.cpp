

#include "ns_ob1\OB1Token.h"
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Link.h"


BB1Token::BB1Token()
{
  _type               = ADD;
  _object             = NULL;
  AttributesChange    = NULL;
  LinkChange          = NULL;
  where_I_am          =NULL;
}

BB1Token::BB1Token(EventType type, BB1Object* pTr)
{
  _type           = ADD ;
  _object         = pTr ;
  AttributesChange= NULL;
  LinkChange      = NULL;
  where_I_am      = NULL;
}

BB1Token::BB1Token(EventType type, BB1Object* pTr, AVPSet* Change, LinkSet*  chan)
{
  _type             = ADD ;
  _object           = pTr ;
  LinkChange        = chan ;
  AttributesChange  = Change ;
  where_I_am        = NULL ;
}

BB1Token::BB1Token(const BB1Token& tok)
{
  _type               = tok.getEventType() ;
  _object             = tok.getObject() ;
  LinkChange          = tok.getChangeLinks() ;
  AttributesChange    = tok.getAttributesChange() ;
  where_I_am          = tok.Node();
}

BB1Token::~BB1Token()
{
}


EventType BB1Token::getEventType()
{
  return (_type);
}



/*
**  return the BB1OBject
*/
BB1Object* BB1Token::getObject()
{
  return (_object);
}



Cycle   BB1Token::getCreatedCycle()
{
  return (_object->CycleCreated());
}

AVPSet* BB1Token::getAttributesChange()
{
  return (AttributesChange);
}

LinkSet* BB1Token::getChangeLinks()
{
  return (LinkChange);
}

bool    BB1Token::hasAnwser()
{
  if (_object != NULL)
    {
    const LinkSet*	answerSet = 	_object->Links();
    if ((answerSet != NULL) && (answerSet->size() > 0))
      {
        BB1Link* answer = answerSet->back();
        BB1Object& ansObj = answer->ToObject();
        NSPatPathoArray	*result = ansObj.Value("node", NULLPATHO) ;
        if (result != NULL)
          return (true);
      }
    }
  return (false);
}

int    BB1Token::getObjectClass()
{
    //FIXME
    if (_object != NULL)
    {
      std::string name =((BB1Object*)_object->Class())->Name();
      return (0);
    }
    return (-42);
}
