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
int Maxmin(int checker_count, int* array_number) {
    for (int i = 0; i < checker_count; i++) {
        cout << array_number[i] + 1 << " ";
    }
    cout << "\n";
    int imper = 1;
    while (imper) {
        int vybor = Foolproof<int>("");
        bool k = false;
        for (int i = 0; i < checker_count; i++) {
            if (vybor == array_number[i] + 1) {
                k = true;
            }
        }
        if (k) {
            imper--;
            return vybor;
        }
        else {
            cout << "\033[31mWrong index, try again!\033[0m\n";
        }
    }
    return -1;
}

void Run() {
    int array[50], n, max = 0, min = 0, ittermin = 0, ittermax = 0, proizv = 1, checker_min_count = 0, checker_max_count = 0, array_number_min[50], array_number_max[50];
    do {
        n = Foolproof<int>("Enter int size of array more than 0 and less than 51: ");
        if (n > 50 || n <= 0) {
            cout << "\033[31m'n' must be  more than 0 and less than 51!\033[0m\n";
        }
    } while (n > 50 || n <= 0);

    for (int i = 0; i < n; i++) {
        array[i] = Foolproof<int>("Enter int element of array: ");
    }
    cout << "Your array:\n";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << "\n";
    min = array[0];
    max = array[0];
    for (int i = 0; i < n; i++) {
        if (array[i] > max) {
            max = array[i];
            ittermax = i;
        }
        else if (array[i] < min) {
            min = array[i];
            ittermin = i;
        }
    }
    if (ittermin == ittermax) {
        cout << "All the elements of massive are the same";
        return;
    }
    for (int i = 0; i < n; i++) {
        if (array[i] == max) {
            checker_max_count++;
        }
        if (array[i] == min) {
            checker_min_count++;
        }
    }
    if (checker_max_count > 1) {
        int j = 0, m = 0;
        for (int i = 0; i < n; i++) {
            if (array[i] == max) {
                array_number_max[m] = i;
                m++;
            }
        }
        cout << "There are multiple max elements\n";
        cout << "Choose index of max element:\n";
        ittermax = Maxmin(checker_max_count, array_number_max) - 1;
    }
    if (checker_min_count > 1) {
        int j = 0, m = 0;
        for (int i = 0; i < n; i++) {
            if (array[i] == min) {
                array_number_min[m] = i;
                m++;
            }
        }
        cout << "There are multiple min elements\n";
        cout << "Choose index of min element:\n";
        ittermin = Maxmin(checker_min_count, array_number_min) - 1;
    }

    if (abs(ittermin - ittermax) == 1) {
        cout << "max and min elements are nearby";
        return;
    }
    else if (ittermin > ittermax) {
        for (int i = ittermax + 1; i < ittermin; i++) {
            proizv *= array[i];
        }
    }
    else if (ittermin < ittermax) {
        for (int i = ittermin + 1; i < ittermax; i++) {
            proizv *= array[i];
        }
    }
    cout << "Product of elements between min and max: " << proizv;
}

int main() {
    int choice;
    do {
        Run();
        cout << "\nDo you want another try? 1 - Yes, else - no\n";
        cin >> choice;
    } while (choice == 1);
    return 0;
}
