#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
string fileToRead;

struct AnalysisResults 
{
int lineTotal = 0; int commentTotal = 0; int commandTotal = 0; int boldTotal = 0; int italicTotal = 0; int regularTotal = 0; int underlineTotal = 0;
};

string makeLowerCase(string input) 
{
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

string identifyCommand(const string& text) 
{
    if (text.size() >= 3 && text.substr(0, 3) == "<<<") {
        size_t idx = 3;
        while (idx < text.size() && (text[idx] == ' ' || text[idx] == '\t')) idx++;
        return (idx < text.size()) ? "" : "";
    }
    if (text.size() >= 4 && text.substr(0, 4) == ">>> ") {
        return "";
    }
    if (text.size() < 2 || text.substr(0, 2) != ">>") {
        return "";
    }
    string commandExtracted;
    size_t pos = 2;
    while (pos < text.size() && (text[pos] == ' ' || text[pos] == '\t')) pos++;
    while (pos < text.size() && text[pos] != ' ' && text[pos] != '\t') {
        commandExtracted += text[pos++];
    }
    return commandExtracted;
}

AnalysisResults evaluateFile(const string& fileName) 
{
    AnalysisResults stats;
    ifstream fileInput(fileName);
    string textLine;
    int currentLine = 1;

    if (!fileInput) 
    {
        cout << "\nCANNOT OPEN THE FILE " << fileName << endl;
        exit(1);
    }

    if (fileInput.peek() == ifstream::traits_type::eof()) 
    {
        cout << "The File is Empty." << endl;
        exit(1);
    }

    while (getline(fileInput, textLine)) 
    {
        stats.lineTotal++;
        if (textLine.size() >= 2 && textLine.substr(0, 2) == "//") 
        {
            stats.commentTotal++;
            currentLine++;
            continue;
        }
        if ((textLine.size() >= 2 && textLine.substr(0, 2) == ">>") ||
            (textLine.size() >= 3 && textLine.substr(0, 3) == "<<<") ||
            (textLine.size() >= 3 && textLine.substr(0, 3) == ">>>")) 
        {
            string cmd = identifyCommand(textLine);
            if (cmd.empty()) 
            {
                cout << "Error: No command name following a command prompt in line " << currentLine << endl;
                currentLine++;
                continue;
            }
            string lowerCmd = makeLowerCase(cmd);
            if (lowerCmd == "bold" || lowerCmd == "bo") {
                stats.boldTotal++;
                stats.commandTotal++;
            } else if (lowerCmd == "italic" || lowerCmd == "it") 
            {
                stats.italicTotal++;
                stats.commandTotal++;
            }
            else if (lowerCmd == "regular" || lowerCmd == "re") {
                stats.regularTotal++;
                stats.commandTotal++;
            } 
            else if (lowerCmd == "underline" || lowerCmd == "un") 
            {
                stats.underlineTotal++;
                stats.commandTotal++;
            } else 
            {
                cout << "Error: Unrecognizable command name \"" << cmd << "\" in line " << currentLine << endl;
            }
        }
        currentLine++;
    }

    if (currentLine > 1) 
    {
        cout << endl;
    }
    fileInput.close();
    return stats;
}

int main() 
{
    cout << "Enter the file name to read from:" << endl;
    cin >> fileToRead;
    AnalysisResults output = evaluateFile(fileToRead);
    cout << "Total lines: " << output.lineTotal << endl;
    cout << "Commented lines: " << output.commentTotal << endl;
    cout << "Command lines: " << output.commandTotal << endl;
    cout << "Bold commands: " << output.boldTotal << endl;
    cout << "Italic commands: " << output.italicTotal << endl;
    cout << "Regular commands: " << output.regularTotal << endl;
    cout << "Underline commands: " << output.underlineTotal << endl;

    return 0;
}