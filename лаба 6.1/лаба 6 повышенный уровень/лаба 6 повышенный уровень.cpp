#include <iostream>
#include <conio.h> 
using namespace std;

template <class T>
T Foolproof(const char* ch) {
    T o; cout << ch; cin >> o;
    while (cin.get() != '\n')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[31mWrong input\033[0m \n" << ch; cin >> o;
    }
    return o;
}

int getInput(const char* h) {
    cout << h;
    char value;
    while (true) {
        value = _getch();
        if (value == '0' || value == '1') {
            cout << value;
            return value - '0';
        }
    }
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
void TransposeMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void ReverseRows(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[i][n - j - 1];
            matrix[i][n - j - 1] = temp;
        }
    }
}

void rotateMatrix(int** matrix, int n) {
    TransposeMatrix(matrix, n);
    ReverseRows(matrix, n);
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
                matrix[i][j] = getInput("Enter the element of the matrix (0 or 1): ");
                cout << "\n";
            }
        }

        PrintMatrix(matrix, n);

        rotateMatrix(matrix, n);

        cout << "Rotated ";

        PrintMatrix(matrix, n);

        for (int i = 0; i < n; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;

        cout << "\n" << "Do you want one more try? 1 - restart, else - end\n";
        cin >> choice;
    } while (choice == '1');

    return 0;
}

