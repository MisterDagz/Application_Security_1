#include <stdio.h>
#include "dictionary.h"
int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Not enough or too many arguments, two expected (Document, Dictionary)\n");
		exit(0);
	}
	
	const char *text_file = argv[1];
	const char *dict_file = argv[2];
	hashmap_t hashtable[HASH_SIZE];
	
	load_dictionary(dict_file, hashtable);
	
	char *misspelled[MAX_MISSPELLED];
	FILE *fp = fopen(text_file, "r");
	
	int num_misspelled = check_words(fp, hashtable, misspelled);
	printf("%d Mistakes\n", num_misspelled);
	for(int i=0; i<num_misspelled; i++){
		printf("%s\n", misspelled[i]);
	}
	
	return 0;
}
