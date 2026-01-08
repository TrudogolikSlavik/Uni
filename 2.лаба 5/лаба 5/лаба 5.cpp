#include <iostream>
#include <string>
#include <limits>
using namespace std;

// Структура узла дерева
struct TreeNode {
    int key;
    string data;
    TreeNode* left;
    TreeNode* right;
    int height;

    TreeNode(int k, const string& d) : key(k), data(d), left(nullptr), right(nullptr), height(1) {}
};

// Получение высоты узла
int getHeight(TreeNode* node) {
    return node ? node->height : 0;
}

// Получение баланса узла (разность высот левого и правого поддеревьев)
int getBalance(TreeNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Возврат максимального значения из двух чисел
int maxVal(int a, int b) {
    return (a > b) ? a : b;
}

// Правый поворот узла
TreeNode* rotateRight(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Левый поворот узла
TreeNode* rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Функция вставки узла 
TreeNode* insert(TreeNode* root, int key, const string& data) {
    if (!root)
        return new TreeNode(key, data);

    if (key < root->key)
        root->left = insert(root->left, key, data);
    else if (key > root->key)
        root->right = insert(root->right, key, data);
    else {
        // Дубликат обнаружен, выводим сообщение и возвращаем существующий узел
        cout << "Duplicate key entered." << endl;
        return root;
    }

    // Обновление высоты узла
    root->height = maxVal(getHeight(root->left), getHeight(root->right)) + 1;

    int balance = getBalance(root);

    // Левое-Левое
    if (balance > 1 && key < root->left->key)
        return rotateRight(root);

    // Правое-Правое
    if (balance < -1 && key > root->right->key)
        return rotateLeft(root);

    // Левое-Правое
    if (balance > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Правое-Левое
    if (balance < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Функция поиска узла по ключу
TreeNode* search(TreeNode* root, int key) {
    if (!root || root->key == key)
        return root;
    if (key < root->key)
        return search(root->left, key);
    return search(root->right, key);
}

// Функция поиска узла с минимальным ключом
TreeNode* findMinimum(TreeNode* root) {
    while (root && root->left)
        root = root->left;
    return root;
}

// Функция поиска узла с максимальным ключом
TreeNode* findMaximum(TreeNode* root) {
    while (root && root->right)
        root = root->right;
    return root;
}

// Функция удаления узла по ключу
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Узел с 0 или 1 ребенком
        if (!root->left || !root->right) {
            TreeNode* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        }
        else {
            // Узел с двумя детьми: нахождение минимального узла в правом поддереве
            TreeNode* temp = findMinimum(root->right);
            root->key = temp->key;
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // Обновление высоты узла
    root->height = maxVal(getHeight(root->left), getHeight(root->right)) + 1;
    int balance = getBalance(root);

    // Балансировка дерева
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Прямой (Preorder) обход дерева
void preorder(TreeNode* root) {
    if (root) {
        cout << root->key << " - " << root->data << "\n";
        preorder(root->left);
        preorder(root->right);
    }
}

// Обратный (Postorder) обход дерева
void postorder(TreeNode* root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        cout << root->key << " - " << root->data << "\n";
    }
}

// Симметричный (Inorder) обход дерева
void inorder(TreeNode* root) {
    if (root) {
        inorder(root->left);
        cout << root->key << " - " << root->data << "\n";
        inorder(root->right);
    }
}

// Графический вывод дерева 
void printTreeGraphical(TreeNode* root, const string& prefix = "", bool isTail = true) {
    if (!root)
        return;
    // Для правого поддерева добавляем префикс: если это последний узел, добавляем "    ", иначе "|   "
    if (root->right)
        printTreeGraphical(root->right, prefix + (isTail ? "    " : "|   "), false);
    // Выводим текущий узел с ASCII-символами веток: "`-- " если узел последний, иначе "/-- "
    cout << prefix << (isTail ? "`-- " : "/-- ") << root->key << " (" << root->data << ")" << "\n";
    // Аналогично для левого поддерева
    if (root->left)
        printTreeGraphical(root->left, prefix + (isTail ? "    " : "|   "), true);
}

// Функция подсчёта узлов в дереве
int countNodes(TreeNode* root) {
    if (!root)
        return 0;
    return countNodes(root->left) + countNodes(root->right) + 1;
}

// Сохранение узлов дерева в массиве (inorder обход)
void storeInorder(TreeNode* root, TreeNode** arr, int& index) {
    if (!root)
        return;
    storeInorder(root->left, arr, index);
    arr[index++] = root;
    storeInorder(root->right, arr, index);
}

// Построение идеально сбалансированного дерева из отсортированного массива узлов
TreeNode* buildBalancedTree(TreeNode** arr, int start, int end) {
    if (start > end)
        return nullptr;
    int mid = (start + end) / 2;
    TreeNode* node = arr[mid];
    node->left = buildBalancedTree(arr, start, mid - 1);
    node->right = buildBalancedTree(arr, mid + 1, end);
    node->height = maxVal(getHeight(node->left), getHeight(node->right)) + 1;
    return node;
}

// Перебалансировка дерева
TreeNode* rebalanceTree(TreeNode* root) {
    int n = countNodes(root);
    if (n == 0)
        return root;
    TreeNode** arr = new TreeNode * [n];
    int index = 0;
    storeInorder(root, arr, index);
    TreeNode* newRoot = buildBalancedTree(arr, 0, n - 1);
    delete[] arr;
    return newRoot;
}

// Обмен данных между узлами с минимальным и максимальным ключами
void swapMinMax(TreeNode* root) {
    if (!root)
        return;
    TreeNode* minNode = findMinimum(root);
    TreeNode* maxNode = findMaximum(root);
    if (minNode && maxNode) {
        string temp = minNode->data;
        minNode->data = maxNode->data;
        maxNode->data = temp;
    }
}

template <typename T>
T Foolproof(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again." << endl;
        }
    }
}

// Функция ввода целочисленного ключа
int inputKey() {
    return Foolproof<int>("Enter integer key: ");
}

// Функция ввода строковых данных
string inputData() {
    string data;
    cout << "Enter string data: ";
    getline(cin, data);
    return data;
}

// Рекурсивное освобождение памяти дерева 
void destroyTree(TreeNode* root) {
    if (!root)
        return;
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
}

// Функция безопасной вставки записи
TreeNode* safeInsert(TreeNode* root, int key, const string& data) {
    // Если запись с данным ключом уже существует, запрашиваем подтверждение замены
    TreeNode* existing = search(root, key);
    if (existing != nullptr) {
        cout << "Key " << key << " already exists with data: " << existing->data << "\n";
        cout << "Do you want to replace the data? (Y/N): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 'Y' || choice == 'y') {
            existing->data = data;
            cout << "Data replaced successfully.\n";
            return root;
        }
        else {
            cout << "Please enter a new key.\n";
            int newKey = inputKey();
            string newData = inputData();
            return safeInsert(root, newKey, newData);
        }
    }
    else {
        return insert(root, key, data);
    }
}

int main() {
    TreeNode* root = nullptr;
    int numRecords = Foolproof<int>("How many records do you want to enter? ");

    // Инициализация дерева
    for (int i = 0; i < numRecords; ++i) {
        cout << "\nRecord #" << (i + 1) << ":\n";
        int key = inputKey();
        string data = inputData();
        root = safeInsert(root, key, data);
    }

    while (true) {
        cout << "\nMenu:\n";
        cout << "1 - Add new record\n";
        cout << "2 - Search record by key\n";
        cout << "3 - Delete record by key\n";
        cout << "4 - Print tree traversals (Preorder, Postorder, Inorder)\n";
        cout << "5 - Swap data in nodes with minimum and maximum keys\n";
        cout << "6 - Rebalance tree\n";
        cout << "7 - Graphically display tree\n";
        cout << "Press any other key to exit.\n";
        cout << "Enter your choice: ";

        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case '1': {
            int key = inputKey();
            string data = inputData();
            root = safeInsert(root, key, data);
            break;
        }
        case '2': {
            if (!root) {
                cout << "Error: The tree is not created or is empty." << endl;
                break;
            }
            int key = inputKey();
            TreeNode* result = search(root, key);
            if (result)
                cout << "Found: " << result->key << " - " << result->data << "\n";
            else
                cout << "Record with key " << key << " not found.\n";
            break;
        }
        case '3': {
            if (!root) {
                cout << "Error: The tree is not created or is empty." << endl;
                break;
            }
            int key = inputKey();
            root = deleteNode(root, key);
            cout << "Record with key " << key << " deleted (if it existed)." << endl;
            break;
        }
        case '4': {
            if (!root) {
                cout << "Error: The tree is not created or is empty." << endl;
                break;
            }
            cout << "\nPreorder traversal:\n";
            preorder(root);
            cout << "\nPostorder traversal:\n";
            postorder(root);
            cout << "\nInorder traversal:\n";
            inorder(root);
            break;
        }
        case '5': {
            if (!root) {
                cout << "Error: The tree is not created or is empty." << endl;
                break;
            }
            swapMinMax(root);
            cout << "Data in nodes with minimum and maximum keys have been swapped." << endl;
            break;
        }
        case '6': {
            if (!root) {
                cout << "Error: The tree is not created or is empty." << endl;
                break;
            }
            root = rebalanceTree(root);
            cout << "Tree has been rebalanced." << endl;
            break;
        }
        case '7': {
            if (!root) {
                cout << "Error: The tree is not created or is empty." << endl;
                break;
            }
            cout << "\nGraphical tree display:\n";
            printTreeGraphical(root);
            break;
        }
        default:
            destroyTree(root);
            return 0;
        }
    }

    destroyTree(root);
    return 0;
}
