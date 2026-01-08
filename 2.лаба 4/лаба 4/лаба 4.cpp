#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <set>
#include <stdexcept>

using namespace std;

template <typename T>
class MyStack {
private:
    T* data;               // Массив для хранения элементов стека
    int capacity;          // Текущая емкость массива
    int topIndex;          // Индекс вершины стека (начинается с -1)

    // Метод для увеличения размера массива при переполнении
    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < capacity; i++) {
            newData[i] = data[i]; // Копирование элементов
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Конструктор стека с начальной емкостью
    MyStack(int initCapacity = 100) : capacity(initCapacity), topIndex(-1) {
        data = new T[capacity];
    }

    // Деструктор для освобождения памяти
    ~MyStack() {
        delete[] data;
    }

    // Метод для добавления элемента в стек
    void push(const T& value) {
        if (topIndex + 1 >= capacity) {
            resize();
        }
        data[++topIndex] = value;
    }

    // Метод проверки, пуст ли стек
    bool empty() const {
        return topIndex < 0;
    }

    // Метод для удаления элемента с вершины стека
    void pop() {
        if (empty())
            throw runtime_error("Stack underflow");
        topIndex--;
    }

    // Метод для получения элемента с вершины стека без его удаления
    T top() const {
        if (empty())
            throw runtime_error("Stack is empty");
        return data[topIndex];
    }

    // Метод для получения количества элементов в стеке
    int size() const {
        return topIndex + 1;
    }
};

// Функция проверки корректности математического выражения
bool isValidExpression(const string& expression) {
    bool foundAny = false;
    int parentheses_balance = 0;
    bool lastWasOperator = true; // В начале ожидается операнд, чтобы поддержать унарный минус

    for (char c : expression) {
        if (isspace(c))
            continue;
        foundAny = true;
        if (isdigit(c) || isalpha(c) || c == '.') {
            lastWasOperator = false;
        }
        else if (c == '(') {
            if (!lastWasOperator)
                return false;
            parentheses_balance++;
            lastWasOperator = true;
        }
        else if (c == ')') {
            parentheses_balance--;
            if (parentheses_balance < 0)
                return false;
            lastWasOperator = false; // Закрывающая скобка считается операндом
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            if (lastWasOperator && c != '-')
                return false;
            lastWasOperator = true;
        }
        else {
            return false; // Обнаружен недопустимый символ
        }
    }
    return foundAny && (parentheses_balance == 0) && !lastWasOperator;
}

// Функция для извлечения уникальных переменных (букв) из выражения.
set<char> extractVariables(const string& expression) {
    set<char> vars;
    for (char c : expression) {
        if (isalpha(c))
            vars.insert(c);
    }
    return vars;
}

// Класс для обработки математических выражений: преобразование инфиксного выражения в опз и вычисление результата.
class Mather {
    string infixExpr;       // Исходное инфиксное выражение
    string postfixExpr;     // Постфиксная запись (опз)
    unordered_map<char, int> operationPriority; // Карта приоритетов операторов

    // Инициализация приоритетов операторов
    void initializeOperationPriority() {
        operationPriority['('] = 0;
        operationPriority['+'] = 1;
        operationPriority['-'] = 1;
        operationPriority['*'] = 2;
        operationPriority['/'] = 2;
        operationPriority['^'] = 3;
        operationPriority['~'] = 4; // Специальное обозначение для унарного минуса
    }

    // Проверка, является ли символ оператором
    bool isOperator(char c) {
        return operationPriority.find(c) != operationPriority.end();
    }

