#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


#define MAX_WORD_LEN 64 //najveca dozvoljena duzina reci, uz \0
#define LETTERS 26 //broj slova u abecedi i broj dece u trie

#define prefix 			pthread_mutex_t
#define uzmi_prefix		pthread_mutex_lock
#define spusti_prefix	pthread_mutex_unlock

extern char prefiks[MAX_WORD_LEN];
extern int active_printing;
struct trie_node *root;
char *ans, *p;

typedef struct trie_node //cvor unutar trie strukture
{
    char c;												//slovo ovog cvora
    int notification;
	int endOfWord; 										//flag za kraj reci
    int subwords; 										//broj reci u podstablu, ne racunajuci sebe
    struct trie_node *parent;			    			//pokazivac ka roditelju
    struct trie_node *children[LETTERS];				//deca
	pthread_mutex_t prefix;

}trie_node;

typedef struct search_result //rezultat pretrage
{
    int result_count; //duzina niza
    char **words;	  //niz stringova, svaki string duzine MAX_WORD_LEN
} search_result;

trie_node* 
createNode(trie_node *parent, char c){
	
    struct trie_node *newNode;
	newNode = malloc(sizeof(trie_node));
	newNode->notification = 0;	
	newNode->c = c;
	newNode->parent = parent;
	newNode->subwords = 0;
    newNode->endOfWord = 0;
	pthread_mutex_init(&newNode->prefix, NULL);
	
    for(int i = 0 ; i < LETTERS ; i++)
        newNode->children[i] = NULL;

    return newNode;
}

void
insert(trie_node* node, char *str, int i)
{			
		if(i < strlen(str)){
			int index = str[i] - 'a';
			
			if(node->children[index] == NULL)		    	
				node->children[index] = createNode(node, str[i]);
			
			uzmi_prefix(&node->prefix);	  // mutex_lock
			
			insert(node->children[index], str, i + 1);

			if(node->notification == 1){
				node->subwords ++;
				node->notification = 0;
				if(node->parent != NULL)
					node->parent->notification = 1;
				else
					root->notification = 1;
			}

			spusti_prefix(&node->prefix); // mutex_unlock
			
		}else{
			
			if(node->endOfWord == 1)
				return;
			// nova rec u stablu
			node->parent->notification = 1;
		    node->endOfWord = 1;
		}
}

//operacija za dodavanje reci
void
trie_add_word(char *word)
{	
	insert(root, word, 0);
	
	if(root->notification == 1)
	{

		if(active_printing == 1 && strlen(prefiks) > 0 && strncmp(prefiks, word, strlen(prefiks)) == 0)
			printf("%s\n", word);
		
		root->notification = 0;	
	}

}

void
traverse(trie_node* node, char *str, int x)
{
	for(int i=0 ; i < LETTERS ; i++){

		if(node->children[i] == NULL)
			continue;
		if(node->children[i]->endOfWord == 1){
			str[x] = i + 'a';
			strncpy(ans, str, MAX_WORD_LEN - 1);
			ans[MAX_WORD_LEN - 1] = '\0';			
			ans += MAX_WORD_LEN;
		}
		else
			str[x] = i + 'a';		
		
		traverse(node->children[i], str, x + 1);
		str[x] = 0;		
	}
	
}

//operacija za pretragu ; neophodno je proveriti da li se na prefiksu nalazi endOfWord, da bi smo znali da li i tu rec da racunamo
search_result
*trie_get_words(char *prefix)
{	
	int k = 0;
	struct trie_node *tmp = root;

	for(int i=0 ; i < strlen(prefix) ; i++){
		k = prefix[i] - 'a';
		if(tmp->children[k] == NULL)
			return NULL;		
		tmp = tmp->children[k];
	}
	
	uzmi_prefix(&tmp->prefix); // mutex_lock

	struct search_result *result = malloc(sizeof(search_result));

	result->result_count = tmp->subwords; 
	if(tmp->endOfWord)
		result->result_count ++;
	
	p = malloc(result->result_count*(sizeof(char))*MAX_WORD_LEN);
	ans = p;	
	result->words = &p;
	
	// ako je trazeni prefix rec u stablu 	
	if(tmp->endOfWord){
		strncpy(ans, prefix, MAX_WORD_LEN - 1);
		ans[MAX_WORD_LEN - 1] = '\0';			
		ans += MAX_WORD_LEN;
	}

	char str[MAX_WORD_LEN];
	int x = strlen(prefix);		
	strncpy(str, prefix, strlen(prefix));

	traverse(tmp, str, x);
	
	spusti_prefix(&tmp->prefix); // mutex_unlock

	return result;
}

//rezultat se dinamicki alocira pri pretrazi, tako da treba da postoji funkcija za oslobadjanje tog rezultata
void
trie_free_result(search_result *result)
{
	free((char*)(*result->words));
}

void
solve(trie_node* node)
{
	if(node == NULL)
		return;		

	for(int i=0 ; i<26 ; i++)
	{
		pthread_mutex_destroy(&node->prefix);
		solve(node->children[i]);	
			
	}

}

void
destroy_mutex_in_trie()
{
	solve(root);
}


void
trie_init()
{	
	root = malloc(sizeof(trie_node));
	root->notification = 0;
	root->c = 0;
	root->parent = NULL;
	root->subwords = 0;
    root->endOfWord = 0;
	pthread_mutex_init(&root->prefix, NULL);

    for(int i = 0 ; i < LETTERS ; i++){
        root->children[i] = NULL;
	}
}
