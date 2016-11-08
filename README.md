# Hash Table implementation in C
The goal was to create a word search engine using hash table. Hash table was created
from the words given in the files. If the searched word is found, it shows in
which documents it is.


## Stop Words
at, of, is and similar words are taken as stop words, as they can be found in
every document many times. 

## Creating hash table for searching
* If the word is one of the stop words, any further operation will not take place.
* If the word is not already in the hash table, add it with the document that it
  was found in.
* If the word is already in the hash table, but the document name is not. Then
  add the document name.
* Ex: snow, winter.txt, snowman.txt, weather.txt

## Usage

```shell
$ c99 hash-table.c -o hash-table

# if c99 doesnt work 
$ gcc hash-table.c -o hash-table -std=c99

$ ./hash-table
```
