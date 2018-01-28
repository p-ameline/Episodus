#include <string.h>

#include "nautilus/nssuper.h"
#include "nsoutil/nsexport.h"
#include "nsoutil/nsHtmlToPdf.h"

#ifdef HTMLTOPDF_DYNAMIC

int                           (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_Init)   (int) = NULL ;
int                           (FAR* NSHtmlToPdfConverter::pwkhtmltopdf_DeInit) (void) = NULL ;
wkhtmltopdf_global_settings * (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_CreateGlobalSettings) (void) = NULL ;
wkhtmltopdf_object_settings * (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_CreateObjectSettings) (void) = NULL ;
int                           (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_SetGlobalSetting) (wkhtmltopdf_global_settings*, const char*, const char*) = NULL ;
int                           (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_SetObjectSetting) (wkhtmltopdf_object_settings*, const char*, const char*) = NULL ;
wkhtmltopdf_converter *       (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_CreateConverter)  (wkhtmltopdf_global_settings*) = NULL ;
void                          (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_AddObject)        (wkhtmltopdf_converter*, wkhtmltopdf_object_settings*, const char*) = NULL ;
int                           (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_Convert)          (wkhtmltopdf_converter*) = NULL ;
int                           (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_HttpErrorCode)    (wkhtmltopdf_converter*) = NULL ;
void                          (FAR* NSHtmlToPdfConverter::pWkHtmlToPdf_DestroyConverter) (wkhtmltopdf_converter*) = NULL ;

#endif

NSHtmlToPdfConverter::NSHtmlToPdfConverter(NSSuper* pSuper)
                     :NSSuperRoot(pSuper)
{
  pConverterModule = (TModule*) 0 ;

#ifdef HTMLTOPDF_DYNAMIC
  init() ;
#endif

  // Init wkhtmltopdf in graphics less mode
  //
	WkHtmlToPdf_Init(0) ;
}

