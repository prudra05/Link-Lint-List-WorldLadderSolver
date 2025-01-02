/*---------------------------------------------------------------------------------------------------------------------------------------------------------
Fall Semester 2024
Project 5: Link Lint List - Find Shortest Word Ladder
Course: CS 211 Programming Practicum
Section: 4pm Lecture Wed & Fri, 12pm Lab Tue
System: Zybooks Advanced IDE and Replitt
Author: Priyanjali Rudra

Description:
The Purpose of this game is to to find the shortest word ladder, the word ladder starts and ends with user's given words.
This is an extension of Project 3.
------------------------------------------- -----------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

/*
countWordsOfLenght(char* filename, int wordSize), reads a file and counts how many of the words in the file are the same length 
as wordSize, where a "word" is ANY set of characters that falls between two whitespaces (or tabs, or newlines, etc.)
This method is mainly utilized in main() to better streamline iteration. This function returns count if file can be opened 
else returns -1.
*/
int countWordsOfLength(char* filename, int wordSize) { 

    FILE *file_ptr;
    //opens user-given filename
    file_ptr = fopen(filename, "r");
    //variable stores #words of wordSize length
    int count = 0;

    //if file cannot be opened, func. return -1
    if (NULL == file_ptr) {
        printf("file can't be opened \n");
        return -1;
    }

    //buffer stores each word while file is being read, if buffer is the size of WordSize count is incremented. 
    char buffer[200];
    while(fscanf(file_ptr, "%s", buffer) == 1){
        int length = strlen(buffer);
        if(length == wordSize){ 
        count++;
        }
    } 
    //file is closed
    fclose(file_ptr);

    //file was succesfully read therefore count is returned
    return count;

}

/*
function fills the pre-allocated word array words with only words that are exactly wordSize letters long.
The file should contain exactly numWords words, found using countWordsOfLength(). 
Similar to the previous function, buildWordArray() reads a file given in filename, 
then fils empty char array pointer with numWords words of size wordSize. Returns true if array was
built succesfully else returns false.
*/
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 

    FILE *file_ptr;
    //opens user-given filename
    file_ptr = fopen(filename, "r");
    //variable stores #words of wordSize length
    int count = 0;

    //if file cannot be opened, func. return false
    if (NULL == file_ptr) {
        printf("file can't be opened \n");
        return false;
    }

    //buffer stores each word while file is being read, if buffer is the size of WordSize count is incremented. 
    char buffer[200];
    while(fscanf(file_ptr, "%s", buffer) == 1){
        int length = strlen(buffer);
        if(length == wordSize){ 
        strcpy(words[count], buffer);
        count++;
        }
    } 
    
    //file is closed
    fclose(file_ptr);

    //checks if array was built accuarately
    //count is accurate, therefore equal to numWords
    if(count == numWords){
        //all added words in array words is of accurate length
        for(int i = 0; i < count; i++) {
            //returns false if size is not accurate
            if(strlen(words[i]) != wordSize){
                return false;
            }
        }
        //array is built succesfully
        return true;
    }
    
    return false;
}

/*
finds the index of a given word aWord in words array between indexes loInd and hiInd.
Returns index if word was found between specified indexes, else returns -1.
*/
int findWord(char** words, char* aWord, int loInd, int hiInd) { 

    //traverses words between indexes loInd and hiInd, and if word is found, index is returned
    for (int i = loInd; i <= hiInd; i++) {
        if (strcmp(words[i], aWord) == 0) { 
            return i;
        }
    }
  //word was not found, therefore -1 is returned.
  return -1; 

}

/*
frees words array and individual words within words array to avoid memory leaks. 
Called in main() after words array is printed. 
Takes in built words array and number of words within words array.
*/
void freeWords(char** words, int numWords) {
    //iterates through words array and frees each individual memory allocation
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    //frees words array
    free(words);
}
/*
function inserts given word at the front of the word ladder.
Function takes in the current word ladder, Ladder, and the new word to be inserted, newWord.
*/
void insertWordAtFront(WordNode** ladder, char* newWord) {

    //allocate space for a new WordNode
    WordNode* newNode = (WordNode*) malloc(sizeof(WordNode));
    //head stores the first word in the word ladder, ladder
    WordNode* head = *ladder;
    //the new WordNode is set to be newWord, which is the new word to be inserted. 
    //newWord is now the first word in the word ladder
    newNode->myWord = newWord;
    //the second word in the word ladder is now the word stored in head, the prev first word.
    newNode->next = head;
    //head and ladder* is reset to be equal to newNode, the current first word in the word ladder
    head = newNode;
    *ladder = head;  
}

