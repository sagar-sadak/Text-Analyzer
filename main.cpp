/*
Description: Main driver file which handles file input and output, creates the BST,
             and displays a user interface to show some information about the text analysis of the file
Usage: First run the Makefile, then simply run the executable as such : ./exe
*/

// import necessary libraries
#include <iostream>
#include <fstream>
#include <cstring>
#include "bst.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;

// function prototypes
int getFrequency(WordEntry* array, string word, int len);
bool isAlpha(char* word);
WordEntry getMax(WordEntry* array, WordEntry* maxArray, int thresh, int len);
WordEntry getMin(WordEntry* array, WordEntry* minArray, int thresh, int len);

// main driver function
int main(int argc, char** argv)
{
    // check if the user used the correct syntax to run the executable
    if (argc > 1)
    {
        cout << "Error Usage Syntax: <exe>" << endl;
        return 0;
    }

    // Prompt user for the text file
    string file;
    cout << "Please enter the text file you would like to analyze: ";
    cin >> file;

    // Create a char buffer to put the file data into
    const size_t buffer_size = 100000;
    char buffer[buffer_size] = {0};
    ifstream inFile(file);
    
    // if file does not exist, tell the user and exit program
    if (!inFile)
    {
        cout << "File error. Please ensure you entered the input file name correctly." << endl;
        return 0;
    }

    // create the BST object on the heap
    int numWords = 0;
    BST<WordEntry>* tree = new BST<WordEntry>();
    // while there is text in the inFile, keep reading data
    while(inFile)
    {
        // read in a huge size of text into the buffer
        inFile.read(buffer, buffer_size - 1);
        size_t bytes = inFile.gcount();
        // if no bytes were read in this time, exit the loopp
        if (!bytes)
            break;

        // declare delimiters to read the file
        char delimiter[] = "-@!#$%&*()[]{}\".,;:~`?<>+=-_ /|\n\r";
        // tokenize the text based on the delimiter
        char* word = strtok(buffer, delimiter);
        while (word != NULL)
        {
            // make each word lowercase
            for (int i = 0, j = strlen(word); i < j; i++)
                word[i] = tolower(word[i]);

            numWords++;
            // create WordEntry object to insert into BST
            WordEntry newWord = WordEntry();
            newWord.word = word;
            newWord.frequency = 1;
            // check if word is an actual word, not just punctuation or numbers
            if (isAlpha(word))
                tree->insert(newWord);
            // call strtok again for next loop
            word = strtok(NULL, delimiter);
        }
    }

    // get number of nodes in the tree, and output to the user
    int numNodes = tree->count();
    cout << "Total # of words: " << numWords << endl;
    cout << "Total # of unique words: " << numNodes << endl;
    WordEntry* array = tree->toArray();

    // allow user to ask for frequency of any word in the text
    string userIn = "";
    while (true)
    {
        cout << "Enter a word to get its frequency (enter 'q' to quit): ";
        cin >> userIn;
        // if user enters 'q', break loop
        if (userIn == "q")
            break;

        // get frequency of that specific word and display to user
        int res = getFrequency(array, userIn, numNodes);
        cout << "The word '" << userIn << "' appears " << res << " times.\n" << endl;
    }

    // create an array of 5 most frequent words and fill it by calling the getMax function
    WordEntry* maxArray = new WordEntry[5];
    maxArray[0] = getMax(array, maxArray, numNodes, numNodes);
    for (int i = 1; i < 5; i++)
        maxArray[i] = getMax(array, maxArray, maxArray[i - 1].frequency, numNodes);

    // output those words to the user
    cout << "Five most frequently used words in this text: " << endl;
    for (int i = 0; i < 5; i++)
        cout << "\t" << i+1 << ") " << maxArray[i].word << ": " << maxArray[i].frequency << endl;

    cout << endl;

    // do the same thing but for the least frequent words
    WordEntry* minArray = new WordEntry[5];
    minArray[0] = getMin(array, minArray, -1, numNodes);
    for (int i = 1; i < 5; i++)
        minArray[i] = getMin(array, minArray, minArray[i - 1].frequency, numNodes);

    // display them to the user
    cout << "Five least frequently used words in this text: " << endl;
    for (int i = 0; i < 5; i++)
        cout << "\t" << i+1 << ") " << minArray[i].word << ": " << minArray[i].frequency << endl;

    cout << endl;

    // ask user if they want to save the frequency analysis to a file
    cout << "If you would like to output the frequency analysis to a file, enter the file name, else enter 'q': ";
    cin >> userIn;

    // if they do
    if (userIn != "q")
    {
        // then open the file
        ofstream file(userIn);
        if (!file)
        {
            // display if there was an error with the file
            cout << "File error. Please ensure you entered the output file name correctly." << endl;
            return 0;
        }
        // write to the file
        file << "WORD : FREQUENCY" << endl;
        for (int i = 0; i < numNodes; i++)
            file << array[i].word << " : " << array[i].frequency << endl;

        // tell the user that file has been written and close the file
        cout << "Content, arranged alphabetically, successfully outputted to the file!" << endl;
        file.close();
    }

    // delete all the memory allocated on the heap to prevent memory leaks
    delete[] array;
    delete[] maxArray;
    delete[] minArray;
    delete tree;
    // close input file and end main function
    inFile.close();
    return 1;
}

// function that decides whether a word is an actual word, and not punctuation or number
bool isAlpha(char* word)
{
    // for each char in the word, check if that is the case
    for (int i = 0, j = strlen(word); i < j; i++)
    {
        // if not, return false, i.e., it is not a word
        if (!(std::isalpha(word[i]) || word[i] == '\''))
            return false;
    }
    return true;
}

// function that returns the frequency of any word in the text
int getFrequency(WordEntry* array, string word, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (array[i].word == word)
            return array[i].frequency;
    }
    return 0;
}

// function that gets the max frequency in the array, based on a threshold val
WordEntry getMax(WordEntry* array, WordEntry* maxArray, int thresh, int len)
{
    WordEntry max = array[10];
    for (int i = 0; i < len; i++)
    {
        // check if this word is already in the array
        bool found = false;
        for (int j = 0; j < 5; j++)
        {
            if (array[i].word == maxArray[j].word)
                found = true;
        }
        // if all of these conditions hold, make this the new max object
        if ((array[i].frequency >= max.frequency) && (array[i].frequency <= thresh) && (found == false))
            max = array[i];
    }
    // return it
    return max;
}

// function that gets the min frequency in the array, based on a threshold val
WordEntry getMin(WordEntry* array, WordEntry* minArray, int thresh, int len)
{
    WordEntry min = array[10];
    for (int i = 0; i < len; i++)
    {
        // check if this word is already in the array
        bool found = false;
        for (int j = 0; j < 5; j++)
        {
            if (array[i].word == minArray[j].word)
                found = true;
        }
        // if all of these conditions hold, make this the new min object
        if ((array[i].frequency <= min.frequency) && (array[i].frequency >= thresh) && (found == false))
            min = array[i];
    }
    // return it
    return min;
}