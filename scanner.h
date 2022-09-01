#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#define MAX_WORD_LEN 64 //najveca dozvoljena duzina reci, uz \0
#define LETTERS 26 		//broj slova u abecedi i broj dece u trie


struct scanned_file { 		 //datoteka koju je scanner vec skenirao
    char file_name[256];	 //naziv datoteke
    long int lastmdify_time; //vreme poslednje modifikacije datoteke
};

struct scanned_file scanned_file[100];
extern pthread_mutex_t x_lock;
extern void trie_add_word(char *word);
extern char prefiks[MAX_WORD_LEN];
int k = 0;

void *scanner_work(void *_args)
{
	DIR *folder; // *((char*)_args);
	FILE *f;
	char *c = ((char*)_args);
	char dirpath[50] = "./data/";
	struct dirent *entry;
	struct stat filestat;
	
	strcat(dirpath, c);
	strcat(dirpath, "/");

	folder = opendir(dirpath);	
    if(folder == NULL){
		perror("Unable to read directory");
		return(0);
    }
	
	for(;;){

		while( (entry=readdir(folder)) ){
			
			if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				continue;

			char filepath[50];
			strcpy(filepath, dirpath);
			strcat(filepath, entry->d_name);
			
			f = fopen(filepath, "r");
			int fd = fileno(f);
			if(fstat(fd, &filestat) == -1){
				printf("something went wrong\n");
				continue;
			}

			pthread_mutex_lock(&x_lock);

			int flag=0;
			for(int i=0 ; i<k ; i++){
				if((strcmp(scanned_file[i].file_name, entry->d_name) == 0) && (scanned_file[i].lastmdify_time == filestat.st_mtime)){
					flag=1;
					fclose(f); 
					break;
				}
			}
			if(flag){
				pthread_mutex_unlock(&x_lock);
				continue;	
			}
			strncpy(scanned_file[k].file_name, entry->d_name, 30);
			scanned_file[k++].lastmdify_time = filestat.st_mtime;
	
			pthread_mutex_unlock(&x_lock);

			char word[MAX_WORD_LEN], c;
			int i=0, isValidWord=1;
			while((c = fgetc(f)) != EOF){	
				if(c == ' ' || c == '\n' || c == '\t' || i > MAX_WORD_LEN){						
					
					if(isValidWord && strlen(word) > 1)
						trie_add_word(word);
					
					isValidWord = 1, i = 0;
					memset(word, 0, MAX_WORD_LEN);
					continue;
				}
				if(!(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'))
					isValidWord = 0;
				if(c >= 'A' && c <= 'Z')
					c = c + 32;	
				word[i++] = c;					
			}
			fclose(f);	
		}

	sleep(5);
	closedir(folder);
	folder = opendir(dirpath);
	}
}


