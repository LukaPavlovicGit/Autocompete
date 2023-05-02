# Autocompete functionality - multithread solution

## [Official project specification](OS-Domaci4.pdf)

This application was intended for Linux machine and provides autocomplete functionality for the given prefix.<br/>
User passes the prefix and application starts listing all words on the screen starting with a given prefix. Words are read from textual file given by the user and are stored in memory using **TRIE STRUCTURE** which needs to support concurrent insertion and search for the words.<br/>

User gives commands through command line as shown below.<br/>

1. 	**\_add\_** \<dir\><br/>
	Adds new directory with textual files. At this point a new thread 'scanner' is created.<br/>

2. 	**\_stop\_**<br/>
	Shut down application.<br/>

3. 	**\<prefix\>**<br/>
	Prefix by which words are matched with. If user enter more than one prefix, only last one is picked other are ignored. Application starts search in **trie structure** for the words starting with a given prefix, also if new word if found in textual file starting with a given prefix, it will be printed on the screen. User can terminate search by pushing STRL+D and next command can be invoked after that.<br/>

## Scanner thread

 Scanner thread goes through directory and search for textual files. When file is found thread reads all words from it. Scanner thread knows which files are read so far and their last modifed time. In other words, scanner won't read file which is read before and didn't change from the last time.<br/>

 Textual file is supposed to contain words separated with SPACE(' '), TAB ('\T') OR ENTER ('\N'). Words are supposed to contain characters of english alphabet. Maximum word lenght is 63 caracters. Word is ignored if any of those conditions are not meet.