/*
finds & returns number of words in word ladder, ladder.
*/
int getLadderHeight(WordNode* ladder) {
    //traverse linked list ladder and increments count per pointer.
    int count = 0;
    WordNode* curr = ladder;
    while(curr != NULL){
        count++;
        curr = curr->next;
    }
    //returns number of words in ladder
    return count;

}

/*
function makes a complete copy of ladder and return the copy.
This allows ladder to be constantly updating, when new words are added.
*/
WordNode* copyLadder(WordNode* ladder) {

    //if ladder is empty nothing is returned
    if(ladder == NULL) {
        return NULL;
    }

    //inititalise wordNode that will hold copied ladder
    WordNode* copyLadder = NULL;
    //initialize pointer for traversing copied ladder array
    WordNode* currC = NULL;
    //pointer for traversing original ladder
    WordNode* curr = ladder;
    //pointer stores the previous pointer
    WordNode* prev = NULL;

    //while end of original array is not reached
    while(curr != NULL) {
        //space is allocated for currC
        currC = (WordNode*) malloc(sizeof(WordNode));
        //second check for if ladder is empty
        if(currC == NULL){
            return NULL;
        }
        //currC is set equal to curr
        currC->myWord = curr->myWord;
        //currC->next is created but empty
        currC->next = NULL;

        //copyLadder is completely updated
        if(prev == NULL) {
            copyLadder = currC;
        } else {
            prev->next = currC;
        }

        //prev and curr are updated
        prev = currC;
        curr = curr->next;
    }

    //copied ladder is returned
    return copyLadder;
}

/*
frees ladder and individual words within to avoid memory leaks. 
Called in main() after ladder is printed. 
Takes in built ladder.
*/
void freeLadder(WordNode* ladder) {

    //curr is set equal to the fisrt word in the word ladder
    WordNode* curr = ladder;
    //wordNode which will store the next pointer
    WordNode* next = NULL;

    //while end of ladder is not reached
    while(curr != NULL){
        //next pointer is stored
        next = curr->next;
        //current pointer is freed
        free(curr);
        //curr is updated
        curr = next;
    }
}

/*
function inserts a possible ladder at the back of list, which stores all possible ladders.
Takes in LadderNode** list which stores all current possible ladders and WordNode* newLadder,
which is the new ladder that will be inserted at the back of list.
*/
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {

    //allocate space for a new LadderNode
    LadderNode* newNode = (LadderNode*) malloc(sizeof(LadderNode));
    //head stores the first ladder within list
    LadderNode* head = *list;
    //newNode is set to the given new ladder, that will be inserted, newLadder
    newNode->topWord = newLadder;
    //newNode next is initialized
    newNode->next = NULL;

    //if list is empty, newNode is the first word
    if (*list == NULL) { 
        *list = newNode;
    }
    
    //if list is empty, newNode is the first word, head is updated
    if (head == NULL) {
        head = newNode;
        return;
    }

    //if list is not empty, list is traversed to the end, and then newNode is added to the end of the list
    while (head->next != NULL) {
        head = head->next;
    }
    head->next = newNode;
}

/*
function deletes the first ladder from the front of the list,
and returnes the deleted ladder. 
Takes in list, which stores all current word ladder possibilities and returns WordNode, which is the first word from the ladder that was deleted.
*/
WordNode* popLadderFromFront(LadderNode** list) {

    //head stores first ladder from list
    LadderNode* head = *list;
    //topWord stores first word from the first ladder in list
    WordNode* topWord = head->topWord;
    //list's front is now the second ladder in list
    *list = head->next;
    //head is freed
    free(head);
    //first word from deleted ladder is returned
    return topWord;

}

