
#pragma warning( disable: 4049 )  /* more than 64k source lines */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0359 */
/* Compiler settings for mssoap30.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __mssoap30_h__
#define __mssoap30_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISoapReader_FWD_DEFINED__
#define __ISoapReader_FWD_DEFINED__
typedef interface ISoapReader ISoapReader;
#endif 	/* __ISoapReader_FWD_DEFINED__ */


#ifndef __ISoapSerializer_FWD_DEFINED__
#define __ISoapSerializer_FWD_DEFINED__
typedef interface ISoapSerializer ISoapSerializer;
#endif 	/* __ISoapSerializer_FWD_DEFINED__ */


#ifndef __ISoapMapper_FWD_DEFINED__
#define __ISoapMapper_FWD_DEFINED__
typedef interface ISoapMapper ISoapMapper;
#endif 	/* __ISoapMapper_FWD_DEFINED__ */


#ifndef __IDataEncoder_FWD_DEFINED__
#define __IDataEncoder_FWD_DEFINED__
typedef interface IDataEncoder IDataEncoder;
#endif 	/* __IDataEncoder_FWD_DEFINED__ */


#ifndef __IDataEncoderFactory_FWD_DEFINED__
#define __IDataEncoderFactory_FWD_DEFINED__
typedef interface IDataEncoderFactory IDataEncoderFactory;
#endif 	/* __IDataEncoderFactory_FWD_DEFINED__ */


#ifndef __IAttachment_FWD_DEFINED__
#define __IAttachment_FWD_DEFINED__
typedef interface IAttachment IAttachment;
#endif 	/* __IAttachment_FWD_DEFINED__ */


#ifndef __IFileAttachment_FWD_DEFINED__
#define __IFileAttachment_FWD_DEFINED__
typedef interface IFileAttachment IFileAttachment;
#endif 	/* __IFileAttachment_FWD_DEFINED__ */


#ifndef __FileAttachment30_FWD_DEFINED__
#define __FileAttachment30_FWD_DEFINED__

#ifdef __cplusplus
typedef class FileAttachment30 FileAttachment30;
#else
typedef struct FileAttachment30 FileAttachment30;
#endif /* __cplusplus */

#endif 	/* __FileAttachment30_FWD_DEFINED__ */


#ifndef __IStringAttachment_FWD_DEFINED__
#define __IStringAttachment_FWD_DEFINED__
typedef interface IStringAttachment IStringAttachment;
#endif 	/* __IStringAttachment_FWD_DEFINED__ */


#ifndef __StringAttachment30_FWD_DEFINED__
#define __StringAttachment30_FWD_DEFINED__

#ifdef __cplusplus
typedef class StringAttachment30 StringAttachment30;
#else
typedef struct StringAttachment30 StringAttachment30;
#endif /* __cplusplus */

#endif 	/* __StringAttachment30_FWD_DEFINED__ */


#ifndef __IByteArrayAttachment_FWD_DEFINED__
#define __IByteArrayAttachment_FWD_DEFINED__
typedef interface IByteArrayAttachment IByteArrayAttachment;
#endif 	/* __IByteArrayAttachment_FWD_DEFINED__ */


#ifndef __ByteArrayAttachment30_FWD_DEFINED__
#define __ByteArrayAttachment30_FWD_DEFINED__

#ifdef __cplusplus
typedef class ByteArrayAttachment30 ByteArrayAttachment30;
#else
typedef struct ByteArrayAttachment30 ByteArrayAttachment30;
#endif /* __cplusplus */

#endif 	/* __ByteArrayAttachment30_FWD_DEFINED__ */


#ifndef __IStreamAttachment_FWD_DEFINED__
#define __IStreamAttachment_FWD_DEFINED__
typedef interface IStreamAttachment IStreamAttachment;
#endif 	/* __IStreamAttachment_FWD_DEFINED__ */


#ifndef __StreamAttachment30_FWD_DEFINED__
#define __StreamAttachment30_FWD_DEFINED__

#ifdef __cplusplus
typedef class StreamAttachment30 StreamAttachment30;
#else
typedef struct StreamAttachment30 StreamAttachment30;
#endif /* __cplusplus */

#endif 	/* __StreamAttachment30_FWD_DEFINED__ */


#ifndef __IReceivedAttachment_FWD_DEFINED__
#define __IReceivedAttachment_FWD_DEFINED__
typedef interface IReceivedAttachment IReceivedAttachment;
#endif 	/* __IReceivedAttachment_FWD_DEFINED__ */


#ifndef __ISentAttachments_FWD_DEFINED__
#define __ISentAttachments_FWD_DEFINED__
typedef interface ISentAttachments ISentAttachments;
#endif 	/* __ISentAttachments_FWD_DEFINED__ */


#ifndef __SentAttachments30_FWD_DEFINED__
#define __SentAttachments30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SentAttachments30 SentAttachments30;
#else
typedef struct SentAttachments30 SentAttachments30;
#endif /* __cplusplus */

#endif 	/* __SentAttachments30_FWD_DEFINED__ */


#ifndef __IReceivedAttachments_FWD_DEFINED__
#define __IReceivedAttachments_FWD_DEFINED__
typedef interface IReceivedAttachments IReceivedAttachments;
#endif 	/* __IReceivedAttachments_FWD_DEFINED__ */


#ifndef __ReceivedAttachments30_FWD_DEFINED__
#define __ReceivedAttachments30_FWD_DEFINED__

#ifdef __cplusplus
typedef class ReceivedAttachments30 ReceivedAttachments30;
#else
typedef struct ReceivedAttachments30 ReceivedAttachments30;
#endif /* __cplusplus */

#endif 	/* __ReceivedAttachments30_FWD_DEFINED__ */


#ifndef __IComposerDestination_FWD_DEFINED__
#define __IComposerDestination_FWD_DEFINED__
typedef interface IComposerDestination IComposerDestination;
#endif 	/* __IComposerDestination_FWD_DEFINED__ */


#ifndef __IGetComposerDestination_FWD_DEFINED__
#define __IGetComposerDestination_FWD_DEFINED__
typedef interface IGetComposerDestination IGetComposerDestination;
#endif 	/* __IGetComposerDestination_FWD_DEFINED__ */


#ifndef __IMessageComposer_FWD_DEFINED__
#define __IMessageComposer_FWD_DEFINED__
typedef interface IMessageComposer IMessageComposer;
#endif 	/* __IMessageComposer_FWD_DEFINED__ */


#ifndef __IDimeComposer_FWD_DEFINED__
#define __IDimeComposer_FWD_DEFINED__
typedef interface IDimeComposer IDimeComposer;
#endif 	/* __IDimeComposer_FWD_DEFINED__ */


#ifndef __ISimpleComposer_FWD_DEFINED__
#define __ISimpleComposer_FWD_DEFINED__
typedef interface ISimpleComposer ISimpleComposer;
#endif 	/* __ISimpleComposer_FWD_DEFINED__ */


#ifndef __IParserSource_FWD_DEFINED__
#define __IParserSource_FWD_DEFINED__
typedef interface IParserSource IParserSource;
#endif 	/* __IParserSource_FWD_DEFINED__ */


#ifndef __IGetParserSource_FWD_DEFINED__
#define __IGetParserSource_FWD_DEFINED__
typedef interface IGetParserSource IGetParserSource;
#endif 	/* __IGetParserSource_FWD_DEFINED__ */


#ifndef __IMessageParser_FWD_DEFINED__
#define __IMessageParser_FWD_DEFINED__
typedef interface IMessageParser IMessageParser;
#endif 	/* __IMessageParser_FWD_DEFINED__ */


#ifndef __IDimeParser_FWD_DEFINED__
#define __IDimeParser_FWD_DEFINED__
typedef interface IDimeParser IDimeParser;
#endif 	/* __IDimeParser_FWD_DEFINED__ */


#ifndef __ISimpleParser_FWD_DEFINED__
#define __ISimpleParser_FWD_DEFINED__
typedef interface ISimpleParser ISimpleParser;
#endif 	/* __ISimpleParser_FWD_DEFINED__ */


#ifndef __DataEncoderFactory30_FWD_DEFINED__
#define __DataEncoderFactory30_FWD_DEFINED__

#ifdef __cplusplus
typedef class DataEncoderFactory30 DataEncoderFactory30;
#else
typedef struct DataEncoderFactory30 DataEncoderFactory30;
#endif /* __cplusplus */

#endif 	/* __DataEncoderFactory30_FWD_DEFINED__ */


#ifndef __DimeComposer30_FWD_DEFINED__
#define __DimeComposer30_FWD_DEFINED__

#ifdef __cplusplus
typedef class DimeComposer30 DimeComposer30;
#else
typedef struct DimeComposer30 DimeComposer30;
#endif /* __cplusplus */

#endif 	/* __DimeComposer30_FWD_DEFINED__ */


#ifndef __DimeParser30_FWD_DEFINED__
#define __DimeParser30_FWD_DEFINED__

#ifdef __cplusplus
typedef class DimeParser30 DimeParser30;
#else
typedef struct DimeParser30 DimeParser30;
#endif /* __cplusplus */

#endif 	/* __DimeParser30_FWD_DEFINED__ */


#ifndef __SimpleComposer30_FWD_DEFINED__
#define __SimpleComposer30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SimpleComposer30 SimpleComposer30;
#else
typedef struct SimpleComposer30 SimpleComposer30;
#endif /* __cplusplus */

#endif 	/* __SimpleComposer30_FWD_DEFINED__ */


#ifndef __SimpleParser30_FWD_DEFINED__
#define __SimpleParser30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SimpleParser30 SimpleParser30;
#else
typedef struct SimpleParser30 SimpleParser30;
#endif /* __cplusplus */

#endif 	/* __SimpleParser30_FWD_DEFINED__ */


#ifndef __SoapReader30_FWD_DEFINED__
#define __SoapReader30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoapReader30 SoapReader30;
#else
typedef struct SoapReader30 SoapReader30;
#endif /* __cplusplus */

#endif 	/* __SoapReader30_FWD_DEFINED__ */


#ifndef __SoapSerializer30_FWD_DEFINED__
#define __SoapSerializer30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoapSerializer30 SoapSerializer30;
#else
typedef struct SoapSerializer30 SoapSerializer30;
#endif /* __cplusplus */

#endif 	/* __SoapSerializer30_FWD_DEFINED__ */


#ifndef __ISoapClient_FWD_DEFINED__
#define __ISoapClient_FWD_DEFINED__
typedef interface ISoapClient ISoapClient;
#endif 	/* __ISoapClient_FWD_DEFINED__ */


#ifndef __ISoapServer_FWD_DEFINED__
#define __ISoapServer_FWD_DEFINED__
typedef interface ISoapServer ISoapServer;
#endif 	/* __ISoapServer_FWD_DEFINED__ */


#ifndef __SoapServer30_FWD_DEFINED__
#define __SoapServer30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoapServer30 SoapServer30;
#else
typedef struct SoapServer30 SoapServer30;
#endif /* __cplusplus */

#endif 	/* __SoapServer30_FWD_DEFINED__ */


#ifndef __SoapClient30_FWD_DEFINED__
#define __SoapClient30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoapClient30 SoapClient30;
#else
typedef struct SoapClient30 SoapClient30;
#endif /* __cplusplus */

#endif 	/* __SoapClient30_FWD_DEFINED__ */


#ifndef __ISoapTypeMapperFactory_FWD_DEFINED__
#define __ISoapTypeMapperFactory_FWD_DEFINED__
typedef interface ISoapTypeMapperFactory ISoapTypeMapperFactory;
#endif 	/* __ISoapTypeMapperFactory_FWD_DEFINED__ */


#ifndef __IHeaderHandler_FWD_DEFINED__
#define __IHeaderHandler_FWD_DEFINED__
typedef interface IHeaderHandler IHeaderHandler;
#endif 	/* __IHeaderHandler_FWD_DEFINED__ */


#ifndef __ISoapTypeMapper_FWD_DEFINED__
#define __ISoapTypeMapper_FWD_DEFINED__
typedef interface ISoapTypeMapper ISoapTypeMapper;
#endif 	/* __ISoapTypeMapper_FWD_DEFINED__ */


#ifndef __IEnumSoapMappers_FWD_DEFINED__
#define __IEnumSoapMappers_FWD_DEFINED__
typedef interface IEnumSoapMappers IEnumSoapMappers;
#endif 	/* __IEnumSoapMappers_FWD_DEFINED__ */


#ifndef __IWSDLMessage_FWD_DEFINED__
#define __IWSDLMessage_FWD_DEFINED__
typedef interface IWSDLMessage IWSDLMessage;
#endif 	/* __IWSDLMessage_FWD_DEFINED__ */


#ifndef __IWSDLOperation_FWD_DEFINED__
#define __IWSDLOperation_FWD_DEFINED__
typedef interface IWSDLOperation IWSDLOperation;
#endif 	/* __IWSDLOperation_FWD_DEFINED__ */


#ifndef __IEnumWSDLOperations_FWD_DEFINED__
#define __IEnumWSDLOperations_FWD_DEFINED__
typedef interface IEnumWSDLOperations IEnumWSDLOperations;
#endif 	/* __IEnumWSDLOperations_FWD_DEFINED__ */


#ifndef __IWSDLPort_FWD_DEFINED__
#define __IWSDLPort_FWD_DEFINED__
typedef interface IWSDLPort IWSDLPort;
#endif 	/* __IWSDLPort_FWD_DEFINED__ */


#ifndef __IEnumWSDLPorts_FWD_DEFINED__
#define __IEnumWSDLPorts_FWD_DEFINED__
typedef interface IEnumWSDLPorts IEnumWSDLPorts;
#endif 	/* __IEnumWSDLPorts_FWD_DEFINED__ */


#ifndef __IWSDLService_FWD_DEFINED__
#define __IWSDLService_FWD_DEFINED__
typedef interface IWSDLService IWSDLService;
#endif 	/* __IWSDLService_FWD_DEFINED__ */


#ifndef __IEnumWSDLService_FWD_DEFINED__
#define __IEnumWSDLService_FWD_DEFINED__
typedef interface IEnumWSDLService IEnumWSDLService;
#endif 	/* __IEnumWSDLService_FWD_DEFINED__ */


#ifndef __IWSDLReader_FWD_DEFINED__
#define __IWSDLReader_FWD_DEFINED__
typedef interface IWSDLReader IWSDLReader;
#endif 	/* __IWSDLReader_FWD_DEFINED__ */


#ifndef __IWSDLBinding_FWD_DEFINED__
#define __IWSDLBinding_FWD_DEFINED__
typedef interface IWSDLBinding IWSDLBinding;
#endif 	/* __IWSDLBinding_FWD_DEFINED__ */


#ifndef __WSDLReader30_FWD_DEFINED__
#define __WSDLReader30_FWD_DEFINED__

#ifdef __cplusplus
typedef class WSDLReader30 WSDLReader30;
#else
typedef struct WSDLReader30 WSDLReader30;
#endif /* __cplusplus */

#endif 	/* __WSDLReader30_FWD_DEFINED__ */


#ifndef __SoapTypeMapperFactory30_FWD_DEFINED__
#define __SoapTypeMapperFactory30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoapTypeMapperFactory30 SoapTypeMapperFactory30;
#else
typedef struct SoapTypeMapperFactory30 SoapTypeMapperFactory30;
#endif /* __cplusplus */

#endif 	/* __SoapTypeMapperFactory30_FWD_DEFINED__ */


#ifndef __GenericCustomTypeMapper30_FWD_DEFINED__
#define __GenericCustomTypeMapper30_FWD_DEFINED__

#ifdef __cplusplus
typedef class GenericCustomTypeMapper30 GenericCustomTypeMapper30;
#else
typedef struct GenericCustomTypeMapper30 GenericCustomTypeMapper30;
#endif /* __cplusplus */

#endif 	/* __GenericCustomTypeMapper30_FWD_DEFINED__ */


#ifndef __UDTMapper30_FWD_DEFINED__
#define __UDTMapper30_FWD_DEFINED__

#ifdef __cplusplus
typedef class UDTMapper30 UDTMapper30;
#else
typedef struct UDTMapper30 UDTMapper30;
#endif /* __cplusplus */

#endif 	/* __UDTMapper30_FWD_DEFINED__ */


#ifndef __ISoapConnector_FWD_DEFINED__
#define __ISoapConnector_FWD_DEFINED__
typedef interface ISoapConnector ISoapConnector;
#endif 	/* __ISoapConnector_FWD_DEFINED__ */


#ifndef __ISoapConnectorFactory_FWD_DEFINED__
#define __ISoapConnectorFactory_FWD_DEFINED__
typedef interface ISoapConnectorFactory ISoapConnectorFactory;
#endif 	/* __ISoapConnectorFactory_FWD_DEFINED__ */


#ifndef __SoapConnector30_FWD_DEFINED__
#define __SoapConnector30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoapConnector30 SoapConnector30;
#else
typedef struct SoapConnector30 SoapConnector30;
#endif /* __cplusplus */

#endif 	/* __SoapConnector30_FWD_DEFINED__ */


#ifndef __SoapConnectorFactory30_FWD_DEFINED__
#define __SoapConnectorFactory30_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoapConnectorFactory30 SoapConnectorFactory30;
#else
typedef struct SoapConnectorFactory30 SoapConnectorFactory30;
#endif /* __cplusplus */

#endif 	/* __SoapConnectorFactory30_FWD_DEFINED__ */


#ifndef __HttpConnector30_FWD_DEFINED__
#define __HttpConnector30_FWD_DEFINED__

#ifdef __cplusplus
typedef class HttpConnector30 HttpConnector30;
#else
typedef struct HttpConnector30 HttpConnector30;
#endif /* __cplusplus */

#endif 	/* __HttpConnector30_FWD_DEFINED__ */


#ifndef __ISoapError_FWD_DEFINED__
#define __ISoapError_FWD_DEFINED__
typedef interface ISoapError ISoapError;
#endif 	/* __ISoapError_FWD_DEFINED__ */


#ifndef __ISoapErrorInfo_FWD_DEFINED__
#define __ISoapErrorInfo_FWD_DEFINED__
typedef interface ISoapErrorInfo ISoapErrorInfo;
#endif 	/* __ISoapErrorInfo_FWD_DEFINED__ */


#ifndef __IGCTMObjectFactory_FWD_DEFINED__
#define __IGCTMObjectFactory_FWD_DEFINED__
typedef interface IGCTMObjectFactory IGCTMObjectFactory;
#endif 	/* __IGCTMObjectFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_mssoap30_0000 */
/* [local] */ 

#pragma once


extern RPC_IF_HANDLE __MIDL_itf_mssoap30_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mssoap30_0000_v0_0_s_ifspec;


#ifndef __MSSOAPLib30_LIBRARY_DEFINED__
#define __MSSOAPLib30_LIBRARY_DEFINED__

/* library MSSOAPLib30 */
/* [helpstring][version][uuid] */ 




typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_mssoap30_0000_0001
    {	cdMayRequireResend	= 0x1,
	cdRequiresTotalSize	= 0x2
    } 	ComposerDestinationFlags;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_mssoap30_0132_0001
    {	elDefaultLocation	= 0,
	elEndOfEnvelope	= 1,
	elEndOfBody	= 2,
	elEndOfHeader	= 3
    } 	enElementLocation;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_mssoap30_0135_0001
    {	smInput	= -1,
	smOutput	= 0,
	smInOut	= 1
    } 	smIsInputEnum;

typedef /* [public][public][public] */ 
enum __MIDL___MIDL_itf_mssoap30_0135_0002
    {	stNone	= 0,
	stAttachment	= stNone + 1,
	stSentAttachments	= stAttachment + 1,
	stReceivedAttachments	= stSentAttachments + 1
    } 	enSpecialType;

typedef /* [public][public][public][public][public] */ 
enum __MIDL___MIDL_itf_mssoap30_0135_0003
    {	enXSDUndefined	= -1,
	enXSDDOM	= 0,
	enXSDstring	= enXSDDOM + 1,
	enXSDboolean	= enXSDstring + 1,
	enXSDfloat	= enXSDboolean + 1,
	enXSDDouble	= enXSDfloat + 1,
	enXSDdecimal	= enXSDDouble + 1,
	enXSDtimeDuration	= enXSDdecimal + 1,
	enXSDduration	= enXSDtimeDuration,
	enXSDrecurringDuration	= enXSDduration + 1,
	enXSDbinary	= enXSDrecurringDuration + 1,
	enXSDbase64binary	= enXSDbinary,
	enXSDuriReference	= enXSDbase64binary + 1,
	enXSDanyURI	= enXSDuriReference,
	enXSDid	= enXSDanyURI + 1,
	enXSDidRef	= enXSDid + 1,
	enXSDentity	= enXSDidRef + 1,
	enXSDQName	= enXSDentity + 1,
	enXSDcdata	= enXSDQName + 1,
	enXSDnormalizedString	= enXSDcdata,
	enXSDtoken	= enXSDnormalizedString + 1,
	enXSDlanguage	= enXSDtoken + 1,
	enXSDidRefs	= enXSDlanguage + 1,
	enXSDentities	= enXSDidRefs + 1,
	enXSDnmtoken	= enXSDentities + 1,
	enXSDnmtokens	= enXSDnmtoken + 1,
	enXSDname	= enXSDnmtokens + 1,
	enXSDncname	= enXSDname + 1,
	enXSDnotation	= enXSDncname + 1,
	enXSDinteger	= enXSDnotation + 1,
	enXSDnonpositiveInteger	= enXSDinteger + 1,
	enXSDlong	= enXSDnonpositiveInteger + 1,
	enXSDint	= enXSDlong + 1,
	enXSDshort	= enXSDint + 1,
	enXSDbyte	= enXSDshort + 1,
	enXSDnonNegativeInteger	= enXSDbyte + 1,
	enXSDnegativeInteger	= enXSDnonNegativeInteger + 1,
	enXSDunsignedLong	= enXSDnegativeInteger + 1,
	enXSDunsignedInt	= enXSDunsignedLong + 1,
	enXSDunsignedShort	= enXSDunsignedInt + 1,
	enXSDunsignedByte	= enXSDunsignedShort + 1,
	enXSDpositiveInteger	= enXSDunsignedByte + 1,
	enXSDtimeInstant	= enXSDpositiveInteger + 1,
	enXSDdatetime	= enXSDtimeInstant,
	enXSDtime	= enXSDdatetime + 1,
	enXSDtimePeriod	= enXSDtime + 1,
	enXSDdate	= enXSDtimePeriod + 1,
	enXSDmonth	= enXSDdate + 1,
	enXSDgMonth	= enXSDmonth,
	enXSDgYearMonth	= enXSDmonth,
	enXSDyear	= enXSDgYearMonth + 1,
	enXSDgYear	= enXSDyear,
	enXSDcentury	= enXSDgYear + 1,
	enXSDrecurringDate	= enXSDcentury + 1,
	enXSDgMonthDay	= enXSDrecurringDate,
	enXSDrecurringDay	= enXSDgMonthDay + 1,
	enXSDgDay	= enXSDrecurringDay,
	enXSDarray	= enXSDgDay + 1,
	enXSDanyType	= enXSDarray + 1,
	enTKempty	= enXSDanyType + 1,
	enXSDhexbinary	= enTKempty + 1,
	enXSDEndOfBuildin	= enXSDhexbinary + 1
    } 	enXSDType;

typedef /* [public][public][public][public][public][public] */ 
enum __MIDL___MIDL_itf_mssoap30_0135_0004
    {	enDocumentLiteral	= 0,
	enDocumentEncoded	= 0x1,
	enRPCLiteral	= 0x2,
	enRPCEncoded	= 0x4
    } 	enEncodingStyle;

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_mssoap30_0135_0005
    {	enOneWay	= 0,
	enRequestResponse	= 0x1
    } 	enOperationType;



EXTERN_C const IID LIBID_MSSOAPLib30;

#ifndef __ISoapReader_INTERFACE_DEFINED__
#define __ISoapReader_INTERFACE_DEFINED__

