#ifndef __NSBBKDLG_H__

# define __NSBBKDLG_H__

# ifndef _DANSBBKDLL
#  define _BBKEXPORT __import
# else
#  define _BBKEXPORT __export
# endif

# include <owl\dialog.h>
# include <owl\button.h>

# include "nsbb\nsutidlg.h"
# include "nautilus\nssuper.h"


class _BBKEXPORT NSBBKDlg : public NSUtilDialog
{
	public:

		OWL::TButton	*pInit ;
		OWL::TButton	*pGoOneCycle ;
		OWL::TButton	*pGo ;
		OWL::TButton	*pFinish ;
		OWL::TButton	*pVidal ;

		NSBBKDlg(TWindow *parent , NSContexte *pCtx, TModule *module = 0) ;
		~NSBBKDlg() ;

		void					SetupWindow() ;
		void					CmOk() ;
		void					CmCancel() ;
		void					CmHelp() ;
		void					CmInitBBK() ;
		void					CmGoOneCycle() ;
		void					CmGo() ;
		void					CmFinishBBK() ;
		void					CmDPIO() ;
		void					CmVidal() ;

		DECLARE_RESPONSE_TABLE(NSBBKDlg);
} ;

#endif