/*
frees ladder list and individual ladders within to avoid memory leaks. 
Called in main() after shortest ladder is printed. 
Takes in built ladder list.
*/
void freeLadderList(LadderNode* myList) {

    //curr holds first ladder in myList
    LadderNode* curr = myList;
    //next pointer is initialized
    LadderNode* next = NULL;
    //Unitl last ladder in myList is reached
    while(curr != NULL){
        //next is updated
        next = curr->next;
        //first ladder is freed
        freeLadder(curr->topWord);
        //curr pointer is freed
        free(curr);
        //curr is updated
        curr = next;
    }
}

/*Helper function used in findShortestWordLadder() and copied from Project 3. 
Function compares two words and finds the number of difference between them.
Takes in two words and returns int number of difference.
*/
int strCmpCnt(char* word1, char* word2) {

    int differences = 0; //keeps count of number of difference
    while (*word1 != '\0' || *word2 != '\0') { //keeps incrementing word pointers until BOTH word pointers have reache \0 (Null Character)

        if (*word1 != *word2) { 
            differences++; //Increments differences variable once a difference is found

        }
        if(*word1 != '\0'){ //Ensures that word1 pointer incrementats only if \0 is not met, else only word2 pointer is incremented
            word1++; //moves on to next character of word1

        }
        if(*word2 != '\0'){ //Ensures that word2 pointer incrementats only if \0 is not met, else only word1 pointer is incremented
             word2++; //moves on to next character of word1

        }

    }

    return differences; //returns number of difference between word1 and word2

}

/*
algorithm finds the shortest word ladder from startWord to finalWord in the words word array, 
where each word is wordSize long and there are numWords total words.
usedWord is a bool array and only true if words[i] has previously been entered into a ladder, 
and should therefore not be added to any other ladders. Retursn a pointer to the shortest ladder or NULL if no ladder is possible.
Function also frees all heap-allocated memory that is created here that is not the returned ladder.
*/
WordNode* findShortestWordLadder(char** words, bool* usedWord, int numWords, int wordSize, char* startWord, char* finalWord) {

    //Create myList, an empty list of LadderNode structs
    LadderNode* myList = NULL;
    //Create myLadder, an empty list of WordNode structs
    WordNode* myLadder = NULL;
    //Prepend w1 to the front of myLadder
    insertWordAtFront(&myLadder, startWord);
    //Append myLadder to the back of myList
    insertLadderAtBack(&myList, myLadder);
    //Set w1 to used
    usedWord[findWord(words, startWord, 0, numWords - 1)] = true;
    //While myList is not empty: 
    while(myList != NULL){
        //Pop the head LadderNode off the front of myList, call it myLadder
        myLadder = popLadderFromFront(&myList);
        //For each word in the words array that is a neighbor of the head myWord of myLadder:
        for(int i = 0; i < numWords; i++){
            //If the neighbor word has not already been used in a ladder to this point:
            if(!usedWord[i] && strCmpCnt(words[i], myLadder->myWord) == 1){
                //If the neighbor word is w2:
                if(strcmp(words[i], finalWord) == 0){
                    //Prepend w2 to the front of myLadder
                    insertWordAtFront(&myLadder, words[i]);
                    printf("Hooray! We found the shortest word ladder\n");
                    freeLadderList(myList);
                    //freeLadder(myLadder);
                    return myLadder;
                }
                else {
                    //Set w2 to used
                    usedWord[i] = true;
                    //Copy myLadder to anotherLadder
                    WordNode* anotherLadder = copyLadder(myLadder);
                    //Prepend neighbor word to the front of anotherLadder
                    insertWordAtFront(&anotherLadder, words[i]);
                    //Append anotherLadder to the back of myList
                    insertLadderAtBack(&myList, anotherLadder);
                }
            }
        }
        //once ladder is returned myLadder is freed
        freeLadder(myLadder);
    }
    //no ladder is possible
    return NULL;

}

// interactive user-input to set a word; 
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

//MAIN FUNCTION STARTS HERE:
//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main(), except for 
//  testing-related purposes (such as command-line 
//  arguments for "TESTING MODE" to call a test case 
//  master function, or something similar)
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