/* interface ISoapReader */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b21f31ca-0f45-4046-a231-cfb386e9e45f")
    ISoapReader : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ VARIANT par_source,
            /* [defaultvalue][in] */ BSTR par_soapAction,
            /* [retval][out] */ VARIANT_BOOL *par_success) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadWithParser( 
            /* [in] */ VARIANT par_source,
            /* [in] */ IMessageParser *par_parser,
            /* [defaultvalue][in] */ BSTR par_soapAction,
            /* [retval][out] */ VARIANT_BOOL *par_success) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadXml( 
            /* [in] */ BSTR par_xml,
            /* [retval][out] */ VARIANT_BOOL *par_success) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Dom( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMDocument **par_IXMLDOMDocument) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Envelope( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Body( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Header( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Fault( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_FaultString( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_FaultCode( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_FaultActor( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_FaultDetail( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_HeaderEntry( 
            /* [in] */ BSTR par_LocalName,
            /* [defaultvalue][in] */ BSTR par_NamespaceURI,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_MustUnderstandHeaderEntries( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_HeaderEntries( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_BodyEntries( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_BodyEntry( 
            /* [in] */ BSTR par_LocalName,
            /* [defaultvalue][in] */ BSTR par_NamespaceURI,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_RpcStruct( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_RpcParameter( 
            /* [in] */ BSTR par_LocalName,
            /* [defaultvalue][in] */ BSTR par_NamespaceURI,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_RpcResult( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SoapAction( 
            /* [retval][out] */ BSTR *par_SoapAction) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetContextItem( 
            /* [in] */ BSTR par_key,
            /* [retval][out] */ VARIANT *par_value) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetContextItem( 
            /* [in] */ BSTR par_key,
            /* [in] */ VARIANT par_value) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Attachments( 
            /* [retval][out] */ IReceivedAttachments **par_attachments) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetReferencedNode( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_node) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetReferencedAttachment( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ IReceivedAttachment **par_attachment) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsAttachmentReference( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ VARIANT_BOOL *par_result) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsNodeReference( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ VARIANT_BOOL *par_result) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Parser( 
            /* [retval][out] */ IMessageParser **par_parser) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_XmlVersion( 
            /* [in] */ BSTR par_XmlVersion) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapReader * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapReader * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapReader * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapReader * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapReader * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            ISoapReader * This,
            /* [in] */ VARIANT par_source,
            /* [defaultvalue][in] */ BSTR par_soapAction,
            /* [retval][out] */ VARIANT_BOOL *par_success);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadWithParser )( 
            ISoapReader * This,
            /* [in] */ VARIANT par_source,
            /* [in] */ IMessageParser *par_parser,
            /* [defaultvalue][in] */ BSTR par_soapAction,
            /* [retval][out] */ VARIANT_BOOL *par_success);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadXml )( 
            ISoapReader * This,
            /* [in] */ BSTR par_xml,
            /* [retval][out] */ VARIANT_BOOL *par_success);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Dom )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMDocument **par_IXMLDOMDocument);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Envelope )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Body )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Header )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Fault )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultString )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultCode )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultActor )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultDetail )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HeaderEntry )( 
            ISoapReader * This,
            /* [in] */ BSTR par_LocalName,
            /* [defaultvalue][in] */ BSTR par_NamespaceURI,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MustUnderstandHeaderEntries )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HeaderEntries )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyEntries )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BodyEntry )( 
            ISoapReader * This,
            /* [in] */ BSTR par_LocalName,
            /* [defaultvalue][in] */ BSTR par_NamespaceURI,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RpcStruct )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RpcParameter )( 
            ISoapReader * This,
            /* [in] */ BSTR par_LocalName,
            /* [defaultvalue][in] */ BSTR par_NamespaceURI,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RpcResult )( 
            ISoapReader * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SoapAction )( 
            ISoapReader * This,
            /* [retval][out] */ BSTR *par_SoapAction);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetContextItem )( 
            ISoapReader * This,
            /* [in] */ BSTR par_key,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetContextItem )( 
            ISoapReader * This,
            /* [in] */ BSTR par_key,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Attachments )( 
            ISoapReader * This,
            /* [retval][out] */ IReceivedAttachments **par_attachments);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetReferencedNode )( 
            ISoapReader * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_node);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetReferencedAttachment )( 
            ISoapReader * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ IReceivedAttachment **par_attachment);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsAttachmentReference )( 
            ISoapReader * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ VARIANT_BOOL *par_result);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *IsNodeReference )( 
            ISoapReader * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ VARIANT_BOOL *par_result);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Parser )( 
            ISoapReader * This,
            /* [retval][out] */ IMessageParser **par_parser);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_XmlVersion )( 
            ISoapReader * This,
            /* [in] */ BSTR par_XmlVersion);
        
        END_INTERFACE
    } ISoapReaderVtbl;

    interface ISoapReader
    {
        CONST_VTBL struct ISoapReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapReader_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapReader_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapReader_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapReader_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapReader_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapReader_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapReader_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapReader_Load(This,par_source,par_soapAction,par_success)	\
    (This)->lpVtbl -> Load(This,par_source,par_soapAction,par_success)

#define ISoapReader_LoadWithParser(This,par_source,par_parser,par_soapAction,par_success)	\
    (This)->lpVtbl -> LoadWithParser(This,par_source,par_parser,par_soapAction,par_success)

#define ISoapReader_LoadXml(This,par_xml,par_success)	\
    (This)->lpVtbl -> LoadXml(This,par_xml,par_success)

#define ISoapReader_get_Dom(This,par_IXMLDOMDocument)	\
    (This)->lpVtbl -> get_Dom(This,par_IXMLDOMDocument)

#define ISoapReader_get_Envelope(This,par_Element)	\
    (This)->lpVtbl -> get_Envelope(This,par_Element)

#define ISoapReader_get_Body(This,par_Element)	\
    (This)->lpVtbl -> get_Body(This,par_Element)

#define ISoapReader_get_Header(This,par_Element)	\
    (This)->lpVtbl -> get_Header(This,par_Element)

#define ISoapReader_get_Fault(This,par_Element)	\
    (This)->lpVtbl -> get_Fault(This,par_Element)

#define ISoapReader_get_FaultString(This,par_Element)	\
    (This)->lpVtbl -> get_FaultString(This,par_Element)

#define ISoapReader_get_FaultCode(This,par_Element)	\
    (This)->lpVtbl -> get_FaultCode(This,par_Element)

#define ISoapReader_get_FaultActor(This,par_Element)	\
    (This)->lpVtbl -> get_FaultActor(This,par_Element)

#define ISoapReader_get_FaultDetail(This,par_Element)	\
    (This)->lpVtbl -> get_FaultDetail(This,par_Element)

#define ISoapReader_get_HeaderEntry(This,par_LocalName,par_NamespaceURI,par_Element)	\
    (This)->lpVtbl -> get_HeaderEntry(This,par_LocalName,par_NamespaceURI,par_Element)

#define ISoapReader_get_MustUnderstandHeaderEntries(This,par_NodeList)	\
    (This)->lpVtbl -> get_MustUnderstandHeaderEntries(This,par_NodeList)

#define ISoapReader_get_HeaderEntries(This,par_NodeList)	\
    (This)->lpVtbl -> get_HeaderEntries(This,par_NodeList)

#define ISoapReader_get_BodyEntries(This,par_NodeList)	\
    (This)->lpVtbl -> get_BodyEntries(This,par_NodeList)

#define ISoapReader_get_BodyEntry(This,par_LocalName,par_NamespaceURI,par_Element)	\
    (This)->lpVtbl -> get_BodyEntry(This,par_LocalName,par_NamespaceURI,par_Element)

#define ISoapReader_get_RpcStruct(This,par_Element)	\
    (This)->lpVtbl -> get_RpcStruct(This,par_Element)

#define ISoapReader_get_RpcParameter(This,par_LocalName,par_NamespaceURI,par_Element)	\
    (This)->lpVtbl -> get_RpcParameter(This,par_LocalName,par_NamespaceURI,par_Element)

#define ISoapReader_get_RpcResult(This,par_Element)	\
    (This)->lpVtbl -> get_RpcResult(This,par_Element)

#define ISoapReader_get_SoapAction(This,par_SoapAction)	\
    (This)->lpVtbl -> get_SoapAction(This,par_SoapAction)

#define ISoapReader_GetContextItem(This,par_key,par_value)	\
    (This)->lpVtbl -> GetContextItem(This,par_key,par_value)

#define ISoapReader_SetContextItem(This,par_key,par_value)	\
    (This)->lpVtbl -> SetContextItem(This,par_key,par_value)

#define ISoapReader_get_Attachments(This,par_attachments)	\
    (This)->lpVtbl -> get_Attachments(This,par_attachments)

#define ISoapReader_GetReferencedNode(This,par_context,par_node)	\
    (This)->lpVtbl -> GetReferencedNode(This,par_context,par_node)

#define ISoapReader_GetReferencedAttachment(This,par_context,par_attachment)	\
    (This)->lpVtbl -> GetReferencedAttachment(This,par_context,par_attachment)

#define ISoapReader_IsAttachmentReference(This,par_context,par_result)	\
    (This)->lpVtbl -> IsAttachmentReference(This,par_context,par_result)

#define ISoapReader_IsNodeReference(This,par_context,par_result)	\
    (This)->lpVtbl -> IsNodeReference(This,par_context,par_result)

#define ISoapReader_get_Parser(This,par_parser)	\
    (This)->lpVtbl -> get_Parser(This,par_parser)

#define ISoapReader_put_XmlVersion(This,par_XmlVersion)	\
    (This)->lpVtbl -> put_XmlVersion(This,par_XmlVersion)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapReader_Load_Proxy( 
    ISoapReader * This,
    /* [in] */ VARIANT par_source,
    /* [defaultvalue][in] */ BSTR par_soapAction,
    /* [retval][out] */ VARIANT_BOOL *par_success);


void __RPC_STUB ISoapReader_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapReader_LoadWithParser_Proxy( 
    ISoapReader * This,
    /* [in] */ VARIANT par_source,
    /* [in] */ IMessageParser *par_parser,
    /* [defaultvalue][in] */ BSTR par_soapAction,
    /* [retval][out] */ VARIANT_BOOL *par_success);


void __RPC_STUB ISoapReader_LoadWithParser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapReader_LoadXml_Proxy( 
    ISoapReader * This,
    /* [in] */ BSTR par_xml,
    /* [retval][out] */ VARIANT_BOOL *par_success);


void __RPC_STUB ISoapReader_LoadXml_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_Dom_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMDocument **par_IXMLDOMDocument);


void __RPC_STUB ISoapReader_get_Dom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_Envelope_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_Envelope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_Body_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_Body_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_Header_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_Header_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_Fault_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_Fault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_FaultString_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_FaultString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_FaultCode_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_FaultCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_FaultActor_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_FaultActor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_FaultDetail_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_FaultDetail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_HeaderEntry_Proxy( 
    ISoapReader * This,
    /* [in] */ BSTR par_LocalName,
    /* [defaultvalue][in] */ BSTR par_NamespaceURI,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_HeaderEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_MustUnderstandHeaderEntries_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList);


void __RPC_STUB ISoapReader_get_MustUnderstandHeaderEntries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_HeaderEntries_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList);


void __RPC_STUB ISoapReader_get_HeaderEntries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_BodyEntries_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMNodeList **par_NodeList);


void __RPC_STUB ISoapReader_get_BodyEntries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_BodyEntry_Proxy( 
    ISoapReader * This,
    /* [in] */ BSTR par_LocalName,
    /* [defaultvalue][in] */ BSTR par_NamespaceURI,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_BodyEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_RpcStruct_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_RpcStruct_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_RpcParameter_Proxy( 
    ISoapReader * This,
    /* [in] */ BSTR par_LocalName,
    /* [defaultvalue][in] */ BSTR par_NamespaceURI,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_RpcParameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_RpcResult_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMElement **par_Element);


void __RPC_STUB ISoapReader_get_RpcResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_SoapAction_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ BSTR *par_SoapAction);


void __RPC_STUB ISoapReader_get_SoapAction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapReader_GetContextItem_Proxy( 
    ISoapReader * This,
    /* [in] */ BSTR par_key,
    /* [retval][out] */ VARIANT *par_value);


void __RPC_STUB ISoapReader_GetContextItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapReader_SetContextItem_Proxy( 
    ISoapReader * This,
    /* [in] */ BSTR par_key,
    /* [in] */ VARIANT par_value);


void __RPC_STUB ISoapReader_SetContextItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_Attachments_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ IReceivedAttachments **par_attachments);


void __RPC_STUB ISoapReader_get_Attachments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapReader_GetReferencedNode_Proxy( 
    ISoapReader * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
    /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_node);


void __RPC_STUB ISoapReader_GetReferencedNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapReader_GetReferencedAttachment_Proxy( 
    ISoapReader * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
    /* [retval][out] */ IReceivedAttachment **par_attachment);


void __RPC_STUB ISoapReader_GetReferencedAttachment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapReader_IsAttachmentReference_Proxy( 
    ISoapReader * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
    /* [retval][out] */ VARIANT_BOOL *par_result);


void __RPC_STUB ISoapReader_IsAttachmentReference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapReader_IsNodeReference_Proxy( 
    ISoapReader * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
    /* [retval][out] */ VARIANT_BOOL *par_result);


void __RPC_STUB ISoapReader_IsNodeReference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapReader_get_Parser_Proxy( 
    ISoapReader * This,
    /* [retval][out] */ IMessageParser **par_parser);


void __RPC_STUB ISoapReader_get_Parser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISoapReader_put_XmlVersion_Proxy( 
    ISoapReader * This,
    /* [in] */ BSTR par_XmlVersion);


void __RPC_STUB ISoapReader_put_XmlVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapReader_INTERFACE_DEFINED__ */


#ifndef __ISoapSerializer_INTERFACE_DEFINED__
#define __ISoapSerializer_INTERFACE_DEFINED__

/* interface ISoapSerializer */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapSerializer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("23bdf2b5-2304-4550-bbe2-f197e2cc47b6")
    ISoapSerializer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ VARIANT par_output) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitWithComposer( 
            /* [in] */ VARIANT par_output,
            /* [in] */ IMessageComposer *par_composer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartEnvelope( 
            /* [defaultvalue][in] */ BSTR par_Prefix = L"",
            /* [defaultvalue][in] */ BSTR par_enc_style_uri = L"NONE",
            /* [defaultvalue][in] */ BSTR par_encoding = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndEnvelope( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartHeader( 
            /* [defaultvalue][in] */ BSTR par_enc_style_uri = L"NONE") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartHeaderElement( 
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri = L"",
            /* [defaultvalue][in] */ int par_mustUnderstand = 0,
            /* [defaultvalue][in] */ BSTR par_actor_uri = L"",
            /* [defaultvalue][in] */ BSTR par_enc_style_uri = L"NONE",
            /* [defaultvalue][in] */ BSTR par_prefix = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndHeaderElement( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndHeader( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartBody( 
            /* [defaultvalue][in] */ BSTR par_enc_style_uri = L"NONE") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndBody( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartElement( 
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri = L"",
            /* [defaultvalue][in] */ BSTR par_enc_style_uri = L"NONE",
            /* [defaultvalue][in] */ BSTR par_prefix = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndElement( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoapAttribute( 
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri = L"",
            /* [defaultvalue][in] */ BSTR par_value = L"",
            /* [defaultvalue][in] */ BSTR par_prefix = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoapNamespace( 
            /* [in] */ BSTR par_prefix,
            /* [in] */ BSTR par_ns_uri) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoapDefaultNamespace( 
            /* [in] */ BSTR par_ns_uri) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteString( 
            /* [in] */ BSTR par_string) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteBuffer( 
            /* [in] */ long par_len,
            /* [in] */ char *par_buffer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartFault( 
            /* [in] */ BSTR par_FaultCode,
            /* [in] */ BSTR par_FaultString,
            /* [defaultvalue][in] */ BSTR par_FaultActor = L"",
            /* [defaultvalue][in] */ BSTR par_FaultCodeNS = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartFaultDetail( 
            /* [defaultvalue][in] */ BSTR par_enc_style_uri = L"NONE") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndFaultDetail( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndFault( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteXml( 
            /* [in] */ BSTR par_string) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPrefixForNamespace( 
            /* [in] */ BSTR par_ns_string,
            /* [retval][out] */ BSTR *par_ns_prefix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndHrefedElement( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartHrefedElement( 
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri,
            /* [defaultvalue][in] */ BSTR par_prefix,
            /* [defaultvalue][in] */ enElementLocation par_location,
            /* [defaultvalue][in] */ BSTR par_hrefidinput,
            /* [retval][out] */ BSTR *par_hrefid) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SoapFault( 
            /* [retval][out] */ VARIANT_BOOL *par_fault) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetContextItem( 
            /* [in] */ BSTR par_key,
            /* [retval][out] */ VARIANT *par_value) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetContextItem( 
            /* [in] */ BSTR par_key,
            /* [in] */ VARIANT par_value) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddAttachment( 
            /* [in] */ IAttachment *par_att) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddAttachmentAndReference( 
            /* [in] */ IAttachment *par_att) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Finished( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Composer( 
            /* [retval][out] */ IMessageComposer **par_composer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateHRefId( 
            /* [retval][out] */ BSTR *par_hrefid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapSerializerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapSerializer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapSerializer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapSerializer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapSerializer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapSerializer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapSerializer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapSerializer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            ISoapSerializer * This,
            /* [in] */ VARIANT par_output);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitWithComposer )( 
            ISoapSerializer * This,
            /* [in] */ VARIANT par_output,
            /* [in] */ IMessageComposer *par_composer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartEnvelope )( 
            ISoapSerializer * This,
            /* [defaultvalue][in] */ BSTR par_Prefix,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri,
            /* [defaultvalue][in] */ BSTR par_encoding);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndEnvelope )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartHeader )( 
            ISoapSerializer * This,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartHeaderElement )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri,
            /* [defaultvalue][in] */ int par_mustUnderstand,
            /* [defaultvalue][in] */ BSTR par_actor_uri,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri,
            /* [defaultvalue][in] */ BSTR par_prefix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndHeaderElement )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndHeader )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartBody )( 
            ISoapSerializer * This,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndBody )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartElement )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri,
            /* [defaultvalue][in] */ BSTR par_prefix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndElement )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoapAttribute )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri,
            /* [defaultvalue][in] */ BSTR par_value,
            /* [defaultvalue][in] */ BSTR par_prefix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoapNamespace )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_prefix,
            /* [in] */ BSTR par_ns_uri);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoapDefaultNamespace )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_ns_uri);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteString )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_string);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteBuffer )( 
            ISoapSerializer * This,
            /* [in] */ long par_len,
            /* [in] */ char *par_buffer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartFault )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_FaultCode,
            /* [in] */ BSTR par_FaultString,
            /* [defaultvalue][in] */ BSTR par_FaultActor,
            /* [defaultvalue][in] */ BSTR par_FaultCodeNS);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartFaultDetail )( 
            ISoapSerializer * This,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndFaultDetail )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndFault )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteXml )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_string);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPrefixForNamespace )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_ns_string,
            /* [retval][out] */ BSTR *par_ns_prefix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndHrefedElement )( 
            ISoapSerializer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartHrefedElement )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ BSTR par_ns_uri,
            /* [defaultvalue][in] */ BSTR par_enc_style_uri,
            /* [defaultvalue][in] */ BSTR par_prefix,
            /* [defaultvalue][in] */ enElementLocation par_location,
            /* [defaultvalue][in] */ BSTR par_hrefidinput,
            /* [retval][out] */ BSTR *par_hrefid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SoapFault )( 
            ISoapSerializer * This,
            /* [retval][out] */ VARIANT_BOOL *par_fault);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetContextItem )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_key,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetContextItem )( 
            ISoapSerializer * This,
            /* [in] */ BSTR par_key,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddAttachment )( 
            ISoapSerializer * This,
            /* [in] */ IAttachment *par_att);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddAttachmentAndReference )( 
            ISoapSerializer * This,
            /* [in] */ IAttachment *par_att);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Finished )( 
            ISoapSerializer * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Composer )( 
            ISoapSerializer * This,
            /* [retval][out] */ IMessageComposer **par_composer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateHRefId )( 
            ISoapSerializer * This,
            /* [retval][out] */ BSTR *par_hrefid);
        
        END_INTERFACE
    } ISoapSerializerVtbl;

    interface ISoapSerializer
    {
        CONST_VTBL struct ISoapSerializerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapSerializer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapSerializer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapSerializer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapSerializer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapSerializer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapSerializer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapSerializer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapSerializer_Init(This,par_output)	\
    (This)->lpVtbl -> Init(This,par_output)

#define ISoapSerializer_InitWithComposer(This,par_output,par_composer)	\
    (This)->lpVtbl -> InitWithComposer(This,par_output,par_composer)

#define ISoapSerializer_StartEnvelope(This,par_Prefix,par_enc_style_uri,par_encoding)	\
    (This)->lpVtbl -> StartEnvelope(This,par_Prefix,par_enc_style_uri,par_encoding)

#define ISoapSerializer_EndEnvelope(This)	\
    (This)->lpVtbl -> EndEnvelope(This)

#define ISoapSerializer_StartHeader(This,par_enc_style_uri)	\
    (This)->lpVtbl -> StartHeader(This,par_enc_style_uri)

#define ISoapSerializer_StartHeaderElement(This,par_name,par_ns_uri,par_mustUnderstand,par_actor_uri,par_enc_style_uri,par_prefix)	\
    (This)->lpVtbl -> StartHeaderElement(This,par_name,par_ns_uri,par_mustUnderstand,par_actor_uri,par_enc_style_uri,par_prefix)

#define ISoapSerializer_EndHeaderElement(This)	\
    (This)->lpVtbl -> EndHeaderElement(This)

#define ISoapSerializer_EndHeader(This)	\
    (This)->lpVtbl -> EndHeader(This)

#define ISoapSerializer_StartBody(This,par_enc_style_uri)	\
    (This)->lpVtbl -> StartBody(This,par_enc_style_uri)

#define ISoapSerializer_EndBody(This)	\
    (This)->lpVtbl -> EndBody(This)

#define ISoapSerializer_StartElement(This,par_name,par_ns_uri,par_enc_style_uri,par_prefix)	\
    (This)->lpVtbl -> StartElement(This,par_name,par_ns_uri,par_enc_style_uri,par_prefix)

#define ISoapSerializer_EndElement(This)	\
    (This)->lpVtbl -> EndElement(This)

#define ISoapSerializer_SoapAttribute(This,par_name,par_ns_uri,par_value,par_prefix)	\
    (This)->lpVtbl -> SoapAttribute(This,par_name,par_ns_uri,par_value,par_prefix)

#define ISoapSerializer_SoapNamespace(This,par_prefix,par_ns_uri)	\
    (This)->lpVtbl -> SoapNamespace(This,par_prefix,par_ns_uri)

#define ISoapSerializer_SoapDefaultNamespace(This,par_ns_uri)	\
    (This)->lpVtbl -> SoapDefaultNamespace(This,par_ns_uri)

#define ISoapSerializer_WriteString(This,par_string)	\
    (This)->lpVtbl -> WriteString(This,par_string)

#define ISoapSerializer_WriteBuffer(This,par_len,par_buffer)	\
    (This)->lpVtbl -> WriteBuffer(This,par_len,par_buffer)

#define ISoapSerializer_StartFault(This,par_FaultCode,par_FaultString,par_FaultActor,par_FaultCodeNS)	\
    (This)->lpVtbl -> StartFault(This,par_FaultCode,par_FaultString,par_FaultActor,par_FaultCodeNS)

#define ISoapSerializer_StartFaultDetail(This,par_enc_style_uri)	\
    (This)->lpVtbl -> StartFaultDetail(This,par_enc_style_uri)

#define ISoapSerializer_EndFaultDetail(This)	\
    (This)->lpVtbl -> EndFaultDetail(This)

#define ISoapSerializer_EndFault(This)	\
    (This)->lpVtbl -> EndFault(This)

#define ISoapSerializer_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define ISoapSerializer_WriteXml(This,par_string)	\
    (This)->lpVtbl -> WriteXml(This,par_string)

#define ISoapSerializer_GetPrefixForNamespace(This,par_ns_string,par_ns_prefix)	\
    (This)->lpVtbl -> GetPrefixForNamespace(This,par_ns_string,par_ns_prefix)

#define ISoapSerializer_EndHrefedElement(This)	\
    (This)->lpVtbl -> EndHrefedElement(This)

#define ISoapSerializer_StartHrefedElement(This,par_name,par_ns_uri,par_enc_style_uri,par_prefix,par_location,par_hrefidinput,par_hrefid)	\
    (This)->lpVtbl -> StartHrefedElement(This,par_name,par_ns_uri,par_enc_style_uri,par_prefix,par_location,par_hrefidinput,par_hrefid)

#define ISoapSerializer_get_SoapFault(This,par_fault)	\
    (This)->lpVtbl -> get_SoapFault(This,par_fault)

#define ISoapSerializer_GetContextItem(This,par_key,par_value)	\
    (This)->lpVtbl -> GetContextItem(This,par_key,par_value)

#define ISoapSerializer_SetContextItem(This,par_key,par_value)	\
    (This)->lpVtbl -> SetContextItem(This,par_key,par_value)

#define ISoapSerializer_AddAttachment(This,par_att)	\
    (This)->lpVtbl -> AddAttachment(This,par_att)

#define ISoapSerializer_AddAttachmentAndReference(This,par_att)	\
    (This)->lpVtbl -> AddAttachmentAndReference(This,par_att)

#define ISoapSerializer_Finished(This)	\
    (This)->lpVtbl -> Finished(This)

#define ISoapSerializer_get_Composer(This,par_composer)	\
    (This)->lpVtbl -> get_Composer(This,par_composer)

#define ISoapSerializer_CreateHRefId(This,par_hrefid)	\
    (This)->lpVtbl -> CreateHRefId(This,par_hrefid)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_Init_Proxy( 
    ISoapSerializer * This,
    /* [in] */ VARIANT par_output);


void __RPC_STUB ISoapSerializer_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_InitWithComposer_Proxy( 
    ISoapSerializer * This,
    /* [in] */ VARIANT par_output,
    /* [in] */ IMessageComposer *par_composer);


void __RPC_STUB ISoapSerializer_InitWithComposer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartEnvelope_Proxy( 
    ISoapSerializer * This,
    /* [defaultvalue][in] */ BSTR par_Prefix,
    /* [defaultvalue][in] */ BSTR par_enc_style_uri,
    /* [defaultvalue][in] */ BSTR par_encoding);


void __RPC_STUB ISoapSerializer_StartEnvelope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndEnvelope_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndEnvelope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartHeader_Proxy( 
    ISoapSerializer * This,
    /* [defaultvalue][in] */ BSTR par_enc_style_uri);


void __RPC_STUB ISoapSerializer_StartHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartHeaderElement_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_name,
    /* [defaultvalue][in] */ BSTR par_ns_uri,
    /* [defaultvalue][in] */ int par_mustUnderstand,
    /* [defaultvalue][in] */ BSTR par_actor_uri,
    /* [defaultvalue][in] */ BSTR par_enc_style_uri,
    /* [defaultvalue][in] */ BSTR par_prefix);


void __RPC_STUB ISoapSerializer_StartHeaderElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndHeaderElement_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndHeaderElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndHeader_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartBody_Proxy( 
    ISoapSerializer * This,
    /* [defaultvalue][in] */ BSTR par_enc_style_uri);


void __RPC_STUB ISoapSerializer_StartBody_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndBody_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndBody_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartElement_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_name,
    /* [defaultvalue][in] */ BSTR par_ns_uri,
    /* [defaultvalue][in] */ BSTR par_enc_style_uri,
    /* [defaultvalue][in] */ BSTR par_prefix);


void __RPC_STUB ISoapSerializer_StartElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndElement_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_SoapAttribute_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_name,
    /* [defaultvalue][in] */ BSTR par_ns_uri,
    /* [defaultvalue][in] */ BSTR par_value,
    /* [defaultvalue][in] */ BSTR par_prefix);


void __RPC_STUB ISoapSerializer_SoapAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_SoapNamespace_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_prefix,
    /* [in] */ BSTR par_ns_uri);


void __RPC_STUB ISoapSerializer_SoapNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_SoapDefaultNamespace_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_ns_uri);


void __RPC_STUB ISoapSerializer_SoapDefaultNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_WriteString_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_string);


void __RPC_STUB ISoapSerializer_WriteString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_WriteBuffer_Proxy( 
    ISoapSerializer * This,
    /* [in] */ long par_len,
    /* [in] */ char *par_buffer);


void __RPC_STUB ISoapSerializer_WriteBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartFault_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_FaultCode,
    /* [in] */ BSTR par_FaultString,
    /* [defaultvalue][in] */ BSTR par_FaultActor,
    /* [defaultvalue][in] */ BSTR par_FaultCodeNS);


void __RPC_STUB ISoapSerializer_StartFault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartFaultDetail_Proxy( 
    ISoapSerializer * This,
    /* [defaultvalue][in] */ BSTR par_enc_style_uri);


void __RPC_STUB ISoapSerializer_StartFaultDetail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndFaultDetail_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndFaultDetail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndFault_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndFault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_Reset_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_WriteXml_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_string);


