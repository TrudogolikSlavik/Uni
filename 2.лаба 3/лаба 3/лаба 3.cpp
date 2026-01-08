#include <stdio.h>
#include <stdlib.h>

#define RESET "\033[0m"
#define RED   "\033[31m"
#define GREEN "\033[0;32m"

// Структура узла двунаправленного списка
typedef struct QueueNode {
    int info;
    struct QueueNode* next;
    struct QueueNode* prev; 
} QueueNode;

// Структура очереди
typedef struct Queue {
    QueueNode* front; 
    QueueNode* rear;  
} Queue;

// Функция инициализации очереди
void initializeQueue(Queue* q) {
    q->front = q->rear = NULL;
}

// Функция добавления элемента в очередь (в начало или конец)
// Если atFront == 1, добавление в начало, иначе — в конец
void enqueue(Queue* q, int info, int atFront) {
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (new_node == NULL) {
        printf(RED "Memory allocation error.\n" RESET);
        exit(1);
    }
    new_node->info = info;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (atFront) { // Добавление в начало
        new_node->next = q->front;
        if (q->front != NULL)
            q->front->prev = new_node;
        q->front = new_node;
        if (q->rear == NULL)
            q->rear = new_node;
    }
    else { // Добавление в конец
        new_node->prev = q->rear;
        if (q->rear != NULL)
            q->rear->next = new_node;
        q->rear = new_node;
        if (q->front == NULL)
            q->front = new_node;
    }
}

// Функция удаления элемента по заданному индексу (индексация с 1)
int deleteElementAtIndex(Queue* q, int index) {
    if (q->front == NULL) {
        printf(RED "Queue is empty.\n" RESET);
        return 0;
    }
    if (index < 1) {
        printf(RED "Invalid index. Index must be >= 1.\n" RESET);
        return 0;
    }
    QueueNode* current = q->front;
    int pos = 1;
    while (current != NULL && pos < index) {
        current = current->next;
        pos++;
    }
    if (current == NULL) {
        printf(RED "Invalid index. No element found at position %d.\n" RESET, index);
        return 0;
    }
    // Обновляем связи соседних узлов
    if (current->prev != NULL)
        current->prev->next = current->next;
    else
        q->front = current->next;

    if (current->next != NULL)
        current->next->prev = current->prev;
    else
        q->rear = current->prev;

    free(current);
    printf("Element at position %d has been deleted.\n", index);
    return 1;
}

// Функция вывода очереди с начала
void printQueue(Queue* q) {
    QueueNode* current = q->front;
    if (current == NULL) {
        printf("Queue is empty.\n");
        return;
    }
    while (current != NULL) {
        printf("%d ", current->info);
        current = current->next;
    }
    printf("\n");
}

// Функция вывода очереди с конца
void printQueueReverse(Queue* q) {
    QueueNode* current = q->rear;
    if (current == NULL) {
        printf("Queue is empty.\n");
        return;
    }
    while (current != NULL) {
        printf("%d ", current->info);
        current = current->prev;
    }
    printf("\n");
}

// Функция очистки очереди и освобождения памяти
void deleteQueue(Queue* q) {
    QueueNode* current = q->front;
    while (current != NULL) {
        QueueNode* nextNode = current->next;
        free(current);
        current = nextNode;
    }
    q->front = q->rear = NULL;
}

// Сортировка очереди методом перестановки указателей (узлов)
// Реализована пузырьковая сортировка с обменом узлов; обновляются next и prev
void sortQueuePointers(Queue* q) {
    if (q->front == NULL || q->front->next == NULL)
        return;
    int swapped;
    do {
        swapped = 0;
        QueueNode* current = q->front;
        while (current != NULL && current->next != NULL) {
            if (current->info > current->next->info) {
                QueueNode* first = current;
                QueueNode* second = current->next;

                if (first->prev != NULL)
                    first->prev->next = second;
                else
                    q->front = second;

                if (second->next != NULL)
                    second->next->prev = first;
                else
                    q->rear = first;

                first->next = second->next;
                second->prev = first->prev;
                second->next = first;
                first->prev = second;

                swapped = 1;
                // После обмена начинаем проход заново
                break;
            }
            else {
                current = current->next;
            }
        }
    } while (swapped);
}