void
NSHtmlToPdfConverter::init()
{
try
{
#ifdef HTMLTOPDF_DYNAMIC
	pConverterModule = new TModule("wkhtmltox.dll", TRUE) ;
	if ((TModule*) NULL == pConverterModule)
		erreur("Can't load wkhtmltox.dll", standardError, 0) ;
  else
	{
/*
  	(FARPROC) pWkHtmlToPdf_Init                 = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_INIT)) ;
    (FARPROC) pwkhtmltopdf_DeInit               = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_DEINIT)) ;
    (FARPROC) pWkHtmlToPdf_CreateGlobalSettings = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_CREATE_GLOBAL_SETTINGS)) ;
    (FARPROC) pWkHtmlToPdf_CreateObjectSettings = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_CREATE_OBJECT_SETTINGS)) ;
    (FARPROC) pWkHtmlToPdf_SetGlobalSetting     = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_SET_GLOBAL_SETTINGS)) ;
    (FARPROC) pWkHtmlToPdf_SetObjectSetting     = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_SET_OBJECT_SETTINGS)) ;
    (FARPROC) pWkHtmlToPdf_CreateConverter      = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_CREATE_CONVERTER)) ;
    (FARPROC) pWkHtmlToPdf_AddObject            = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_ADD_OBJECT)) ;
    (FARPROC) pWkHtmlToPdf_Convert              = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_CONVERT)) ;
    (FARPROC) pWkHtmlToPdf_HttpErrorCode        = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_HTTP_ERROR_CODE)) ;
    (FARPROC) pWkHtmlToPdf_DestroyConverter     = pConverterModule->GetProcAddress(MAKEINTRESOURCE(HTMLTOPDF_DESTROY_CONVERTE)) ;
*/
    (FARPROC) pWkHtmlToPdf_Init                 = pConverterModule->GetProcAddress(HTMLTOPDF_INIT) ;
    (FARPROC) pwkhtmltopdf_DeInit               = pConverterModule->GetProcAddress(HTMLTOPDF_DEINIT) ;
    (FARPROC) pWkHtmlToPdf_CreateGlobalSettings = pConverterModule->GetProcAddress(HTMLTOPDF_CREATE_GLOBAL_SETTINGS) ;
    (FARPROC) pWkHtmlToPdf_CreateObjectSettings = pConverterModule->GetProcAddress(HTMLTOPDF_CREATE_OBJECT_SETTINGS) ;
    (FARPROC) pWkHtmlToPdf_SetGlobalSetting     = pConverterModule->GetProcAddress(HTMLTOPDF_SET_GLOBAL_SETTINGS) ;
    (FARPROC) pWkHtmlToPdf_SetObjectSetting     = pConverterModule->GetProcAddress(HTMLTOPDF_SET_OBJECT_SETTINGS) ;
    (FARPROC) pWkHtmlToPdf_CreateConverter      = pConverterModule->GetProcAddress(HTMLTOPDF_CREATE_CONVERTER) ;
    (FARPROC) pWkHtmlToPdf_AddObject            = pConverterModule->GetProcAddress(HTMLTOPDF_ADD_OBJECT) ;
    (FARPROC) pWkHtmlToPdf_Convert              = pConverterModule->GetProcAddress(HTMLTOPDF_CONVERT) ;
    (FARPROC) pWkHtmlToPdf_HttpErrorCode        = pConverterModule->GetProcAddress(HTMLTOPDF_HTTP_ERROR_CODE) ;
    (FARPROC) pWkHtmlToPdf_DestroyConverter     = pConverterModule->GetProcAddress(HTMLTOPDF_DESTROY_CONVERTE) ;

/*
    &wkhtmltopdf_init                   = pConverterModule->GetProcAddress(HTMLTOPDF_INIT_1) ;
    &wkhtmltopdf_deinit                 = pConverterModule->GetProcAddress(HTMLTOPDF_DEINIT_1) ;
    &wkhtmltopdf_create_global_settings = pConverterModule->GetProcAddress(HTMLTOPDF_CREATE_GLOBAL_SETTINGS_1) ;
    &wkhtmltopdf_create_object_settings = pConverterModule->GetProcAddress(HTMLTOPDF_CREATE_OBJECT_SETTINGS_1) ;
    &wkhtmltopdf_set_global_setting     = pConverterModule->GetProcAddress(HTMLTOPDF_SET_GLOBAL_SETTINGS_1) ;
    &wkhtmltopdf_set_object_setting     = pConverterModule->GetProcAddress(HTMLTOPDF_SET_OBJECT_SETTINGS_1) ;
    &wkhtmltopdf_create_converter       = pConverterModule->GetProcAddress(HTMLTOPDF_CREATE_CONVERTER_1) ;
    &wkhtmltopdf_add_object             = pConverterModule->GetProcAddress(HTMLTOPDF_ADD_OBJECT_1) ;
    &wkhtmltopdf_convert                = pConverterModule->GetProcAddress(HTMLTOPDF_CONVERT_1) ;
    &wkhtmltopdf_http_error_code        = pConverterModule->GetProcAddress(HTMLTOPDF_HTTP_ERROR_CODE_1) ;
    &wkhtmltopdf_destroy_converter      = pConverterModule->GetProcAddress(HTMLTOPDF_DESTROY_CONVERTE_1) ;
*/
	}
#endif
}
catch (TXInvalidModule e)
{
	string sErreur = string("Exception NSHtmlToPdfConverter ctor : ") + e.why() ;
	erreur(sErreur.c_str(), standardError, 0) ;
  pConverterModule = (TModule*) 0 ;
}
catch (...)
{
	erreur("Exception NSHtmlToPdfConverter ctor", standardError, 0) ;
}
}

NSHtmlToPdfConverter::~NSHtmlToPdfConverter()
{
  // We will no longer be needing wkhtmltopdf funcionality
  //
  WkHtmlToPdf_DeInit() ;

	if (pConverterModule)
		delete pConverterModule ;
}