void __RPC_STUB ISoapSerializer_WriteXml_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_GetPrefixForNamespace_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_ns_string,
    /* [retval][out] */ BSTR *par_ns_prefix);


void __RPC_STUB ISoapSerializer_GetPrefixForNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_EndHrefedElement_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_EndHrefedElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_StartHrefedElement_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_name,
    /* [defaultvalue][in] */ BSTR par_ns_uri,
    /* [defaultvalue][in] */ BSTR par_enc_style_uri,
    /* [defaultvalue][in] */ BSTR par_prefix,
    /* [defaultvalue][in] */ enElementLocation par_location,
    /* [defaultvalue][in] */ BSTR par_hrefidinput,
    /* [retval][out] */ BSTR *par_hrefid);


void __RPC_STUB ISoapSerializer_StartHrefedElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_get_SoapFault_Proxy( 
    ISoapSerializer * This,
    /* [retval][out] */ VARIANT_BOOL *par_fault);


void __RPC_STUB ISoapSerializer_get_SoapFault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_GetContextItem_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_key,
    /* [retval][out] */ VARIANT *par_value);


void __RPC_STUB ISoapSerializer_GetContextItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_SetContextItem_Proxy( 
    ISoapSerializer * This,
    /* [in] */ BSTR par_key,
    /* [in] */ VARIANT par_value);


void __RPC_STUB ISoapSerializer_SetContextItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_AddAttachment_Proxy( 
    ISoapSerializer * This,
    /* [in] */ IAttachment *par_att);


void __RPC_STUB ISoapSerializer_AddAttachment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_AddAttachmentAndReference_Proxy( 
    ISoapSerializer * This,
    /* [in] */ IAttachment *par_att);


void __RPC_STUB ISoapSerializer_AddAttachmentAndReference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_Finished_Proxy( 
    ISoapSerializer * This);


void __RPC_STUB ISoapSerializer_Finished_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_get_Composer_Proxy( 
    ISoapSerializer * This,
    /* [retval][out] */ IMessageComposer **par_composer);


void __RPC_STUB ISoapSerializer_get_Composer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapSerializer_CreateHRefId_Proxy( 
    ISoapSerializer * This,
    /* [retval][out] */ BSTR *par_hrefid);


void __RPC_STUB ISoapSerializer_CreateHRefId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapSerializer_INTERFACE_DEFINED__ */


#ifndef __ISoapMapper_INTERFACE_DEFINED__
#define __ISoapMapper_INTERFACE_DEFINED__

/* interface ISoapMapper */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapMapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c1e6061a-f8dc-4ca8-a952-faf7419f1029")
    ISoapMapper : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ElementName( 
            /* [retval][out] */ BSTR *par_ElementName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_PartName( 
            /* [retval][out] */ BSTR *par_PartName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ElementType( 
            /* [retval][out] */ BSTR *par_ElementType) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_IsInput( 
            /* [retval][out] */ smIsInputEnum *par_Input) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ComValue( 
            /* [retval][out] */ VARIANT *par_VarOut) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_ComValue( 
            /* [in] */ VARIANT par_varIn) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_CallIndex( 
            /* [retval][out] */ LONG *par_CallIndex) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ParameterOrder( 
            /* [retval][out] */ LONG *par_paraOrder) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_XmlNamespace( 
            /* [retval][out] */ BSTR *par_xmlNameSpace) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_VariantType( 
            /* [retval][out] */ long *par_Type) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_XsdType( 
            /* [retval][out] */ enXSDType *par_Type) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SpecialType( 
            /* [retval][out] */ enSpecialType *par_SpecialType) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            BSTR par_Encoding,
            enEncodingStyle par_enSaveStyle,
            BSTR par_MessageNamespace,
            long par_Flags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_Node,
            BSTR par_Encoding,
            enEncodingStyle par_enStyle,
            BSTR par_MessageNamespace,
            long par_Flags) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SchemaNode( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_TypeNode) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SchemaNamespace( 
            /* [retval][out] */ BSTR *par_Namespace) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_PartName( 
            /* [in] */ BSTR par_partName) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_SpecialType( 
            /* [in] */ enSpecialType par_SpecialType) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_CallIndex( 
            /* [in] */ LONG par_CallIndex) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapMapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapMapper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapMapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapMapper * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ElementName )( 
            ISoapMapper * This,
            /* [retval][out] */ BSTR *par_ElementName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PartName )( 
            ISoapMapper * This,
            /* [retval][out] */ BSTR *par_PartName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ElementType )( 
            ISoapMapper * This,
            /* [retval][out] */ BSTR *par_ElementType);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsInput )( 
            ISoapMapper * This,
            /* [retval][out] */ smIsInputEnum *par_Input);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ComValue )( 
            ISoapMapper * This,
            /* [retval][out] */ VARIANT *par_VarOut);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ComValue )( 
            ISoapMapper * This,
            /* [in] */ VARIANT par_varIn);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CallIndex )( 
            ISoapMapper * This,
            /* [retval][out] */ LONG *par_CallIndex);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParameterOrder )( 
            ISoapMapper * This,
            /* [retval][out] */ LONG *par_paraOrder);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XmlNamespace )( 
            ISoapMapper * This,
            /* [retval][out] */ BSTR *par_xmlNameSpace);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VariantType )( 
            ISoapMapper * This,
            /* [retval][out] */ long *par_Type);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XsdType )( 
            ISoapMapper * This,
            /* [retval][out] */ enXSDType *par_Type);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SpecialType )( 
            ISoapMapper * This,
            /* [retval][out] */ enSpecialType *par_SpecialType);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            ISoapMapper * This,
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            BSTR par_Encoding,
            enEncodingStyle par_enSaveStyle,
            BSTR par_MessageNamespace,
            long par_Flags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            ISoapMapper * This,
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_Node,
            BSTR par_Encoding,
            enEncodingStyle par_enStyle,
            BSTR par_MessageNamespace,
            long par_Flags);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SchemaNode )( 
            ISoapMapper * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_TypeNode);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SchemaNamespace )( 
            ISoapMapper * This,
            /* [retval][out] */ BSTR *par_Namespace);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PartName )( 
            ISoapMapper * This,
            /* [in] */ BSTR par_partName);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SpecialType )( 
            ISoapMapper * This,
            /* [in] */ enSpecialType par_SpecialType);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CallIndex )( 
            ISoapMapper * This,
            /* [in] */ LONG par_CallIndex);
        
        END_INTERFACE
    } ISoapMapperVtbl;

    interface ISoapMapper
    {
        CONST_VTBL struct ISoapMapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapMapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapMapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapMapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapMapper_get_ElementName(This,par_ElementName)	\
    (This)->lpVtbl -> get_ElementName(This,par_ElementName)

#define ISoapMapper_get_PartName(This,par_PartName)	\
    (This)->lpVtbl -> get_PartName(This,par_PartName)

#define ISoapMapper_get_ElementType(This,par_ElementType)	\
    (This)->lpVtbl -> get_ElementType(This,par_ElementType)

#define ISoapMapper_get_IsInput(This,par_Input)	\
    (This)->lpVtbl -> get_IsInput(This,par_Input)

#define ISoapMapper_get_ComValue(This,par_VarOut)	\
    (This)->lpVtbl -> get_ComValue(This,par_VarOut)

#define ISoapMapper_put_ComValue(This,par_varIn)	\
    (This)->lpVtbl -> put_ComValue(This,par_varIn)

#define ISoapMapper_get_CallIndex(This,par_CallIndex)	\
    (This)->lpVtbl -> get_CallIndex(This,par_CallIndex)

#define ISoapMapper_get_ParameterOrder(This,par_paraOrder)	\
    (This)->lpVtbl -> get_ParameterOrder(This,par_paraOrder)

#define ISoapMapper_get_XmlNamespace(This,par_xmlNameSpace)	\
    (This)->lpVtbl -> get_XmlNamespace(This,par_xmlNameSpace)

#define ISoapMapper_get_VariantType(This,par_Type)	\
    (This)->lpVtbl -> get_VariantType(This,par_Type)

#define ISoapMapper_get_XsdType(This,par_Type)	\
    (This)->lpVtbl -> get_XsdType(This,par_Type)

#define ISoapMapper_get_SpecialType(This,par_SpecialType)	\
    (This)->lpVtbl -> get_SpecialType(This,par_SpecialType)

#define ISoapMapper_Save(This,par_ISoapSerializer,par_Encoding,par_enSaveStyle,par_MessageNamespace,par_Flags)	\
    (This)->lpVtbl -> Save(This,par_ISoapSerializer,par_Encoding,par_enSaveStyle,par_MessageNamespace,par_Flags)

#define ISoapMapper_Load(This,par_ISoapReader,par_Node,par_Encoding,par_enStyle,par_MessageNamespace,par_Flags)	\
    (This)->lpVtbl -> Load(This,par_ISoapReader,par_Node,par_Encoding,par_enStyle,par_MessageNamespace,par_Flags)

#define ISoapMapper_get_SchemaNode(This,par_TypeNode)	\
    (This)->lpVtbl -> get_SchemaNode(This,par_TypeNode)

#define ISoapMapper_get_SchemaNamespace(This,par_Namespace)	\
    (This)->lpVtbl -> get_SchemaNamespace(This,par_Namespace)

#define ISoapMapper_put_PartName(This,par_partName)	\
    (This)->lpVtbl -> put_PartName(This,par_partName)

#define ISoapMapper_put_SpecialType(This,par_SpecialType)	\
    (This)->lpVtbl -> put_SpecialType(This,par_SpecialType)

#define ISoapMapper_put_CallIndex(This,par_CallIndex)	\
    (This)->lpVtbl -> put_CallIndex(This,par_CallIndex)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_ElementName_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ BSTR *par_ElementName);


void __RPC_STUB ISoapMapper_get_ElementName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_PartName_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ BSTR *par_PartName);


void __RPC_STUB ISoapMapper_get_PartName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_ElementType_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ BSTR *par_ElementType);


void __RPC_STUB ISoapMapper_get_ElementType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_IsInput_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ smIsInputEnum *par_Input);


void __RPC_STUB ISoapMapper_get_IsInput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_ComValue_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ VARIANT *par_VarOut);


void __RPC_STUB ISoapMapper_get_ComValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISoapMapper_put_ComValue_Proxy( 
    ISoapMapper * This,
    /* [in] */ VARIANT par_varIn);


void __RPC_STUB ISoapMapper_put_ComValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_CallIndex_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ LONG *par_CallIndex);


void __RPC_STUB ISoapMapper_get_CallIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_ParameterOrder_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ LONG *par_paraOrder);


void __RPC_STUB ISoapMapper_get_ParameterOrder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_XmlNamespace_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ BSTR *par_xmlNameSpace);


void __RPC_STUB ISoapMapper_get_XmlNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_VariantType_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ long *par_Type);


void __RPC_STUB ISoapMapper_get_VariantType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_XsdType_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ enXSDType *par_Type);


void __RPC_STUB ISoapMapper_get_XsdType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_SpecialType_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ enSpecialType *par_SpecialType);


void __RPC_STUB ISoapMapper_get_SpecialType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapMapper_Save_Proxy( 
    ISoapMapper * This,
    /* [in] */ ISoapSerializer *par_ISoapSerializer,
    BSTR par_Encoding,
    enEncodingStyle par_enSaveStyle,
    BSTR par_MessageNamespace,
    long par_Flags);


void __RPC_STUB ISoapMapper_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapMapper_Load_Proxy( 
    ISoapMapper * This,
    /* [in] */ ISoapReader *par_ISoapReader,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_Node,
    BSTR par_Encoding,
    enEncodingStyle par_enStyle,
    BSTR par_MessageNamespace,
    long par_Flags);


void __RPC_STUB ISoapMapper_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_SchemaNode_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_TypeNode);


void __RPC_STUB ISoapMapper_get_SchemaNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapMapper_get_SchemaNamespace_Proxy( 
    ISoapMapper * This,
    /* [retval][out] */ BSTR *par_Namespace);


void __RPC_STUB ISoapMapper_get_SchemaNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISoapMapper_put_PartName_Proxy( 
    ISoapMapper * This,
    /* [in] */ BSTR par_partName);


void __RPC_STUB ISoapMapper_put_PartName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISoapMapper_put_SpecialType_Proxy( 
    ISoapMapper * This,
    /* [in] */ enSpecialType par_SpecialType);


void __RPC_STUB ISoapMapper_put_SpecialType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISoapMapper_put_CallIndex_Proxy( 
    ISoapMapper * This,
    /* [in] */ LONG par_CallIndex);


void __RPC_STUB ISoapMapper_put_CallIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapMapper_INTERFACE_DEFINED__ */


#ifndef __IDataEncoder_INTERFACE_DEFINED__
#define __IDataEncoder_INTERFACE_DEFINED__

/* interface IDataEncoder */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IDataEncoder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("663EB158-8D95-4657-AE32-B7C60DE6122F")
    IDataEncoder : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Encoding( 
            /* [retval][out] */ BSTR *par_encoding) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SizeToEncode( 
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [out] */ DWORD *par_To) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SizeToDecode( 
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [out] */ DWORD *par_To) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Encode( 
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [length_is][size_is][out] */ void *par_To,
            /* [out][in] */ DWORD *par_ToSize) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Decode( 
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [length_is][size_is][out] */ void *par_To,
            /* [out][in] */ DWORD *par_ToSize) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SizeToEncodeStream( 
            /* [in] */ IStream *par_From,
            /* [out] */ DWORD *par_To) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SizeToDecodeStream( 
            /* [in] */ IStream *par_From,
            /* [out] */ DWORD *par_To) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EncodeStream( 
            /* [in] */ IStream *par_Form,
            /* [in] */ IStream *par_To) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DecodeStream( 
            /* [in] */ IStream *par_From,
            /* [in] */ IStream *par_To) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDataEncoderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDataEncoder * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDataEncoder * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDataEncoder * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Encoding )( 
            IDataEncoder * This,
            /* [retval][out] */ BSTR *par_encoding);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SizeToEncode )( 
            IDataEncoder * This,
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [out] */ DWORD *par_To);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SizeToDecode )( 
            IDataEncoder * This,
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [out] */ DWORD *par_To);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Encode )( 
            IDataEncoder * This,
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [length_is][size_is][out] */ void *par_To,
            /* [out][in] */ DWORD *par_ToSize);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Decode )( 
            IDataEncoder * This,
            /* [size_is][in] */ const void *par_From,
            /* [in] */ DWORD par_FromSize,
            /* [length_is][size_is][out] */ void *par_To,
            /* [out][in] */ DWORD *par_ToSize);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SizeToEncodeStream )( 
            IDataEncoder * This,
            /* [in] */ IStream *par_From,
            /* [out] */ DWORD *par_To);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SizeToDecodeStream )( 
            IDataEncoder * This,
            /* [in] */ IStream *par_From,
            /* [out] */ DWORD *par_To);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EncodeStream )( 
            IDataEncoder * This,
            /* [in] */ IStream *par_Form,
            /* [in] */ IStream *par_To);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DecodeStream )( 
            IDataEncoder * This,
            /* [in] */ IStream *par_From,
            /* [in] */ IStream *par_To);
        
        END_INTERFACE
    } IDataEncoderVtbl;

    interface IDataEncoder
    {
        CONST_VTBL struct IDataEncoderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataEncoder_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDataEncoder_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDataEncoder_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDataEncoder_get_Encoding(This,par_encoding)	\
    (This)->lpVtbl -> get_Encoding(This,par_encoding)

#define IDataEncoder_SizeToEncode(This,par_From,par_FromSize,par_To)	\
    (This)->lpVtbl -> SizeToEncode(This,par_From,par_FromSize,par_To)

#define IDataEncoder_SizeToDecode(This,par_From,par_FromSize,par_To)	\
    (This)->lpVtbl -> SizeToDecode(This,par_From,par_FromSize,par_To)

#define IDataEncoder_Encode(This,par_From,par_FromSize,par_To,par_ToSize)	\
    (This)->lpVtbl -> Encode(This,par_From,par_FromSize,par_To,par_ToSize)

#define IDataEncoder_Decode(This,par_From,par_FromSize,par_To,par_ToSize)	\
    (This)->lpVtbl -> Decode(This,par_From,par_FromSize,par_To,par_ToSize)

#define IDataEncoder_SizeToEncodeStream(This,par_From,par_To)	\
    (This)->lpVtbl -> SizeToEncodeStream(This,par_From,par_To)

#define IDataEncoder_SizeToDecodeStream(This,par_From,par_To)	\
    (This)->lpVtbl -> SizeToDecodeStream(This,par_From,par_To)

#define IDataEncoder_EncodeStream(This,par_Form,par_To)	\
    (This)->lpVtbl -> EncodeStream(This,par_Form,par_To)

#define IDataEncoder_DecodeStream(This,par_From,par_To)	\
    (This)->lpVtbl -> DecodeStream(This,par_From,par_To)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IDataEncoder_get_Encoding_Proxy( 
    IDataEncoder * This,
    /* [retval][out] */ BSTR *par_encoding);


void __RPC_STUB IDataEncoder_get_Encoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_SizeToEncode_Proxy( 
    IDataEncoder * This,
    /* [size_is][in] */ const void *par_From,
    /* [in] */ DWORD par_FromSize,
    /* [out] */ DWORD *par_To);


void __RPC_STUB IDataEncoder_SizeToEncode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_SizeToDecode_Proxy( 
    IDataEncoder * This,
    /* [size_is][in] */ const void *par_From,
    /* [in] */ DWORD par_FromSize,
    /* [out] */ DWORD *par_To);


void __RPC_STUB IDataEncoder_SizeToDecode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_Encode_Proxy( 
    IDataEncoder * This,
    /* [size_is][in] */ const void *par_From,
    /* [in] */ DWORD par_FromSize,
    /* [length_is][size_is][out] */ void *par_To,
    /* [out][in] */ DWORD *par_ToSize);


void __RPC_STUB IDataEncoder_Encode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_Decode_Proxy( 
    IDataEncoder * This,
    /* [size_is][in] */ const void *par_From,
    /* [in] */ DWORD par_FromSize,
    /* [length_is][size_is][out] */ void *par_To,
    /* [out][in] */ DWORD *par_ToSize);


void __RPC_STUB IDataEncoder_Decode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_SizeToEncodeStream_Proxy( 
    IDataEncoder * This,
    /* [in] */ IStream *par_From,
    /* [out] */ DWORD *par_To);


void __RPC_STUB IDataEncoder_SizeToEncodeStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_SizeToDecodeStream_Proxy( 
    IDataEncoder * This,
    /* [in] */ IStream *par_From,
    /* [out] */ DWORD *par_To);


void __RPC_STUB IDataEncoder_SizeToDecodeStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_EncodeStream_Proxy( 
    IDataEncoder * This,
    /* [in] */ IStream *par_Form,
    /* [in] */ IStream *par_To);


void __RPC_STUB IDataEncoder_EncodeStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoder_DecodeStream_Proxy( 
    IDataEncoder * This,
    /* [in] */ IStream *par_From,
    /* [in] */ IStream *par_To);


void __RPC_STUB IDataEncoder_DecodeStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDataEncoder_INTERFACE_DEFINED__ */


#ifndef __IDataEncoderFactory_INTERFACE_DEFINED__
#define __IDataEncoderFactory_INTERFACE_DEFINED__

/* interface IDataEncoderFactory */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IDataEncoderFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("456C5AB4-2A2A-4289-9D4C-0C28BF739EE4")
    IDataEncoderFactory : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddDataEncoder( 
            /* [in] */ BSTR par_encoding,
            /* [in] */ IDataEncoder *par_encoder) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDataEncoder( 
            /* [in] */ BSTR par_encoding,
            /* [retval][out] */ IDataEncoder **par_encoder) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDataEncoderFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDataEncoderFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDataEncoderFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDataEncoderFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDataEncoderFactory * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDataEncoderFactory * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDataEncoderFactory * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDataEncoderFactory * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddDataEncoder )( 
            IDataEncoderFactory * This,
            /* [in] */ BSTR par_encoding,
            /* [in] */ IDataEncoder *par_encoder);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDataEncoder )( 
            IDataEncoderFactory * This,
            /* [in] */ BSTR par_encoding,
            /* [retval][out] */ IDataEncoder **par_encoder);
        
        END_INTERFACE
    } IDataEncoderFactoryVtbl;

    interface IDataEncoderFactory
    {
        CONST_VTBL struct IDataEncoderFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataEncoderFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDataEncoderFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDataEncoderFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDataEncoderFactory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDataEncoderFactory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDataEncoderFactory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDataEncoderFactory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDataEncoderFactory_AddDataEncoder(This,par_encoding,par_encoder)	\
    (This)->lpVtbl -> AddDataEncoder(This,par_encoding,par_encoder)

#define IDataEncoderFactory_GetDataEncoder(This,par_encoding,par_encoder)	\
    (This)->lpVtbl -> GetDataEncoder(This,par_encoding,par_encoder)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoderFactory_AddDataEncoder_Proxy( 
    IDataEncoderFactory * This,
    /* [in] */ BSTR par_encoding,
    /* [in] */ IDataEncoder *par_encoder);


void __RPC_STUB IDataEncoderFactory_AddDataEncoder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDataEncoderFactory_GetDataEncoder_Proxy( 
    IDataEncoderFactory * This,
    /* [in] */ BSTR par_encoding,
    /* [retval][out] */ IDataEncoder **par_encoder);


void __RPC_STUB IDataEncoderFactory_GetDataEncoder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDataEncoderFactory_INTERFACE_DEFINED__ */


#ifndef __IAttachment_INTERFACE_DEFINED__
#define __IAttachment_INTERFACE_DEFINED__

/* interface IAttachment */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IAttachment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A2C40FB2-B768-4EC8-809A-6ECB4B89C6A7")
    IAttachment : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAttachmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAttachment * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAttachment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAttachment * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAttachment * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAttachment * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAttachment * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAttachment * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IAttachment * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IAttachment * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        END_INTERFACE
    } IAttachmentVtbl;

    interface IAttachment
    {
        CONST_VTBL struct IAttachmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAttachment_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAttachment_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAttachment_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAttachment_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAttachment_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAttachment_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAttachment_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAttachment_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IAttachment_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IAttachment_get_Property_Proxy( 
    IAttachment * This,
    /* [in] */ BSTR par_name,
    /* [retval][out] */ VARIANT *par_value);


void __RPC_STUB IAttachment_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IAttachment_put_Property_Proxy( 
    IAttachment * This,
    /* [in] */ BSTR par_name,
    /* [in] */ VARIANT par_value);


void __RPC_STUB IAttachment_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAttachment_INTERFACE_DEFINED__ */


#ifndef __IFileAttachment_INTERFACE_DEFINED__
#define __IFileAttachment_INTERFACE_DEFINED__

/* interface IFileAttachment */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IFileAttachment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D6DEA9EB-28EA-45C7-A46A-72D26668C1EA")
    IFileAttachment : public IAttachment
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *par_value) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_FileName( 
            /* [in] */ BSTR par_value) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_DeleteAfterSending( 
            /* [in] */ VARIANT_BOOL par_value) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_DeleteAfterSending( 
            /* [retval][out] */ VARIANT_BOOL *par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileAttachmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileAttachment * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileAttachment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileAttachment * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFileAttachment * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFileAttachment * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFileAttachment * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFileAttachment * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IFileAttachment * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IFileAttachment * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IFileAttachment * This,
            /* [retval][out] */ BSTR *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FileName )( 
            IFileAttachment * This,
            /* [in] */ BSTR par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DeleteAfterSending )( 
            IFileAttachment * This,
            /* [in] */ VARIANT_BOOL par_value);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeleteAfterSending )( 
            IFileAttachment * This,
            /* [retval][out] */ VARIANT_BOOL *par_value);
        
        END_INTERFACE
    } IFileAttachmentVtbl;

    interface IFileAttachment
    {
        CONST_VTBL struct IFileAttachmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileAttachment_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFileAttachment_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFileAttachment_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFileAttachment_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFileAttachment_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFileAttachment_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFileAttachment_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFileAttachment_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IFileAttachment_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)


#define IFileAttachment_get_FileName(This,par_value)	\
    (This)->lpVtbl -> get_FileName(This,par_value)

#define IFileAttachment_put_FileName(This,par_value)	\
    (This)->lpVtbl -> put_FileName(This,par_value)

#define IFileAttachment_put_DeleteAfterSending(This,par_value)	\
    (This)->lpVtbl -> put_DeleteAfterSending(This,par_value)

#define IFileAttachment_get_DeleteAfterSending(This,par_value)	\
    (This)->lpVtbl -> get_DeleteAfterSending(This,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IFileAttachment_get_FileName_Proxy( 
    IFileAttachment * This,
    /* [retval][out] */ BSTR *par_value);


void __RPC_STUB IFileAttachment_get_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IFileAttachment_put_FileName_Proxy( 
    IFileAttachment * This,
    /* [in] */ BSTR par_value);


void __RPC_STUB IFileAttachment_put_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IFileAttachment_put_DeleteAfterSending_Proxy( 
    IFileAttachment * This,
    /* [in] */ VARIANT_BOOL par_value);


void __RPC_STUB IFileAttachment_put_DeleteAfterSending_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IFileAttachment_get_DeleteAfterSending_Proxy( 
    IFileAttachment * This,
    /* [retval][out] */ VARIANT_BOOL *par_value);


void __RPC_STUB IFileAttachment_get_DeleteAfterSending_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFileAttachment_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_FileAttachment30;

#ifdef __cplusplus

class DECLSPEC_UUID("90A299F3-26C6-457D-A514-404335109EDD")
FileAttachment30;
#endif

#ifndef __IStringAttachment_INTERFACE_DEFINED__
#define __IStringAttachment_INTERFACE_DEFINED__

/* interface IStringAttachment */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IStringAttachment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8004A743-6A1E-45E4-B2E2-A6D117F06008")
    IStringAttachment : public IAttachment
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_String( 
            /* [retval][out] */ BSTR *par_value) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_String( 
            /* [in] */ BSTR par_value) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ContentCharacterSet( 
            /* [retval][out] */ BSTR *par_value) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_ContentCharacterSet( 
            /* [in] */ BSTR par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStringAttachmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStringAttachment * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStringAttachment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStringAttachment * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStringAttachment * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStringAttachment * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStringAttachment * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStringAttachment * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IStringAttachment * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IStringAttachment * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_String )( 
            IStringAttachment * This,
            /* [retval][out] */ BSTR *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_String )( 
            IStringAttachment * This,
            /* [in] */ BSTR par_value);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ContentCharacterSet )( 
            IStringAttachment * This,
            /* [retval][out] */ BSTR *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ContentCharacterSet )( 
            IStringAttachment * This,
            /* [in] */ BSTR par_value);
        
        END_INTERFACE
    } IStringAttachmentVtbl;

    interface IStringAttachment
    {
        CONST_VTBL struct IStringAttachmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStringAttachment_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStringAttachment_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStringAttachment_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStringAttachment_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IStringAttachment_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IStringAttachment_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IStringAttachment_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IStringAttachment_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IStringAttachment_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)


