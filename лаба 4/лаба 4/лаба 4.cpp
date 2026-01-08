#include <iostream>
#include <iomanip>
#include <conio.h> 

using namespace std;

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

typedef double (*TypeFun)(double, int); // новый тип данных - TypeFun 

double fun(double x, int n = 0) {
    double y = 2 * (pow(cos(x), 2) - 1);
    return y;
}

double sum(double x, int n) {
    double s = 0, r = 1.0;
    double d = (-4) * pow(x, 2);
    for (int k = 1; k <= n; ++k) {
        r *= d / ((2 * k - 1) * (2 * k));
        s += r;
    }
    return s;
}

void Out(TypeFun f, double xn, double xk, double h, int n, int funcChoice) {
    switch (funcChoice) {
        case 1:
            cout << setw(30) << "\033[32msumRes\033[0m" << endl;
            break;
        case 2:
            cout << setw(30) << "\033[32mfunRes\033[0m" << endl;
            break;
        case 3:
            cout << setw(30) << "\033[32mabsRes\033[0m" << endl;
            break;
        default:
            cout << setw(30) << "\033[32msumRes\033[0m" << setw(30) << "\033[32mfunRes\033[0m" << setw(30) << "\033[32mabsRes\033[0m" << endl;
            break;
    }// table header

    for (double x = xn; x <= xk; x += h) {
        double sumRes = sum(x, n);
        double funRes = fun(x);
        double absRes = abs(sumRes - funRes);
        switch(funcChoice) {
            case 1:
                cout << setw(20) << sumRes << endl;
                break;
            case 2:
                cout << setw(20) << funRes << endl;
                break;
            case 3:
                cout << setw(20) << absRes << endl;
                break;
            default:
                cout << setw(20) << sumRes << setw(20) << funRes << setw(20) << absRes << endl;
                break;
        }

    }
}

int main() {
    

    double a, b, h;
    int n = 0;
    do {
        int funcChoice = 0;
        cout << "Select the function to calculate:\n";
        cout << "1. sum\n";
        cout << "2. func\n";
        cout << "3. absresult\n";
        cout << "Press the corresponding number to select a function.\n";

        funcChoice = _getch() - '0'; // Преобразуем символ в целое число

        switch (funcChoice) {
        case 1:
            cout << "\033[35mYou selected sum\033[0m" << endl;
            break;
        case 2:
            cout << "\033[35mYou selected func\033[0m" << endl;
            break;
        case 3:
            cout << "\033[35mYou selected absresult\033[0m" << endl;
            break;
        default:
            cout << "\033[35mInvalid selection. All three functions are chosen\033[0m" << endl;
            break;
        }

        a = Foolproof<double>("Enter double a: ");
        do {
            b = Foolproof<double>("Enter double b > a: ");
            if (b <= a) {
                cout << "\033[31m'b' must be greater than 'a'\033[0m\n";
            }
        } while (b <= a);
        do {
            h = Foolproof<double>("Enter double h < (b - a): ");
            if (h > b - a || h <= 0) {
                cout << "\033[31m'h' must be less than 'b' - 'a' and more than zero\033[0m\n";
            }
        } while (h > b - a || h <= 0);
        if (funcChoice != 2) {
            do {
                n = Foolproof<int>("Enter int n > 0: ");
                if (n <= 0) {
                    cout << "\033[31m'n' must be greater than 0\033[0m\n";
                }
            } while (n <= 0);
        }
        
        Out(sum, a, b, h, n, funcChoice);

        cout << "\nDo you want one more try? 1 - restart, else - end\n";
    } while (_getch() == '1');
    return 0;
}