bool
NSHtmlToPdfConverter::bIsReady()
{
#ifdef HTMLTOPDF_DYNAMIC
	if ((NULL == pWkHtmlToPdf_Init)                 ||
      (NULL == pwkhtmltopdf_DeInit)               ||
      (NULL == pWkHtmlToPdf_CreateGlobalSettings) ||
      (NULL == pWkHtmlToPdf_CreateObjectSettings) ||
      (NULL == pWkHtmlToPdf_SetGlobalSetting)     ||
      (NULL == pWkHtmlToPdf_SetObjectSetting)     ||
      (NULL == pWkHtmlToPdf_CreateConverter)      ||
      (NULL == pWkHtmlToPdf_AddObject)            ||
      (NULL == pWkHtmlToPdf_Convert)              ||
      (NULL == pWkHtmlToPdf_HttpErrorCode)        ||
      (NULL == pWkHtmlToPdf_DestroyConverter)
     )
		return false ;
#endif
	return true ;
}

int
NSHtmlToPdfConverter::convertHtmlToPdf(string sHtmlFileName, string sPdfFileName, bool bVerbose)
{
  string sTraceText = string("Html to Pdf conversion ") + sHtmlFileName + string(" -> ") + sPdfFileName ;
  _pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

	//
	// Create a global settings object used to store options that are not
	// related to input objects, note that control of this object is parsed to
	// the converter later, which is then responsible for freeing it
  //
	wkhtmltopdf_global_settings *pGlobalSettings = WkHtmlToPdf_CreateGlobalSettings() ;

	// We want the result to be stored in the file called test.pdf
	// wkhtmltopdf_set_global_setting(gs, "out", "test.pdf");
  //
  WkHtmlToPdf_SetGlobalSetting(pGlobalSettings, "out", sPdfFileName.c_str()) ;
  WkHtmlToPdf_SetGlobalSetting(pGlobalSettings, "outputFormat", "pdf") ;
	// WkHtmlToPdf_SetGlobalSetting(pGlobalSettings, "load.cookieJar", "myjar.jar") ;

	//
	// Create a input object settings object that is used to store settings
	// related to a input object, note again that control of this object is parsed to
	// the converter later, which is then responsible for freeing it
  //
	wkhtmltopdf_object_settings *pObjectSettings = WkHtmlToPdf_CreateObjectSettings() ;

	// We want to convert to convert the qstring documentation page
	// wkhtmltopdf_set_object_setting(os, "page", "http://doc.trolltech.com/4.6/qstring.html");
  //
  WkHtmlToPdf_SetObjectSetting(pObjectSettings, "page", sHtmlFileName.c_str()) ;

	// Create the actual converter object used to convert the pages
  //
	wkhtmltopdf_converter *pConverter = WkHtmlToPdf_CreateConverter(pGlobalSettings) ;

	// Call the progress_changed function when progress changes
	// wkhtmltopdf_set_progress_changed_callback(c, progress_changed);

	// Call the phase _changed function when the phase changes
	// wkhtmltopdf_set_phase_changed_callback(c, phase_changed);

	// Call the error function when an error occurs
	// wkhtmltopdf_set_error_callback(c, error);

	// Call the warning function when a warning is issued
	// wkhtmltopdf_set_warning_callback(c, warning);

	//
	// Add the the settings object describing the qstring documentation page
	// to the list of pages to convert. Objects are converted in the order in which
	// they are added
  //
	WkHtmlToPdf_AddObject(pConverter, pObjectSettings, NULL) ;

	// Perform the actual convertion
  //
  int iConvertResult = WkHtmlToPdf_Convert(pConverter) ;

  if (0 == iConvertResult)
  {
    string sErrorText = string("Html to Pdf conversion failed (") + sHtmlFileName + string(" -> ") + sPdfFileName ;
    if (bVerbose)
      _pSuper->traceAndDisplayError(sErrorText) ;
    else
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  }

	// Output possible http error code encountered
  //
  int iHttpErrorCode = WkHtmlToPdf_HttpErrorCode(pConverter) ;
  printf("httpErrorCode: %d\n", iHttpErrorCode) ;

	// Destroy the converter object since we are done with it
  WkHtmlToPdf_DestroyConverter(pConverter) ;

  if (0 == iConvertResult)
  {
    sTraceText = string("Html to Pdf conversion succeeded") ;
    _pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
  }

	return iConvertResult ;
}