#define IStringAttachment_get_String(This,par_value)	\
    (This)->lpVtbl -> get_String(This,par_value)

#define IStringAttachment_put_String(This,par_value)	\
    (This)->lpVtbl -> put_String(This,par_value)

#define IStringAttachment_get_ContentCharacterSet(This,par_value)	\
    (This)->lpVtbl -> get_ContentCharacterSet(This,par_value)

#define IStringAttachment_put_ContentCharacterSet(This,par_value)	\
    (This)->lpVtbl -> put_ContentCharacterSet(This,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStringAttachment_get_String_Proxy( 
    IStringAttachment * This,
    /* [retval][out] */ BSTR *par_value);


void __RPC_STUB IStringAttachment_get_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IStringAttachment_put_String_Proxy( 
    IStringAttachment * This,
    /* [in] */ BSTR par_value);


void __RPC_STUB IStringAttachment_put_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStringAttachment_get_ContentCharacterSet_Proxy( 
    IStringAttachment * This,
    /* [retval][out] */ BSTR *par_value);


void __RPC_STUB IStringAttachment_get_ContentCharacterSet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IStringAttachment_put_ContentCharacterSet_Proxy( 
    IStringAttachment * This,
    /* [in] */ BSTR par_value);


void __RPC_STUB IStringAttachment_put_ContentCharacterSet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStringAttachment_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_StringAttachment30;

#ifdef __cplusplus

class DECLSPEC_UUID("722C5A81-4FEC-43F7-8656-E16EC6853073")
StringAttachment30;
#endif

#ifndef __IByteArrayAttachment_INTERFACE_DEFINED__
#define __IByteArrayAttachment_INTERFACE_DEFINED__

/* interface IByteArrayAttachment */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IByteArrayAttachment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52088645-8E96-4C18-8621-B46611635303")
    IByteArrayAttachment : public IAttachment
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Array( 
            /* [retval][out] */ VARIANT *par_value) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Array( 
            /* [in] */ VARIANT par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IByteArrayAttachmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IByteArrayAttachment * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IByteArrayAttachment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IByteArrayAttachment * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IByteArrayAttachment * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IByteArrayAttachment * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IByteArrayAttachment * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IByteArrayAttachment * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IByteArrayAttachment * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IByteArrayAttachment * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Array )( 
            IByteArrayAttachment * This,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Array )( 
            IByteArrayAttachment * This,
            /* [in] */ VARIANT par_value);
        
        END_INTERFACE
    } IByteArrayAttachmentVtbl;

    interface IByteArrayAttachment
    {
        CONST_VTBL struct IByteArrayAttachmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IByteArrayAttachment_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IByteArrayAttachment_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IByteArrayAttachment_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IByteArrayAttachment_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IByteArrayAttachment_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IByteArrayAttachment_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IByteArrayAttachment_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IByteArrayAttachment_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IByteArrayAttachment_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)


#define IByteArrayAttachment_get_Array(This,par_value)	\
    (This)->lpVtbl -> get_Array(This,par_value)

#define IByteArrayAttachment_put_Array(This,par_value)	\
    (This)->lpVtbl -> put_Array(This,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IByteArrayAttachment_get_Array_Proxy( 
    IByteArrayAttachment * This,
    /* [retval][out] */ VARIANT *par_value);


void __RPC_STUB IByteArrayAttachment_get_Array_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IByteArrayAttachment_put_Array_Proxy( 
    IByteArrayAttachment * This,
    /* [in] */ VARIANT par_value);


void __RPC_STUB IByteArrayAttachment_put_Array_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IByteArrayAttachment_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ByteArrayAttachment30;

#ifdef __cplusplus

class DECLSPEC_UUID("565FBBE9-8563-4302-BE8A-7C6A64FB0A85")
ByteArrayAttachment30;
#endif

#ifndef __IStreamAttachment_INTERFACE_DEFINED__
#define __IStreamAttachment_INTERFACE_DEFINED__

/* interface IStreamAttachment */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IStreamAttachment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BE1DBCF5-2260-470A-8E1C-E2406D106E0A")
    IStreamAttachment : public IAttachment
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Stream( 
            /* [retval][out] */ IStream **par_value) = 0;
        
        virtual /* [helpstring][propputref] */ HRESULT STDMETHODCALLTYPE putref_Stream( 
            /* [in] */ IStream *par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStreamAttachmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStreamAttachment * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStreamAttachment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStreamAttachment * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStreamAttachment * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStreamAttachment * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStreamAttachment * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStreamAttachment * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IStreamAttachment * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IStreamAttachment * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Stream )( 
            IStreamAttachment * This,
            /* [retval][out] */ IStream **par_value);
        
        /* [helpstring][propputref] */ HRESULT ( STDMETHODCALLTYPE *putref_Stream )( 
            IStreamAttachment * This,
            /* [in] */ IStream *par_value);
        
        END_INTERFACE
    } IStreamAttachmentVtbl;

    interface IStreamAttachment
    {
        CONST_VTBL struct IStreamAttachmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStreamAttachment_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStreamAttachment_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStreamAttachment_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStreamAttachment_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IStreamAttachment_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IStreamAttachment_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IStreamAttachment_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IStreamAttachment_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IStreamAttachment_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)


#define IStreamAttachment_get_Stream(This,par_value)	\
    (This)->lpVtbl -> get_Stream(This,par_value)

#define IStreamAttachment_putref_Stream(This,par_value)	\
    (This)->lpVtbl -> putref_Stream(This,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IStreamAttachment_get_Stream_Proxy( 
    IStreamAttachment * This,
    /* [retval][out] */ IStream **par_value);


void __RPC_STUB IStreamAttachment_get_Stream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propputref] */ HRESULT STDMETHODCALLTYPE IStreamAttachment_putref_Stream_Proxy( 
    IStreamAttachment * This,
    /* [in] */ IStream *par_value);


void __RPC_STUB IStreamAttachment_putref_Stream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStreamAttachment_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_StreamAttachment30;

#ifdef __cplusplus

class DECLSPEC_UUID("05AE7FB3-C4E9-4F79-A5C3-DAB525E31F2C")
StreamAttachment30;
#endif

#ifndef __IReceivedAttachment_INTERFACE_DEFINED__
#define __IReceivedAttachment_INTERFACE_DEFINED__

/* interface IReceivedAttachment */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IReceivedAttachment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C0C9F1C0-0039-427B-8ACC-AD172FE557A8")
    IReceivedAttachment : public IAttachment
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SaveToFile( 
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ VARIANT_BOOL par_override = 0) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAsByteArray( 
            /* [retval][out] */ VARIANT *par_array) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAsString( 
            /* [defaultvalue][in] */ BSTR par_ContentCharacterSet,
            /* [retval][out] */ BSTR *par_string) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IReceivedAttachmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IReceivedAttachment * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IReceivedAttachment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IReceivedAttachment * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IReceivedAttachment * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IReceivedAttachment * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IReceivedAttachment * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IReceivedAttachment * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IReceivedAttachment * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IReceivedAttachment * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SaveToFile )( 
            IReceivedAttachment * This,
            /* [in] */ BSTR par_name,
            /* [defaultvalue][in] */ VARIANT_BOOL par_override);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAsByteArray )( 
            IReceivedAttachment * This,
            /* [retval][out] */ VARIANT *par_array);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAsString )( 
            IReceivedAttachment * This,
            /* [defaultvalue][in] */ BSTR par_ContentCharacterSet,
            /* [retval][out] */ BSTR *par_string);
        
        END_INTERFACE
    } IReceivedAttachmentVtbl;

    interface IReceivedAttachment
    {
        CONST_VTBL struct IReceivedAttachmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReceivedAttachment_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReceivedAttachment_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReceivedAttachment_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReceivedAttachment_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IReceivedAttachment_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IReceivedAttachment_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IReceivedAttachment_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IReceivedAttachment_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IReceivedAttachment_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)


#define IReceivedAttachment_SaveToFile(This,par_name,par_override)	\
    (This)->lpVtbl -> SaveToFile(This,par_name,par_override)

#define IReceivedAttachment_GetAsByteArray(This,par_array)	\
    (This)->lpVtbl -> GetAsByteArray(This,par_array)

#define IReceivedAttachment_GetAsString(This,par_ContentCharacterSet,par_string)	\
    (This)->lpVtbl -> GetAsString(This,par_ContentCharacterSet,par_string)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReceivedAttachment_SaveToFile_Proxy( 
    IReceivedAttachment * This,
    /* [in] */ BSTR par_name,
    /* [defaultvalue][in] */ VARIANT_BOOL par_override);


void __RPC_STUB IReceivedAttachment_SaveToFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReceivedAttachment_GetAsByteArray_Proxy( 
    IReceivedAttachment * This,
    /* [retval][out] */ VARIANT *par_array);


void __RPC_STUB IReceivedAttachment_GetAsByteArray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IReceivedAttachment_GetAsString_Proxy( 
    IReceivedAttachment * This,
    /* [defaultvalue][in] */ BSTR par_ContentCharacterSet,
    /* [retval][out] */ BSTR *par_string);


void __RPC_STUB IReceivedAttachment_GetAsString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IReceivedAttachment_INTERFACE_DEFINED__ */


#ifndef __ISentAttachments_INTERFACE_DEFINED__
#define __ISentAttachments_INTERFACE_DEFINED__

/* interface ISentAttachments */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISentAttachments;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("95A098C0-EB61-4895-91C7-78873251322E")
    ISentAttachments : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *par_value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long par_index,
            /* [retval][out] */ IAttachment **par_att) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IAttachment *par_att) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISentAttachmentsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISentAttachments * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISentAttachments * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISentAttachments * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISentAttachments * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISentAttachments * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISentAttachments * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISentAttachments * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ISentAttachments * This,
            /* [retval][out] */ long *par_value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ISentAttachments * This,
            /* [in] */ long par_index,
            /* [retval][out] */ IAttachment **par_att);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            ISentAttachments * This,
            /* [in] */ IAttachment *par_att);
        
        END_INTERFACE
    } ISentAttachmentsVtbl;

    interface ISentAttachments
    {
        CONST_VTBL struct ISentAttachmentsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISentAttachments_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISentAttachments_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISentAttachments_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISentAttachments_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISentAttachments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISentAttachments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISentAttachments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISentAttachments_get_Count(This,par_value)	\
    (This)->lpVtbl -> get_Count(This,par_value)

#define ISentAttachments_get_Item(This,par_index,par_att)	\
    (This)->lpVtbl -> get_Item(This,par_index,par_att)

#define ISentAttachments_Add(This,par_att)	\
    (This)->lpVtbl -> Add(This,par_att)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISentAttachments_get_Count_Proxy( 
    ISentAttachments * This,
    /* [retval][out] */ long *par_value);


void __RPC_STUB ISentAttachments_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISentAttachments_get_Item_Proxy( 
    ISentAttachments * This,
    /* [in] */ long par_index,
    /* [retval][out] */ IAttachment **par_att);


void __RPC_STUB ISentAttachments_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISentAttachments_Add_Proxy( 
    ISentAttachments * This,
    /* [in] */ IAttachment *par_att);


void __RPC_STUB ISentAttachments_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISentAttachments_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SentAttachments30;

#ifdef __cplusplus

class DECLSPEC_UUID("CE071800-E681-4ADF-9422-A3D0BD0D51CB")
SentAttachments30;
#endif

#ifndef __IReceivedAttachments_INTERFACE_DEFINED__
#define __IReceivedAttachments_INTERFACE_DEFINED__

/* interface IReceivedAttachments */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IReceivedAttachments;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("176B81CD-4F22-4CA0-9F54-9FE5935A595B")
    IReceivedAttachments : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *par_value) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT par_index,
            /* [retval][out] */ IReceivedAttachment **par_att) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ItemWithContext( 
            /* [in] */ VARIANT par_index,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ IReceivedAttachment **par_att) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IReceivedAttachmentsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IReceivedAttachments * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IReceivedAttachments * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IReceivedAttachments * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IReceivedAttachments * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IReceivedAttachments * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IReceivedAttachments * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IReceivedAttachments * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IReceivedAttachments * This,
            /* [retval][out] */ long *par_value);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IReceivedAttachments * This,
            /* [in] */ VARIANT par_index,
            /* [retval][out] */ IReceivedAttachment **par_att);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ItemWithContext )( 
            IReceivedAttachments * This,
            /* [in] */ VARIANT par_index,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
            /* [retval][out] */ IReceivedAttachment **par_att);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IReceivedAttachments * This,
            /* [retval][out] */ IUnknown **par_value);
        
        END_INTERFACE
    } IReceivedAttachmentsVtbl;

    interface IReceivedAttachments
    {
        CONST_VTBL struct IReceivedAttachmentsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IReceivedAttachments_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReceivedAttachments_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IReceivedAttachments_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IReceivedAttachments_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IReceivedAttachments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IReceivedAttachments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IReceivedAttachments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IReceivedAttachments_get_Count(This,par_value)	\
    (This)->lpVtbl -> get_Count(This,par_value)

#define IReceivedAttachments_get_Item(This,par_index,par_att)	\
    (This)->lpVtbl -> get_Item(This,par_index,par_att)

#define IReceivedAttachments_get_ItemWithContext(This,par_index,par_context,par_att)	\
    (This)->lpVtbl -> get_ItemWithContext(This,par_index,par_context,par_att)

#define IReceivedAttachments_get__NewEnum(This,par_value)	\
    (This)->lpVtbl -> get__NewEnum(This,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IReceivedAttachments_get_Count_Proxy( 
    IReceivedAttachments * This,
    /* [retval][out] */ long *par_value);


void __RPC_STUB IReceivedAttachments_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IReceivedAttachments_get_Item_Proxy( 
    IReceivedAttachments * This,
    /* [in] */ VARIANT par_index,
    /* [retval][out] */ IReceivedAttachment **par_att);


void __RPC_STUB IReceivedAttachments_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IReceivedAttachments_get_ItemWithContext_Proxy( 
    IReceivedAttachments * This,
    /* [in] */ VARIANT par_index,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_context,
    /* [retval][out] */ IReceivedAttachment **par_att);


void __RPC_STUB IReceivedAttachments_get_ItemWithContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IReceivedAttachments_get__NewEnum_Proxy( 
    IReceivedAttachments * This,
    /* [retval][out] */ IUnknown **par_value);


void __RPC_STUB IReceivedAttachments_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IReceivedAttachments_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ReceivedAttachments30;

#ifdef __cplusplus

class DECLSPEC_UUID("AF9B6377-6505-4934-AD85-BAB87E15EF65")
ReceivedAttachments30;
#endif

#ifndef __IComposerDestination_INTERFACE_DEFINED__
#define __IComposerDestination_INTERFACE_DEFINED__

/* interface IComposerDestination */
/* [hidden][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IComposerDestination;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8E62C4B1-EE0C-48FB-9161-3EE041A03153")
    IComposerDestination : public IUnknown
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_TotalSize( 
            /* [in] */ long par_value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE BeginSending( 
            /* [out] */ ISequentialStream **par_stream,
            /* [out] */ ComposerDestinationFlags *par_flags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EndSending( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IComposerDestinationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComposerDestination * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComposerDestination * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComposerDestination * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_TotalSize )( 
            IComposerDestination * This,
            /* [in] */ long par_value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IComposerDestination * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IComposerDestination * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *BeginSending )( 
            IComposerDestination * This,
            /* [out] */ ISequentialStream **par_stream,
            /* [out] */ ComposerDestinationFlags *par_flags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EndSending )( 
            IComposerDestination * This);
        
        END_INTERFACE
    } IComposerDestinationVtbl;

    interface IComposerDestination
    {
        CONST_VTBL struct IComposerDestinationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComposerDestination_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IComposerDestination_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IComposerDestination_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IComposerDestination_put_TotalSize(This,par_value)	\
    (This)->lpVtbl -> put_TotalSize(This,par_value)

#define IComposerDestination_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IComposerDestination_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)

#define IComposerDestination_BeginSending(This,par_stream,par_flags)	\
    (This)->lpVtbl -> BeginSending(This,par_stream,par_flags)

#define IComposerDestination_EndSending(This)	\
    (This)->lpVtbl -> EndSending(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IComposerDestination_put_TotalSize_Proxy( 
    IComposerDestination * This,
    /* [in] */ long par_value);


void __RPC_STUB IComposerDestination_put_TotalSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IComposerDestination_get_Property_Proxy( 
    IComposerDestination * This,
    /* [in] */ BSTR par_name,
    /* [retval][out] */ VARIANT *par_value);


void __RPC_STUB IComposerDestination_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IComposerDestination_put_Property_Proxy( 
    IComposerDestination * This,
    /* [in] */ BSTR par_name,
    /* [in] */ VARIANT par_value);


void __RPC_STUB IComposerDestination_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IComposerDestination_BeginSending_Proxy( 
    IComposerDestination * This,
    /* [out] */ ISequentialStream **par_stream,
    /* [out] */ ComposerDestinationFlags *par_flags);


void __RPC_STUB IComposerDestination_BeginSending_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IComposerDestination_EndSending_Proxy( 
    IComposerDestination * This);


void __RPC_STUB IComposerDestination_EndSending_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IComposerDestination_INTERFACE_DEFINED__ */


#ifndef __IGetComposerDestination_INTERFACE_DEFINED__
#define __IGetComposerDestination_INTERFACE_DEFINED__

/* interface IGetComposerDestination */
/* [hidden][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IGetComposerDestination;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9E6CDFEF-4C42-411B-BACA-FE96F7A13C04")
    IGetComposerDestination : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ComposerDestination( 
            /* [retval][out] */ IComposerDestination **par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGetComposerDestinationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGetComposerDestination * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGetComposerDestination * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGetComposerDestination * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ComposerDestination )( 
            IGetComposerDestination * This,
            /* [retval][out] */ IComposerDestination **par_value);
        
        END_INTERFACE
    } IGetComposerDestinationVtbl;

    interface IGetComposerDestination
    {
        CONST_VTBL struct IGetComposerDestinationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGetComposerDestination_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGetComposerDestination_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGetComposerDestination_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGetComposerDestination_get_ComposerDestination(This,par_value)	\
    (This)->lpVtbl -> get_ComposerDestination(This,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IGetComposerDestination_get_ComposerDestination_Proxy( 
    IGetComposerDestination * This,
    /* [retval][out] */ IComposerDestination **par_value);


void __RPC_STUB IGetComposerDestination_get_ComposerDestination_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGetComposerDestination_INTERFACE_DEFINED__ */


#ifndef __IMessageComposer_INTERFACE_DEFINED__
#define __IMessageComposer_INTERFACE_DEFINED__

/* interface IMessageComposer */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IMessageComposer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("906A72B9-FF88-4A49-AFA2-CC4CAB5104EC")
    IMessageComposer : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StartComposing( 
            /* [in] */ IComposerDestination *par_destination) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EndComposing( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StartEnvelope( 
            /* [in] */ BSTR par_charSet,
            /* [retval][out] */ ISequentialStream **par_envelope) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EndEnvelope( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddAttachment( 
            /* [in] */ IAttachment *par_att,
            /* [retval][out] */ BSTR *par_reference) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SaveSpecialTypeMapper( 
            /* [in] */ ISoapMapper *par_ISoapMapper,
            /* [in] */ ISoapSerializer *par_ISoapSerializer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessageComposerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMessageComposer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMessageComposer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMessageComposer * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StartComposing )( 
            IMessageComposer * This,
            /* [in] */ IComposerDestination *par_destination);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EndComposing )( 
            IMessageComposer * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StartEnvelope )( 
            IMessageComposer * This,
            /* [in] */ BSTR par_charSet,
            /* [retval][out] */ ISequentialStream **par_envelope);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EndEnvelope )( 
            IMessageComposer * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddAttachment )( 
            IMessageComposer * This,
            /* [in] */ IAttachment *par_att,
            /* [retval][out] */ BSTR *par_reference);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SaveSpecialTypeMapper )( 
            IMessageComposer * This,
            /* [in] */ ISoapMapper *par_ISoapMapper,
            /* [in] */ ISoapSerializer *par_ISoapSerializer);
        
        END_INTERFACE
    } IMessageComposerVtbl;

    interface IMessageComposer
    {
        CONST_VTBL struct IMessageComposerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessageComposer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMessageComposer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMessageComposer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMessageComposer_StartComposing(This,par_destination)	\
    (This)->lpVtbl -> StartComposing(This,par_destination)

#define IMessageComposer_EndComposing(This)	\
    (This)->lpVtbl -> EndComposing(This)

#define IMessageComposer_StartEnvelope(This,par_charSet,par_envelope)	\
    (This)->lpVtbl -> StartEnvelope(This,par_charSet,par_envelope)

#define IMessageComposer_EndEnvelope(This)	\
    (This)->lpVtbl -> EndEnvelope(This)

#define IMessageComposer_AddAttachment(This,par_att,par_reference)	\
    (This)->lpVtbl -> AddAttachment(This,par_att,par_reference)

#define IMessageComposer_SaveSpecialTypeMapper(This,par_ISoapMapper,par_ISoapSerializer)	\
    (This)->lpVtbl -> SaveSpecialTypeMapper(This,par_ISoapMapper,par_ISoapSerializer)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageComposer_StartComposing_Proxy( 
    IMessageComposer * This,
    /* [in] */ IComposerDestination *par_destination);


void __RPC_STUB IMessageComposer_StartComposing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageComposer_EndComposing_Proxy( 
    IMessageComposer * This);


void __RPC_STUB IMessageComposer_EndComposing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageComposer_StartEnvelope_Proxy( 
    IMessageComposer * This,
    /* [in] */ BSTR par_charSet,
    /* [retval][out] */ ISequentialStream **par_envelope);


void __RPC_STUB IMessageComposer_StartEnvelope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageComposer_EndEnvelope_Proxy( 
    IMessageComposer * This);


void __RPC_STUB IMessageComposer_EndEnvelope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageComposer_AddAttachment_Proxy( 
    IMessageComposer * This,
    /* [in] */ IAttachment *par_att,
    /* [retval][out] */ BSTR *par_reference);


void __RPC_STUB IMessageComposer_AddAttachment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageComposer_SaveSpecialTypeMapper_Proxy( 
    IMessageComposer * This,
    /* [in] */ ISoapMapper *par_ISoapMapper,
    /* [in] */ ISoapSerializer *par_ISoapSerializer);


void __RPC_STUB IMessageComposer_SaveSpecialTypeMapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMessageComposer_INTERFACE_DEFINED__ */


#ifndef __IDimeComposer_INTERFACE_DEFINED__
#define __IDimeComposer_INTERFACE_DEFINED__

/* interface IDimeComposer */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IDimeComposer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ABAADE34-EEF6-408A-8896-65BE669D27FA")
    IDimeComposer : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [defaultvalue][in] */ BSTR par_tempFolder = L"",
            /* [defaultvalue][in] */ long par_maxSize = 10,
            /* [defaultvalue][in] */ long par_chunkSize = -1) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDimeComposerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDimeComposer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDimeComposer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDimeComposer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDimeComposer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDimeComposer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDimeComposer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDimeComposer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IDimeComposer * This,
            /* [defaultvalue][in] */ BSTR par_tempFolder,
            /* [defaultvalue][in] */ long par_maxSize,
            /* [defaultvalue][in] */ long par_chunkSize);
        
        END_INTERFACE
    } IDimeComposerVtbl;

    interface IDimeComposer
    {
        CONST_VTBL struct IDimeComposerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDimeComposer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDimeComposer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDimeComposer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDimeComposer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDimeComposer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDimeComposer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDimeComposer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDimeComposer_Initialize(This,par_tempFolder,par_maxSize,par_chunkSize)	\
    (This)->lpVtbl -> Initialize(This,par_tempFolder,par_maxSize,par_chunkSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDimeComposer_Initialize_Proxy( 
    IDimeComposer * This,
    /* [defaultvalue][in] */ BSTR par_tempFolder,
    /* [defaultvalue][in] */ long par_maxSize,
    /* [defaultvalue][in] */ long par_chunkSize);


void __RPC_STUB IDimeComposer_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDimeComposer_INTERFACE_DEFINED__ */


#ifndef __ISimpleComposer_INTERFACE_DEFINED__
#define __ISimpleComposer_INTERFACE_DEFINED__

/* interface ISimpleComposer */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISimpleComposer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70824404-7A18-412A-9A83-A9EC0F3FF045")
    ISimpleComposer : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISimpleComposerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimpleComposer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimpleComposer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimpleComposer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISimpleComposer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISimpleComposer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISimpleComposer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISimpleComposer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISimpleComposerVtbl;

    interface ISimpleComposer
    {
        CONST_VTBL struct ISimpleComposerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimpleComposer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISimpleComposer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISimpleComposer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISimpleComposer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISimpleComposer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISimpleComposer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISimpleComposer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISimpleComposer_INTERFACE_DEFINED__ */


#ifndef __IParserSource_INTERFACE_DEFINED__
#define __IParserSource_INTERFACE_DEFINED__

/* interface IParserSource */
/* [hidden][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IParserSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("282C694F-D69F-4044-B076-6F4AC1748A90")
    IParserSource : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE BeginReceiving( 
            /* [retval][out] */ ISequentialStream **par_stream) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EndReceiving( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParserSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IParserSource * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IParserSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IParserSource * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IParserSource * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IParserSource * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *BeginReceiving )( 
            IParserSource * This,
            /* [retval][out] */ ISequentialStream **par_stream);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *EndReceiving )( 
            IParserSource * This);
        
        END_INTERFACE
    } IParserSourceVtbl;

    interface IParserSource
    {
        CONST_VTBL struct IParserSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParserSource_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParserSource_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParserSource_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParserSource_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define IParserSource_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)

#define IParserSource_BeginReceiving(This,par_stream)	\
    (This)->lpVtbl -> BeginReceiving(This,par_stream)

