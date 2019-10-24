#include "dictionary.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]){
	
	int num_misspelled = 0;
	//Initialized line value fixed one "Conditional jump or move depends on uninitialised value(s)" which occurs in while loop when checking values during getline
	char *line = "";
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
			
			

			if(strlen(word) < LENGTH+1){
				//Handle Blank "Word" case.
	  			if(check_word(word, hashtable) == false && strcmp(word, "") !=0){
					bool number = false; 
					int counter =0;
					for(int j=0; j < strlen(word); j++){
						if(word[j] >= '0' && word[j] <= '9'){
							counter++;
						}
					}
					if(counter == strlen(word)){
						number = true;
					}
					if(number == false){		
						char *new_word = NULL;
						new_word = malloc(LENGTH+1);

						strcpy(new_word, word);
		  				misspelled[num_misspelled] = new_word;
		  				num_misspelled++;
					}
	  			} 
		  		
			}
			word = strtok(NULL, " ");

  		}
		//Addresses memory leak in creating char * to read words in line
		free(word);
		

	}
	//Addresses memory leak in creating char * to read lines
	free(line);
		
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
	//Initialized line value fixed one "Conditional jump or move depends on uninitialised value(s)" which occurs in while loop when checking values during getline, this seems to also fix the same problem with the conditional statement in the loop which checks the sizeof the line variable
	char *line = "";
	size_t len = 0;
	ssize_t read;
	while(getline(&line, &len, fpd) != -1){
		//Malloc 1024 chars fixed write size 8 issue
		char *word = malloc(sizeof(char)*1024);	
		hashmap_t new_node = {word, NULL};
		//Fixes "Conditional jump or move depends on uninitialised value(s)" in check_word where the nodes "next" variable was not initialized as NULL in the above statement.
		new_node->next = NULL;

		if(line[strlen(line)-1] == '\n'){
			line[strlen(line)-1] = '\0';
		}
		
		if(strlen(line) < LENGTH+1){
			
			strcpy(new_node->word, line);
			int bucket = hash_function(new_node->word);
			//AFL generated input triggered a condition where the bucket number was negative adding additional checks
			if(bucket >= 0 && bucket < HASH_SIZE){
				if(hashtable[bucket] == NULL){

					hashtable[bucket] = new_node;

				} else{
					
					new_node->next = hashtable[bucket];

					hashtable[bucket] = new_node;

				}
			}
			
		}
	}
	fclose(fpd);
	//Addresses memory leak in creating char * to read file lines
	free(line);
	return true;

}

bool check_word(const char* word, hashmap_t hashtable[]){
	
	int bucket = hash_function(word);
	//AFL generated input triggered a condition where the bucket number was -1064
	if(bucket < 0){
		return false;
	}
	hashmap_t cursor = hashtable[bucket];

	while(cursor != NULL){

		if(strcmp(word, cursor->word)==0){

			return true;
			
		} else {

			cursor = cursor->next;
		}
	}

	char *check_word = malloc(sizeof(char)*1024);
	strcpy(check_word, word);
	for(int ind=0; ind < strlen(check_word); ind ++){
		check_word[ind] = tolower(check_word[ind]);
	}
	
	bucket = hash_function(check_word);
	if(bucket < 0){
		free(check_word);
		return false;
	}
	cursor = hashtable[bucket];
	while(cursor){
		if(strcmp(check_word, cursor->word)==0){
			free(check_word);
			return true;
		} else{
			cursor = cursor->next;
		}
	}
	free(check_word);
	return false;
}




