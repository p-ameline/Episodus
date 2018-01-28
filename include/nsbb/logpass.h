#ifndef __LOG_PASS_H__
#define   __LOG_PASS_H__

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb\nsutidlg.h"
#include "nsutil\md5.h"
#include <owl/static.h>
#include <owl/edit.h>
#include <owl/groupbox.h.h>
#include <owl/checkbox.h.h>
#include "nautilus\nssuper.h"

#include <iostream>
#include <string>


/*
**  Identification windows
**	used ot identificate a user for global transaction
*/
class _NSBBCLASSE LogPassInterface  : public NSUtilDialog
{
	public:
  	LogPassInterface(TWindow* parent,  NSContexte *pCtx, std::string log = "", std::string pas = "", bool session = false);

    void SetupWindow();
    void CmOk();
    void CmCancel();
    void CmHelp();

    ~LogPassInterface();

  public:
  	inline
  	std::string getLogin() {  return 	_login; }

    inline
    std::string getPassword() { return  _password;}

    // indique si le pass et le login sont mis NSutilisateur choisis
    inline
    bool IsOnMemory() { return  _session; }

  protected:
  	void setLogin(std::string login);
    void setpassWord(std::string pass);

  protected:
  	std::string 	_login;
    std::string		_password;
    bool          _session;     // Save the patient during the session

  private:
  	void ComputePassword();
    void ComputeLogin();
    void ComputeSession();

	private:
  	OWL::TEdit*	    _log_edit;
    OWL::TEdit*		_pass_edit;
    OWL::TGroupBox*	_group_log;
    OWL::TGroupBox*	_group_pass;
    OWL::TCheckBox*	_Check;

    DECLARE_RESPONSE_TABLE(LogPassInterface) ;
};

#endif