#define IParserSource_EndReceiving(This)	\
    (This)->lpVtbl -> EndReceiving(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IParserSource_get_Property_Proxy( 
    IParserSource * This,
    /* [in] */ BSTR par_name,
    /* [retval][out] */ VARIANT *par_value);


void __RPC_STUB IParserSource_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IParserSource_put_Property_Proxy( 
    IParserSource * This,
    /* [in] */ BSTR par_name,
    /* [in] */ VARIANT par_value);


void __RPC_STUB IParserSource_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IParserSource_BeginReceiving_Proxy( 
    IParserSource * This,
    /* [retval][out] */ ISequentialStream **par_stream);


void __RPC_STUB IParserSource_BeginReceiving_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IParserSource_EndReceiving_Proxy( 
    IParserSource * This);


void __RPC_STUB IParserSource_EndReceiving_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParserSource_INTERFACE_DEFINED__ */


#ifndef __IGetParserSource_INTERFACE_DEFINED__
#define __IGetParserSource_INTERFACE_DEFINED__

/* interface IGetParserSource */
/* [hidden][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IGetParserSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BB63287E-1407-40E3-89AB-38CB2746547F")
    IGetParserSource : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ParserSource( 
            /* [retval][out] */ IParserSource **par_value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGetParserSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGetParserSource * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGetParserSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGetParserSource * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ParserSource )( 
            IGetParserSource * This,
            /* [retval][out] */ IParserSource **par_value);
        
        END_INTERFACE
    } IGetParserSourceVtbl;

    interface IGetParserSource
    {
        CONST_VTBL struct IGetParserSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGetParserSource_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGetParserSource_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGetParserSource_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGetParserSource_get_ParserSource(This,par_value)	\
    (This)->lpVtbl -> get_ParserSource(This,par_value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IGetParserSource_get_ParserSource_Proxy( 
    IGetParserSource * This,
    /* [retval][out] */ IParserSource **par_value);


void __RPC_STUB IGetParserSource_get_ParserSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGetParserSource_INTERFACE_DEFINED__ */


#ifndef __IMessageParser_INTERFACE_DEFINED__
#define __IMessageParser_INTERFACE_DEFINED__

/* interface IMessageParser */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IMessageParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3B2A98E6-F76A-48B1-8F7D-0139A8D0258C")
    IMessageParser : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_binding,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_composer,
            /* [in] */ BSTR par_tempFolder,
            /* [in] */ long par_maxSize) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LoadMessage( 
            /* [in] */ IParserSource *par_source,
            /* [in] */ ISequentialStream *par_envelope,
            /* [retval][out] */ IReceivedAttachments **par_att) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LoadSpecialTypeMapper( 
            /* [in] */ ISoapMapper *par_soapmapper,
            /* [in] */ ISoapReader *par_soapreader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMessageParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMessageParser * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMessageParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMessageParser * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IMessageParser * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_binding,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_composer,
            /* [in] */ BSTR par_tempFolder,
            /* [in] */ long par_maxSize);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *LoadMessage )( 
            IMessageParser * This,
            /* [in] */ IParserSource *par_source,
            /* [in] */ ISequentialStream *par_envelope,
            /* [retval][out] */ IReceivedAttachments **par_att);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *LoadSpecialTypeMapper )( 
            IMessageParser * This,
            /* [in] */ ISoapMapper *par_soapmapper,
            /* [in] */ ISoapReader *par_soapreader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_context);
        
        END_INTERFACE
    } IMessageParserVtbl;

    interface IMessageParser
    {
        CONST_VTBL struct IMessageParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMessageParser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMessageParser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMessageParser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMessageParser_Initialize(This,par_binding,par_composer,par_tempFolder,par_maxSize)	\
    (This)->lpVtbl -> Initialize(This,par_binding,par_composer,par_tempFolder,par_maxSize)

#define IMessageParser_LoadMessage(This,par_source,par_envelope,par_att)	\
    (This)->lpVtbl -> LoadMessage(This,par_source,par_envelope,par_att)

#define IMessageParser_LoadSpecialTypeMapper(This,par_soapmapper,par_soapreader,par_context)	\
    (This)->lpVtbl -> LoadSpecialTypeMapper(This,par_soapmapper,par_soapreader,par_context)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageParser_Initialize_Proxy( 
    IMessageParser * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_binding,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_composer,
    /* [in] */ BSTR par_tempFolder,
    /* [in] */ long par_maxSize);


void __RPC_STUB IMessageParser_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageParser_LoadMessage_Proxy( 
    IMessageParser * This,
    /* [in] */ IParserSource *par_source,
    /* [in] */ ISequentialStream *par_envelope,
    /* [retval][out] */ IReceivedAttachments **par_att);


void __RPC_STUB IMessageParser_LoadMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMessageParser_LoadSpecialTypeMapper_Proxy( 
    IMessageParser * This,
    /* [in] */ ISoapMapper *par_soapmapper,
    /* [in] */ ISoapReader *par_soapreader,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_context);


void __RPC_STUB IMessageParser_LoadSpecialTypeMapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMessageParser_INTERFACE_DEFINED__ */


#ifndef __IDimeParser_INTERFACE_DEFINED__
#define __IDimeParser_INTERFACE_DEFINED__

/* interface IDimeParser */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IDimeParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E3F8BAA5-8A05-4641-91CE-3FBC533D1EDB")
    IDimeParser : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [defaultvalue][in] */ BSTR par_tempFolder = L"",
            /* [defaultvalue][in] */ long par_maxSize = 10) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDimeParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDimeParser * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDimeParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDimeParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDimeParser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDimeParser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDimeParser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDimeParser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IDimeParser * This,
            /* [defaultvalue][in] */ BSTR par_tempFolder,
            /* [defaultvalue][in] */ long par_maxSize);
        
        END_INTERFACE
    } IDimeParserVtbl;

    interface IDimeParser
    {
        CONST_VTBL struct IDimeParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDimeParser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDimeParser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDimeParser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDimeParser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDimeParser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDimeParser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDimeParser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDimeParser_Initialize(This,par_tempFolder,par_maxSize)	\
    (This)->lpVtbl -> Initialize(This,par_tempFolder,par_maxSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDimeParser_Initialize_Proxy( 
    IDimeParser * This,
    /* [defaultvalue][in] */ BSTR par_tempFolder,
    /* [defaultvalue][in] */ long par_maxSize);


void __RPC_STUB IDimeParser_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDimeParser_INTERFACE_DEFINED__ */


#ifndef __ISimpleParser_INTERFACE_DEFINED__
#define __ISimpleParser_INTERFACE_DEFINED__

/* interface ISimpleParser */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISimpleParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B313A227-0798-4A87-9074-48CA2164D0F7")
    ISimpleParser : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISimpleParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimpleParser * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimpleParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimpleParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISimpleParser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISimpleParser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISimpleParser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISimpleParser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISimpleParserVtbl;

    interface ISimpleParser
    {
        CONST_VTBL struct ISimpleParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimpleParser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISimpleParser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISimpleParser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISimpleParser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISimpleParser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISimpleParser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISimpleParser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISimpleParser_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DataEncoderFactory30;

#ifdef __cplusplus

class DECLSPEC_UUID("7A51A663-4790-4885-B0E4-124D4BDADB3E")
DataEncoderFactory30;
#endif

EXTERN_C const CLSID CLSID_DimeComposer30;

#ifdef __cplusplus

class DECLSPEC_UUID("B85E6E71-1493-442F-BC97-B511BE0D5D96")
DimeComposer30;
#endif

EXTERN_C const CLSID CLSID_DimeParser30;

#ifdef __cplusplus

class DECLSPEC_UUID("DFC2FA0B-CC72-4486-B9F4-06FE8A75D58F")
DimeParser30;
#endif

EXTERN_C const CLSID CLSID_SimpleComposer30;

#ifdef __cplusplus

class DECLSPEC_UUID("F7E00C3F-D6C7-4E53-9887-61A2D4EBF0E8")
SimpleComposer30;
#endif

EXTERN_C const CLSID CLSID_SimpleParser30;

#ifdef __cplusplus

class DECLSPEC_UUID("4D602A27-DC39-45D6-A6B1-7003DE2E173C")
SimpleParser30;
#endif

EXTERN_C const CLSID CLSID_SoapReader30;

#ifdef __cplusplus

class DECLSPEC_UUID("A8D986B6-9257-11D5-87EA-00B0D0BE6479")
SoapReader30;
#endif

EXTERN_C const CLSID CLSID_SoapSerializer30;

#ifdef __cplusplus

class DECLSPEC_UUID("B76585B0-9257-11D5-87EA-00B0D0BE6479")
SoapSerializer30;
#endif

#ifndef __ISoapClient_INTERFACE_DEFINED__
#define __ISoapClient_INTERFACE_DEFINED__

/* interface ISoapClient */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7F017F92-9257-11D5-87EA-00B0D0BE6479")
    ISoapClient : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MSSoapInit( 
            /* [in] */ BSTR par_WSDLFile,
            /* [defaultvalue][in] */ BSTR par_ServiceName = L"",
            /* [defaultvalue][in] */ BSTR par_Port = L"",
            /* [defaultvalue][in] */ BSTR par_WSMLFile = L"") = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultCode( 
            /* [retval][out] */ BSTR *par_Faultcode) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultString( 
            /* [retval][out] */ BSTR *par_Faultstring) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultActor( 
            /* [retval][out] */ BSTR *par_Actor) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Detail( 
            /* [retval][out] */ BSTR *par_Detail) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ClientProperty( 
            /* [in] */ BSTR par_PropertyName,
            /* [retval][out] */ VARIANT *par_PropertyValue) = 0;
        
        virtual /* [helpstring][propputref] */ HRESULT STDMETHODCALLTYPE putref_HeaderHandler( 
            /* [in] */ IDispatch *par_HeaderHandler) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_ClientProperty( 
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropertyValue) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectorProperty( 
            /* [in] */ BSTR par_PropertyName,
            /* [retval][out] */ VARIANT *par_PropertyValue) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_ConnectorProperty( 
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropertyValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MSSoapInit2( 
            /* [in] */ VARIANT par_WSDLFile,
            /* [in] */ VARIANT par_WSMLFile,
            /* [in] */ BSTR par_ServiceName,
            /* [in] */ BSTR par_Port,
            /* [in] */ BSTR par_Namespace) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultCodeNamespace( 
            /* [retval][out] */ BSTR *par_faultcodeNamespace) = 0;
        
        virtual /* [helpstring][propputref] */ HRESULT STDMETHODCALLTYPE putref_ClientProperty( 
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropertyValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapClient * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapClient * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapClient * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapClient * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapClient * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapClient * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MSSoapInit )( 
            ISoapClient * This,
            /* [in] */ BSTR par_WSDLFile,
            /* [defaultvalue][in] */ BSTR par_ServiceName,
            /* [defaultvalue][in] */ BSTR par_Port,
            /* [defaultvalue][in] */ BSTR par_WSMLFile);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultCode )( 
            ISoapClient * This,
            /* [retval][out] */ BSTR *par_Faultcode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultString )( 
            ISoapClient * This,
            /* [retval][out] */ BSTR *par_Faultstring);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultActor )( 
            ISoapClient * This,
            /* [retval][out] */ BSTR *par_Actor);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Detail )( 
            ISoapClient * This,
            /* [retval][out] */ BSTR *par_Detail);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ClientProperty )( 
            ISoapClient * This,
            /* [in] */ BSTR par_PropertyName,
            /* [retval][out] */ VARIANT *par_PropertyValue);
        
        /* [helpstring][propputref] */ HRESULT ( STDMETHODCALLTYPE *putref_HeaderHandler )( 
            ISoapClient * This,
            /* [in] */ IDispatch *par_HeaderHandler);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ClientProperty )( 
            ISoapClient * This,
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropertyValue);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConnectorProperty )( 
            ISoapClient * This,
            /* [in] */ BSTR par_PropertyName,
            /* [retval][out] */ VARIANT *par_PropertyValue);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ConnectorProperty )( 
            ISoapClient * This,
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropertyValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MSSoapInit2 )( 
            ISoapClient * This,
            /* [in] */ VARIANT par_WSDLFile,
            /* [in] */ VARIANT par_WSMLFile,
            /* [in] */ BSTR par_ServiceName,
            /* [in] */ BSTR par_Port,
            /* [in] */ BSTR par_Namespace);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultCodeNamespace )( 
            ISoapClient * This,
            /* [retval][out] */ BSTR *par_faultcodeNamespace);
        
        /* [helpstring][propputref] */ HRESULT ( STDMETHODCALLTYPE *putref_ClientProperty )( 
            ISoapClient * This,
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropertyValue);
        
        END_INTERFACE
    } ISoapClientVtbl;

    interface ISoapClient
    {
        CONST_VTBL struct ISoapClientVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapClient_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapClient_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapClient_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapClient_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapClient_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapClient_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapClient_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapClient_MSSoapInit(This,par_WSDLFile,par_ServiceName,par_Port,par_WSMLFile)	\
    (This)->lpVtbl -> MSSoapInit(This,par_WSDLFile,par_ServiceName,par_Port,par_WSMLFile)

#define ISoapClient_get_FaultCode(This,par_Faultcode)	\
    (This)->lpVtbl -> get_FaultCode(This,par_Faultcode)

#define ISoapClient_get_FaultString(This,par_Faultstring)	\
    (This)->lpVtbl -> get_FaultString(This,par_Faultstring)

#define ISoapClient_get_FaultActor(This,par_Actor)	\
    (This)->lpVtbl -> get_FaultActor(This,par_Actor)

#define ISoapClient_get_Detail(This,par_Detail)	\
    (This)->lpVtbl -> get_Detail(This,par_Detail)

#define ISoapClient_get_ClientProperty(This,par_PropertyName,par_PropertyValue)	\
    (This)->lpVtbl -> get_ClientProperty(This,par_PropertyName,par_PropertyValue)

#define ISoapClient_putref_HeaderHandler(This,par_HeaderHandler)	\
    (This)->lpVtbl -> putref_HeaderHandler(This,par_HeaderHandler)

#define ISoapClient_put_ClientProperty(This,par_PropertyName,par_PropertyValue)	\
    (This)->lpVtbl -> put_ClientProperty(This,par_PropertyName,par_PropertyValue)

#define ISoapClient_get_ConnectorProperty(This,par_PropertyName,par_PropertyValue)	\
    (This)->lpVtbl -> get_ConnectorProperty(This,par_PropertyName,par_PropertyValue)

#define ISoapClient_put_ConnectorProperty(This,par_PropertyName,par_PropertyValue)	\
    (This)->lpVtbl -> put_ConnectorProperty(This,par_PropertyName,par_PropertyValue)

#define ISoapClient_MSSoapInit2(This,par_WSDLFile,par_WSMLFile,par_ServiceName,par_Port,par_Namespace)	\
    (This)->lpVtbl -> MSSoapInit2(This,par_WSDLFile,par_WSMLFile,par_ServiceName,par_Port,par_Namespace)

#define ISoapClient_get_FaultCodeNamespace(This,par_faultcodeNamespace)	\
    (This)->lpVtbl -> get_FaultCodeNamespace(This,par_faultcodeNamespace)

#define ISoapClient_putref_ClientProperty(This,par_PropertyName,par_PropertyValue)	\
    (This)->lpVtbl -> putref_ClientProperty(This,par_PropertyName,par_PropertyValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapClient_MSSoapInit_Proxy( 
    ISoapClient * This,
    /* [in] */ BSTR par_WSDLFile,
    /* [defaultvalue][in] */ BSTR par_ServiceName,
    /* [defaultvalue][in] */ BSTR par_Port,
    /* [defaultvalue][in] */ BSTR par_WSMLFile);


void __RPC_STUB ISoapClient_MSSoapInit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapClient_get_FaultCode_Proxy( 
    ISoapClient * This,
    /* [retval][out] */ BSTR *par_Faultcode);


void __RPC_STUB ISoapClient_get_FaultCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapClient_get_FaultString_Proxy( 
    ISoapClient * This,
    /* [retval][out] */ BSTR *par_Faultstring);


void __RPC_STUB ISoapClient_get_FaultString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapClient_get_FaultActor_Proxy( 
    ISoapClient * This,
    /* [retval][out] */ BSTR *par_Actor);


void __RPC_STUB ISoapClient_get_FaultActor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapClient_get_Detail_Proxy( 
    ISoapClient * This,
    /* [retval][out] */ BSTR *par_Detail);


void __RPC_STUB ISoapClient_get_Detail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapClient_get_ClientProperty_Proxy( 
    ISoapClient * This,
    /* [in] */ BSTR par_PropertyName,
    /* [retval][out] */ VARIANT *par_PropertyValue);


void __RPC_STUB ISoapClient_get_ClientProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propputref] */ HRESULT STDMETHODCALLTYPE ISoapClient_putref_HeaderHandler_Proxy( 
    ISoapClient * This,
    /* [in] */ IDispatch *par_HeaderHandler);


void __RPC_STUB ISoapClient_putref_HeaderHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISoapClient_put_ClientProperty_Proxy( 
    ISoapClient * This,
    /* [in] */ BSTR par_PropertyName,
    /* [in] */ VARIANT par_PropertyValue);


void __RPC_STUB ISoapClient_put_ClientProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE ISoapClient_get_ConnectorProperty_Proxy( 
    ISoapClient * This,
    /* [in] */ BSTR par_PropertyName,
    /* [retval][out] */ VARIANT *par_PropertyValue);


void __RPC_STUB ISoapClient_get_ConnectorProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE ISoapClient_put_ConnectorProperty_Proxy( 
    ISoapClient * This,
    /* [in] */ BSTR par_PropertyName,
    /* [in] */ VARIANT par_PropertyValue);


void __RPC_STUB ISoapClient_put_ConnectorProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapClient_MSSoapInit2_Proxy( 
    ISoapClient * This,
    /* [in] */ VARIANT par_WSDLFile,
    /* [in] */ VARIANT par_WSMLFile,
    /* [in] */ BSTR par_ServiceName,
    /* [in] */ BSTR par_Port,
    /* [in] */ BSTR par_Namespace);


void __RPC_STUB ISoapClient_MSSoapInit2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapClient_get_FaultCodeNamespace_Proxy( 
    ISoapClient * This,
    /* [retval][out] */ BSTR *par_faultcodeNamespace);


void __RPC_STUB ISoapClient_get_FaultCodeNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propputref] */ HRESULT STDMETHODCALLTYPE ISoapClient_putref_ClientProperty_Proxy( 
    ISoapClient * This,
    /* [in] */ BSTR par_PropertyName,
    /* [in] */ VARIANT par_PropertyValue);


void __RPC_STUB ISoapClient_putref_ClientProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapClient_INTERFACE_DEFINED__ */


#ifndef __ISoapServer_INTERFACE_DEFINED__
#define __ISoapServer_INTERFACE_DEFINED__

/* interface ISoapServer */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7F017F93-9257-11D5-87EA-00B0D0BE6479")
    ISoapServer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ BSTR par_WSDLFile,
            /* [in] */ BSTR par_WSMLFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoapInvoke( 
            /* [in] */ VARIANT par_Input,
            /* [in] */ IUnknown *par_OutputStream,
            /* [defaultvalue][in] */ BSTR par_SoapAction = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SoapInvokeEx( 
            /* [in] */ VARIANT par_Input,
            /* [in] */ IUnknown *par_OutputStream,
            /* [in] */ IUnknown *par_ServerObject,
            /* [defaultvalue][in] */ BSTR par_SoapAction = L"",
            /* [defaultvalue][in] */ BSTR par_ContentType = L"") = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XmlVersion( 
            /* [in] */ BSTR par_XmlVersion) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapServer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapServer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            ISoapServer * This,
            /* [in] */ BSTR par_WSDLFile,
            /* [in] */ BSTR par_WSMLFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoapInvoke )( 
            ISoapServer * This,
            /* [in] */ VARIANT par_Input,
            /* [in] */ IUnknown *par_OutputStream,
            /* [defaultvalue][in] */ BSTR par_SoapAction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SoapInvokeEx )( 
            ISoapServer * This,
            /* [in] */ VARIANT par_Input,
            /* [in] */ IUnknown *par_OutputStream,
            /* [in] */ IUnknown *par_ServerObject,
            /* [defaultvalue][in] */ BSTR par_SoapAction,
            /* [defaultvalue][in] */ BSTR par_ContentType);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_XmlVersion )( 
            ISoapServer * This,
            /* [in] */ BSTR par_XmlVersion);
        
        END_INTERFACE
    } ISoapServerVtbl;

    interface ISoapServer
    {
        CONST_VTBL struct ISoapServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapServer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapServer_Init(This,par_WSDLFile,par_WSMLFile)	\
    (This)->lpVtbl -> Init(This,par_WSDLFile,par_WSMLFile)

#define ISoapServer_SoapInvoke(This,par_Input,par_OutputStream,par_SoapAction)	\
    (This)->lpVtbl -> SoapInvoke(This,par_Input,par_OutputStream,par_SoapAction)

#define ISoapServer_SoapInvokeEx(This,par_Input,par_OutputStream,par_ServerObject,par_SoapAction,par_ContentType)	\
    (This)->lpVtbl -> SoapInvokeEx(This,par_Input,par_OutputStream,par_ServerObject,par_SoapAction,par_ContentType)

#define ISoapServer_put_XmlVersion(This,par_XmlVersion)	\
    (This)->lpVtbl -> put_XmlVersion(This,par_XmlVersion)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapServer_Init_Proxy( 
    ISoapServer * This,
    /* [in] */ BSTR par_WSDLFile,
    /* [in] */ BSTR par_WSMLFile);


void __RPC_STUB ISoapServer_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapServer_SoapInvoke_Proxy( 
    ISoapServer * This,
    /* [in] */ VARIANT par_Input,
    /* [in] */ IUnknown *par_OutputStream,
    /* [defaultvalue][in] */ BSTR par_SoapAction);


void __RPC_STUB ISoapServer_SoapInvoke_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapServer_SoapInvokeEx_Proxy( 
    ISoapServer * This,
    /* [in] */ VARIANT par_Input,
    /* [in] */ IUnknown *par_OutputStream,
    /* [in] */ IUnknown *par_ServerObject,
    /* [defaultvalue][in] */ BSTR par_SoapAction,
    /* [defaultvalue][in] */ BSTR par_ContentType);


void __RPC_STUB ISoapServer_SoapInvokeEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISoapServer_put_XmlVersion_Proxy( 
    ISoapServer * This,
    /* [in] */ BSTR par_XmlVersion);


void __RPC_STUB ISoapServer_put_XmlVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapServer_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SoapServer30;

#ifdef __cplusplus

class DECLSPEC_UUID("7F017F96-9257-11D5-87EA-00B0D0BE6479")
SoapServer30;
#endif

EXTERN_C const CLSID CLSID_SoapClient30;

#ifdef __cplusplus

class DECLSPEC_UUID("7F017F97-9257-11D5-87EA-00B0D0BE6479")
SoapClient30;
#endif

#ifndef __ISoapTypeMapperFactory_INTERFACE_DEFINED__
#define __ISoapTypeMapperFactory_INTERFACE_DEFINED__

/* interface ISoapTypeMapperFactory */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapTypeMapperFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("fced9f15-d0a7-4380-87e6-992381acd213")
    ISoapTypeMapperFactory : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddSchema( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_SchemaNode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetElementMapperByName( 
            /* [in] */ BSTR par_ElementName,
            /* [in] */ BSTR par_ElementNamespace,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTypeMapperByName( 
            /* [in] */ BSTR par_TypeName,
            /* [in] */ BSTR par_TypeNamespace,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetElementMapper( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_ElementNode,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTypeMapper( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_TypeNode,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddType( 
            /* [in] */ BSTR par_TypeName,
            /* [in] */ BSTR par_TypeNamespace,
            /* [in] */ BSTR par_ProgID,
            /* [defaultvalue][in] */ BSTR par_IID = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddElement( 
            /* [in] */ BSTR par_ElementName,
            /* [in] */ BSTR par_ElementNamespace,
            /* [in] */ BSTR par_ProgID,
            /* [defaultvalue][in] */ BSTR par_IID = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMapper( 
            /* [in] */ enXSDType par_xsdType,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_SchemaNode,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddCustomMapper( 
            /* [in] */ BSTR par_ProgID,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDefinitionsNode( 
            /* [in] */ BSTR par_BaseName,
            /* [in] */ BSTR par_Namespace,
            /* [in] */ VARIANT_BOOL par_LookForElement,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_SchemaNode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapTypeMapperFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapTypeMapperFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapTypeMapperFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapTypeMapperFactory * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddSchema )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_SchemaNode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetElementMapperByName )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ BSTR par_ElementName,
            /* [in] */ BSTR par_ElementNamespace,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTypeMapperByName )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ BSTR par_TypeName,
            /* [in] */ BSTR par_TypeNamespace,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetElementMapper )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_ElementNode,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTypeMapper )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_TypeNode,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddType )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ BSTR par_TypeName,
            /* [in] */ BSTR par_TypeNamespace,
            /* [in] */ BSTR par_ProgID,
            /* [defaultvalue][in] */ BSTR par_IID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddElement )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ BSTR par_ElementName,
            /* [in] */ BSTR par_ElementNamespace,
            /* [in] */ BSTR par_ProgID,
            /* [defaultvalue][in] */ BSTR par_IID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMapper )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ enXSDType par_xsdType,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_SchemaNode,
            /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddCustomMapper )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ BSTR par_ProgID,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDefinitionsNode )( 
            ISoapTypeMapperFactory * This,
            /* [in] */ BSTR par_BaseName,
            /* [in] */ BSTR par_Namespace,
            /* [in] */ VARIANT_BOOL par_LookForElement,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_SchemaNode);
        
        END_INTERFACE
    } ISoapTypeMapperFactoryVtbl;

    interface ISoapTypeMapperFactory
    {
        CONST_VTBL struct ISoapTypeMapperFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapTypeMapperFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapTypeMapperFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapTypeMapperFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapTypeMapperFactory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapTypeMapperFactory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapTypeMapperFactory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapTypeMapperFactory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapTypeMapperFactory_AddSchema(This,par_SchemaNode)	\
    (This)->lpVtbl -> AddSchema(This,par_SchemaNode)

#define ISoapTypeMapperFactory_GetElementMapperByName(This,par_ElementName,par_ElementNamespace,par_SoapTypeMapper)	\
    (This)->lpVtbl -> GetElementMapperByName(This,par_ElementName,par_ElementNamespace,par_SoapTypeMapper)

#define ISoapTypeMapperFactory_GetTypeMapperByName(This,par_TypeName,par_TypeNamespace,par_SoapTypeMapper)	\
    (This)->lpVtbl -> GetTypeMapperByName(This,par_TypeName,par_TypeNamespace,par_SoapTypeMapper)

#define ISoapTypeMapperFactory_GetElementMapper(This,par_ElementNode,par_SoapTypeMapper)	\
    (This)->lpVtbl -> GetElementMapper(This,par_ElementNode,par_SoapTypeMapper)

#define ISoapTypeMapperFactory_GetTypeMapper(This,par_TypeNode,par_SoapTypeMapper)	\
    (This)->lpVtbl -> GetTypeMapper(This,par_TypeNode,par_SoapTypeMapper)

#define ISoapTypeMapperFactory_AddType(This,par_TypeName,par_TypeNamespace,par_ProgID,par_IID)	\
    (This)->lpVtbl -> AddType(This,par_TypeName,par_TypeNamespace,par_ProgID,par_IID)

#define ISoapTypeMapperFactory_AddElement(This,par_ElementName,par_ElementNamespace,par_ProgID,par_IID)	\
    (This)->lpVtbl -> AddElement(This,par_ElementName,par_ElementNamespace,par_ProgID,par_IID)

#define ISoapTypeMapperFactory_GetMapper(This,par_xsdType,par_SchemaNode,par_SoapTypeMapper)	\
    (This)->lpVtbl -> GetMapper(This,par_xsdType,par_SchemaNode,par_SoapTypeMapper)

#define ISoapTypeMapperFactory_AddCustomMapper(This,par_ProgID,par_WSMLNode)	\
    (This)->lpVtbl -> AddCustomMapper(This,par_ProgID,par_WSMLNode)

#define ISoapTypeMapperFactory_GetDefinitionsNode(This,par_BaseName,par_Namespace,par_LookForElement,par_SchemaNode)	\
    (This)->lpVtbl -> GetDefinitionsNode(This,par_BaseName,par_Namespace,par_LookForElement,par_SchemaNode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_AddSchema_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_SchemaNode);


void __RPC_STUB ISoapTypeMapperFactory_AddSchema_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_GetElementMapperByName_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ BSTR par_ElementName,
    /* [in] */ BSTR par_ElementNamespace,
    /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);


