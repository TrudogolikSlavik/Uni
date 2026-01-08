#include <iostream>
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

void PrintMatrix(int** matrix, int n) {
    cout << "Matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    char choice;
    do {
        int n;
        do {
            n = Foolproof<int>("Enter the size of the matrix: ");
            if (n <= 0) {
                cout << "\033[31mInput error! Please enter a positive integer:\033[0m\n ";
            }
        } while (n <= 0);

        int** matrix = new int* [n];
        for (int i = 0; i < n; ++i) {
            matrix[i] = new int[n];
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                matrix[i][j] = Foolproof<int>("Enter the element of the matrix: ");
            }
        }

        PrintMatrix(matrix, n);

        int sumBelowDiagonal = 0;
        int productAboveDiagonal = 1;

        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                sumBelowDiagonal += matrix[i][j];
            }
        }
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                productAboveDiagonal *= matrix[i][j];
            }
        }

        cout << "Sum of elements below the main diagonal: " << sumBelowDiagonal << endl;
        cout << "Product of elements above the main diagonal: " << productAboveDiagonal << endl;

        for (int i = 0; i < n; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;

        cout << "\n" << "Do you want one more try? 1 - restart, else - end\n";
        cin >> choice;
    } while (choice == '1');

    return 0;
}
