#include <iostream>
#include <iomanip>

using namespace std;

template <class T>
T Foolproof(const char* ch) {
    T o; cout << ch; cin >> o;
    while (cin.get() != '\n')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "\033[31mWrong input\033[0m \n" << ch; cin >> o;
    }
    return o;
}

size_t my_strlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void my_strcopy(char* destination, const char* source, size_t num, bool limit) {
    size_t i = 0;
    while (source[i] != '\0' && (!limit || i < num)) {
        destination[i] = source[i];
        i++;
    }
    if (limit) {
        while (i < num) {
            destination[i] = '\0';
            i++;
        }
    }
    else {
        destination[i] = '\0';
    }
}

int main() {
    char input[1000];
    int k;
    char choice;
    cout << "Enter a string: ";
    cin.getline(input, 1000);

    int length = my_strlen(input);

    do {
        k = Foolproof<int>("Enter the value of k: ");
        if (k > length || k <= 0) {
            cout << "\033[31mThe value of k is not within string boundaries. Please try again.\033[0m" << endl;
        }
    } while (k > length || k <= 0);

    int splitPos = k - 1;
    while (splitPos > 0 && input[splitPos] != ' ') {
        splitPos--;
    }
    if (splitPos == 0) {
        cout << "The value of k falls on the first word" << endl;
        cout << "The first substring is absent, so the second one is yor full string" << endl;
    }
    else {
        char firstPart[1000] = { 0 };
        char secondPart[1000] = { 0 };

        my_strcopy(firstPart, input, splitPos, true);
        my_strcopy(secondPart, input + splitPos + 1, my_strlen(input) - splitPos - 1, false);

        cout << "First substring: " << firstPart << endl;
        cout << "Second substring: " << secondPart << endl;
    }

    return 0;
}