void __RPC_STUB ISoapTypeMapperFactory_GetElementMapperByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_GetTypeMapperByName_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ BSTR par_TypeName,
    /* [in] */ BSTR par_TypeNamespace,
    /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);


void __RPC_STUB ISoapTypeMapperFactory_GetTypeMapperByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_GetElementMapper_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_ElementNode,
    /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);


void __RPC_STUB ISoapTypeMapperFactory_GetElementMapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_GetTypeMapper_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_TypeNode,
    /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);


void __RPC_STUB ISoapTypeMapperFactory_GetTypeMapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_AddType_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ BSTR par_TypeName,
    /* [in] */ BSTR par_TypeNamespace,
    /* [in] */ BSTR par_ProgID,
    /* [defaultvalue][in] */ BSTR par_IID);


void __RPC_STUB ISoapTypeMapperFactory_AddType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_AddElement_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ BSTR par_ElementName,
    /* [in] */ BSTR par_ElementNamespace,
    /* [in] */ BSTR par_ProgID,
    /* [defaultvalue][in] */ BSTR par_IID);


void __RPC_STUB ISoapTypeMapperFactory_AddElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_GetMapper_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ enXSDType par_xsdType,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_SchemaNode,
    /* [retval][out] */ ISoapTypeMapper **par_SoapTypeMapper);


void __RPC_STUB ISoapTypeMapperFactory_GetMapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_AddCustomMapper_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ BSTR par_ProgID,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode);


void __RPC_STUB ISoapTypeMapperFactory_AddCustomMapper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapperFactory_GetDefinitionsNode_Proxy( 
    ISoapTypeMapperFactory * This,
    /* [in] */ BSTR par_BaseName,
    /* [in] */ BSTR par_Namespace,
    /* [in] */ VARIANT_BOOL par_LookForElement,
    /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_SchemaNode);


void __RPC_STUB ISoapTypeMapperFactory_GetDefinitionsNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapTypeMapperFactory_INTERFACE_DEFINED__ */


#ifndef __IHeaderHandler_INTERFACE_DEFINED__
#define __IHeaderHandler_INTERFACE_DEFINED__

/* interface IHeaderHandler */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IHeaderHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("504D4B91-76B8-4D88-95EA-CEB5E0FE41F3")
    IHeaderHandler : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WillWriteHeaders( 
            /* [retval][out] */ VARIANT_BOOL *par_WillWriteHeaders) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteHeaders( 
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            /* [in] */ IDispatch *par_Object) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadHeader( 
            /* [in] */ ISoapReader *par_Reader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_HeaderNode,
            /* [in] */ IDispatch *par_Object,
            /* [retval][out] */ VARIANT_BOOL *par_Understood) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHeaderHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHeaderHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHeaderHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHeaderHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHeaderHandler * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHeaderHandler * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHeaderHandler * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHeaderHandler * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WillWriteHeaders )( 
            IHeaderHandler * This,
            /* [retval][out] */ VARIANT_BOOL *par_WillWriteHeaders);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteHeaders )( 
            IHeaderHandler * This,
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            /* [in] */ IDispatch *par_Object);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadHeader )( 
            IHeaderHandler * This,
            /* [in] */ ISoapReader *par_Reader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_HeaderNode,
            /* [in] */ IDispatch *par_Object,
            /* [retval][out] */ VARIANT_BOOL *par_Understood);
        
        END_INTERFACE
    } IHeaderHandlerVtbl;

    interface IHeaderHandler
    {
        CONST_VTBL struct IHeaderHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHeaderHandler_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHeaderHandler_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHeaderHandler_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHeaderHandler_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHeaderHandler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHeaderHandler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHeaderHandler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHeaderHandler_WillWriteHeaders(This,par_WillWriteHeaders)	\
    (This)->lpVtbl -> WillWriteHeaders(This,par_WillWriteHeaders)

#define IHeaderHandler_WriteHeaders(This,par_ISoapSerializer,par_Object)	\
    (This)->lpVtbl -> WriteHeaders(This,par_ISoapSerializer,par_Object)

#define IHeaderHandler_ReadHeader(This,par_Reader,par_HeaderNode,par_Object,par_Understood)	\
    (This)->lpVtbl -> ReadHeader(This,par_Reader,par_HeaderNode,par_Object,par_Understood)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHeaderHandler_WillWriteHeaders_Proxy( 
    IHeaderHandler * This,
    /* [retval][out] */ VARIANT_BOOL *par_WillWriteHeaders);


void __RPC_STUB IHeaderHandler_WillWriteHeaders_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHeaderHandler_WriteHeaders_Proxy( 
    IHeaderHandler * This,
    /* [in] */ ISoapSerializer *par_ISoapSerializer,
    /* [in] */ IDispatch *par_Object);


void __RPC_STUB IHeaderHandler_WriteHeaders_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IHeaderHandler_ReadHeader_Proxy( 
    IHeaderHandler * This,
    /* [in] */ ISoapReader *par_Reader,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_HeaderNode,
    /* [in] */ IDispatch *par_Object,
    /* [retval][out] */ VARIANT_BOOL *par_Understood);


void __RPC_STUB IHeaderHandler_ReadHeader_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHeaderHandler_INTERFACE_DEFINED__ */


#ifndef __ISoapTypeMapper_INTERFACE_DEFINED__
#define __ISoapTypeMapper_INTERFACE_DEFINED__

/* interface ISoapTypeMapper */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapTypeMapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("29d3f736-1c25-44ee-9cee-3f52f226ba8a")
    ISoapTypeMapper : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ ISoapTypeMapperFactory *par_Factory,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_Schema,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode,
            /* [in] */ enXSDType par_xsdType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ ISoapReader *par_SoapReader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_Node,
            /* [in] */ BSTR par_Encoding,
            /* [in] */ enEncodingStyle par_encodingMode,
            /* [in] */ long par_Flags,
            /* [retval][out] */ VARIANT *par_var) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            /* [in] */ BSTR par_Encoding,
            /* [in] */ enEncodingStyle par_encodingMode,
            /* [in] */ long par_Flags,
            /* [in] */ VARIANT *par_var) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VarType( 
            /* [retval][out] */ long *par_Type) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iid( 
            /* [retval][out] */ BSTR *par_IIDAsString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SchemaNode( 
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_schemaNode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XsdType( 
            /* [retval][out] */ enXSDType *par_xsdType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapTypeMapperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapTypeMapper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapTypeMapper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapTypeMapper * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapTypeMapper * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapTypeMapper * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapTypeMapper * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapTypeMapper * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            ISoapTypeMapper * This,
            /* [in] */ ISoapTypeMapperFactory *par_Factory,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_Schema,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode,
            /* [in] */ enXSDType par_xsdType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Read )( 
            ISoapTypeMapper * This,
            /* [in] */ ISoapReader *par_SoapReader,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_Node,
            /* [in] */ BSTR par_Encoding,
            /* [in] */ enEncodingStyle par_encodingMode,
            /* [in] */ long par_Flags,
            /* [retval][out] */ VARIANT *par_var);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Write )( 
            ISoapTypeMapper * This,
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            /* [in] */ BSTR par_Encoding,
            /* [in] */ enEncodingStyle par_encodingMode,
            /* [in] */ long par_Flags,
            /* [in] */ VARIANT *par_var);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VarType )( 
            ISoapTypeMapper * This,
            /* [retval][out] */ long *par_Type);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Iid )( 
            ISoapTypeMapper * This,
            /* [retval][out] */ BSTR *par_IIDAsString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SchemaNode )( 
            ISoapTypeMapper * This,
            /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_schemaNode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *XsdType )( 
            ISoapTypeMapper * This,
            /* [retval][out] */ enXSDType *par_xsdType);
        
        END_INTERFACE
    } ISoapTypeMapperVtbl;

    interface ISoapTypeMapper
    {
        CONST_VTBL struct ISoapTypeMapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapTypeMapper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapTypeMapper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapTypeMapper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapTypeMapper_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapTypeMapper_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapTypeMapper_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapTypeMapper_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapTypeMapper_Init(This,par_Factory,par_Schema,par_WSMLNode,par_xsdType)	\
    (This)->lpVtbl -> Init(This,par_Factory,par_Schema,par_WSMLNode,par_xsdType)

#define ISoapTypeMapper_Read(This,par_SoapReader,par_Node,par_Encoding,par_encodingMode,par_Flags,par_var)	\
    (This)->lpVtbl -> Read(This,par_SoapReader,par_Node,par_Encoding,par_encodingMode,par_Flags,par_var)

#define ISoapTypeMapper_Write(This,par_ISoapSerializer,par_Encoding,par_encodingMode,par_Flags,par_var)	\
    (This)->lpVtbl -> Write(This,par_ISoapSerializer,par_Encoding,par_encodingMode,par_Flags,par_var)

#define ISoapTypeMapper_VarType(This,par_Type)	\
    (This)->lpVtbl -> VarType(This,par_Type)

#define ISoapTypeMapper_Iid(This,par_IIDAsString)	\
    (This)->lpVtbl -> Iid(This,par_IIDAsString)

#define ISoapTypeMapper_SchemaNode(This,par_schemaNode)	\
    (This)->lpVtbl -> SchemaNode(This,par_schemaNode)

#define ISoapTypeMapper_XsdType(This,par_xsdType)	\
    (This)->lpVtbl -> XsdType(This,par_xsdType)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapper_Init_Proxy( 
    ISoapTypeMapper * This,
    /* [in] */ ISoapTypeMapperFactory *par_Factory,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_Schema,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode,
    /* [in] */ enXSDType par_xsdType);


void __RPC_STUB ISoapTypeMapper_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapper_Read_Proxy( 
    ISoapTypeMapper * This,
    /* [in] */ ISoapReader *par_SoapReader,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_Node,
    /* [in] */ BSTR par_Encoding,
    /* [in] */ enEncodingStyle par_encodingMode,
    /* [in] */ long par_Flags,
    /* [retval][out] */ VARIANT *par_var);


void __RPC_STUB ISoapTypeMapper_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapper_Write_Proxy( 
    ISoapTypeMapper * This,
    /* [in] */ ISoapSerializer *par_ISoapSerializer,
    /* [in] */ BSTR par_Encoding,
    /* [in] */ enEncodingStyle par_encodingMode,
    /* [in] */ long par_Flags,
    /* [in] */ VARIANT *par_var);


void __RPC_STUB ISoapTypeMapper_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapper_VarType_Proxy( 
    ISoapTypeMapper * This,
    /* [retval][out] */ long *par_Type);


void __RPC_STUB ISoapTypeMapper_VarType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapper_Iid_Proxy( 
    ISoapTypeMapper * This,
    /* [retval][out] */ BSTR *par_IIDAsString);


void __RPC_STUB ISoapTypeMapper_Iid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapper_SchemaNode_Proxy( 
    ISoapTypeMapper * This,
    /* [retval][out] */ /* external definition not present */ IXMLDOMNode **par_schemaNode);


void __RPC_STUB ISoapTypeMapper_SchemaNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapTypeMapper_XsdType_Proxy( 
    ISoapTypeMapper * This,
    /* [retval][out] */ enXSDType *par_xsdType);


void __RPC_STUB ISoapTypeMapper_XsdType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapTypeMapper_INTERFACE_DEFINED__ */


#ifndef __IEnumSoapMappers_INTERFACE_DEFINED__
#define __IEnumSoapMappers_INTERFACE_DEFINED__

/* interface IEnumSoapMappers */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IEnumSoapMappers;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("acddced6-6db8-497a-bf10-068711629924")
    IEnumSoapMappers : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Next( 
            long par_celt,
            /* [out] */ ISoapMapper **par_SoapMapper,
            /* [out] */ long *par_Fetched) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Skip( 
            long par_celt) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumSoapMappers **par_enum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumSoapMappersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumSoapMappers * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumSoapMappers * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumSoapMappers * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            IEnumSoapMappers * This,
            long par_celt,
            /* [out] */ ISoapMapper **par_SoapMapper,
            /* [out] */ long *par_Fetched);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumSoapMappers * This,
            long par_celt);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumSoapMappers * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumSoapMappers * This,
            /* [out] */ IEnumSoapMappers **par_enum);
        
        END_INTERFACE
    } IEnumSoapMappersVtbl;

    interface IEnumSoapMappers
    {
        CONST_VTBL struct IEnumSoapMappersVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumSoapMappers_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumSoapMappers_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumSoapMappers_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumSoapMappers_Next(This,par_celt,par_SoapMapper,par_Fetched)	\
    (This)->lpVtbl -> Next(This,par_celt,par_SoapMapper,par_Fetched)

#define IEnumSoapMappers_Skip(This,par_celt)	\
    (This)->lpVtbl -> Skip(This,par_celt)

#define IEnumSoapMappers_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumSoapMappers_Clone(This,par_enum)	\
    (This)->lpVtbl -> Clone(This,par_enum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumSoapMappers_Next_Proxy( 
    IEnumSoapMappers * This,
    long par_celt,
    /* [out] */ ISoapMapper **par_SoapMapper,
    /* [out] */ long *par_Fetched);


void __RPC_STUB IEnumSoapMappers_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumSoapMappers_Skip_Proxy( 
    IEnumSoapMappers * This,
    long par_celt);


void __RPC_STUB IEnumSoapMappers_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumSoapMappers_Reset_Proxy( 
    IEnumSoapMappers * This);


void __RPC_STUB IEnumSoapMappers_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumSoapMappers_Clone_Proxy( 
    IEnumSoapMappers * This,
    /* [out] */ IEnumSoapMappers **par_enum);


void __RPC_STUB IEnumSoapMappers_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumSoapMappers_INTERFACE_DEFINED__ */


#ifndef __IWSDLMessage_INTERFACE_DEFINED__
#define __IWSDLMessage_INTERFACE_DEFINED__

/* interface IWSDLMessage */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IWSDLMessage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("49f9421c-dc88-43e1-825f-70e788e9a9a9")
    IWSDLMessage : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_EncodingStyle( 
            /* [retval][out] */ enEncodingStyle *par_enStyle) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_EncodingNamespace( 
            /* [retval][out] */ BSTR *par_encodingNamespace) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_MessageName( 
            /* [retval][out] */ BSTR *par_messageName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_MessageNamespace( 
            /* [retval][out] */ BSTR *par_messageNamespace) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_MessageParts( 
            /* [retval][out] */ IEnumSoapMappers **par_IEnumSoapMappers) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetComposer( 
            /* [defaultvalue][in] */ BSTR par_TempFolder,
            /* [defaultvalue][in] */ long par_MaxSize,
            /* [retval][out] */ IMessageComposer **par_Composer) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetParser( 
            /* [defaultvalue][in] */ BSTR par_TempFolder,
            /* [defaultvalue][in] */ long par_MaxSize,
            /* [retval][out] */ IMessageParser **par_Parser) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddAttachmentCollection( 
            BSTR par_bstrPartName,
            LONG par_lserverSideCallIndex) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWSDLMessageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWSDLMessage * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWSDLMessage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWSDLMessage * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EncodingStyle )( 
            IWSDLMessage * This,
            /* [retval][out] */ enEncodingStyle *par_enStyle);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EncodingNamespace )( 
            IWSDLMessage * This,
            /* [retval][out] */ BSTR *par_encodingNamespace);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MessageName )( 
            IWSDLMessage * This,
            /* [retval][out] */ BSTR *par_messageName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MessageNamespace )( 
            IWSDLMessage * This,
            /* [retval][out] */ BSTR *par_messageNamespace);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MessageParts )( 
            IWSDLMessage * This,
            /* [retval][out] */ IEnumSoapMappers **par_IEnumSoapMappers);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetComposer )( 
            IWSDLMessage * This,
            /* [defaultvalue][in] */ BSTR par_TempFolder,
            /* [defaultvalue][in] */ long par_MaxSize,
            /* [retval][out] */ IMessageComposer **par_Composer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetParser )( 
            IWSDLMessage * This,
            /* [defaultvalue][in] */ BSTR par_TempFolder,
            /* [defaultvalue][in] */ long par_MaxSize,
            /* [retval][out] */ IMessageParser **par_Parser);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddAttachmentCollection )( 
            IWSDLMessage * This,
            BSTR par_bstrPartName,
            LONG par_lserverSideCallIndex);
        
        END_INTERFACE
    } IWSDLMessageVtbl;

    interface IWSDLMessage
    {
        CONST_VTBL struct IWSDLMessageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWSDLMessage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWSDLMessage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWSDLMessage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWSDLMessage_get_EncodingStyle(This,par_enStyle)	\
    (This)->lpVtbl -> get_EncodingStyle(This,par_enStyle)

#define IWSDLMessage_get_EncodingNamespace(This,par_encodingNamespace)	\
    (This)->lpVtbl -> get_EncodingNamespace(This,par_encodingNamespace)

#define IWSDLMessage_get_MessageName(This,par_messageName)	\
    (This)->lpVtbl -> get_MessageName(This,par_messageName)

#define IWSDLMessage_get_MessageNamespace(This,par_messageNamespace)	\
    (This)->lpVtbl -> get_MessageNamespace(This,par_messageNamespace)

#define IWSDLMessage_get_MessageParts(This,par_IEnumSoapMappers)	\
    (This)->lpVtbl -> get_MessageParts(This,par_IEnumSoapMappers)

#define IWSDLMessage_GetComposer(This,par_TempFolder,par_MaxSize,par_Composer)	\
    (This)->lpVtbl -> GetComposer(This,par_TempFolder,par_MaxSize,par_Composer)

#define IWSDLMessage_GetParser(This,par_TempFolder,par_MaxSize,par_Parser)	\
    (This)->lpVtbl -> GetParser(This,par_TempFolder,par_MaxSize,par_Parser)

#define IWSDLMessage_AddAttachmentCollection(This,par_bstrPartName,par_lserverSideCallIndex)	\
    (This)->lpVtbl -> AddAttachmentCollection(This,par_bstrPartName,par_lserverSideCallIndex)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_get_EncodingStyle_Proxy( 
    IWSDLMessage * This,
    /* [retval][out] */ enEncodingStyle *par_enStyle);


void __RPC_STUB IWSDLMessage_get_EncodingStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_get_EncodingNamespace_Proxy( 
    IWSDLMessage * This,
    /* [retval][out] */ BSTR *par_encodingNamespace);


void __RPC_STUB IWSDLMessage_get_EncodingNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_get_MessageName_Proxy( 
    IWSDLMessage * This,
    /* [retval][out] */ BSTR *par_messageName);


void __RPC_STUB IWSDLMessage_get_MessageName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_get_MessageNamespace_Proxy( 
    IWSDLMessage * This,
    /* [retval][out] */ BSTR *par_messageNamespace);


void __RPC_STUB IWSDLMessage_get_MessageNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_get_MessageParts_Proxy( 
    IWSDLMessage * This,
    /* [retval][out] */ IEnumSoapMappers **par_IEnumSoapMappers);


void __RPC_STUB IWSDLMessage_get_MessageParts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_GetComposer_Proxy( 
    IWSDLMessage * This,
    /* [defaultvalue][in] */ BSTR par_TempFolder,
    /* [defaultvalue][in] */ long par_MaxSize,
    /* [retval][out] */ IMessageComposer **par_Composer);


void __RPC_STUB IWSDLMessage_GetComposer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_GetParser_Proxy( 
    IWSDLMessage * This,
    /* [defaultvalue][in] */ BSTR par_TempFolder,
    /* [defaultvalue][in] */ long par_MaxSize,
    /* [retval][out] */ IMessageParser **par_Parser);


void __RPC_STUB IWSDLMessage_GetParser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLMessage_AddAttachmentCollection_Proxy( 
    IWSDLMessage * This,
    BSTR par_bstrPartName,
    LONG par_lserverSideCallIndex);


void __RPC_STUB IWSDLMessage_AddAttachmentCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWSDLMessage_INTERFACE_DEFINED__ */


#ifndef __IWSDLOperation_INTERFACE_DEFINED__
#define __IWSDLOperation_INTERFACE_DEFINED__

/* interface IWSDLOperation */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IWSDLOperation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a0b762a7-9f3e-48d8-b333-770e5fa72a1e")
    IWSDLOperation : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Documentation( 
            /* [retval][out] */ BSTR *par_Documentation) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *par_OperationName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SoapAction( 
            /* [retval][out] */ BSTR *par_SoapAction) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ObjectProgId( 
            /* [retval][out] */ BSTR *par_ObjectProgId) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ObjectMethod( 
            /* [retval][out] */ BSTR *par_ObjectMethod) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_InputMessage( 
            /* [retval][out] */ IWSDLMessage **par_InputMessage) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_OutputMessage( 
            /* [retval][out] */ IWSDLMessage **par_OutputMessage) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_PreferredEncoding( 
            /* [retval][out] */ BSTR *par_preferredEncoding) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetOperationParts( 
            /* [out] */ IEnumSoapMappers **par_IEnumSoapMappers) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ExecuteOperation( 
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [in] */ ISoapSerializer *par_ISoapSerializer) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ExecuteOperationEx( 
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            /* [in] */ IUnknown *par_ServerObject) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            VARIANT_BOOL par_Input) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ ISoapReader *par_ISoapReader,
            VARIANT_BOOL par_Input) = 0;
        
        virtual /* [hidden][helpstring] */ const WCHAR *STDMETHODCALLTYPE GetNameRef( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ enOperationType *par_Type) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWSDLOperationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWSDLOperation * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWSDLOperation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWSDLOperation * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Documentation )( 
            IWSDLOperation * This,
            /* [retval][out] */ BSTR *par_Documentation);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IWSDLOperation * This,
            /* [retval][out] */ BSTR *par_OperationName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SoapAction )( 
            IWSDLOperation * This,
            /* [retval][out] */ BSTR *par_SoapAction);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ObjectProgId )( 
            IWSDLOperation * This,
            /* [retval][out] */ BSTR *par_ObjectProgId);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ObjectMethod )( 
            IWSDLOperation * This,
            /* [retval][out] */ BSTR *par_ObjectMethod);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InputMessage )( 
            IWSDLOperation * This,
            /* [retval][out] */ IWSDLMessage **par_InputMessage);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OutputMessage )( 
            IWSDLOperation * This,
            /* [retval][out] */ IWSDLMessage **par_OutputMessage);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PreferredEncoding )( 
            IWSDLOperation * This,
            /* [retval][out] */ BSTR *par_preferredEncoding);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetOperationParts )( 
            IWSDLOperation * This,
            /* [out] */ IEnumSoapMappers **par_IEnumSoapMappers);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ExecuteOperation )( 
            IWSDLOperation * This,
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [in] */ ISoapSerializer *par_ISoapSerializer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ExecuteOperationEx )( 
            IWSDLOperation * This,
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            /* [in] */ IUnknown *par_ServerObject);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IWSDLOperation * This,
            /* [in] */ ISoapSerializer *par_ISoapSerializer,
            VARIANT_BOOL par_Input);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IWSDLOperation * This,
            /* [in] */ ISoapReader *par_ISoapReader,
            VARIANT_BOOL par_Input);
        
        /* [hidden][helpstring] */ const WCHAR *( STDMETHODCALLTYPE *GetNameRef )( 
            IWSDLOperation * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IWSDLOperation * This,
            /* [retval][out] */ enOperationType *par_Type);
        
        END_INTERFACE
    } IWSDLOperationVtbl;

    interface IWSDLOperation
    {
        CONST_VTBL struct IWSDLOperationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWSDLOperation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWSDLOperation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWSDLOperation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWSDLOperation_get_Documentation(This,par_Documentation)	\
    (This)->lpVtbl -> get_Documentation(This,par_Documentation)

#define IWSDLOperation_get_Name(This,par_OperationName)	\
    (This)->lpVtbl -> get_Name(This,par_OperationName)

#define IWSDLOperation_get_SoapAction(This,par_SoapAction)	\
    (This)->lpVtbl -> get_SoapAction(This,par_SoapAction)

#define IWSDLOperation_get_ObjectProgId(This,par_ObjectProgId)	\
    (This)->lpVtbl -> get_ObjectProgId(This,par_ObjectProgId)

#define IWSDLOperation_get_ObjectMethod(This,par_ObjectMethod)	\
    (This)->lpVtbl -> get_ObjectMethod(This,par_ObjectMethod)

#define IWSDLOperation_get_InputMessage(This,par_InputMessage)	\
    (This)->lpVtbl -> get_InputMessage(This,par_InputMessage)

#define IWSDLOperation_get_OutputMessage(This,par_OutputMessage)	\
    (This)->lpVtbl -> get_OutputMessage(This,par_OutputMessage)

#define IWSDLOperation_get_PreferredEncoding(This,par_preferredEncoding)	\
    (This)->lpVtbl -> get_PreferredEncoding(This,par_preferredEncoding)

#define IWSDLOperation_GetOperationParts(This,par_IEnumSoapMappers)	\
    (This)->lpVtbl -> GetOperationParts(This,par_IEnumSoapMappers)

#define IWSDLOperation_ExecuteOperation(This,par_ISoapReader,par_ISoapSerializer)	\
    (This)->lpVtbl -> ExecuteOperation(This,par_ISoapReader,par_ISoapSerializer)

#define IWSDLOperation_ExecuteOperationEx(This,par_ISoapReader,par_ISoapSerializer,par_ServerObject)	\
    (This)->lpVtbl -> ExecuteOperationEx(This,par_ISoapReader,par_ISoapSerializer,par_ServerObject)

#define IWSDLOperation_Save(This,par_ISoapSerializer,par_Input)	\
    (This)->lpVtbl -> Save(This,par_ISoapSerializer,par_Input)

#define IWSDLOperation_Load(This,par_ISoapReader,par_Input)	\
    (This)->lpVtbl -> Load(This,par_ISoapReader,par_Input)

#define IWSDLOperation_GetNameRef(This)	\
    (This)->lpVtbl -> GetNameRef(This)

#define IWSDLOperation_get_Type(This,par_Type)	\
    (This)->lpVtbl -> get_Type(This,par_Type)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_Documentation_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ BSTR *par_Documentation);


