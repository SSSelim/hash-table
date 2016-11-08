#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 100
#define TABLEINITIALIZER '\0'
#define MAXWORDLENGTH 50
#define DEBUG 1

void debug(char* msg){
  if( DEBUG ){
    printf("\n%s", msg);
  }
}

FILE* getFilePointer(char* fileName){
  FILE *fp = fopen(fileName, "r");

  if( fp == 0 ){
    printf("Error! opening file");
    exit(1);
  }

  if( DEBUG ){
    printf("\n%s file has been opened for reading...", fileName);
  }

  return fp;
}

int h1(int key, int M){ return key % M; }

int h2(int key, int M){ return 1 + (key % (M-1)); }

int h(int key, int i, int M){ return ( h1(key,M) + i*h2(key,M) ) % M; }


void toLowerCase(char *str){
  // 65-90 > A-Z and 97-122 > a-z
  int i=0;

  while( str[i] != '\0' ){
    if( str[i] < 91 ){
      str[i] += 32;
    }
    i++;
  }
}

int length(char *str){
  int len=0;
  int i=0;

  while( str[i] != '\0' ){
    len++;
    i++;
  }

  return len;
}

void copyString(char *dest, char *source){
  int i=0;

  while( source[i] != '\0' ){
    dest[i] = source[i];
    i++;
  }

  dest[i] = source[i]; // for terminating char
}

// compares two string char by char, return 1 if same, 0 if different
int compareStr(char* str1, char* str2){
  int i=0;

  //printf("\nDEBUG-compareStr:word->%s, stopword->%s", str1, str2);

  while( str1[i] != '\0' && str2[i] != '\0'  ){
    if( str1[i] != str2[i] ){
      return 0; // different
    }
    i++;
  }

  return 1;
}

int getKey(char *str){
  int k=0;

  for( int i=0; i<length(str); i++ ){
    k += (str[i] - 'a') * 26 * i;
  }

  return k;
}

void initializeTable(char **table, int M){
  for( int i=0; i<M; i++ ){
    table[i][0] = TABLEINITIALIZER;
  }
}

char** allocateMatrix(int rows, int cols){
  char **mat = 0;

  mat = (char**)malloc(rows * sizeof(char*));

  if ( mat != 0 ){
    for(int i = 0; i < rows; i++){
      mat[i] = (char *)malloc(cols * sizeof(char));
    }
  } else {
    printf("an error occured!!");
  }

  initializeTable(mat, rows);

  return mat;
}

// returns the index of a word if found
// if not, it returns the negatif index, which it can be added
int searchWord(char** table, int M, char* word){
  int i=-1;
  int aramaSayisi=0; // search operation count
  int key=0;
  int index=0;
  toLowerCase(word); // lower case the word jic

  key = getKey(word);

  do{
    aramaSayisi++;
    i++;
    index = h(key, i, M);

    //printf("\nDEBUG-isStopWord:key->%d, hash->%d, word->%s", key,index,SWT[index]);

    // if SWT[index] = '\0', is not a stop word
    if( table[index][0] == TABLEINITIALIZER ){
      printf("\n%s took %d searching times", word,aramaSayisi);
      return -1*index;
    }

    // if SWT[index] != '\0', just if is same word
  }while( !compareStr(word, table[index]) );

  printf("\n%s took %d searching times", word,aramaSayisi);
  return index;

}

int addWord(char** table, int M, char* word){
  int index=0;

  index = searchWord(table, M, word);

  if( index < 0 ){
    index = index * -1;

    copyString(table[index], word);
  }

  return index;
}

// in general word table, storing words with file name that they are located in
// they are combined with a comma between them
// splitting the string by comma and creating an array
// being used to check whether adding document or not
char** getWords(char* row){
  int ch=0;
  int i=0;
  int j=0;
  char** words = allocateMatrix(10,20);
  initializeTable(words, 10);

  while( row[ch] != '\0' ){
    if( row[ch] == ',' ){
      // add terminating character
      words[i][j] = '\0';
      i++;
      j=0;
    } else {
      words[i][j] = row[ch];
      j++;
    }

    ch++;
  }

  return words;
}

