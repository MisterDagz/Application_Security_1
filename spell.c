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
		char *word;
		word = strtok(line, " ");
		while(word != NULL){
  			if(check_word(word, hashtable) == false){
  				misspelled[num_misspelled] = word;
  				num_misspelled++;
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
	char *line;
	size_t len = 0;
	ssize_t read;
	if(fpd == NULL){

		return false;
	}
	//printf("%s ", dictionary_file);
	//printf("%s ", word);
	while(getline(&line, &len, fpd) != -1){
		hashmap_t new_node;
		//printf("%s", line);


		strcpy(new_node->word, line);
		int bucket = hash_function(new_node->word);

		new_node->next = NULL;
		//new_node->next = NULL;
		//printf("%i", bucket);
		if(hashtable[bucket] == NULL){
			hashtable[bucket] = new_node;
		} else{
			new_node->next = hashtable[bucket];
			hashtable[bucket] = new_node;
		}
		//printf("%s", &(hashtable[97]->word));
		//exit(1);
		
		
	}
	
	fclose(fpd);
	printf("aa%saa", &(hashtable[97]->word));
	return true;
}

bool check_word(const char* word, hashmap_t hashtable[]){
	int bucket =  hash_function(word);
	hashmap_t cursor = hashtable[bucket];
	//printf("%s %i %s", word, bucket, cursor->word);
	while(cursor != NULL){
		//printf("One word: %s ", cursor->word);
		if(word == &(cursor->word)){
			//printf("true");
			return true;
		} else {
			cursor = cursor->next;
		}
	}

	char *check_word = word;
	check_word[0] = tolower(check_word[0]);
	check_word[strlen(check_word)-1] = tolower(check_word[strlen(check_word)-1]);
	bucket =  hash_function(check_word);

	cursor = hashtable[bucket];
	//printf("%s %i %s", check_word, bucket, cursor->word);
	while(cursor != NULL){
		if(check_word == &(cursor->word)){
			return true;
		} else{
			cursor = cursor->next;
		}
	}
	
	//printf("%s ", check_word);
	return false;
}




