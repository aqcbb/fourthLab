#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <stdio.h>
#include "lab.h"

using namespace std;

const int MAX_WORDS = 50;
const int MAX_LENGTH = 11;
const int MAX_TEXT_LENGTH = MAX_WORDS * MAX_LENGTH;
const int ALPHABET_SIZE = 256;

void inputText(char text[], bool fromFile) {
    if (fromFile) {
        ifstream file("input.txt");
        file.getline(text, MAX_TEXT_LENGTH);
        file.close();
    }
    else {
        cout << "Ââåäèòå ïîñëåäîâàòåëüíîñòü ñëîâ (îêîí÷àíèå - òî÷êà):" << endl;
        cin.get();
        cout.flush();
        cin.getline(text, MAX_TEXT_LENGTH);
    }
}

void editText(char input[], char output[]) {
    bool isFirstLetter = true, lastWasSpace = false, lastWasPunct = false, inWord = false;
    int i = 0, j = 0;
    while (input[i] == ' ') i++;
    while (input[i] != '\0') {
        char c = input[i];
        if (input[i] == '.' && input[i + 1] == '\0') {
            output[j++] = c;
            break;
        }
        if (isalnum(c)) {
            if (isalpha(c)) {
                if (isFirstLetter) {
                    output[j++] = toupper(c);
                    isFirstLetter = false;
                }
                else {
                    output[j++] = tolower(c);
                }
            }
            else {
                output[j++] = c;
            }
            inWord = true;
            lastWasPunct = false;
            lastWasSpace = false;
        }
        else if (ispunct(c) && c != '.' && inWord) {
            output[j++] = c;
            output[j++] = ' ';
            if (c != ',') isFirstLetter = true;
            lastWasPunct = true;
            inWord = false;
            lastWasSpace = false;
        }
        else if (c == '.' || c == '-') {
            if (i > 0 && i < MAX_TEXT_LENGTH - 1 && input[i - 1] == '.' && input[i + 1] == '.') {
                output[j++] = '.';
                output[j++] = '.';
                output[j++] = '.';
            }
            else if (c == '-') {
                output[j++] = c;
                output[j++] = ' ';
            }
        }
        else if (c == ' ') {
            if (inWord && !lastWasSpace) {
                output[j++] = ' ';
                lastWasSpace = true;
                inWord = false;
            }
            lastWasPunct = false;
        }
        i++;
    }
}

void outputResult(char text[]) {
    cout << "Ðåçóëüòàò îáðàáîòêè:" << endl
        << text << endl;
}

int extractWords(char text[], char words[][MAX_LENGTH]) {
    int wordCount = 0;
    int i = 0, j = 0;
    bool inWord = false;
    while (text[i] != '\0' && wordCount < MAX_WORDS) {
        char c = text[i];
        if (isalnum(c)) {
            if (!inWord) {
                inWord = true;
                j = 0;
            }
            if (j < MAX_LENGTH - 1) {
                words[wordCount][j++] = c;
            }
        }
        else if (inWord) {
            words[wordCount][j] = '\0';
            wordCount++;
            inWord = false;
        }
        i++;
    }
    if (inWord && wordCount < MAX_WORDS) {
        words[wordCount][j] = '\0';
        wordCount++;
    }
    return wordCount;
}

bool containsDigit(char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (isdigit(word[i])) return true;
    }
    return false;
}

void printWordsWithoutDigits(char words[][MAX_LENGTH], int wordCount) {
    bool found = false;
    cout << "Ñëîâà, íå ñîäåðæàùèå öèôðû:" << endl;
    for (int i = 0; i < wordCount; i++) {
        if (!containsDigit(words[i])) {
            cout << words[i] << " ";
            found = true;
        }
    }
    if (!found) cout << "Íåò ñëîâ áåç öèôð" << endl;
}

void capitalizeFirstLetter(char input[], char output[]) {
    int i = 0, j = 0;
    bool newWord = true, lastWasSpace = false, lastWasPunct = false, inWord = false;
    while (input[i] == ' ') i++;
    while (input[i] != '\0') {
        char c = input[i];
        if (input[i] == '.' && input[i + 1] == '\0') {
            output[j++] = '.';
            break;
        }
        if (isalnum(c)) {
            if (isalpha(c)) {
                if (newWord) {
                    output[j++] = toupper(c);
                }
                else {
                    output[j++] = tolower(c);
                }
            }
            else output[j++] = c;
            newWord = false;
            inWord = true;
            lastWasPunct = false;
            lastWasSpace = false;
        }
        else if (ispunct(c) && c != '.' && inWord) {
            output[j++] = c;
            if (c != ',') newWord = true;
            lastWasPunct = true;
            inWord = false;
            lastWasSpace = false;
            newWord = true;
        }
        else if (c == '.' || c == '-') {
            if (i > 0 && i < MAX_TEXT_LENGTH - 1 && input[i - 1] == '.' && input[i + 1] == '.') {
                output[j++] = '.';
                output[j++] = '.';
                output[j++] = '.';
                lastWasSpace = false;
            }
            else if (c == '-') {
                output[j++] = c;
                output[j++] = ' ';
                lastWasSpace = true;
            }
            newWord = true;
            lastWasPunct = true;
            inWord = false;
        }
        else if (c == ' ') {
            if (!lastWasSpace) {
                output[j++] = ' ';
                lastWasSpace = true;
                newWord = true;
                inWord = false;
            }
            lastWasPunct = false;
        }
        i++;
    }
}