void __RPC_STUB IWSDLOperation_get_Documentation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_Name_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ BSTR *par_OperationName);


void __RPC_STUB IWSDLOperation_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_SoapAction_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ BSTR *par_SoapAction);


void __RPC_STUB IWSDLOperation_get_SoapAction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_ObjectProgId_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ BSTR *par_ObjectProgId);


void __RPC_STUB IWSDLOperation_get_ObjectProgId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_ObjectMethod_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ BSTR *par_ObjectMethod);


void __RPC_STUB IWSDLOperation_get_ObjectMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_InputMessage_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ IWSDLMessage **par_InputMessage);


void __RPC_STUB IWSDLOperation_get_InputMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_OutputMessage_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ IWSDLMessage **par_OutputMessage);


void __RPC_STUB IWSDLOperation_get_OutputMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_PreferredEncoding_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ BSTR *par_preferredEncoding);


void __RPC_STUB IWSDLOperation_get_PreferredEncoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_GetOperationParts_Proxy( 
    IWSDLOperation * This,
    /* [out] */ IEnumSoapMappers **par_IEnumSoapMappers);


void __RPC_STUB IWSDLOperation_GetOperationParts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_ExecuteOperation_Proxy( 
    IWSDLOperation * This,
    /* [in] */ ISoapReader *par_ISoapReader,
    /* [in] */ ISoapSerializer *par_ISoapSerializer);


void __RPC_STUB IWSDLOperation_ExecuteOperation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_ExecuteOperationEx_Proxy( 
    IWSDLOperation * This,
    /* [in] */ ISoapReader *par_ISoapReader,
    /* [in] */ ISoapSerializer *par_ISoapSerializer,
    /* [in] */ IUnknown *par_ServerObject);


void __RPC_STUB IWSDLOperation_ExecuteOperationEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_Save_Proxy( 
    IWSDLOperation * This,
    /* [in] */ ISoapSerializer *par_ISoapSerializer,
    VARIANT_BOOL par_Input);


void __RPC_STUB IWSDLOperation_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_Load_Proxy( 
    IWSDLOperation * This,
    /* [in] */ ISoapReader *par_ISoapReader,
    VARIANT_BOOL par_Input);


void __RPC_STUB IWSDLOperation_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][helpstring] */ const WCHAR *STDMETHODCALLTYPE IWSDLOperation_GetNameRef_Proxy( 
    IWSDLOperation * This);


void __RPC_STUB IWSDLOperation_GetNameRef_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLOperation_get_Type_Proxy( 
    IWSDLOperation * This,
    /* [retval][out] */ enOperationType *par_Type);


void __RPC_STUB IWSDLOperation_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWSDLOperation_INTERFACE_DEFINED__ */


#ifndef __IEnumWSDLOperations_INTERFACE_DEFINED__
#define __IEnumWSDLOperations_INTERFACE_DEFINED__

/* interface IEnumWSDLOperations */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IEnumWSDLOperations;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b0bba669-55f7-4e9c-941e-49bc4715c834")
    IEnumWSDLOperations : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Next( 
            long par_celt,
            /* [out] */ IWSDLOperation **par_WSDLOperation,
            /* [out] */ long *par_Fetched) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Skip( 
            long par_celt) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumWSDLOperations **par_enum) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Find( 
            BSTR par_OperationToFind,
            /* [out] */ IWSDLOperation **par_IWSDLOperation) = 0;
        
        virtual /* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE Size( 
            /* [out] */ long *par_Size) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumWSDLOperationsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumWSDLOperations * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumWSDLOperations * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumWSDLOperations * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            IEnumWSDLOperations * This,
            long par_celt,
            /* [out] */ IWSDLOperation **par_WSDLOperation,
            /* [out] */ long *par_Fetched);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumWSDLOperations * This,
            long par_celt);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumWSDLOperations * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumWSDLOperations * This,
            /* [out] */ IEnumWSDLOperations **par_enum);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Find )( 
            IEnumWSDLOperations * This,
            BSTR par_OperationToFind,
            /* [out] */ IWSDLOperation **par_IWSDLOperation);
        
        /* [hidden][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Size )( 
            IEnumWSDLOperations * This,
            /* [out] */ long *par_Size);
        
        END_INTERFACE
    } IEnumWSDLOperationsVtbl;

    interface IEnumWSDLOperations
    {
        CONST_VTBL struct IEnumWSDLOperationsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumWSDLOperations_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumWSDLOperations_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumWSDLOperations_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumWSDLOperations_Next(This,par_celt,par_WSDLOperation,par_Fetched)	\
    (This)->lpVtbl -> Next(This,par_celt,par_WSDLOperation,par_Fetched)

#define IEnumWSDLOperations_Skip(This,par_celt)	\
    (This)->lpVtbl -> Skip(This,par_celt)

#define IEnumWSDLOperations_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumWSDLOperations_Clone(This,par_enum)	\
    (This)->lpVtbl -> Clone(This,par_enum)

#define IEnumWSDLOperations_Find(This,par_OperationToFind,par_IWSDLOperation)	\
    (This)->lpVtbl -> Find(This,par_OperationToFind,par_IWSDLOperation)

#define IEnumWSDLOperations_Size(This,par_Size)	\
    (This)->lpVtbl -> Size(This,par_Size)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLOperations_Next_Proxy( 
    IEnumWSDLOperations * This,
    long par_celt,
    /* [out] */ IWSDLOperation **par_WSDLOperation,
    /* [out] */ long *par_Fetched);


void __RPC_STUB IEnumWSDLOperations_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLOperations_Skip_Proxy( 
    IEnumWSDLOperations * This,
    long par_celt);


void __RPC_STUB IEnumWSDLOperations_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLOperations_Reset_Proxy( 
    IEnumWSDLOperations * This);


void __RPC_STUB IEnumWSDLOperations_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLOperations_Clone_Proxy( 
    IEnumWSDLOperations * This,
    /* [out] */ IEnumWSDLOperations **par_enum);


void __RPC_STUB IEnumWSDLOperations_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLOperations_Find_Proxy( 
    IEnumWSDLOperations * This,
    BSTR par_OperationToFind,
    /* [out] */ IWSDLOperation **par_IWSDLOperation);


void __RPC_STUB IEnumWSDLOperations_Find_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLOperations_Size_Proxy( 
    IEnumWSDLOperations * This,
    /* [out] */ long *par_Size);


void __RPC_STUB IEnumWSDLOperations_Size_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumWSDLOperations_INTERFACE_DEFINED__ */


#ifndef __IWSDLPort_INTERFACE_DEFINED__
#define __IWSDLPort_INTERFACE_DEFINED__

/* interface IWSDLPort */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IWSDLPort;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4d40b730-f5fa-472c-8819-ddcd183bd0de")
    IWSDLPort : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *par_PortName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Address( 
            /* [retval][out] */ BSTR *par_PortAddress) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_BindStyle( 
            /* [retval][out] */ BSTR *par_BindStyle) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Transport( 
            /* [retval][out] */ BSTR *par_Transport) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Documentation( 
            /* [retval][out] */ BSTR *par_Documentation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSoapOperations( 
            /* [out] */ IEnumWSDLOperations **par_IWSDLOperations) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWSDLPortVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWSDLPort * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWSDLPort * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWSDLPort * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IWSDLPort * This,
            /* [retval][out] */ BSTR *par_PortName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Address )( 
            IWSDLPort * This,
            /* [retval][out] */ BSTR *par_PortAddress);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BindStyle )( 
            IWSDLPort * This,
            /* [retval][out] */ BSTR *par_BindStyle);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Transport )( 
            IWSDLPort * This,
            /* [retval][out] */ BSTR *par_Transport);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Documentation )( 
            IWSDLPort * This,
            /* [retval][out] */ BSTR *par_Documentation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSoapOperations )( 
            IWSDLPort * This,
            /* [out] */ IEnumWSDLOperations **par_IWSDLOperations);
        
        END_INTERFACE
    } IWSDLPortVtbl;

    interface IWSDLPort
    {
        CONST_VTBL struct IWSDLPortVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWSDLPort_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWSDLPort_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWSDLPort_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWSDLPort_get_Name(This,par_PortName)	\
    (This)->lpVtbl -> get_Name(This,par_PortName)

#define IWSDLPort_get_Address(This,par_PortAddress)	\
    (This)->lpVtbl -> get_Address(This,par_PortAddress)

#define IWSDLPort_get_BindStyle(This,par_BindStyle)	\
    (This)->lpVtbl -> get_BindStyle(This,par_BindStyle)

#define IWSDLPort_get_Transport(This,par_Transport)	\
    (This)->lpVtbl -> get_Transport(This,par_Transport)

#define IWSDLPort_get_Documentation(This,par_Documentation)	\
    (This)->lpVtbl -> get_Documentation(This,par_Documentation)

#define IWSDLPort_GetSoapOperations(This,par_IWSDLOperations)	\
    (This)->lpVtbl -> GetSoapOperations(This,par_IWSDLOperations)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLPort_get_Name_Proxy( 
    IWSDLPort * This,
    /* [retval][out] */ BSTR *par_PortName);


void __RPC_STUB IWSDLPort_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLPort_get_Address_Proxy( 
    IWSDLPort * This,
    /* [retval][out] */ BSTR *par_PortAddress);


void __RPC_STUB IWSDLPort_get_Address_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLPort_get_BindStyle_Proxy( 
    IWSDLPort * This,
    /* [retval][out] */ BSTR *par_BindStyle);


void __RPC_STUB IWSDLPort_get_BindStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLPort_get_Transport_Proxy( 
    IWSDLPort * This,
    /* [retval][out] */ BSTR *par_Transport);


void __RPC_STUB IWSDLPort_get_Transport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLPort_get_Documentation_Proxy( 
    IWSDLPort * This,
    /* [retval][out] */ BSTR *par_Documentation);


void __RPC_STUB IWSDLPort_get_Documentation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLPort_GetSoapOperations_Proxy( 
    IWSDLPort * This,
    /* [out] */ IEnumWSDLOperations **par_IWSDLOperations);


void __RPC_STUB IWSDLPort_GetSoapOperations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWSDLPort_INTERFACE_DEFINED__ */


#ifndef __IEnumWSDLPorts_INTERFACE_DEFINED__
#define __IEnumWSDLPorts_INTERFACE_DEFINED__

/* interface IEnumWSDLPorts */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IEnumWSDLPorts;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ec189c1c-31b3-4193-bdca-98ec44ff3ee0")
    IEnumWSDLPorts : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Next( 
            long par_celt,
            /* [out] */ IWSDLPort **WSDLPort,
            long *par_Fetched) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Skip( 
            long par_celt) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumWSDLPorts **par_enum) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Find( 
            BSTR par_PortToFind,
            /* [out] */ IWSDLPort **par_IWSDLPort) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumWSDLPortsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumWSDLPorts * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumWSDLPorts * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumWSDLPorts * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            IEnumWSDLPorts * This,
            long par_celt,
            /* [out] */ IWSDLPort **WSDLPort,
            long *par_Fetched);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumWSDLPorts * This,
            long par_celt);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumWSDLPorts * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumWSDLPorts * This,
            /* [out] */ IEnumWSDLPorts **par_enum);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Find )( 
            IEnumWSDLPorts * This,
            BSTR par_PortToFind,
            /* [out] */ IWSDLPort **par_IWSDLPort);
        
        END_INTERFACE
    } IEnumWSDLPortsVtbl;

    interface IEnumWSDLPorts
    {
        CONST_VTBL struct IEnumWSDLPortsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumWSDLPorts_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumWSDLPorts_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumWSDLPorts_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumWSDLPorts_Next(This,par_celt,WSDLPort,par_Fetched)	\
    (This)->lpVtbl -> Next(This,par_celt,WSDLPort,par_Fetched)

#define IEnumWSDLPorts_Skip(This,par_celt)	\
    (This)->lpVtbl -> Skip(This,par_celt)

#define IEnumWSDLPorts_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumWSDLPorts_Clone(This,par_enum)	\
    (This)->lpVtbl -> Clone(This,par_enum)

#define IEnumWSDLPorts_Find(This,par_PortToFind,par_IWSDLPort)	\
    (This)->lpVtbl -> Find(This,par_PortToFind,par_IWSDLPort)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLPorts_Next_Proxy( 
    IEnumWSDLPorts * This,
    long par_celt,
    /* [out] */ IWSDLPort **WSDLPort,
    long *par_Fetched);


void __RPC_STUB IEnumWSDLPorts_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLPorts_Skip_Proxy( 
    IEnumWSDLPorts * This,
    long par_celt);


void __RPC_STUB IEnumWSDLPorts_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLPorts_Reset_Proxy( 
    IEnumWSDLPorts * This);


void __RPC_STUB IEnumWSDLPorts_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLPorts_Clone_Proxy( 
    IEnumWSDLPorts * This,
    /* [out] */ IEnumWSDLPorts **par_enum);


void __RPC_STUB IEnumWSDLPorts_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLPorts_Find_Proxy( 
    IEnumWSDLPorts * This,
    BSTR par_PortToFind,
    /* [out] */ IWSDLPort **par_IWSDLPort);


void __RPC_STUB IEnumWSDLPorts_Find_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumWSDLPorts_INTERFACE_DEFINED__ */


#ifndef __IWSDLService_INTERFACE_DEFINED__
#define __IWSDLService_INTERFACE_DEFINED__

/* interface IWSDLService */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IWSDLService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9b5d8d63-ea54-41f6-9f12-f77a13111ec6")
    IWSDLService : public IUnknown
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *par_ServiceName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Documentation( 
            /* [retval][out] */ BSTR *par_Documentation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSoapPorts( 
            /* [out] */ IEnumWSDLPorts **par_IWSDLPorts) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWSDLServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWSDLService * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWSDLService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWSDLService * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IWSDLService * This,
            /* [retval][out] */ BSTR *par_ServiceName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Documentation )( 
            IWSDLService * This,
            /* [retval][out] */ BSTR *par_Documentation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSoapPorts )( 
            IWSDLService * This,
            /* [out] */ IEnumWSDLPorts **par_IWSDLPorts);
        
        END_INTERFACE
    } IWSDLServiceVtbl;

    interface IWSDLService
    {
        CONST_VTBL struct IWSDLServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWSDLService_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWSDLService_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWSDLService_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWSDLService_get_Name(This,par_ServiceName)	\
    (This)->lpVtbl -> get_Name(This,par_ServiceName)

#define IWSDLService_get_Documentation(This,par_Documentation)	\
    (This)->lpVtbl -> get_Documentation(This,par_Documentation)

#define IWSDLService_GetSoapPorts(This,par_IWSDLPorts)	\
    (This)->lpVtbl -> GetSoapPorts(This,par_IWSDLPorts)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLService_get_Name_Proxy( 
    IWSDLService * This,
    /* [retval][out] */ BSTR *par_ServiceName);


void __RPC_STUB IWSDLService_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLService_get_Documentation_Proxy( 
    IWSDLService * This,
    /* [retval][out] */ BSTR *par_Documentation);


void __RPC_STUB IWSDLService_get_Documentation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLService_GetSoapPorts_Proxy( 
    IWSDLService * This,
    /* [out] */ IEnumWSDLPorts **par_IWSDLPorts);


void __RPC_STUB IWSDLService_GetSoapPorts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWSDLService_INTERFACE_DEFINED__ */


#ifndef __IEnumWSDLService_INTERFACE_DEFINED__
#define __IEnumWSDLService_INTERFACE_DEFINED__

/* interface IEnumWSDLService */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IEnumWSDLService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("104f6816-093e-41d7-a68b-8e1cc408b279")
    IEnumWSDLService : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Next( 
            long par_celt,
            /* [out] */ IWSDLService **par_IWSDLService,
            long *par_Fetched) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Skip( 
            long par_celt) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumWSDLService **par_enum) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Find( 
            BSTR par_ServiceToFind,
            /* [out] */ IWSDLService **par_WSDLService) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumWSDLServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumWSDLService * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumWSDLService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumWSDLService * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            IEnumWSDLService * This,
            long par_celt,
            /* [out] */ IWSDLService **par_IWSDLService,
            long *par_Fetched);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumWSDLService * This,
            long par_celt);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumWSDLService * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumWSDLService * This,
            /* [out] */ IEnumWSDLService **par_enum);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Find )( 
            IEnumWSDLService * This,
            BSTR par_ServiceToFind,
            /* [out] */ IWSDLService **par_WSDLService);
        
        END_INTERFACE
    } IEnumWSDLServiceVtbl;

    interface IEnumWSDLService
    {
        CONST_VTBL struct IEnumWSDLServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumWSDLService_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumWSDLService_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumWSDLService_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumWSDLService_Next(This,par_celt,par_IWSDLService,par_Fetched)	\
    (This)->lpVtbl -> Next(This,par_celt,par_IWSDLService,par_Fetched)

#define IEnumWSDLService_Skip(This,par_celt)	\
    (This)->lpVtbl -> Skip(This,par_celt)

#define IEnumWSDLService_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumWSDLService_Clone(This,par_enum)	\
    (This)->lpVtbl -> Clone(This,par_enum)

#define IEnumWSDLService_Find(This,par_ServiceToFind,par_WSDLService)	\
    (This)->lpVtbl -> Find(This,par_ServiceToFind,par_WSDLService)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLService_Next_Proxy( 
    IEnumWSDLService * This,
    long par_celt,
    /* [out] */ IWSDLService **par_IWSDLService,
    long *par_Fetched);


void __RPC_STUB IEnumWSDLService_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLService_Skip_Proxy( 
    IEnumWSDLService * This,
    long par_celt);


void __RPC_STUB IEnumWSDLService_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLService_Reset_Proxy( 
    IEnumWSDLService * This);


void __RPC_STUB IEnumWSDLService_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLService_Clone_Proxy( 
    IEnumWSDLService * This,
    /* [out] */ IEnumWSDLService **par_enum);


void __RPC_STUB IEnumWSDLService_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IEnumWSDLService_Find_Proxy( 
    IEnumWSDLService * This,
    BSTR par_ServiceToFind,
    /* [out] */ IWSDLService **par_WSDLService);


void __RPC_STUB IEnumWSDLService_Find_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumWSDLService_INTERFACE_DEFINED__ */


#ifndef __IWSDLReader_INTERFACE_DEFINED__
#define __IWSDLReader_INTERFACE_DEFINED__

/* interface IWSDLReader */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IWSDLReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("de523fd4-afb8-4643-ba90-9deb3c7fb4a3")
    IWSDLReader : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Load( 
            BSTR par_WSDLFile,
            BSTR par_WSMLFile) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Load2( 
            VARIANT par_WSDLFile,
            VARIANT par_WSMLFile,
            BSTR par_StartingNamespace) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSoapServices( 
            /* [out] */ IEnumWSDLService **par_IWSDLServiceEnum) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ParseRequest( 
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [out] */ IWSDLPort **par_IWSDLPort,
            /* [out] */ IWSDLOperation **par_IWSDLOperation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropValue) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_TypeFactory( 
            /* [retval][out] */ ISoapTypeMapperFactory **par_Factory) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWSDLReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWSDLReader * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWSDLReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWSDLReader * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IWSDLReader * This,
            BSTR par_WSDLFile,
            BSTR par_WSMLFile);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load2 )( 
            IWSDLReader * This,
            VARIANT par_WSDLFile,
            VARIANT par_WSMLFile,
            BSTR par_StartingNamespace);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSoapServices )( 
            IWSDLReader * This,
            /* [out] */ IEnumWSDLService **par_IWSDLServiceEnum);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ParseRequest )( 
            IWSDLReader * This,
            /* [in] */ ISoapReader *par_ISoapReader,
            /* [out] */ IWSDLPort **par_IWSDLPort,
            /* [out] */ IWSDLOperation **par_IWSDLOperation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetProperty )( 
            IWSDLReader * This,
            /* [in] */ BSTR par_PropertyName,
            /* [in] */ VARIANT par_PropValue);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TypeFactory )( 
            IWSDLReader * This,
            /* [retval][out] */ ISoapTypeMapperFactory **par_Factory);
        
        END_INTERFACE
    } IWSDLReaderVtbl;

    interface IWSDLReader
    {
        CONST_VTBL struct IWSDLReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWSDLReader_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWSDLReader_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWSDLReader_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWSDLReader_Load(This,par_WSDLFile,par_WSMLFile)	\
    (This)->lpVtbl -> Load(This,par_WSDLFile,par_WSMLFile)

#define IWSDLReader_Load2(This,par_WSDLFile,par_WSMLFile,par_StartingNamespace)	\
    (This)->lpVtbl -> Load2(This,par_WSDLFile,par_WSMLFile,par_StartingNamespace)

#define IWSDLReader_GetSoapServices(This,par_IWSDLServiceEnum)	\
    (This)->lpVtbl -> GetSoapServices(This,par_IWSDLServiceEnum)

#define IWSDLReader_ParseRequest(This,par_ISoapReader,par_IWSDLPort,par_IWSDLOperation)	\
    (This)->lpVtbl -> ParseRequest(This,par_ISoapReader,par_IWSDLPort,par_IWSDLOperation)

#define IWSDLReader_SetProperty(This,par_PropertyName,par_PropValue)	\
    (This)->lpVtbl -> SetProperty(This,par_PropertyName,par_PropValue)

#define IWSDLReader_get_TypeFactory(This,par_Factory)	\
    (This)->lpVtbl -> get_TypeFactory(This,par_Factory)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLReader_Load_Proxy( 
    IWSDLReader * This,
    BSTR par_WSDLFile,
    BSTR par_WSMLFile);


void __RPC_STUB IWSDLReader_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLReader_Load2_Proxy( 
    IWSDLReader * This,
    VARIANT par_WSDLFile,
    VARIANT par_WSMLFile,
    BSTR par_StartingNamespace);


void __RPC_STUB IWSDLReader_Load2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLReader_GetSoapServices_Proxy( 
    IWSDLReader * This,
    /* [out] */ IEnumWSDLService **par_IWSDLServiceEnum);


void __RPC_STUB IWSDLReader_GetSoapServices_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLReader_ParseRequest_Proxy( 
    IWSDLReader * This,
    /* [in] */ ISoapReader *par_ISoapReader,
    /* [out] */ IWSDLPort **par_IWSDLPort,
    /* [out] */ IWSDLOperation **par_IWSDLOperation);


void __RPC_STUB IWSDLReader_ParseRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWSDLReader_SetProperty_Proxy( 
    IWSDLReader * This,
    /* [in] */ BSTR par_PropertyName,
    /* [in] */ VARIANT par_PropValue);


void __RPC_STUB IWSDLReader_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IWSDLReader_get_TypeFactory_Proxy( 
    IWSDLReader * This,
    /* [retval][out] */ ISoapTypeMapperFactory **par_Factory);


void __RPC_STUB IWSDLReader_get_TypeFactory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWSDLReader_INTERFACE_DEFINED__ */


#ifndef __IWSDLBinding_INTERFACE_DEFINED__
#define __IWSDLBinding_INTERFACE_DEFINED__

/* interface IWSDLBinding */
/* [hidden][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IWSDLBinding;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AB0E0268-304D-43FC-8603-B1105F3A7512")
    IWSDLBinding : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            /* external definition not present */ IXMLDOMNode *pWSMLBindingNode,
            BSTR *pbstrNamespace) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ParseBinding( 
            /* external definition not present */ IXMLDOMNode *pWSDLInputOutputNode,
            /* external definition not present */ IXMLDOMNode **ppSoapBodyNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ApplyBinding( 
            IWSDLMessage *pWSDLMessage,
            /* external definition not present */ IXMLDOMNode *pWSDLInputOutputNode,
            /* external definition not present */ IXMLDOMNode *pWSMLOperationNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetComposer( 
            BSTR bstrTemporaryAttachmentFolder,
            long lMaxAttachmentSize,
            long lReserved,
            IMessageComposer **ppComposer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParser( 
            BSTR bstrTemporaryAttachmentFolder,
            long lMaxAttachmentSize,
            IMessageParser **ppParser) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWSDLBindingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWSDLBinding * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWSDLBinding * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWSDLBinding * This);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IWSDLBinding * This,
            /* external definition not present */ IXMLDOMNode *pWSMLBindingNode,
            BSTR *pbstrNamespace);
        
        HRESULT ( STDMETHODCALLTYPE *ParseBinding )( 
            IWSDLBinding * This,
            /* external definition not present */ IXMLDOMNode *pWSDLInputOutputNode,
            /* external definition not present */ IXMLDOMNode **ppSoapBodyNode);
        
        HRESULT ( STDMETHODCALLTYPE *ApplyBinding )( 
            IWSDLBinding * This,
            IWSDLMessage *pWSDLMessage,
            /* external definition not present */ IXMLDOMNode *pWSDLInputOutputNode,
            /* external definition not present */ IXMLDOMNode *pWSMLOperationNode);
        
        HRESULT ( STDMETHODCALLTYPE *GetComposer )( 
            IWSDLBinding * This,
            BSTR bstrTemporaryAttachmentFolder,
            long lMaxAttachmentSize,
            long lReserved,
            IMessageComposer **ppComposer);
        
        HRESULT ( STDMETHODCALLTYPE *GetParser )( 
            IWSDLBinding * This,
            BSTR bstrTemporaryAttachmentFolder,
            long lMaxAttachmentSize,
            IMessageParser **ppParser);
        
        END_INTERFACE
    } IWSDLBindingVtbl;

    interface IWSDLBinding
    {
        CONST_VTBL struct IWSDLBindingVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWSDLBinding_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWSDLBinding_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWSDLBinding_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWSDLBinding_Initialize(This,pWSMLBindingNode,pbstrNamespace)	\
    (This)->lpVtbl -> Initialize(This,pWSMLBindingNode,pbstrNamespace)

#define IWSDLBinding_ParseBinding(This,pWSDLInputOutputNode,ppSoapBodyNode)	\
    (This)->lpVtbl -> ParseBinding(This,pWSDLInputOutputNode,ppSoapBodyNode)

#define IWSDLBinding_ApplyBinding(This,pWSDLMessage,pWSDLInputOutputNode,pWSMLOperationNode)	\
    (This)->lpVtbl -> ApplyBinding(This,pWSDLMessage,pWSDLInputOutputNode,pWSMLOperationNode)

#define IWSDLBinding_GetComposer(This,bstrTemporaryAttachmentFolder,lMaxAttachmentSize,lReserved,ppComposer)	\
    (This)->lpVtbl -> GetComposer(This,bstrTemporaryAttachmentFolder,lMaxAttachmentSize,lReserved,ppComposer)

#define IWSDLBinding_GetParser(This,bstrTemporaryAttachmentFolder,lMaxAttachmentSize,ppParser)	\
    (This)->lpVtbl -> GetParser(This,bstrTemporaryAttachmentFolder,lMaxAttachmentSize,ppParser)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWSDLBinding_Initialize_Proxy( 
    IWSDLBinding * This,
    /* external definition not present */ IXMLDOMNode *pWSMLBindingNode,
    BSTR *pbstrNamespace);


void __RPC_STUB IWSDLBinding_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWSDLBinding_ParseBinding_Proxy( 
    IWSDLBinding * This,
    /* external definition not present */ IXMLDOMNode *pWSDLInputOutputNode,
    /* external definition not present */ IXMLDOMNode **ppSoapBodyNode);


void __RPC_STUB IWSDLBinding_ParseBinding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWSDLBinding_ApplyBinding_Proxy( 
    IWSDLBinding * This,
    IWSDLMessage *pWSDLMessage,
    /* external definition not present */ IXMLDOMNode *pWSDLInputOutputNode,
    /* external definition not present */ IXMLDOMNode *pWSMLOperationNode);


void __RPC_STUB IWSDLBinding_ApplyBinding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWSDLBinding_GetComposer_Proxy( 
    IWSDLBinding * This,
    BSTR bstrTemporaryAttachmentFolder,
    long lMaxAttachmentSize,
    long lReserved,
    IMessageComposer **ppComposer);


void __RPC_STUB IWSDLBinding_GetComposer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWSDLBinding_GetParser_Proxy( 
    IWSDLBinding * This,
    BSTR bstrTemporaryAttachmentFolder,
    long lMaxAttachmentSize,
    IMessageParser **ppParser);


void __RPC_STUB IWSDLBinding_GetParser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWSDLBinding_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_WSDLReader30;

#ifdef __cplusplus

class DECLSPEC_UUID("EF90A70C-925B-11D5-87EA-00B0D0BE6479")
WSDLReader30;
#endif

EXTERN_C const CLSID CLSID_SoapTypeMapperFactory30;

#ifdef __cplusplus

class DECLSPEC_UUID("EF90A715-925B-11D5-87EA-00B0D0BE6479")
SoapTypeMapperFactory30;
#endif

EXTERN_C const CLSID CLSID_GenericCustomTypeMapper30;

#ifdef __cplusplus

class DECLSPEC_UUID("EF90A716-925B-11D5-87EA-00B0D0BE6479")
GenericCustomTypeMapper30;
#endif

EXTERN_C const CLSID CLSID_UDTMapper30;

#ifdef __cplusplus

class DECLSPEC_UUID("8BCD9554-86C7-435d-A8C8-BCB3C72FBEE9")
UDTMapper30;
#endif

#ifndef __ISoapConnector_INTERFACE_DEFINED__
#define __ISoapConnector_INTERFACE_DEFINED__

/* interface ISoapConnector */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapConnector;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0AF40C4E-9257-11D5-87EA-00B0D0BE6479")
    ISoapConnector : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InputStream( 
            /* [retval][out] */ IStream **par_input) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OutputStream( 
            /* [retval][out] */ IStream **par_output) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConnectWSDL( 
            /* [in] */ IWSDLPort *par_port) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE BeginMessageWSDL( 
            /* [in] */ IWSDLOperation *par_operation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EndMessage( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE BeginMessage( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapConnectorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapConnector * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapConnector * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapConnector * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapConnector * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapConnector * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapConnector * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapConnector * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InputStream )( 
            ISoapConnector * This,
            /* [retval][out] */ IStream **par_input);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OutputStream )( 
            ISoapConnector * This,
            /* [retval][out] */ IStream **par_output);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            ISoapConnector * This,
            /* [in] */ BSTR par_name,
            /* [retval][out] */ VARIANT *par_value);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            ISoapConnector * This,
            /* [in] */ BSTR par_name,
            /* [in] */ VARIANT par_value);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectWSDL )( 
            ISoapConnector * This,
            /* [in] */ IWSDLPort *par_port);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *BeginMessageWSDL )( 
            ISoapConnector * This,
            /* [in] */ IWSDLOperation *par_operation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EndMessage )( 
            ISoapConnector * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            ISoapConnector * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ISoapConnector * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *BeginMessage )( 
            ISoapConnector * This);
        
        END_INTERFACE
    } ISoapConnectorVtbl;

    interface ISoapConnector
    {
        CONST_VTBL struct ISoapConnectorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapConnector_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapConnector_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapConnector_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapConnector_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapConnector_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapConnector_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapConnector_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapConnector_get_InputStream(This,par_input)	\
    (This)->lpVtbl -> get_InputStream(This,par_input)

#define ISoapConnector_get_OutputStream(This,par_output)	\
    (This)->lpVtbl -> get_OutputStream(This,par_output)

#define ISoapConnector_get_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> get_Property(This,par_name,par_value)

#define ISoapConnector_put_Property(This,par_name,par_value)	\
    (This)->lpVtbl -> put_Property(This,par_name,par_value)

#define ISoapConnector_ConnectWSDL(This,par_port)	\
    (This)->lpVtbl -> ConnectWSDL(This,par_port)

#define ISoapConnector_BeginMessageWSDL(This,par_operation)	\
    (This)->lpVtbl -> BeginMessageWSDL(This,par_operation)

#define ISoapConnector_EndMessage(This)	\
    (This)->lpVtbl -> EndMessage(This)

#define ISoapConnector_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define ISoapConnector_Connect(This)	\
    (This)->lpVtbl -> Connect(This)

#define ISoapConnector_BeginMessage(This)	\
    (This)->lpVtbl -> BeginMessage(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapConnector_get_InputStream_Proxy( 
    ISoapConnector * This,
    /* [retval][out] */ IStream **par_input);


void __RPC_STUB ISoapConnector_get_InputStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapConnector_get_OutputStream_Proxy( 
    ISoapConnector * This,
    /* [retval][out] */ IStream **par_output);


void __RPC_STUB ISoapConnector_get_OutputStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapConnector_get_Property_Proxy( 
    ISoapConnector * This,
    /* [in] */ BSTR par_name,
    /* [retval][out] */ VARIANT *par_value);


void __RPC_STUB ISoapConnector_get_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISoapConnector_put_Property_Proxy( 
    ISoapConnector * This,
    /* [in] */ BSTR par_name,
    /* [in] */ VARIANT par_value);


void __RPC_STUB ISoapConnector_put_Property_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapConnector_ConnectWSDL_Proxy( 
    ISoapConnector * This,
    /* [in] */ IWSDLPort *par_port);


void __RPC_STUB ISoapConnector_ConnectWSDL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapConnector_BeginMessageWSDL_Proxy( 
    ISoapConnector * This,
    /* [in] */ IWSDLOperation *par_operation);


void __RPC_STUB ISoapConnector_BeginMessageWSDL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapConnector_EndMessage_Proxy( 
    ISoapConnector * This);


void __RPC_STUB ISoapConnector_EndMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapConnector_Reset_Proxy( 
    ISoapConnector * This);


void __RPC_STUB ISoapConnector_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapConnector_Connect_Proxy( 
    ISoapConnector * This);


void __RPC_STUB ISoapConnector_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapConnector_BeginMessage_Proxy( 
    ISoapConnector * This);


void __RPC_STUB ISoapConnector_BeginMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapConnector_INTERFACE_DEFINED__ */


#ifndef __ISoapConnectorFactory_INTERFACE_DEFINED__
#define __ISoapConnectorFactory_INTERFACE_DEFINED__

/* interface ISoapConnectorFactory */
/* [dual][helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapConnectorFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0AF40C50-9257-11D5-87EA-00B0D0BE6479")
    ISoapConnectorFactory : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreatePortConnector( 
            /* [in] */ IWSDLPort *par_port,
            /* [retval][out] */ ISoapConnector **par_connector) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapConnectorFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapConnectorFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapConnectorFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapConnectorFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoapConnectorFactory * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoapConnectorFactory * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoapConnectorFactory * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoapConnectorFactory * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreatePortConnector )( 
            ISoapConnectorFactory * This,
            /* [in] */ IWSDLPort *par_port,
            /* [retval][out] */ ISoapConnector **par_connector);
        
        END_INTERFACE
    } ISoapConnectorFactoryVtbl;

    interface ISoapConnectorFactory
    {
        CONST_VTBL struct ISoapConnectorFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapConnectorFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapConnectorFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapConnectorFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapConnectorFactory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISoapConnectorFactory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISoapConnectorFactory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISoapConnectorFactory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISoapConnectorFactory_CreatePortConnector(This,par_port,par_connector)	\
    (This)->lpVtbl -> CreatePortConnector(This,par_port,par_connector)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISoapConnectorFactory_CreatePortConnector_Proxy( 
    ISoapConnectorFactory * This,
    /* [in] */ IWSDLPort *par_port,
    /* [retval][out] */ ISoapConnector **par_connector);


void __RPC_STUB ISoapConnectorFactory_CreatePortConnector_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapConnectorFactory_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SoapConnector30;

#ifdef __cplusplus

class DECLSPEC_UUID("0AF40C52-9257-11D5-87EA-00B0D0BE6479")
SoapConnector30;
#endif

EXTERN_C const CLSID CLSID_SoapConnectorFactory30;

#ifdef __cplusplus

class DECLSPEC_UUID("0AF40C58-9257-11D5-87EA-00B0D0BE6479")
SoapConnectorFactory30;
#endif

EXTERN_C const CLSID CLSID_HttpConnector30;

#ifdef __cplusplus

class DECLSPEC_UUID("0AF40C53-9257-11D5-87EA-00B0D0BE6479")
HttpConnector30;
#endif

#ifndef __ISoapError_INTERFACE_DEFINED__
#define __ISoapError_INTERFACE_DEFINED__

/* interface ISoapError */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapError;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7F017F94-9257-11D5-87EA-00B0D0BE6479")
    ISoapError : public IUnknown
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultCode( 
            /* [retval][out] */ BSTR *par_FaultCode) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultString( 
            /* [retval][out] */ BSTR *par_Faultstring) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultActor( 
            /* [retval][out] */ BSTR *par_Actor) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Detail( 
            /* [retval][out] */ BSTR *par_Detail) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FaultCodeNamespace( 
            /* [retval][out] */ BSTR *par_Namespace) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapErrorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapError * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapError * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapError * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultCode )( 
            ISoapError * This,
            /* [retval][out] */ BSTR *par_FaultCode);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultString )( 
            ISoapError * This,
            /* [retval][out] */ BSTR *par_Faultstring);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultActor )( 
            ISoapError * This,
            /* [retval][out] */ BSTR *par_Actor);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Detail )( 
            ISoapError * This,
            /* [retval][out] */ BSTR *par_Detail);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FaultCodeNamespace )( 
            ISoapError * This,
            /* [retval][out] */ BSTR *par_Namespace);
        
        END_INTERFACE
    } ISoapErrorVtbl;

    interface ISoapError
    {
        CONST_VTBL struct ISoapErrorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapError_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapError_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapError_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapError_get_FaultCode(This,par_FaultCode)	\
    (This)->lpVtbl -> get_FaultCode(This,par_FaultCode)

#define ISoapError_get_FaultString(This,par_Faultstring)	\
    (This)->lpVtbl -> get_FaultString(This,par_Faultstring)

#define ISoapError_get_FaultActor(This,par_Actor)	\
    (This)->lpVtbl -> get_FaultActor(This,par_Actor)

#define ISoapError_get_Detail(This,par_Detail)	\
    (This)->lpVtbl -> get_Detail(This,par_Detail)

#define ISoapError_get_FaultCodeNamespace(This,par_Namespace)	\
    (This)->lpVtbl -> get_FaultCodeNamespace(This,par_Namespace)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapError_get_FaultCode_Proxy( 
    ISoapError * This,
    /* [retval][out] */ BSTR *par_FaultCode);


void __RPC_STUB ISoapError_get_FaultCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapError_get_FaultString_Proxy( 
    ISoapError * This,
    /* [retval][out] */ BSTR *par_Faultstring);


void __RPC_STUB ISoapError_get_FaultString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapError_get_FaultActor_Proxy( 
    ISoapError * This,
    /* [retval][out] */ BSTR *par_Actor);


void __RPC_STUB ISoapError_get_FaultActor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapError_get_Detail_Proxy( 
    ISoapError * This,
    /* [retval][out] */ BSTR *par_Detail);


void __RPC_STUB ISoapError_get_Detail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISoapError_get_FaultCodeNamespace_Proxy( 
    ISoapError * This,
    /* [retval][out] */ BSTR *par_Namespace);


void __RPC_STUB ISoapError_get_FaultCodeNamespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapError_INTERFACE_DEFINED__ */


#ifndef __ISoapErrorInfo_INTERFACE_DEFINED__
#define __ISoapErrorInfo_INTERFACE_DEFINED__

/* interface ISoapErrorInfo */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_ISoapErrorInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C0871607-8C99-4824-92CD-85CBD4C7273F")
    ISoapErrorInfo : public IErrorInfo
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetActor( 
            /* [in] */ BSTR par_Actor) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetFaultCode( 
            BSTR par_FaultCode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddErrorEntry( 
            /* [in] */ BSTR par_Description,
            /* [in] */ BSTR par_Component,
            /* [in] */ HRESULT par_ErrorCode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddSoapError( 
            /* [in] */ BSTR par_FaultString,
            /* [in] */ BSTR par_FaultActor,
            /* [in] */ BSTR par_Detail,
            /* [in] */ BSTR par_FaultCode,
            /* [in] */ BSTR par_Namespace) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddErrorInfo( 
            /* [in] */ BSTR par_Description,
            /* [in] */ BSTR par_Source,
            /* [in] */ BSTR par_Helpfile,
            /* [in] */ DWORD par_HelpContext,
            /* [in] */ HRESULT par_hrFromErrorInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LoadFault( 
            /* [in] */ /* external definition not present */ IXMLDOMDocument *par_Document) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetHresult( 
            /* [out] */ HRESULT *par_HR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetErrorEntry( 
            /* [in] */ DWORD par_EntryID,
            /* [out] */ BSTR *par_Description,
            /* [out] */ BSTR *par_Component,
            /* [out] */ HRESULT *par_HR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetActor( 
            /* [out] */ BSTR *par_Actor) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetErrorInfo( 
            /* [in] */ BSTR *par_Description,
            /* [in] */ BSTR *par_Source,
            /* [in] */ BSTR *par_Helpfile,
            /* [in] */ DWORD *par_HelpContext,
            /* [in] */ HRESULT *par_hrFromErrorInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSoapError( 
            /* [in] */ BSTR *par_FaultString,
            /* [in] */ BSTR *par_FaultActor,
            /* [in] */ BSTR *par_Detail,
            /* [in] */ BSTR *par_FaultCode,
            /* [in] */ BSTR *par_Namespace) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoapErrorInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoapErrorInfo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoapErrorInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoapErrorInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            ISoapErrorInfo * This,
            /* [out] */ GUID *pGUID);
        
        HRESULT ( STDMETHODCALLTYPE *GetSource )( 
            ISoapErrorInfo * This,
            /* [out] */ BSTR *pBstrSource);
        
        HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            ISoapErrorInfo * This,
            /* [out] */ BSTR *pBstrDescription);
        
        HRESULT ( STDMETHODCALLTYPE *GetHelpFile )( 
            ISoapErrorInfo * This,
            /* [out] */ BSTR *pBstrHelpFile);
        
        HRESULT ( STDMETHODCALLTYPE *GetHelpContext )( 
            ISoapErrorInfo * This,
            /* [out] */ DWORD *pdwHelpContext);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetActor )( 
            ISoapErrorInfo * This,
            /* [in] */ BSTR par_Actor);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFaultCode )( 
            ISoapErrorInfo * This,
            BSTR par_FaultCode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddErrorEntry )( 
            ISoapErrorInfo * This,
            /* [in] */ BSTR par_Description,
            /* [in] */ BSTR par_Component,
            /* [in] */ HRESULT par_ErrorCode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddSoapError )( 
            ISoapErrorInfo * This,
            /* [in] */ BSTR par_FaultString,
            /* [in] */ BSTR par_FaultActor,
            /* [in] */ BSTR par_Detail,
            /* [in] */ BSTR par_FaultCode,
            /* [in] */ BSTR par_Namespace);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddErrorInfo )( 
            ISoapErrorInfo * This,
            /* [in] */ BSTR par_Description,
            /* [in] */ BSTR par_Source,
            /* [in] */ BSTR par_Helpfile,
            /* [in] */ DWORD par_HelpContext,
            /* [in] */ HRESULT par_hrFromErrorInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *LoadFault )( 
            ISoapErrorInfo * This,
            /* [in] */ /* external definition not present */ IXMLDOMDocument *par_Document);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHresult )( 
            ISoapErrorInfo * This,
            /* [out] */ HRESULT *par_HR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetErrorEntry )( 
            ISoapErrorInfo * This,
            /* [in] */ DWORD par_EntryID,
            /* [out] */ BSTR *par_Description,
            /* [out] */ BSTR *par_Component,
            /* [out] */ HRESULT *par_HR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetActor )( 
            ISoapErrorInfo * This,
            /* [out] */ BSTR *par_Actor);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetErrorInfo )( 
            ISoapErrorInfo * This,
            /* [in] */ BSTR *par_Description,
            /* [in] */ BSTR *par_Source,
            /* [in] */ BSTR *par_Helpfile,
            /* [in] */ DWORD *par_HelpContext,
            /* [in] */ HRESULT *par_hrFromErrorInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSoapError )( 
            ISoapErrorInfo * This,
            /* [in] */ BSTR *par_FaultString,
            /* [in] */ BSTR *par_FaultActor,
            /* [in] */ BSTR *par_Detail,
            /* [in] */ BSTR *par_FaultCode,
            /* [in] */ BSTR *par_Namespace);
        
        END_INTERFACE
    } ISoapErrorInfoVtbl;

    interface ISoapErrorInfo
    {
        CONST_VTBL struct ISoapErrorInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoapErrorInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISoapErrorInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISoapErrorInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISoapErrorInfo_GetGUID(This,pGUID)	\
    (This)->lpVtbl -> GetGUID(This,pGUID)

#define ISoapErrorInfo_GetSource(This,pBstrSource)	\
    (This)->lpVtbl -> GetSource(This,pBstrSource)

#define ISoapErrorInfo_GetDescription(This,pBstrDescription)	\
    (This)->lpVtbl -> GetDescription(This,pBstrDescription)

#define ISoapErrorInfo_GetHelpFile(This,pBstrHelpFile)	\
    (This)->lpVtbl -> GetHelpFile(This,pBstrHelpFile)

#define ISoapErrorInfo_GetHelpContext(This,pdwHelpContext)	\
    (This)->lpVtbl -> GetHelpContext(This,pdwHelpContext)


#define ISoapErrorInfo_SetActor(This,par_Actor)	\
    (This)->lpVtbl -> SetActor(This,par_Actor)

#define ISoapErrorInfo_SetFaultCode(This,par_FaultCode)	\
    (This)->lpVtbl -> SetFaultCode(This,par_FaultCode)

#define ISoapErrorInfo_AddErrorEntry(This,par_Description,par_Component,par_ErrorCode)	\
    (This)->lpVtbl -> AddErrorEntry(This,par_Description,par_Component,par_ErrorCode)

#define ISoapErrorInfo_AddSoapError(This,par_FaultString,par_FaultActor,par_Detail,par_FaultCode,par_Namespace)	\
    (This)->lpVtbl -> AddSoapError(This,par_FaultString,par_FaultActor,par_Detail,par_FaultCode,par_Namespace)

#define ISoapErrorInfo_AddErrorInfo(This,par_Description,par_Source,par_Helpfile,par_HelpContext,par_hrFromErrorInfo)	\
    (This)->lpVtbl -> AddErrorInfo(This,par_Description,par_Source,par_Helpfile,par_HelpContext,par_hrFromErrorInfo)

#define ISoapErrorInfo_LoadFault(This,par_Document)	\
    (This)->lpVtbl -> LoadFault(This,par_Document)

#define ISoapErrorInfo_GetHresult(This,par_HR)	\
    (This)->lpVtbl -> GetHresult(This,par_HR)

#define ISoapErrorInfo_GetErrorEntry(This,par_EntryID,par_Description,par_Component,par_HR)	\
    (This)->lpVtbl -> GetErrorEntry(This,par_EntryID,par_Description,par_Component,par_HR)

#define ISoapErrorInfo_GetActor(This,par_Actor)	\
    (This)->lpVtbl -> GetActor(This,par_Actor)

#define ISoapErrorInfo_GetErrorInfo(This,par_Description,par_Source,par_Helpfile,par_HelpContext,par_hrFromErrorInfo)	\
    (This)->lpVtbl -> GetErrorInfo(This,par_Description,par_Source,par_Helpfile,par_HelpContext,par_hrFromErrorInfo)

#define ISoapErrorInfo_GetSoapError(This,par_FaultString,par_FaultActor,par_Detail,par_FaultCode,par_Namespace)	\
    (This)->lpVtbl -> GetSoapError(This,par_FaultString,par_FaultActor,par_Detail,par_FaultCode,par_Namespace)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_SetActor_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ BSTR par_Actor);


