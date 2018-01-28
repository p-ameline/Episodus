
#include "ns_ob1\Visitor.h"
#include "ns_ob1\OB1Node.h"


Visitor::Visitor()
{
}

Visitor::Visitor(const Visitor& visit)
{
}


void Visitor::Visit(OB1Node* node_to)
{

}



TokenVisitor::TokenVisitor()
{
}

TokenVisitor::TokenVisitor(const TokenVisitor* tok)
{
}

void TokenVisitor::Visit(BB1Token* token)
{
}
