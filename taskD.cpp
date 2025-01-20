/*
Author: Mariam Dobie
Course: CSCI-135
Instructor: Brandon Foster
Assignment: Project1 TaskD

This program is a Hangman game
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <stdlib.h>
using namespace std;

const int g_MAX_WORDS = 1000;
int g_word_count = 0;

string g_words [g_MAX_WORDS];
string g_definitions[g_MAX_WORDS];
string g_pos[g_MAX_WORDS];

void readWords(string filename){
    string word, pos, colon, definition;
    int getSpaces = 0;
    ifstream fin(filename);

    if (fin.fail()){
        cerr << "File cannot be opened for reading." << endl;
        exit(1);
    }

    while (fin >> word >> pos >> colon){
        //gets and stores definition 
        getline(fin, definition);

        //removes spaces from definition string
        while(definition[getSpaces] == ' '){
            getSpaces++;
        }
        definition = definition.substr(getSpaces);

        //storing the strings into global arrays
        g_words[g_word_count] = word;
        g_pos[g_word_count] = pos;  
        g_definitions[g_word_count] = definition;
        g_word_count++;
    }
    fin.close();
}

int getIndex(string word){
    for (int i=0; i < g_word_count; i++){
        if (g_words[i] == word){
            return i;
        }
    }
    return -1;
}

string getDefinition(string word){
    for (int i=0; i < g_word_count; i++){
        if (g_words[i] == word){
            return g_definitions[i];
        }
    }
    return "NOT_FOUND";
}

string getPOS(string word){
    for (int i=0; i < g_word_count; i++){
        if (g_words[i] == word){
            return g_pos[i];
        }
    }
    return "NOT_FOUND";
}

int countPrefix(string prefix){
    int count = 0;

    for (int i = 0; i < g_word_count; i++){
        if (g_words[i].substr(0, prefix.length()) == prefix){
            count ++;
        }
    }
    return count;
}

bool addWord(string word, string definition, string pos){
    if (getIndex(word) != -1 || g_word_count >= g_MAX_WORDS){
        return false;
    }
    g_words[g_word_count] = word;
    g_definitions[g_word_count] = definition;
    g_pos[g_word_count] = pos;
    g_word_count++;

    return true;
}

bool editWord(string word, string definition, string pos){
    if (getIndex(word) != -1){
        g_definitions[getIndex(word)] = definition;
        g_pos[getIndex(word)] = pos;
        return true;
    } else{
        return false;
    }
}

bool removeWord(string word){
    int index = getIndex(word);
    if (index != -1){
        for (int i= index + 1; i < g_word_count; i++){
            g_words[i-1] = g_words[i];
            g_pos[i-1] = g_pos[i];
            g_definitions[i-1] = g_definitions[i];
        }
        g_words[g_word_count - 1] = "";
        g_pos[g_word_count - 1] = "";
        g_definitions[g_word_count - 1] = "";
        g_word_count--;
        return true;
    } else{
        return false;
    }
}
//chooses random words
string getRandomWord() {
    srand((unsigned) time(NULL));
    int index = rand() % g_word_count;
    return g_words[index];
}

string maskWord(string word){
    string dashes;
    for (int i=0; i<word.length(); i++){
        dashes += "_";
    }
    return dashes;
}

int getTries(int difficulty){
    int attempts;
    if (difficulty == 0){
        attempts = 9;
    } else if (difficulty == 1){
        attempts = 7;
    } else if (difficulty == 2){
        attempts = 5;
    }
    return attempts;
}

void printAttempts(int tries, int difficulty){
    difficulty = getTries(difficulty);

    for (int i=0; i < tries; i++){
        cout << "O";
    }
    for (int i =0; i < (difficulty - tries); i++){
        cout << "X";
    }
}

bool revealLetter(string word, char letter, string& current){
    bool found = false;
    for(int i=0; i < word.length(); i++){
        if (word[i] == letter){
            current[i] = letter;
            found = true;
        }
    }
    return found;
}

//game-loop for Hangman
void gameLoop() {
    int difficulty, tries;
    string word, current;
    char letter;
    while (true) {
        cout << "Welcome to Hangman!" << endl;
        cout <<  "0. easy\n1. normal\n2. hard\n3. exit\nChoose a difficulty: ";
        cin >> difficulty;
        while (difficulty < 0 || difficulty > 3) {
            cout <<  "Enough horseplay >_< !\n0. easy\n1. normal\n2. hard\n3. exit\nChoose a difficulty: ";
            cin >> difficulty;
        }
        if (difficulty == 3) {
            cout << "If you're hangry, go grab a bite! See what I did there?" << endl;
            break;
        }
        word = getRandomWord();
        current = maskWord(word);
        tries = getTries(difficulty);
        while (tries != 0) {
            cout << "Life: ";
            printAttempts(tries, difficulty);
            cout << endl << "Word: "<< current << endl;
            cout << "Enter a letter: ";
            cin >> letter;
            if (!revealLetter(word, letter, current)) {
                tries--;
            }
            if (current == word) {
                break;
            }
            if (tries == 2) {
                cout << "The part of speech of the word is "<< getPOS(word) << endl;
            }
            if (tries == 1) {
                cout << "Definition of the word: " << getDefinition(word) << endl;
            }
        }
        if (tries == 0) {
            cout << "The word is \"" << word << "\". Better luck next time! You're getting the ..ahem.. hang of it." << endl;
        }
        else {
            cout << "Congrats!!!" << endl;
        }
    }
}

int main(){
    readWords("dictionary.txt");
    gameLoop();
    return 0;
}