#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

// Function to check if a character is a valid English letter or space
bool isValidChar(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ';
}

// Function to input a string with only English letters and spaces
void Input(char* s) {
    int i = 0;
    while (1) {
        s[i] = _getch();
        if (s[i] == 13) break; // Enter key
        if (!isValidChar(s[i])) {
            continue;
        }
        cout << s[i];
        i++;
    }
    s[i] = '\0';
}

// Function to split a string into words
int splitWords(char* str, char words[][100]) {
    int wordCount = 0;
    int wordIndex = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == ' ') {
            if (wordIndex != 0) {
                words[wordCount++][wordIndex] = '\0';
                //wordCount++;
                wordIndex = 0;
            }
        }
        else {
            words[wordCount][wordIndex++] = str[i];
        }
    }
    if (wordIndex != 0) {
        words[wordCount++][wordIndex] = '\0';
        //wordCount++;
    }
    return wordCount;
}

char toLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

// Function to compare two strings (for sorting)
bool compareWords(char* a, char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        char lowerA = toLower(a[i]);
        char lowerB = toLower(b[i]);
        if (lowerA < lowerB) return true;
        if (lowerA > lowerB) return false;
        i++;
    }
    return a[i] == '\0' && b[i] != '\0';
}

// Function to copy a string with a limit
void my_strcopy(char* destination, const char* source) {
    size_t i = 0;
    while (source[i] != '\0' ) {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

// Function to sort words in alphabetical order
void sortWords(char words[][100], int wordCount) {
    for (int i = 0; i < wordCount - 1; ++i) {
        for (int j = i + 1; j < wordCount; ++j) {
            if (compareWords(words[j], words[i])) {
                char temp[100];
                my_strcopy(temp, words[i]);
                my_strcopy(words[i], words[j]);
                my_strcopy(words[j], temp);
            }
        }
    }
}

template <class T>
T Foolproof(const char* ch) {
    T o; cout << ch; cin >> o;
    while (cin.get() != '\n') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[31mWrong input\033[0m \n" << ch; cin >> o;
    }
    return o;
}

// Function to get the length of a string
size_t my_strlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void task1() {
    const int MAX_LENGTH = 1000;
    char input[MAX_LENGTH];
    char words[100][100];

    cout << "\nEnter a string with English letters and spaces only\n";
    Input(input);

    int wordCount = splitWords(input, words);
    sortWords(words, wordCount);

    cout << "\nWords in alphabetical order:" << endl;
    for (int i = 0; i < wordCount; ++i) {
        cout << words[i] << endl;
    }
}

void task2() {
    char input[1000];
    int k;

    cout << "Enter a string: ";
    cin.getline(input, 1000);

    int length = my_strlen(input);

    do {
        k = Foolproof<int>("Enter the value of k: ");
        if (k > length || k <= 0) {
            cout << "\033[31mThe value of k is not within string boundaries. Please try again.\033[0m" << endl;
        }
    } while (k > length || k <= 0);

    int splitPos = k;
    while (splitPos > 0 && input[splitPos] != ' ') {
        splitPos--;
    }

    if (splitPos == 0) {
        cout << "The value of k falls on the first word" << endl;
        cout << "The first substring is absent, so the second one is your full string" << endl;
    }
    else {
        char firstPart[1000] = { 0 };
        char secondPart[1000] = { 0 };

        my_strcopy(firstPart, input);
        my_strcopy(secondPart, input + splitPos + 1);

        cout << "First substring: " << firstPart << endl;
        cout << "Second substring: " << secondPart << endl;
    }
}

int main() {
    int choice;
    cout << "Choose a task:\n1. Sort words in alphabetical order\n2. Split string at position k (default)\n";
    cout << "Enter your choice(1 - the first task, else - the second one):\n ";

    if (_getch() == 1) {
        cout << "1. Sort words in alphabetical order\n";
        task1();
    }
    else {
        cout << "2. Split string at position k\n";
        task2();
    }

    //system("pause");
    return 0;
}