int getStringLength(char text[]) {
    int len = 0;
    for (int i = 0; text[i] != '\0'; i++) len++;
    return len;
}

void preprocessBadChar(char pattern[], int patternLen, int badChar[]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) badChar[i] = patternLen;
    for (int i = 0; i < patternLen - 1; i++) badChar[(unsigned char)pattern[i]] = patternLen - 1 - i;
}

void boyerMooreSearch(char text[], char pattern[]) {
    int stringLen = getStringLength(text);
    int patternLen = getStringLength(pattern);
    bool found = false;
    if (patternLen == 0 || patternLen > stringLen) return;
    int badChar[ALPHABET_SIZE];
    preprocessBadChar(pattern, patternLen, badChar);
    int shift = 0;
    while (shift <= stringLen - patternLen) {
        int j = patternLen - 1;
        while (j >= 0 && pattern[j] == text[shift + j]) j--;
        if (j < 0) {
            cout << "Íàéäåíî íà ïîçèöèè: " << shift << endl;
            found = true;
            shift += (shift + patternLen < stringLen) ? badChar[(unsigned char)text[shift + patternLen]] : 1;
        }
        else {
            char badCharInText = text[shift + j];
            shift += max(1, badChar[(unsigned char)badCharInText] - (patternLen - 1 - j));
        }
    }
    if (!found) cout << "Ïîäñòðîêà íå íàéäåíà" << endl;
}

void searchSubstring(char text[], char pattern[]) {
    int stringLength = getStringLength(text);
    int substringLength = getStringLength(pattern);
    bool found = false;
    for (int i = 0; i <= stringLength - substringLength; i++) {
        int j;
        for (j = 0; j < substringLength; j++) {
            if (text[i + j] != pattern[j]) break;
        }
        if (j == substringLength) {
            cout << "Ïîäñòðîêà íàéäåíà íà ïîçèöèè " << i << endl;
            found = true;
        }
    }
    if (!found) cout << "Ïîäñòðîêà íå íàéäåíà" << endl;
}

int main() {
    char input[MAX_TEXT_LENGTH] = {0}, output[MAX_TEXT_LENGTH] = {0};
    char words[MAX_WORDS][MAX_LENGTH];
    char substring[MAX_TEXT_LENGTH] = { 0 };
    int choice;
    cout << endl << "Çàäàíèå 1." << endl;
    cout << "Âûáåðèòå ñïîñîá ââîäà:" << endl
    << "1) Ñ êëàâèàòóðû" << endl
    << "2) Ñ ôàéëà" << endl;
    cin >> choice;
    if (choice == 1 || choice == 2) {
        bool fromFile = (choice == 2);
        cin.clear();
        cin.sync();
        inputText(input, fromFile);
        editText(input, output);
        cout << endl << "Çàäàíèå 2." << endl;
        outputResult(output);
        cout << endl
            << "Çàäàíèå 3." << endl; // 3 Âûâåñòè íà ýêðàí ñëîâà ïîñëåäîâàòåëüíîñòè, íå ñîäåðæàùèå öèôð.
        int wordCount = extractWords(output, words);
        printWordsWithoutDigits(words, wordCount);
        cout << endl << endl << 
            "Çàäàíèå 4." << endl; // 1 Âûâåñòè íà ýêðàí òó æå ïîñëåäîâàòåëüíîñòü, çàìåíèâ âî âñåõ ñëîâàõ ïåðâóþ áóêâó ñîîòâåòñòâóþùåé ïðîïèñíîé áóêâîé.
            capitalizeFirstLetter(input, output);
            outputResult(output);
        cout << endl;
        cout << "Çàäàíèå 5." << endl
            << "Ââåäèòå ïîäñòðîêó, êîòîðóþ íóæíî íàéòè:" << endl;
        cin.get();
        cout.flush();
        cin.getline(substring, MAX_TEXT_LENGTH);
        cout << "Ëèíåéíûé ïîèñê:" << endl;
        searchSubstring(output, substring);
        cout << "Àëãîðèòì Áîéåðà-Ìóðà:" << endl;
        boyerMooreSearch(output, substring);
        cout << endl;
        main(); 
    }
    else {
        cout << "Âûáðàí íåñóùåñòâóþùèé ïóíêò. Âûáåðèòå äðóãîé ïóíêò" << endl;
        main();
    }
}

