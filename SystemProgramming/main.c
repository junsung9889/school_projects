//-----------------------------------------------------------
//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

void indexBuilder(char* inputFileNm);
void indexPrinter(char* inputFileNm);

int main(int argc, char** argv) {
	int i;
	struct timeval start_t, end_t, result_t;

	if(argc < 3) {
		printf("Usage : %s [bible text file] [bible text file] ...  [output file]\n", argv[0]);
		return 1;
	}

	/*************************************
	 * Time Start
	 *************************************/
	gettimeofday(&start_t, NULL);
	
	for(i=0; i<argc-2; i++) {
		indexBuilder(argv[i+1]);
		indexPrinter(argv[argc-1]);
	}

	/*************************************
	 * Time End
	 *************************************/

	gettimeofday(&end_t, NULL);
	
	timersub(&end_t, &start_t, &result_t);
	
	printf("   Elapsed Time  : %ld(usec)\n",
			1000000*(result_t.tv_sec)+result_t.tv_usec);
	
	return 0;
}
