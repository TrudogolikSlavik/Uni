#include <iostream>
#include <limits>
using namespace std;

template <class T>
T Foolproof(const char* prompt) {
    T o;
    cout << prompt;
    cin >> o;
    while (cin.get() != '\n') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[31mWrong input\033[0m \n" << prompt;
        cin >> o;
    }
    return o;
}

// Iterative method for converting a number to a given base.
char* convertIterative(int N, int base, const char* digits) {
    bool negative = false;
    unsigned int num;

    if (N < 0) {
        negative = true;
        num = static_cast<unsigned int>(-N);
    }
    else {
        num = static_cast<unsigned int>(N);
    }

    int count = 0;
    if (num == 0) {
        count = 1;
    }
    else {
        unsigned int tmp = num;
        while (tmp > 0) {
            count++;
            tmp /= base;
        }
    }

    // Add extra space for the negative sign if needed.
    int total = count + (negative ? 1 : 0);
    char* result = new char[total + 1];  // +1 for the null terminator
    result[total] = '\0';

    int index = total - 1;
    if (num == 0) {
        result[index--] = '0';
    }
    else {
        while (num > 0) {
            result[index--] = digits[num % base];
            num /= base;
        }
    }

    if (negative)
        result[0] = '-';

    return result;
}

void convertRecursiveHelper(unsigned int num, int base, char* buffer, int& idx, const char* digits) {
    if (num >= static_cast<unsigned int>(base)) {
        convertRecursiveHelper(num / base, base, buffer, idx, digits);
    }
    buffer[idx++] = digits[num % base];
}

// Recursive method for converting a number to a given base.
char* convertRecursive(int N, int base, const char* digits) {
    bool negative = false;
    unsigned int num;

    if (N < 0) {
        negative = true;
        num = static_cast<unsigned int>(-N);
    }
    else {
        num = static_cast<unsigned int>(N);
    }

    int count = 0;
    if (num == 0) {
        count = 1;
    }
    else {
        unsigned int tmp = num;
        while (tmp > 0) {
            count++;
            tmp /= base;
        }
    }

    int total = count + (negative ? 1 : 0);
    char* result = new char[total + 1]; // +1 for the null terminator
    int idx = 0;

    if (negative) {
        result[idx++] = '-';
    }

    if (num == 0) {
        result[idx++] = '0';
    }
    else {
        convertRecursiveHelper(num, base, result, idx, digits);
    }

    result[total] = '\0';
    return result;
}

bool myStrCmp(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' || s2[i] != '\0') {
        if (s1[i] != s2[i])
            return false;
        i++;
    }
    return true;
}

int main() {
    const char* baseDigits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int N = Foolproof<int>("Enter decimal number N: ");
    int p;
    do {
        p = Foolproof<int>("Enter target base p (2-36): ");
        if (p < 2 || p > 36) {
            cout << "\033[31mError: Base must be in the range of 2 to 36.\033[0m\n";
        }
    } while (p < 2 || p > 36);

    char* iterativeResult = convertIterative(N, p, baseDigits);
    char* recursiveResult = convertRecursive(N, p, baseDigits);

    cout << "Iterative method: " << iterativeResult << endl;
    cout << "Recursive method: " << recursiveResult << endl;

    if (myStrCmp(iterativeResult, recursiveResult)) {
        cout << "The results of both methods are identical." << endl;
    }
    else {
        cout << "The results of the methods differ!" << endl;
    }

    delete[] iterativeResult;
    delete[] recursiveResult;

    return 0;
}
