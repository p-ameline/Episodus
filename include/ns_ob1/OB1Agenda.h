//
// OB1Agenda.h
//
// Made by (Tabun)
// Login   <giocan_d@epita.fr>
//
// Started on  Jan Sep 16 16:16:25 2004 Tabun
//


#ifndef  OB1AGENDA_H_
#define OB1AGENDA_H_

class OB1Node ;
class OB1Controler ;

#include <vector>
// #include "ns_ob1\OB1Controler.h"
// #include "ns_ob1\OB1Node.h"

class OB1Agenda
{
  protected:
    OB1Controler* _controler;
    std::vector<OB1Node* > _ready_ks;
  public:
    OB1Agenda(OB1Controler* temp);
    ~OB1Agenda();
    void RunAgenda();
  protected:
    void getAllLaunchableKS();
    void DefaultExecution();

};

#endif
