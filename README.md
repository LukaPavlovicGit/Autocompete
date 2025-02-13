# Autocomplete Functionality

## Resources
- [Video demonstration](https://drive.google.com/file/d/1yAI4ONXj9ofOa-Vkxinkzu6wDJ0t_gUw/view?usp=drive_link)
- [Project specification](OS-Domaci4.pdf)

## Overview
This application is designed for Linux computers and provides an autocomplete function for specified prefixes. It reads words from a text file into memory using a **trie structure** that supports concurrent insertion and searching.

## Command Line Interface
The user interacts with the application through the following commands:

1. **`_add\_** \<dir\>
   - Adds a new directory of text files
   - Creates a new 'scanner' thread at initialization

2. **`_stop\_**
   - Shuts down the application

3. **`\<prefix\>**`
   - Specifies which words to match
   - If multiple prefixes are entered, only the last one is used
   - Application searches the trie structure for words starting with the specified prefix
   - New matching words from text files are displayed on screen
   - Search can be stopped using CTRL+D, after which the next command can be entered

## Scanner Thread Details
- The scanner thread monitors the directory for text files
- When it finds a file, it reads all words from that file
- Tracks which files have been read and when they were last modified
- Won't re-read unchanged files that have already been processed

## Text File Requirements
Text files must follow these rules:
- Words should be separated by:
  - SPACE (' ')
  - TAB ('\t')
  - ENTER ('\n')
- Words must contain only English alphabet characters
- Maximum word length is 63 characters
- Words not meeting these conditions will be ignored