    // Преобразование инфиксного выражения в постфиксную запись
    string toPostfix() {
        MyStack<char> operators; // Пользовательский стек для хранения операторов
        string postfix = "";
        bool lastWasOperator = true; // Для корректной обработки унарного минуса

        for (size_t i = 0; i < infixExpr.size(); i++) {
            char c = infixExpr[i];
            if (isspace(c))
                continue;
            // Если символ является цифрой или точкой - добавляем к числу
            if (isdigit(c) || c == '.') {
                postfix += c;
                lastWasOperator = false;
            }
            // Если символ буква - переменная
            else if (isalpha(c)) {
                postfix += c;
                lastWasOperator = false;
            }
            // Если символ открывающая скобка - помещаем в стек
            else if (c == '(') {
                operators.push(c);
                lastWasOperator = true;
            }
            // Если закрывающая скобка - извлекаем операторы до открывающей
            else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix += ' ';
                    postfix += operators.top();
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == '(')
                    operators.pop();
                lastWasOperator = false;
            }
            // Если найден оператор
            else if (isOperator(c)) {
                // Если оператор '-' идёт в месте, где ожидается операнд, трактуем как унарный минус
                if (c == '-' && lastWasOperator) {
                    operators.push('~'); // Унарный минус
                }
                else {
                    postfix += ' ';
                    while (!operators.empty() && operationPriority[operators.top()] >= operationPriority[c]) {
                        postfix += operators.top();
                        postfix += ' ';
                        operators.pop();
                    }
                    operators.push(c);
                }
                lastWasOperator = true;
            }
        }
        // Извлекаем оставшиеся операторы из стека
        while (!operators.empty()) {
            postfix += ' ';
            postfix += operators.top();
            operators.pop();
        }
        return postfix;
    }

public:
    // Конструктор принимает инфиксное выражение, инициализирует приоритеты и преобразует в опз
    Mather(string expression) : infixExpr(expression) {
        initializeOperationPriority();
        postfixExpr = toPostfix();
    }

    // Метод для получения постфиксной записи
    string getPostfixExpr() {
        return postfixExpr;
    }

    // Метод вычисления значения выражения в формате опз, используя значения переменных
    double evaluatePostfix(unordered_map<char, double>& variables) {
        MyStack<double> values; // Пользовательский стек для хранения операндов
        string token = "";

        for (size_t i = 0; i < postfixExpr.size(); i++) {
            char c = postfixExpr[i];
            if (isspace(c)) {
                if (!token.empty()) {
                    // Если токен начинается с цифры или точки, преобразуем его в число
                    if (isdigit(token[0]) || token[0] == '.')
                        values.push(stod(token));
                    else if (variables.find(token[0]) != variables.end())
                        values.push(variables[token[0]]);
                    else {
                        cerr << "Error: Unknown variable " << token << endl;
                        return 0.0;
                    }
                    token = "";
                }
            }
            else if (isOperator(c)) {
                // Обрабатываем унарный минус
                if (c == '~') {
                    if (values.empty()) {
                        cerr << "Error: Insufficient operands for unary operator" << endl;
                        return 0.0;
                    }
                    double operand = values.top();
                    values.pop();
                    values.push(-operand);
                }
                else {
                    if (values.size() < 2) {
                        cerr << "Error: Insufficient operands for operator " << c << endl;
                        return 0.0;
                    }
                    double b = values.top();
                    values.pop();
                    double a = values.top();
                    values.pop();
                    switch (c) {
                    case '+': values.push(a + b); break;
                    case '-': values.push(a - b); break;
                    case '*': values.push(a * b); break;
                    case '/':
                        if (b == 0) {
                            cerr << "Error: Division by zero!" << endl;
                            return 0.0;
                        }
                        values.push(a / b); break;
                    case '^': values.push(pow(a, b)); break;
                    }
                }
            }
            else {
                token += c;
            }
        }
        if (values.size() != 1) {
            cerr << "Error: Incorrect postfix expression" << endl;
            return 0.0;
        }
        return values.top();
    }
};

bool inputDouble(double& value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

int main() {
    cout << "Enter a mathematical expression:" << endl;
    string expression;
    getline(cin, expression);

    while (!isValidExpression(expression)) {
        cout << "Invalid expression! Please enter an expression with digits, letters, operators (+, -, *, /, ^) and parentheses (implicit multiplication is not allowed):" << endl;
        getline(cin, expression);
    }

    // Извлечение переменных из введённого выражения
    set<char> vars = extractVariables(expression);
    unordered_map<char, double> variables;
    if (!vars.empty()) {
        cout << "Enter values for variables:" << endl;
        for (char var : vars) {
            double value;
            cout << var << " = ";
            while (!inputDouble(value)) {
                cout << "Invalid input. Please enter a number for variable " << var << " = ";
            }
            variables[var] = value;
        }
    }
    else {
        cout << "Only numeric values detected, no variables present." << endl;
    }

    // Создаем объект класса Mather для обработки выражения
    Mather mather(expression);
    cout << "\nPostfix notation (RPN): " << mather.getPostfixExpr() << endl;

    // Вычисляем и выводим результат выражения
    double result = mather.evaluatePostfix(variables);
    cout << "Result: " << result << endl;

    return 0;
}
