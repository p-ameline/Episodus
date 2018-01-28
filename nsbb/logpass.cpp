#include "nsbb\logpass.h"
#include "logpass.rh"
#include "nsbb\nsbb.h"

DEFINE_RESPONSE_TABLE1(LogPassInterface, TDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;



LogPassInterface::LogPassInterface(TWindow* parent,  NSContexte *pCtx, std::string log, std::string pass, bool session)
: NSUtilDialog(parent, pCtx, "IDD_LOGPASSDIALOG", pNSDLLModule) ,  _login(log), _password(pass), _session(session)
{
	_log_edit 	= new TEdit(this, IDC_IDENTIFIANT);
  _pass_edit 	= new TEdit(this, IDC_PASSWORD);
  _group_log 	= new TGroupBox(this, IDC_GROUP_LOGIN);
  _group_pass = new TGroupBox(this,IDC_GROUP_PASSWORD );
  _Check 			= new TCheckBox(this, IDC_SESSION);
}


void LogPassInterface::SetupWindow()
{
	std::string pass_string = pContexte->getSuperviseur()->getText("logpasswindow", "passstring");
  std::string log_string = pContexte->getSuperviseur()->getText("logpasswindow", "logstring");
  std::string session_string = pContexte->getSuperviseur()->getText("logpasswindow", "sessionstring");
  std::string windows = pContexte->getSuperviseur()->getText("logpasswindow", "windows");
  this->SetCaption(windows.c_str());
  _Check->SetCaption(session_string.c_str());
  _group_log->SetCaption(log_string.c_str());
  _group_pass->SetCaption(pass_string.c_str());
 	NSUtilDialog::SetupWindow();
}

LogPassInterface::~LogPassInterface()
{
		if (NULL != _log_edit)
    	{
      	delete (_log_edit);
        _log_edit = NULL;
      }
    if (NULL != _pass_edit)
    	{
      	delete (_pass_edit);
        _pass_edit = NULL;
      }
    if (NULL != _group_log)
    	{
      	delete (_group_log);
        _group_log = NULL;
      }
   if (NULL != _group_pass)
   		{
      	delete (_group_pass);
        _group_pass = NULL;
      }
   if (NULL !=	_Check)
   	{
    	delete (_Check);
      _Check = NULL;
    }
}

void LogPassInterface::CmOk()
{
	ComputeSession();
	if (_session)
  	{
    	ComputeLogin();
    	ComputePassword();
      pContexte->getUtilisateur()->setGlobalPassword(_password);
      pContexte->getUtilisateur()->setGlobalLogin(_login);
    }
  else
  	{
  		ComputeLogin();
      ComputePassword();
    }
  NSUtilDialog::CmOk();
}

void LogPassInterface::ComputePassword()
{
	char far pass[80];
	_pass_edit->GetText(pass, 80);
  _password = std::string(pass);
  _password = 	MD5_encrypt(_password);
}

void LogPassInterface::ComputeLogin()
{
	char far login[80];
	_log_edit->GetText(login, 80);
  _login = std::string(login);

}

void LogPassInterface::ComputeSession()
{
  _session = (_Check->GetCheck() == BF_CHECKED) ? true : false;
}

void LogPassInterface::CmCancel()
{
	NSUtilDialog::CmCancel();
}

void LogPassInterface::CmHelp()
{
}
