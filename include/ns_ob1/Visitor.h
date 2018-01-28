//
// visitor.h
//
// Made by (Tabun)
// Login   <giocan_d@epita.fr>
//
// Started on  Jan Sep 16 16:16:25 2004 Tabun
//

#ifndef   VISITOR_H_
# define   VISITOR_H_

class OB1Node;
class BB1Token;

class Visitor
{
public:
  Visitor();
  Visitor(const Visitor& visit);

  virtual void Visit(OB1Node* node_to);
};


class TokenVisitor
{
  TokenVisitor();
  TokenVisitor(const TokenVisitor* tok);

  virtual void Visit(BB1Token* token);
};

#endif