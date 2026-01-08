#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

// Структура для хранения результата поиска корня
struct RootResult {
    double root;       // найденный корень
    double fValue;     // значение функции в найденном корне
    int iterations;    // количество итераций
    bool converged;    // признак сходимости метода
};

template <typename T>
T inputValue(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (cin.get() == '\n')
                return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cerr << "Invalid input. Please enter a valid number." << endl;
    }
}

// Функция f(x) = sin^2(x) - 3*cos(x)
double f(double x) {
    return pow(sin(x), 2) - 3 * cos(x);
}

// Производная функции f(x)
double fprime(double x) {
    return sin(x) * (2 * cos(x) + 3);
}

// Улучшенное начальное приближение для метода Ньютона.
// Выбираем ту точку из {a, (a+b)/2, b}, где |f(x)| минимально.
// Если в выбранной точке производная слишком мала, используем середину отрезка.
double chooseNewtonInitial(double a, double b) {
    double mid = (a + b) / 2.0;
    double fa = fabs(f(a));
    double fmid = fabs(f(mid));
    double fb = fabs(f(b));
    double x0 = a;
    double minVal = fa;
    if (fmid < minVal) { minVal = fmid; x0 = mid; }
    if (fb < minVal) { minVal = fb;  x0 = b; }
    if (fabs(fprime(x0)) < numeric_limits<double>::epsilon()) {
        if (fabs(fprime(mid)) >= numeric_limits<double>::epsilon())
            x0 = mid;
    }
    return x0;
}

// Метод секущих 
// Использует концы интервала в качестве начальных приближений.
RootResult secantMethod(double x0, double x1, double epsilon, int maxIterations) {
    int iterations = 0;
    while (fabs(x1 - x0) >= epsilon && iterations < maxIterations) {
        double f0 = f(x0), f1 = f(x1);
        if (fabs(f1 - f0) < numeric_limits<double>::epsilon()) {
            cerr << "Error: Division by zero in Secant method." << endl;
            return { x1, f(x1), iterations, false };
        }
        double x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        x0 = x1;
        x1 = x2;
        iterations++;
    }
    bool converged = (fabs(f(x1)) < epsilon);
    return { x1, f(x1), iterations, converged };
}

// Метод простой итерации 
// Итерационная функция: phi(x) = x + lambda * f(x)
RootResult simpleIterationMethod(double a, double b, double epsilon, int maxIterations, double lambda) {
    double x = (a + b) / 2.0; // Начальное приближение — середина отрезка
    int iterations = 0;
    // Проверка сходимости: |phi'(x)| = |1 + lambda * f'(x)| < 1
    double phi_deriv = fabs(1 + lambda * fprime(x));
    if (phi_deriv >= 1) {
        cout << "Warning: Iterative method may not converge, |phi'(x)| = "
            << phi_deriv << " >= 1." << endl;
    }
    while (fabs(f(x)) >= epsilon && iterations < maxIterations) {
        double x_new = x + lambda * f(x);
        if (fabs(x_new - x) < epsilon) {
            x = x_new;
            iterations++;
            break;
        }
        x = x_new;
        iterations++;
    }
    bool converged = (fabs(f(x)) < epsilon);
    return { x, f(x), iterations, converged };
}

// Метод Ньютона 
RootResult newtonMethod(double a, double b, double epsilon, int maxIterations) {
    double x = chooseNewtonInitial(a, b);
    int iterations = 0;
    while (fabs(f(x)) >= epsilon && iterations < maxIterations) {
        double d = fprime(x);
        if (fabs(d) < numeric_limits<double>::epsilon()) {
            cerr << "Error: Derivative near zero in Newton's method at x = " << x << endl;
            return { x, f(x), iterations, false };
        }
        double x_next = x - f(x) / d;
        if (fabs(x_next - x) < epsilon) {
            x = x_next;
            iterations++;
            break;
        }
        x = x_next;
        iterations++;
    }
    bool converged = (fabs(f(x)) < epsilon);
    return { x, f(x), iterations, converged };
}

