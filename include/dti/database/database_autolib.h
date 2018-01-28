//
//  database_autolib.h
//  
//  Copyright (c) 2004 Kristian Lippert, Danish Technological Institute, Informatics
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifdef _DEBUG
	# if defined(_MT)
		# if defined(_DLL)
			#pragma comment (lib, "database_mtddll.lib")
		#else
			#pragma comment (lib, "database_mtdlib.lib")
        #endif
	#else
		#pragma comment (lib, "database_stdlib.lib")
	#endif
#else
	# if defined(_MT)
		# if defined(_DLL)
			#pragma comment (lib, "database_mtrdll.lib")
		#else
			#pragma comment (lib, "database_mtrlib.lib")
        #endif
	#else
		#pragma comment (lib, "database_strlib.lib")
	#endif
#endif
