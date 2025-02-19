#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

string toLower(string str) 
{
    string result = str;
    for (int i = 0; i < str.length(); i++) 
    {
        if (str[i] >= 'A' && str[i] <= 'Z') 
        {
            result[i] = str[i] + 32;
        }
    }
    return result;
}

bool isKeyword(string word) 
{
    string keywords[] = 
    { "begin", "end", "if", "else", "while", "for", "break", "continue", "case", "switch", "class", "public", "private", "abstract", "final" };
    word = toLower(word);
    for (int i = 0; i < 15; i++) 
    {
        if (word == keywords[i]) return true;
    }
    return false;
}

bool isLetter(char c) 
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) 
{
    return (c >= '0' && c <= '9');
}

bool isIdentifier(string word) 
{
    if (word.length() == 0 || !isLetter(word[0])) 
    {
        return false;
    }

    for (int i = 1; i < word.length(); i++) 
    {
        if (!isLetter(word[i]) && !isDigit(word[i]) && word[i] != '_') 
        {
            return false;
        }
    }
    return true;
}

char getSpecialWordType(string word) 
{
    if (word.length() == 0) return '\0';

    char first = word[0];
    if (first != '$' && first != '@' && first != '%') 
    {
        return '\0';
    }

    for (int i = 1; i < word.length(); i++) 
    {
        if (!isLetter(word[i]) && !isDigit(word[i]) && word[i] != '_') 
        {
            return '\0';
        }
    }
    return first;
}

bool isInvalidSpecialWord(string word) 
{
    if (word.length() == 0) return false;

    char first = word[0];
    if (first != '$' && first != '@' && first != '%') return false;

    for (int i = 1; i < word.length(); i++) {
        if (!isLetter(word[i]) && !isDigit(word[i]) && word[i] != '_') 
        {
            return true;
        }
    }
    return false;
}

bool isInvalidIdentifier(string word) 
{
    if (word.length() == 0 || !isLetter(word[0])) return false;

    char invalidChars[] = {'.', ',', ':', ')', '\\', '"', ';', '-'};
    for (int i = 0; i < word.length(); i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            if (word[i] == invalidChars[j]) return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        cout << "NO SPECIFIED INPUT FILE NAME.\n";
        return 1;
    }

    string filename = argv[1];
    bool showKeywords = false;
    bool showSpecial = false;
    bool showIdentifiers = false;

    for (int i = 2; i < argc; i++) 
    {
        string arg = argv[i];
        if (arg == "-kw") showKeywords = true;
        if (arg == "-sp") showSpecial = true;
        if (arg == "-id") showIdentifiers = true;
    }

    ifstream inFile(filename);
    if (!inFile.is_open()) 
    {
        cout << "CANNOT OPEN THE FILE " << filename << "\n";
        return 1;
    }

    int totalWords = 0;
    int keywordCount = 0;
    int identifierCount = 0;
    int specialDollar = 0;
    int specialAt = 0;
    int specialPercent = 0;
    map<string, int> keywordOccurrences;
    map<string, int> identifierOccurrences;
    map<string, int> specialWordOccurrences;
    string word;
    string line;
    int lineNumber = 0;
    bool fileIsEmpty = true;

    while (getline(inFile, line)) 
    {
        lineNumber++;
        if (line.length() > 0) fileIsEmpty = false;

        word = "";
        for (int i = 0; i <= line.length(); i++) 
        {
            if (i == line.length() || line[i] == ' ' || line[i] == '\t') 
            {
                if (word.length() > 0) 
                {
                    totalWords++;

                    if (isKeyword(word)) 
                    {
                        keywordCount++;
                        keywordOccurrences[word]++;
                    }
                    else 
                    {
                        char specialType = getSpecialWordType(word);
                        if (specialType == '$') 
                        {
                            specialDollar++;
                            specialWordOccurrences[word]++;
                        }
                        else if (specialType == '@') 
                        {
                            specialAt++;
                            specialWordOccurrences[word]++;
                        }
                        else if (specialType == '%') 
                        {
                            specialPercent++;
                            specialWordOccurrences[word]++;
                        }
                        else if (isIdentifier(word)) 
                        {
                            identifierCount++;
                            identifierOccurrences[word]++;
                        }
                        else 
                        {
                            if (isInvalidIdentifier(word) && showIdentifiers) 
                            {
                                cout << "Invalid Identifier Word at line " << lineNumber << ": " << word << "\n";
                            }
                            else if (isInvalidSpecialWord(word) && showSpecial) 
                            {
                                cout << "Invalid Special Word at line " << lineNumber << ": " << word << "\n";
                            }
                        }
                    }
                    word = "";
                }
            }
            else 
            {
                word += line[i];
            }
        }
    }

    inFile.close();

    if (fileIsEmpty) 
    {
        cout << "File is Empty.\n";
        return 0;
    }

    cout << "Total number of words: " << totalWords << "\n";

    if (showKeywords) 
    {
        cout << "Number of Keywords: " << keywordCount << "\n";
        cout << "List of Keywords and their number of occurrences:\n";
        for (const auto& pair : keywordOccurrences) 
        {
            cout << pair.first << ": " << pair.second << "\n";
        }
    }
    if (showIdentifiers) 
    {
        cout << "Number of Identifiers: " << identifierCount << "\n";
        cout << "List of Identifiers and their number of occurrences:\n";
        for (const auto& pair : identifierOccurrences) 
        {
            cout << pair.first << ": " << pair.second << "\n";
        }
    }
    if (showSpecial) 
    {
        cout << "Number of Special Words Starting with $: " << specialDollar << "\n";
        cout << "Number of Special Words Starting with @: " << specialAt << "\n";
        cout << "Number of Special Words Starting with %: " << specialPercent << "\n";
        cout << "List of Special Words and their number of occurrences:\n";
        for (const auto& pair : specialWordOccurrences) 
        {
            cout << pair.first << ": " << pair.second << "\n";
        }
    }

    return 0;
}