#include <iostream>
#include <iomanip>

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

typedef double (*TypeFun)(double, int);

double fun(double x, int n = 0) {
    double y = 2 * (pow(cos(x), 2) - 1);
    return y;
}

bool checker = true;

double sum(double x, int n) {
    double s = 0, r = 1.0;
    double d = (-4) * pow(x, 2);
    for (int k = 1; k <= n; ++k) {
        r *= d / ((2 * k - 1) * (2 * k));
        s += r;
    }
    return s;
}

void Out(TypeFun f, double xn, double xk, double h, int n) {
    cout << setw(30) << "\033[34mSum Result\033[0m" << setw(30) << "\033[34mFunc Result\033[0m" << setw(30) << "\033[34mAbs Result\033[0m" << endl;
    for (double x = xn; x <= xk; x += h) {
        double sumResult = f(x, n);
        double funcResult = fun(x);
        double absResult = abs(sumResult - funcResult);
        cout << setw(20) << sumResult << setw(20) << funcResult << setw(20) << absResult << endl;
    }
}

int main() {
    double a, b, h;
    int n;
    char choice;
    do {
        checker = true;
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
        do {
            n = Foolproof<int>("Enter int n > 0: ");
            if (n <= 0) {
                cout << "\033[31m'n' must be greater than 0\033[0m\n";
            }
        } while (n <= 0);

        Out(sum, a, b, h, n);
        
        cout << "\n" << "Do you want one more try? 1 - restart, else - end\n";
        cin >> choice;
    } while (choice == '1');
    return 0;
}
