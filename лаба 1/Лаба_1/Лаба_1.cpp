
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

template <class T>
T Foolproof(const char* ch)
{
    T a; cout << ch; cin >> a;

    while (cin.get() != '\n')
    {
        cin.clear();
        cin.ignore(9999, '\n');
        cout << "\033[31mWrong input!\033[0m\n" << ch; cin >> a;
    }
    return a;
}

int main()
{
    double x = Foolproof<double>("Enter double x: ");
    double y = Foolproof<double>("Enter double y: ");
    double z = Foolproof<double>("Enter double z: ");
    double f, chisl_1, chisl, znam;
    chisl_1 = pow(x - 1, 1. / 3.);
    znam = abs(x - y) * (pow(sin(z), 2) + tan(z));
    if (chisl_1 >= 0)
    {
        if (znam != 0)
        {
            chisl = pow(y + chisl_1, 1. / 4.);
            f = chisl / znam;
            cout << "Result = " << f;
        }
        else
        {
            cout << "\033[31mDivision by zero is undefined\033[0m";
        }
    }
    else
    {
        cout << "\033[31mAn even root of a negative number does not exist on the set of real numbers\033[0m";
    }

}


