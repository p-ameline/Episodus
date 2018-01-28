#ifndef __NSONGLET_H
#define __NSONGLET_H


//#include <vbxct1.h>
#include <\bvsp\include\saxtabs1.h>


class NSOnglet : public TVbxSaxTab {
  public:

	int			iOngletActif;

	NSOnglet(TWindow* parent, int resId, TModule* module=0);
	~NSOnglet();

	void activeControle(int activation, string message) {}

	// child id notification handled at the child
	//
	void EvClick(VBXEVENT FAR*);
	//void BNClicked(VBXEVENT FAR *event);  // BN_CLICKED

	DECLARE_RESPONSE_TABLE(NSOnglet);
};

#endif
