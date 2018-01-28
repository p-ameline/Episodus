/*
 * Test module functions. 
 * Note: this assumes that you have a specific module in a specific directory, 
 *with specific function names. Change it to suit your setup.
 */

#include "../include/gocr.h"
#include "../include/gocr_module.h"
#include "../include/gocr_char.h"

int main (int argc, char **argv) {
	int i;
	char *image = NULL;
	gocr_ModuleId m;

	if (gocr_init(argc, argv) != 0) {
		printf("Could not init gocr\n");
		exit(1);
	}

	gocr_setAttribute(VERBOSE, (void *)3);
	gocr_setAttribute(NO_BLOCK, (void *)GOCR_TRUE);
	
	if (argc < 2) {
		fprintf(stderr, "more args\n");
		exit(1);
	}
	/* modules */
	if ( (m = gocr_moduleLoad(argv[1])) == -1 ) {
		fprintf(stderr, "You should get the mdk tester at gocr's homepage.\n"
				"If you already have it, edit module.c and make sure the"
				"path is correct.\n");
		exit(2);
	}
	for (i = 2; i+1 < argc; i += 2)
		if (!strcmp(argv[i], "image"))
			image = argv[i+1];
		else if (!strcmp(argv[i], "filter"))
			gocr_functionAppend(argv[i+1], m, NULL);
		else if (!strcmp(argv[i], "block"))
			gocr_functionAppend(argv[i+1], m, NULL);
		else if (!strcmp(argv[i], "finder"))
			gocr_functionAppend(argv[i+1], m, NULL);
		else if (!strcmp(argv[i], "recog"))
			gocr_functionAppend(argv[i+1], m, NULL);
		else if (!strcmp(argv[i], "context"))
			gocr_functionAppend(argv[i+1], m, NULL);
		else if (!strcmp(argv[i], "output"))
			gocr_functionAppend(argv[i+1], m, NULL);
		else  {
			fprintf(stderr, "%s %s\n", argv[i], argv[i+1]);
			continue;
		}

	if (image == NULL)
		gocr_imageLoad("small.pbm", (void *)GOCR_BW);
	else
		gocr_imageLoad(image, (void *)GOCR_BW);
	
	gocr_runAllModules();

	gocr_mainImageWriteWithData("test.ppm");
	gocr_finalize();
	return 0;
}