void __RPC_STUB ISoapErrorInfo_SetActor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_SetFaultCode_Proxy( 
    ISoapErrorInfo * This,
    BSTR par_FaultCode);


void __RPC_STUB ISoapErrorInfo_SetFaultCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_AddErrorEntry_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ BSTR par_Description,
    /* [in] */ BSTR par_Component,
    /* [in] */ HRESULT par_ErrorCode);


void __RPC_STUB ISoapErrorInfo_AddErrorEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_AddSoapError_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ BSTR par_FaultString,
    /* [in] */ BSTR par_FaultActor,
    /* [in] */ BSTR par_Detail,
    /* [in] */ BSTR par_FaultCode,
    /* [in] */ BSTR par_Namespace);


void __RPC_STUB ISoapErrorInfo_AddSoapError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_AddErrorInfo_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ BSTR par_Description,
    /* [in] */ BSTR par_Source,
    /* [in] */ BSTR par_Helpfile,
    /* [in] */ DWORD par_HelpContext,
    /* [in] */ HRESULT par_hrFromErrorInfo);


void __RPC_STUB ISoapErrorInfo_AddErrorInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_LoadFault_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ /* external definition not present */ IXMLDOMDocument *par_Document);


void __RPC_STUB ISoapErrorInfo_LoadFault_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_GetHresult_Proxy( 
    ISoapErrorInfo * This,
    /* [out] */ HRESULT *par_HR);


void __RPC_STUB ISoapErrorInfo_GetHresult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_GetErrorEntry_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ DWORD par_EntryID,
    /* [out] */ BSTR *par_Description,
    /* [out] */ BSTR *par_Component,
    /* [out] */ HRESULT *par_HR);


void __RPC_STUB ISoapErrorInfo_GetErrorEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_GetActor_Proxy( 
    ISoapErrorInfo * This,
    /* [out] */ BSTR *par_Actor);


void __RPC_STUB ISoapErrorInfo_GetActor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_GetErrorInfo_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ BSTR *par_Description,
    /* [in] */ BSTR *par_Source,
    /* [in] */ BSTR *par_Helpfile,
    /* [in] */ DWORD *par_HelpContext,
    /* [in] */ HRESULT *par_hrFromErrorInfo);


void __RPC_STUB ISoapErrorInfo_GetErrorInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISoapErrorInfo_GetSoapError_Proxy( 
    ISoapErrorInfo * This,
    /* [in] */ BSTR *par_FaultString,
    /* [in] */ BSTR *par_FaultActor,
    /* [in] */ BSTR *par_Detail,
    /* [in] */ BSTR *par_FaultCode,
    /* [in] */ BSTR *par_Namespace);


void __RPC_STUB ISoapErrorInfo_GetSoapError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISoapErrorInfo_INTERFACE_DEFINED__ */


#ifndef __IGCTMObjectFactory_INTERFACE_DEFINED__
#define __IGCTMObjectFactory_INTERFACE_DEFINED__

/* interface IGCTMObjectFactory */
/* [helpstring][unique][object][local][version][uuid][oleautomation] */ 


EXTERN_C const IID IID_IGCTMObjectFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3C87B8BE-F2B7-45c5-B34E-4A46A58A80B0")
    IGCTMObjectFactory : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateObject( 
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode,
            /* [retval][out] */ IDispatch **par_Object) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGCTMObjectFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGCTMObjectFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGCTMObjectFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGCTMObjectFactory * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CreateObject )( 
            IGCTMObjectFactory * This,
            /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode,
            /* [retval][out] */ IDispatch **par_Object);
        
        END_INTERFACE
    } IGCTMObjectFactoryVtbl;

    interface IGCTMObjectFactory
    {
        CONST_VTBL struct IGCTMObjectFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGCTMObjectFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGCTMObjectFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGCTMObjectFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGCTMObjectFactory_CreateObject(This,par_WSMLNode,par_Object)	\
    (This)->lpVtbl -> CreateObject(This,par_WSMLNode,par_Object)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IGCTMObjectFactory_CreateObject_Proxy( 
    IGCTMObjectFactory * This,
    /* [in] */ /* external definition not present */ IXMLDOMNode *par_WSMLNode,
    /* [retval][out] */ IDispatch **par_Object);


void __RPC_STUB IGCTMObjectFactory_CreateObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGCTMObjectFactory_INTERFACE_DEFINED__ */

#endif /* __MSSOAPLib30_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


