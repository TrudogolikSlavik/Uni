#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>

using namespace std;

template <class T>
T Foolproof(const char* prompt) {
    T value;
    cout << prompt;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[31mWrong input\033[0m\n" << prompt;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

double f(double x) {
    return pow(sin(x), 2) - 3 * cos(x);
}

// Интерполяция Лагранжа для n точек
double interpolateLagrange(const double* x, const double* y, int n, double xt) {
    double result = 0.0;
    for (int k = 0; k < n; k++) {
        double term = 1.0;
        for (int i = 0; i < n; i++) {
            if (i != k) {
                term *= (xt - x[i]) / (x[k] - x[i]);
            }
        }
        result += term * y[k];
    }
    return result;
}

// Интерполяция Ньютона с использованием разделенных разностей
double interpolateNewton(const double* x, const double* y, int n, double xt) {
    // Выделяем дополнительную память для массива разделённых разностей
    double* divDiff = new double[n];
    for (int i = 0; i < n; i++) {
        divDiff[i] = y[i];
    }

    double result = divDiff[0];
    double term = 1.0;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < n - i; j++) {
            if (fabs(x[j + i] - x[j]) < 1e-12) { // Проверка деления на 0
                cout << "Division by zero encountered in Newton interpolation difference table.\n";
                delete[] divDiff;
                return NAN;
            }
            divDiff[j] = (divDiff[j + 1] - divDiff[j]) / (x[j + i] - x[j]);
        }
        term *= (xt - x[i - 1]);
        result += term * divDiff[0];
    }

    delete[] divDiff;
    return result;
}

// Линейная интерполяция: ищет ближайший интервал и выполняет линейную аппроксимацию
double interpolateLinear(const double* x, const double* y, int n, double xt) {
    // Если xt вне диапазона, выполняется экстраполяция с предупреждением
    if (xt < x[0] || xt > x[n - 1]) {
        cout << "Warning: x is outside the interpolation range. Extrapolation will be performed.\n";
    }
    int i = 0;
    while (i < n - 1 && xt > x[i + 1]) {
        i++;
    }
    if (fabs(x[i + 1] - x[i]) < 1e-12)
        return y[i];

    double t = (xt - x[i]) / (x[i + 1] - x[i]);
    return y[i] + t * (y[i + 1] - y[i]);
}

double interpolateQuadratic(const double* x, const double* y, int n, double xt) {
    if (n < 3) {
        cout << "Error: At least 3 points are required for quadratic interpolation.\n";
        return NAN;
    }
    int i = 0;
    while (i < n - 1 && xt > x[i + 1])
        i++;
    int i0, i1, i2;
    if (i == 0) {
        i0 = 0; i1 = 1; i2 = 2;
    }
    else if (i >= n - 2) {
        i0 = n - 3; i1 = n - 2; i2 = n - 1;
    }
    else {
        i0 = i - 1; i1 = i; i2 = i + 1;
    }

    double L0 = ((xt - x[i1]) * (xt - x[i2])) / ((x[i0] - x[i1]) * (x[i0] - x[i2]));
    double L1 = ((xt - x[i0]) * (xt - x[i2])) / ((x[i1] - x[i0]) * (x[i1] - x[i2]));
    double L2 = ((xt - x[i0]) * (xt - x[i1])) / ((x[i2] - x[i0]) * (x[i2] - x[i1]));

    return y[i0] * L0 + y[i1] * L1 + y[i2] * L2;
}

double approximateFunction(const double* x, const double* y, int n, double xt, int methodChoice) {
    switch (methodChoice) {
    case 1:
        return interpolateLagrange(x, y, n, xt);
    case 2:
        return interpolateNewton(x, y, n, xt);
    case 3:
        return interpolateLinear(x, y, n, xt);
    case 4: // Квадратичная интерполяция
    default:
        return interpolateQuadratic(x, y, n, xt);
    }
}

int main() {
    cout << "Function approximation using interpolation methods.\n";

    double a, b;
    while (true) {
        a = Foolproof<double>("Enter the start of the interval (a): ");
        b = Foolproof<double>("Enter the end of the interval (b): ");
        if (a <= b)
            break;
        else
            cout << "Error: The start of the interval must be less than or equal to the end. Please try again.\n";
    }

    int methodChoice;
    while (true) {
        cout << "\nChoose interpolation method:\n";
        cout << "1: Lagrange interpolation\n";
        cout << "2: Newton interpolation\n";
        cout << "3: Linear interpolation\n";
        cout << "4: Quadratic interpolation (default)\n";
        methodChoice = Foolproof<int>("Enter your choice (1-4) [default is 4]: ");
        if (methodChoice < 1 || methodChoice > 4) {
            cout << "Error: Invalid method selection. Defaulting to Quadratic interpolation.\n";
            methodChoice = 4;
            break;
        }
        else {
            break;
        }
    }

    int m;
    while (true) {
        if (methodChoice == 4) {
            m = Foolproof<int>("Enter the number of points (m), m must be at least 3: ");
            if (m >= 3)
                break;
            else
                cout << "Error: Quadratic interpolation requires at least 3 points. Please try again.\n";
        }
        else {
            m = Foolproof<int>("Enter the number of points (m), m must be at least 2: ");
            if (m >= 2)
                break;
            else
                cout << "Error: Number of points must be at least 2. Please try again.\n";
        }
    }

    // Выделение динамической памяти и генерация таблицы значений функции
    double* x = new double[m];
    double* y = new double[m];
    for (int i = 0; i < m; i++) {
        x[i] = a + i * (b - a) / (m - 1);
        y[i] = f(x[i]);
    }

    while (true) {
        double xt = Foolproof<double>("Enter the point at which to approximate the function: ");
        double approxValue = approximateFunction(x, y, m, xt, methodChoice);

        cout << "\nTable of function values:\n";
        for (int i = 0; i < m; i++) {
            cout << "x[" << i << "] = " << x[i] << ", y[" << i << "] = " << y[i] << "\n";
        }

        cout << "\nApproximated value at x = " << xt << " is: " << approxValue << "\n";
        cout << "Actual value at x = " << xt << " is: " << f(xt) << "\n";
        cout << "Absolute error: " << fabs(approxValue - f(xt)) << "\n";

        cout << "\nDo you want to approximate another point? (y/n): ";
        char ch;
        cin >> ch;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (ch == 'n' || ch == 'N')
            break;
    }

    delete[] x;
    delete[] y;

    return 0;
}
