#ifndef __NS_INTERFACE_H#define __NS_INTERFACE_H
void _export nsmtoolsInformation(string far *psToolName, string far *psDescription, string far *psPseudoProtocolName) ;void _export nsmToolsAction(TWindow far *pere, NSContexte far *pCtx, bool far *psIsSuccess, string far *psActionName, string far *psActionResult, NSVectPatPathoArray far *pPptResult) ;
#define docsSeparator  "|$|"
#define filesSeparator '|'
#define blockOpener    '['
#define blockCloser    ']'

#endif

