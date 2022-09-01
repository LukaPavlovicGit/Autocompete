#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "trie.h"
#include "scanner.h"
#include <signal.h>
#include <termios.h>

#define MAX_WORD_LEN 64 //najveca dozvoljena duzina reci, uz \0
#define LETTERS 26 		//broj slova u abecedi i broj dece u trie

extern void *scanner_work(void *_args);
extern search_result *trie_get_words(char *prefix);
extern void trie_init();
extern struct trie_node *root;
extern void trie_free_result(search_result *result);
extern void destroy_mutex_in_trie();

#define prefix 				pthread_mutex_t
#define uzmi_prefix			pthread_mutex_lock
#define spusti_prefix		pthread_mutex_unlock

 
pthread_mutex_t x_lock;
pthread_t threads[30];
char prefiks[MAX_WORD_LEN];
int i=0, active_printing = 0;

int next_input(void) {
  int rtn = fgetc(stdin);
  return(rtn);
}

int main()
{	
	trie_init();
	char cmd[30];
	struct search_result *result;
	pthread_mutex_init(&x_lock, NULL);
	
	for(;;){
		
		scanf("%[^\n]%*c", cmd);
		
		if(strncmp("_add_", cmd, 5) == 0)
			pthread_create(&threads[i], NULL, scanner_work, (void*)(&cmd[6]));
		
		else if(strcmp("_stop_", cmd) == 0)
			break;
		
		else{
			memset(prefiks,0,MAX_WORD_LEN);	
			//strncpy(prefiks, cmd, strlen(cmd));
			
			int x = 0;			
					
			for(int k=0 ; k<strlen(cmd) && k<MAX_WORD_LEN ; k++, x++){
			
				if(cmd[k] == ' '){
					memset(prefiks, 0, x);
					x = -1;
					continue;
				}
				
				prefiks[x] = cmd[k];			
			}		

			if((result = trie_get_words(prefiks)) == NULL)
				continue;

			active_printing = 1;
		
			char *p = *result->words;
			for(int i=0 ; i < result->result_count ; i++){	
				printf("%s\n", p);
				p += MAX_WORD_LEN;	
			}
			trie_free_result(result);

			int key;
			while(1){
				if ((key = next_input()) == -1) {
					active_printing = 0;
					break; 
				}
			}

		}

	}

	destroy_mutex_in_trie();
	
	pthread_mutex_destroy(&x_lock);		


    return(0);
}