// Метод Вегстейна 
// Использует итерационную функцию: g(x) = x + lambda * f(x)
RootResult wegsteinMethod(double a, double b, double epsilon, int maxIterations, double lambda) {
    auto g = [lambda](double x) { return x + lambda * f(x); };
    double x0 = (a + b) / 2.0;
    double x1 = g(x0);
    int iterations = 1;
    double s0 = g(x0) - x0;
    double s1 = g(x1) - x1;
    while (fabs(x1 - x0) >= epsilon && iterations < maxIterations) {
        double denominator = s1 - s0;
        if (fabs(denominator) < numeric_limits<double>::epsilon()) {
            cerr << "Error: Division by zero in Wegstein method (denominator = 0)." << endl;
            return { x1, f(x1), iterations, false };
        }
        double x2 = g(x1) - (s1 * s1) / denominator;
        x0 = x1;
        x1 = x2;
        s0 = s1;
        s1 = g(x1) - x1;
        iterations++;
        if (fabs(f(x1)) < epsilon)
            break;
    }
    bool converged = (fabs(f(x1)) < epsilon);
    return { x1, f(x1), iterations, converged };
}


// Функция для поиска корней на интервале [a, b] с шагом h.
void findRoots(double a, double b, double h, double epsilon, int maxIterations, int methodChoice, double lambda) {
    for (double x = a; x <= b - h; x += h) {
        if (f(x) * f(x + h) < 0) {
            cout << "\nInterval [" << x << ", " << x + h << "]" << endl;
            RootResult res;
            switch (methodChoice) {
            case 1:
                res = secantMethod(x, x + h, epsilon, maxIterations);
                break;
            case 2:
                res = simpleIterationMethod(x, x + h, epsilon, maxIterations, lambda);
                break;
            case 3:
                res = newtonMethod(x, x + h, epsilon, maxIterations);
                break;
            case 4:
                res = wegsteinMethod(x, x + h, epsilon, maxIterations, lambda);
                break;
            default:
                cout << "Invalid method choice." << endl;
                return;
            }
            if (res.converged) {
                cout << "Root found: x = " << res.root
                    << ", f(x) = " << res.fValue
                    << ", iterations = " << res.iterations << endl;
            }
            else {
                cout << "Method did not converge in interval [" << x << ", " << x + h << "]." << endl;
            }
        }
    }
}

int main() {
    double a, b, h, epsilon;
    double lambda = -0.1; // Коэффициент для методов простой итерации и метода Вегстейна

    while (true) {
        a = inputValue<double>("Enter the value for a: ");
        b = inputValue<double>("Enter the value for b: ");
        if (a < b) {
            break;
        }
        else {
            cerr << "Invalid interval. 'a' must be less than 'b'. Please try again." << endl;
        }
    }

    while (true) {
        h = inputValue<double>("Enter the step size h: ");
        if (h > 0) {
            break;
        }
        else {
            cerr << "Invalid input. Step size h must be positive. Please try again." << endl;
        }
    }

    while (true) {
        epsilon = inputValue<double>("Enter the precision epsilon: ");
        if (epsilon > 0) {
            break;
        }
        else {
            cerr << "Invalid input. Precision epsilon must be positive. Please try again." << endl;
        }
    }

    int maxIterations = inputValue<int>("Enter the maximum number of iterations: ");

    cout << "\nChoose the root finding method:" << endl;
    cout << "1. Secant method" << endl;
    cout << "2. Simple iteration method" << endl;
    cout << "3. Newton's method (tangent method)" << endl;
    cout << "4. Wegstein's method" << endl;
    int methodChoice = inputValue<int>("Enter method number (1-4): ");

    cout << "\n--- Searching for roots in the interval [" << a << ", " << b << "] ---" << endl;
    findRoots(a, b, h, epsilon, maxIterations, methodChoice, lambda);

    return 0;
}
