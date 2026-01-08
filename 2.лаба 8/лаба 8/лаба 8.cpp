#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

template<typename T>
T getValidInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid value." << endl;
        }
        else {
            return value;
        }
    }
}

double f(double x) {
    return sin(x) * sin(x) - 3 * cos(x);
}

// Method 1: Midpoint rule
double midpointRule(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        // Вычисляем середину каждого отрезка
        double mid = a + (i + 0.5) * h;
        sum += func(mid);
    }
    return sum * h;
}

// Method 2: Trapezoidal rule 
double trapezoidalRule(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.5 * (func(a) + func(b));
    for (int i = 1; i < n; i++) {
        sum += func(a + i * h);
    }
    return sum * h;
}

// Method 3: Simpson's rule 
double simpsonRule(double (*func)(double), double a, double b, int n) {
    // Если число разбиений нечётное, увеличиваем до чётного
    if (n % 2 != 0) {
        n++;
    }
    double h = (b - a) / n;
    double sum = func(a) + func(b);
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 0)
            sum += 2 * func(x);
        else
            sum += 4 * func(x);
    }
    return sum * h / 3;
}

// Method 4: Адаптивный метод с автоматическим выбором шага
double adaptiveSimpsonAux(double (*func)(double), double a, double b, double eps, double whole) {
    double m = (a + b) / 2;
    double left = simpsonRule(func, a, m, 2);
    double right = simpsonRule(func, m, b, 2);
    if (fabs(left + right - whole) < 15 * eps)
        return left + right + (left + right - whole) / 15;
    return adaptiveSimpsonAux(func, a, m, eps / 2, left) + adaptiveSimpsonAux(func, m, b, eps / 2, right);
}

double adaptiveSimpson(double (*func)(double), double a, double b, double eps) {
    double whole = simpsonRule(func, a, b, 2);
    return adaptiveSimpsonAux(func, a, b, eps, whole);
}

// Method 5: Composite Gauss 2-point rule
double gauss2Rule(double (*func)(double), double a, double b) {
    double mid = (a + b) / 2;
    double half = (b - a) / 2;
    // Узлы для 2–точечного правила на отрезке [-1, 1]
    double t1 = -1.0 / sqrt(3.0);
    double t2 = 1.0 / sqrt(3.0);
    return half * (func(mid + half * t1) + func(mid + half * t2));
}

double compositeGauss2(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double sub_a = a + i * h;
        double sub_b = sub_a + h;
        sum += gauss2Rule(func, sub_a, sub_b);
    }
    return sum;
}

// Method 6: Composite Gauss 3-point rule
double gauss3Rule(double (*func)(double), double a, double b) {
    double mid = (a + b) / 2;
    double half = (b - a) / 2;
    double t1 = -sqrt(3.0 / 5.0), t2 = 0.0, t3 = sqrt(3.0 / 5.0);
    // Веса для 3–точечного правила Гаусса: 5/9, 8/9, 5/9
    return half * ((5.0 / 9.0) * func(mid + half * t1) +
        (8.0 / 9.0) * func(mid + half * t2) +
        (5.0 / 9.0) * func(mid + half * t3));
}

double compositeGauss3(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double sub_a = a + i * h;
        double sub_b = sub_a + h;
        sum += gauss3Rule(func, sub_a, sub_b);
    }
    return sum;
}

// Функции для итерационного уточнения результата (для методов с фиксированным n)
typedef double (*IntegrationMethod)(double (*)(double), double, double, int);

double integrateWithEpsilon(IntegrationMethod method, double (*func)(double), double a, double b, double eps, int& nUsed) {
    int n = 4; // начальное число разбиений
    double current = method(func, a, b, n);
    while (true) {
        n *= 2;
        double nextVal = method(func, a, b, n);
        if (fabs(nextVal - current) < eps) {
            nUsed = n;
            return nextVal;
        }
        current = nextVal;
    }
}

// Аналогичная функция для композитных методов (Gauss)
typedef double (*CompositeMethod)(double (*)(double), double, double, int);

double compositeIntegrateWithEpsilon(CompositeMethod method, double (*func)(double), double a, double b, double eps, int& nUsed) {
    int n = 4;
    double current = method(func, a, b, n);
    while (true) {
        n *= 2;
        double nextVal = method(func, a, b, n);
        if (fabs(nextVal - current) < eps) {
            nUsed = n;
            return nextVal;
        }
        current = nextVal;
    }
}