int hasFileName(char** words, char* fileName){
  int i=0;



  while( words[i][0] != '\0' ){

    //printf("\n%s", words[i]);

    if( compareStr(words[i], fileName) ){
      return 1;
    }
    i++;
  }

  return 0;
}


int getIndexOfTerminator(char* row){
  int i=0;

  while( row[i] != '\0' ){
    i++;
  }

  return i;
}

void addFileName(char* row, char* fileName){

  //printf("\nDEBUG: row->%s, fileName->%s", row, fileName);

  if( !hasFileName(getWords(row), fileName) ){
    // file name in not in the row
    // add it
    int index = getIndexOfTerminator(row);
    int i=0;

    row[index++] = ',';

    while( fileName[i] != '\0' ){
      row[index++] = fileName[i++];
    }

    row[index] = '\0';
  }
}

void populateSWT(char *fileName, char** swt, int M){
  FILE* fp = getFilePointer(fileName);
  char word[MAXWORDLENGTH];
  int i=0;
  int step;
  int index;

  while( fscanf(fp, "%s", word) != -1 ){
    toLowerCase(word);
    addWord(swt, M, word);

    // printf("%s %d -> key : %d, hash : %d\n", buff, length(buff), key(buff), h(key(buff), 2, M));
    i++;
  }

}

void populateGWT(char *fileName, char** table, int M, char** swt, int mswt){
  FILE* fp = getFilePointer(fileName);
  char word[MAXWORDLENGTH];
  int i=0;
  int step;
  int index;

  // fscanf, returns -1 when there is no new value to read, otherwise 1
  while( fscanf(fp, "%s", word) != -1 ){
    toLowerCase(word);

    if( searchWord(swt, mswt, word) < 0 ){
      // word is not a stop word
      index = addWord(table, M, word);
      addFileName(table[index], fileName);
    }
    // printf("%s %d -> key : %d, hash : %d\n", buff, length(buff), key(buff), h(key(buff), 2, M));
    i++;
  }

}

int getFileList(char *fileName, char** fileList){
  int fileCount=0;
  FILE *fp=getFilePointer(fileName);

  // fscanf, returns -1 when there is no new value to read, otherwise 1
  while( fscanf(fp, "%s", fileList[fileCount]) != -1 ){
    fileCount++;
  }

  return fileCount;
}

int main(){
  char *stopWordFile = "stopwords.txt";
  char *documentListFile = "documentList.txt";
  char **fileList;
  int fileCount=0;
  int mswt=541; // m for stop word table
  int mgwt=449; // m for general word table
  char **stopWordTable;
  char **generalWordTable;
  char word[MAXWORDLENGTH];
  int index=0;
  char answer='y';

  stopWordTable = allocateMatrix(mswt, MAXWORDLENGTH);

  populateSWT(stopWordFile, stopWordTable, mswt);
  debug("Stop word table was created...");

  generalWordTable = allocateMatrix(mgwt, MAXWORDLENGTH);
  debug("General word table was created...");

  fileList = allocateMatrix(20, MAXWORDLENGTH);
  fileCount = getFileList(documentListFile, fileList);

  for( int i=0; i<fileCount; i++ ){
    populateGWT(fileList[i], generalWordTable, mgwt, stopWordTable, mswt);
  }

  debug("Searching table was created...");

  printf("\n\n");

  while( answer == 'y' || answer == 'Y' ){
    printf( "\nEnter a word >" );

    scanf("%s", word);

    if( searchWord(stopWordTable, mswt, word) > 0 ){
      printf("\nThe given word is a stop word.");
    } else {
      printf("\nThe given word is not a stop word...");

      index = searchWord(generalWordTable, mgwt, word);

      if( index > 0){
        printf("\nThe given word was found in the searcing table.");
        printf("\n%s", generalWordTable[index]);
      } else {
        printf("\nThe given word could not be found in the searching table!");
      }
    }

    scanf("%c", &answer);
    printf("\n\nWant to enter another word? [y/n] > ");
    scanf("%c", &answer);
  }

  printf("\nExiting...\n");

  return 0;
}
