# Autocompete functionality - multithread solution

## [project specification](OS-Domaci4.pdf)
This application is intended for Linux computers and provides an autocomplete function for the specified prefix.<br/>
The user enters the prefix, and the application begins listing all the words on the screen that begin with the specified prefix. The words are read from a text file specified by the user and stored in memory using the **trie structure**, which must support concurrent insertion and searching for the words.<br/>

The user enters commands through the command line, as shown below.<br/>

1. 	**\_add\_** \<dir\><br/>
	Adds a new directory of text files. At this time a new thread 'scanner' is created.<br/>
2. 	**\_stop\_**<br/>
	Shut down application.<br/>

3. 	**\<prefix\>**<br/>
	Prefix with which the words are matched. If the user enters more than one prefix, only the last one is selected, the others are ignored. The application starts searching the **trie structure** for the words starting with the specified prefix. When a new word is found in the text file that starts with the specified prefix, it is output to the screen. The user can stop the search by pressing the STRL+D key combination and then call the next command.<br/>

## Scanner thread

The scanner thread traverses the directory looking for text files. When a file is found, the thread reads all the words from that file. The scanner thread knows which files have been read so far and when they were last modified. In other words, the scanner will not read a file that has already been read and not changed since the last time.<br/>

The text file should contain words separated by SPACE (' '), TAB ('\T') OR ENTER ('\N'). The words must contain characters of the English alphabet. The maximum word length is 63 characters. The word will be ignored if any of these conditions is not met.