int main() {
    cout << "Welcome to the Numerical Integration Program" << endl;

    double a = getValidInput<double>("Enter the lower limit (a): ");
    double b = getValidInput<double>("Enter the upper limit (b): ");

    if (a > b) {
        cout << "Lower limit is greater than upper limit. Swapping the limits." << endl;
        swap(a, b);
    }

    int mode = 0;
    while (true) {
        mode = getValidInput<int>("Select integration mode:\n1) Fixed number of partitions\n2) Desired precision (epsilon)\nEnter 1 or 2: ");
        if (mode == 1 || mode == 2)
            break;
        else
            cout << "Invalid selection. Please choose 1 or 2." << endl;
    }

    int nInput = 0;
    double eps = 0.0;
    if (mode == 1) {
        nInput = getValidInput<int>("Enter the number of partitions (n): ");
        while (nInput <= 0) {
            cout << "Number of partitions must be greater than 0." << endl;
            nInput = getValidInput<int>("Enter the number of partitions (n): ");
        }
    }
    else { 
        eps = getValidInput<double>("Enter the desired precision (epsilon): ");
        while (eps <= 0) {
            cout << "Epsilon must be greater than 0." << endl;
            eps = getValidInput<double>("Enter the desired precision (epsilon): ");
        }
    }

    cout << "Select integration method:" << endl;
    cout << "1) Midpoint rule" << endl;
    cout << "2) Trapezoidal rule" << endl;
    cout << "3) Simpson's rule" << endl;
    cout << "4) Adaptive Simpson method" << endl;
    cout << "5) Gauss 2-point rule" << endl;
    cout << "6) Gauss 3-point rule" << endl;
    int methodChoice = getValidInput<int>("Enter method number (any other key defaults to Midpoint rule): ");

    double result = 0.0;
    int usedPartitions = 0;

    switch (methodChoice) {
    case 1: // Midpoint rule
        if (mode == 1) {
            result = midpointRule(f, a, b, nInput);
            cout << "Result = " << result << " (using n = " << nInput << " partitions)" << endl;
        }
        else {
            result = integrateWithEpsilon(midpointRule, f, a, b, eps, usedPartitions);
            cout << "Result = " << result << " (midpoint rule with n = " << usedPartitions
                << " partitions to achieve eps = " << eps << ")" << endl;
        }
        break;
    case 2: // Trapezoidal rule
        if (mode == 1) {
            result = trapezoidalRule(f, a, b, nInput);
            cout << "Result = " << result << " (using n = " << nInput << " partitions)" << endl;
        }
        else {
            result = integrateWithEpsilon(trapezoidalRule, f, a, b, eps, usedPartitions);
            cout << "Result = " << result << " (trapezoidal rule with n = " << usedPartitions
                << " partitions to achieve eps = " << eps << ")" << endl;
        }
        break;
    case 3: // Simpson's rule
        if (mode == 1) {
            result = simpsonRule(f, a, b, nInput);
            cout << "Result = " << result << " (using n = " << nInput << " partitions)" << endl;
        }
        else {
            result = integrateWithEpsilon(simpsonRule, f, a, b, eps, usedPartitions);
            cout << "Result = " << result << " (Simpson's rule with n = " << usedPartitions
                << " partitions to achieve eps = " << eps << ")" << endl;
        }
        break;
    case 4: // Adaptive Simpson method
        if (mode == 1) {
            cout << "Adaptive Simpson method requires precision mode." << endl;
            eps = getValidInput<double>("Enter the desired precision (epsilon): ");
            while (eps <= 0) {
                cout << "Epsilon must be greater than 0." << endl;
                eps = getValidInput<double>("Enter the desired precision (epsilon): ");
            }
        }
        result = adaptiveSimpson(f, a, b, eps);
        cout << "Result = " << result << " (adaptive Simpson method with tolerance epsilon = "
            << eps << ")" << endl;
        break;
    case 5: // Gauss 2-point rule
        if (mode == 1) {
            result = compositeGauss2(f, a, b, nInput);
            cout << "Result = " << result << " (using n = " << nInput << " subintervals)" << endl;
        }
        else {
            result = compositeIntegrateWithEpsilon(compositeGauss2, f, a, b, eps, usedPartitions);
            cout << "Result = " << result << " (Gauss 2-point rule with n = " << usedPartitions
                << " subintervals to achieve eps = " << eps << ")" << endl;
        }
        break;
    case 6: // Gauss 3-point rule
        if (mode == 1) {
            result = compositeGauss3(f, a, b, nInput);
            cout << "Result = " << result << " (using n = " << nInput << " subintervals)" << endl;
        }
        else {
            result = compositeIntegrateWithEpsilon(compositeGauss3, f, a, b, eps, usedPartitions);
            cout << "Result = " << result << " (Gauss 3-point rule with n = " << usedPartitions
                << " subintervals to achieve eps = " << eps << ")" << endl;
        }
        break;
    default:  
        if (mode == 1) {
            result = midpointRule(f, a, b, nInput);
            cout << "Result = " << result << " (using n = " << nInput << " partitions)" << endl;
        }
        else {
            result = integrateWithEpsilon(midpointRule, f, a, b, eps, usedPartitions);
            cout << "Result = " << result << " (midpoint rule with n = " << usedPartitions
                << " partitions to achieve eps = " << eps << ")" << endl;
        }
        break;
    }

    return 0;
}
