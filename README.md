# Autocompete functionality
## Multithread application

This application was intended for Linux machine and provides autocomplete functionality for the given prefix. User passes the prefix and application starts listing all words on the screen starting with a given prefix. Words are read from textual file given by the user and are stored in memory using **TRIE STRUCTURE** which needs to support concurrent insertion and search for the words. 

User gives commands through command line. Here are the commands:

1. 	**\_add\_** \<dir\> 
	Adds new directory with textual files. At this point a new thread 'scanner' is created.

2. 	**\_stop\_**
	Shut down application.

3. 	**\<prefix\>**
	Prefix by which words are matched with. If user enter more than one prefix, only last one is picked other are ignored. Application starts search in **trie structure** for the words starting with a given prefix, also if new word if found in textual file starting with a given prefix will be printed on the screen. User can terminate search by pushing STRL+D and next command can be invoked after that. 

 <span style="color:red">Scanner thread</span>