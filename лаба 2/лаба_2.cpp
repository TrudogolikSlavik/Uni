#include <iostream>
#include <conio.h>

using namespace std;

template <class T>
T Foolproof(const char* ch)
{
    T a; cout << ch; cin >> a;
    while (cin.get() != '\n')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[31mWrong input!\033[0m\n" << ch; cin >> a;
    }
    return a;
}


int main() {
    double z = Foolproof<double>("Enter double z: ");
    double n = Foolproof<double>("Enter double n: ");
    double k = Foolproof<double>("Enter double k: ");
    double m = Foolproof<double>("Enter double m: ");
    double x, fx;


    if (z > 1) {
        x = z;
        cout << "Condition: z > 1, x = z" << endl;
    }
    else {
        x = pow(z, 2) + 1;
        cout << "Condition: z <= 1, x = z^2 + 1" << endl;
    }

    cout << "Choose the function: 1 - 2*x\t 2 - x^3\t else - x/3" << endl;
    switch (_getch())
    {
    case '1':
        fx = 2 * x;
        cout << "Your choice: f(x) = 2x" << endl;
        break;
    case '2':
        fx = pow(x, 3);
        cout << "Your choice: f(x) = x^3" << endl;
        break;
    default:
        fx = x / 3;
        cout << "Your choice: f(x) = x/3 ( Default !)" << endl;
        break;
    }

    if (m * x <= 0)
    {
        cout << "\033[31mDomain of definition log(a) = (0, +inf)!\033[0m";
    }
    else
    {
        double y = sin(n * fx) + cos(k * x) + log(m * x);
        cout << "Result:" << y;
    }
    
    
}