/*

// Print out loading progress information
void progress_changed(wkhtmltopdf_converter * c, int p) {
	// printf("%3d%%\r",p);
	// fflush(stdout);
}

// Print loading phase information
void phase_changed(wkhtmltopdf_converter * c) {
	// int phase = wkhtmltopdf_current_phase(c);
	// printf("%s\n", wkhtmltopdf_phase_description(c, phase));
}

// Print a message to stderr when an error occures
void error(wkhtmltopdf_converter * c, const char * msg) {
	// fprintf(stderr, "Error: %s\n", msg);
}

// Print a message to stderr when a warning is issued
void warning(wkhtmltopdf_converter * c, const char * msg) {
	// fprintf(stderr, "Warning: %s\n", msg);
}

// Main method convert pdf
int _NSOUTILCLASSE convertHtmlToPdf(NSContexte* pCtx, string sHtmlFileName, string sPdfFileName)
{
	wkhtmltopdf_global_settings * gs;
	wkhtmltopdf_object_settings * os;
	wkhtmltopdf_converter * c;

	// Init wkhtmltopdf in graphics less mode
	wkhtmltopdf_init(false);

	//
	// Create a global settings object used to store options that are not
	// related to input objects, note that control of this object is parsed to
	// the converter later, which is then responsible for freeing it
  //
	gs = wkhtmltopdf_create_global_settings();
	// We want the result to be storred in the file called test.pdf
	// wkhtmltopdf_set_global_setting(gs, "out", "test.pdf");
  wkhtmltopdf_set_global_setting(gs, "out", sPdfFileName.c_str()) ;

	wkhtmltopdf_set_global_setting(gs, "load.cookieJar", "myjar.jar");
	//
	// Create a input object settings object that is used to store settings
	// related to a input object, note again that control of this object is parsed to
	// the converter later, which is then responsible for freeing it
  //
	os = wkhtmltopdf_create_object_settings();
	// We want to convert to convert the qstring documentation page
	// wkhtmltopdf_set_object_setting(os, "page", "http://doc.trolltech.com/4.6/qstring.html");
  wkhtmltopdf_set_object_setting(os, "page", sHtmlFileName.c_str()) ;

	// Create the actual converter object used to convert the pages
	c = wkhtmltopdf_create_converter(gs);

	// Call the progress_changed function when progress changes
	wkhtmltopdf_set_progress_changed_callback(c, progress_changed);

	// Call the phase _changed function when the phase changes
	wkhtmltopdf_set_phase_changed_callback(c, phase_changed);

	// Call the error function when an error occures
	wkhtmltopdf_set_error_callback(c, error);

	// Call the waring function when a warning is issued
	wkhtmltopdf_set_warning_callback(c, warning);

	//
	// Add the the settings object describing the qstring documentation page
	// to the list of pages to convert. Objects are converted in the order in which
	// they are added
  //
	wkhtmltopdf_add_object(c, os, NULL);

	// Perform the actual convertion
	if (!wkhtmltopdf_convert(c))
		fprintf(stderr, "Convertion failed!");

	// Output possible http error code encountered
	printf("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(c));

	// Destroy the converter object since we are done with it
	wkhtmltopdf_destroy_converter(c);

	// We will no longer be needing wkhtmltopdf funcionality
	wkhtmltopdf_deinit();

	return 0;
}

*/

