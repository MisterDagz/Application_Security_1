#include "dictionary.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]){
	
	int num_misspelled = 0;
	char *line;
	size_t len = 0;
	ssize_t read;
	while(getline(&line, &len, fp) != -1){
		char *word = NULL;
		word = strtok(line, " ");
		while(word != NULL){

			if(word[strlen(word)-1] == '\n'){
				word[strlen(word)-1] = '\0';
			}

			if(word[strlen(word)-1] == '.' || word[strlen(word)-1] == '!' || word[strlen(word)-1] == '?' || word[strlen(word)-1] == ',' || word[strlen(word)-1] == ';'){
				word[strlen(word)-1] = '\0';
			}
			
			int bucket = hash_function(word);
			if(strlen(word) < LENGTH+1){
	  			if(check_word(word, hashtable) == false){
					char *new_word = NULL;
					new_word = malloc(LENGTH+1);
					strcpy(new_word, word);
	  				misspelled[num_misspelled] = new_word;
	  				num_misspelled++;
	  			}
		  		
			}
			word = strtok(NULL, " ");

  		}
		

	}
	
	return num_misspelled;
	
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){

	for(int i=0; i< HASH_SIZE; i++){		
		hashtable[i] = NULL;
	}
	FILE *fpd = fopen(dictionary_file, "r");
	if(fpd == NULL){
		return false;
	}
	char *line;
	size_t len = 0;
	ssize_t read;
	int prev = 0 ;
	while(getline(&line, &len, fpd) != -1){

		char *word = NULL;
		word = malloc(LENGTH+1);	
		hashmap_t new_node = {word, NULL};

		if(line[strlen(line)-1] == '\n'){
			line[strlen(line)-1] = '\0';
		}
		if(strlen(line) < LENGTH+1){
			strcpy(new_node->word, line);
			int bucket = hash_function(new_node->word);
			
			if(hashtable[bucket] == NULL){

				hashtable[bucket] = new_node;

			} else{
				
				new_node->next = hashtable[bucket];

				hashtable[bucket] = new_node;

			}
			
			prev = bucket;
		}
	}

	fclose(fpd);
	
	return true;

}

bool check_word(const char* word, hashmap_t hashtable[]){
	
	int bucket = hash_function(word);
	hashmap_t cursor = hashtable[bucket];

	while(cursor != NULL){
		if(strcmp(word, cursor->word)==0){
			return true;
		} else {
			cursor = cursor->next;
		}
	}

	char *check_word = word;
	check_word[0] = tolower(check_word[0]);
	check_word[strlen(check_word)-1] = tolower(check_word[strlen(check_word)-1]);
	bucket = hash_function(check_word);

	cursor = hashtable[bucket];

	while(cursor != NULL){
		if(strcmp(check_word, cursor->word)==0){
			return true;
		} else{
			cursor = cursor->next;
		}
	}
	
	
	return false;
}