// Сортировка очереди методом перестановки данных (значений)
void sortQueueData(Queue* q) {
    if (q->front == NULL || q->front->next == NULL)
        return;
    int swapped;
    do {
        swapped = 0;
        QueueNode* current = q->front;
        while (current != NULL && current->next != NULL) {
            if (current->info > current->next->info) {
                int temp = current->info;
                current->info = current->next->info;
                current->next->info = temp;
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}

void splitQueue(Queue* original, Queue* positive, Queue* negative, int* wasQueueSplit) {
    QueueNode* current = original->front;
    while (current != NULL) {
        if (current->info >= 0)
            enqueue(positive, current->info, 0);
        else
            enqueue(negative, current->info, 0);
        current = current->next;
    }
    // Освобождаем оригинальную очередь и отмечаем, что она была разделена
    deleteQueue(original);
    *wasQueueSplit = 1;
    printf("The original queue has been split into two and freed. "
        "It is not possible to operate on the original queue.\n");
}

void inputQueue(Queue* q) {
    int num_elements;
    printf("Enter the number of elements: ");
    while (scanf_s("%d", &num_elements) != 1 || num_elements <= 0) {
        printf(RED "Invalid input. Please enter a positive integer: " RESET);
        while (getchar() != '\n'); // Очистка буфера ввода
    }
    for (int i = 0; i < num_elements; i++) {
        int value;
        printf("Enter integer value %d: ", i + 1);
        while (scanf_s("%d", &value) != 1) {
            printf(RED "Invalid input. Please enter an integer: " RESET);
            while (getchar() != '\n'); // Очистка буфера ввода
        }
        // Добавление в конец очереди
        enqueue(q, value, 0);
    }
}

int menu() {
    int choice;
    printf("\n" GREEN "Menu:\n" RESET);
    printf("1. Create queue (user input)\n");
    printf("2. Add an element to the queue\n");
    printf("3. Delete an element by index\n");
    printf("4. Print the queue (from beginning)\n");
    printf("5. Print the queue (from end)\n");
    printf("6. Split the queue (positive/negative)\n");
    printf("7. Sort the queue (pointer swapping method)\n");
    printf("8. Sort the queue (data swapping method)\n");
    printf("Other key to exit\n");
    printf("Enter your choice: ");

    if (scanf_s("%d", &choice) != 1) {
        while (getchar() != '\n'); // Очистка буфера ввода
        return -1;
    }
    return choice;
}

int main() {
    Queue queue;
    Queue positiveQueue;
    Queue negativeQueue;

    initializeQueue(&queue);
    initializeQueue(&positiveQueue);
    initializeQueue(&negativeQueue);

    // Флаг, показывающий, что очередь создана через пользовательский ввод
    int isQueueCreated = 0;
    // Флаг, показывающий, что оригинальная очередь была разделена
    int wasQueueSplit = 0;
    int choice;

    do {
        choice = menu();
        // Если очередь ещё не создана (либо была разделена)
        if (choice != 1 && !isQueueCreated) {
            if (wasQueueSplit)
                printf(RED "The original queue has been split and cannot be operated on. Please create a new queue.\n" RESET);
            else
                printf(RED "Queue has not been created yet. Please create a queue first.\n" RESET);
            continue;
        }
        switch (choice) {
        case 1:
            deleteQueue(&queue);
            initializeQueue(&queue);
            inputQueue(&queue);
            isQueueCreated = 1;
            wasQueueSplit = 0; // создаём новую очередь – сбрасываем флаг разделения
            printf("Queue created.\n");
            break;
        case 2: {
            int value, addPosition;
            printf("Enter an integer to add: ");
            while (scanf_s("%d", &value) != 1) {
                printf(RED "Invalid input. Please enter an integer: " RESET);
                while (getchar() != '\n');
            }
            printf("Where do you want to add the element?\n");
            printf("1. Add to the front\n");
            printf("2. Add to the end\n");
            printf("Enter your choice: ");
            while (scanf_s("%d", &addPosition) != 1 || (addPosition != 1 && addPosition != 2)) {
                printf(RED "Invalid input. Please enter 1 or 2: " RESET);
                while (getchar() != '\n');
            }
            enqueue(&queue, value, addPosition == 1);
            printf("Element added to the %s of the queue.\n", addPosition == 1 ? "front" : "end");
            break;
        }
        case 3: {
            if (queue.front == NULL) {
                printf(RED "Queue is empty.\n" RESET);
                break;
            }
            int index, deleted = 0;
            while (!deleted) {
                printf("Enter the index of the element to delete (starting from 1): ");
                while (scanf_s("%d", &index) != 1) {
                    printf(RED "Invalid input. Please enter a valid index: " RESET);
                    while (getchar() != '\n');
                }
                deleted = deleteElementAtIndex(&queue, index);
            }
            break;
        }
        case 4:
            // Вывод очереди с начала
            if (queue.front == NULL)
                printf("Queue is empty.\n");
            else {
                printf("Queue from beginning: ");
                printQueue(&queue);
            }
            break;
        case 5:
            // Вывод очереди с конца
            if (queue.rear == NULL)
                printf("Queue is empty.\n");
            else {
                printf("Queue from end: ");
                printQueueReverse(&queue);
            }
            break;
        case 6:
            // Разбиение очереди на положительные и отрицательные числа.
            if (queue.front == NULL) {
                printf("Queue is empty.\n");
            }
            else {
                // Очистка вспомогательных очередей перед разбиением
                deleteQueue(&positiveQueue);
                initializeQueue(&positiveQueue);
                deleteQueue(&negativeQueue);
                initializeQueue(&negativeQueue);

                splitQueue(&queue, &positiveQueue, &negativeQueue, &wasQueueSplit);
                // После разделения оригинальная очередь считается неактивной
                isQueueCreated = 0;

                printf("Positive numbers: ");
                if (positiveQueue.front == NULL)
                    printf("No positive numbers.\n");
                else
                    printQueue(&positiveQueue);

                printf("Negative numbers: ");
                if (negativeQueue.front == NULL)
                    printf("No negative numbers.\n");
                else
                    printQueue(&negativeQueue);
            }
            break;
        case 7:
            // Сортировка очереди методом перестановки указателей
            if (queue.front == NULL)
                printf("Queue is empty.\n");
            else {
                sortQueuePointers(&queue);
                printf("Queue sorted using pointer swapping method.\n");
            }
            break;
        case 8:
            // Сортировка очереди методом перестановки данных
            if (queue.front == NULL)
                printf("Queue is empty.\n");
            else {
                sortQueueData(&queue);
                printf("Queue sorted using data swapping method.\n");
            }
            break;
        default:
            printf("Exiting the program.\n");
            break;
        }
    } while (choice >= 1 && choice <= 8);

    deleteQueue(&queue);
    deleteQueue(&positiveQueue);
    deleteQueue(&negativeQueue);

    return 0;
}
