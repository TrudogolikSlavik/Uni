#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"  // Красный
#define GREEN   "\033[32m"  // Зеленый

typedef struct Stack {
    int info;
    struct Stack* next;
} Stack;

// Функция для добавления элемента в стек
void push(Stack** p, int info) {
    Stack* new_node = (Stack*)malloc(sizeof(Stack));
    if (new_node == NULL) {
        printf(RED "Memory allocation error.\n" RESET);
        exit(1);
    }
    new_node->info = info;
    new_node->next = *p;
    *p = new_node;
}

void delete_element_from_stack(Stack** p) {
    int value;
    if (*p == NULL) {
        printf(RED "The stack is empty; nothing to delete.\n" RESET);
        return;
    }
    printf("Enter the value of the element to delete: ");
    while (scanf_s("%d", &value) != 1) {
        printf(RED "Invalid input. Please enter an integer: " RESET);
        while (getchar() != '\n');
    }
    Stack* current = *p;
    Stack* prev = NULL;
    // Поиск первого элемента со значением value
    while (current != NULL) {
        if (current->info == value) {
            break;
        }
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf(RED "Element %d not found in the stack.\n" RESET, value);
        return;
    }
    // Если удаляется первый элемент, корректируем указатель на голову стека
    if (prev == NULL) {
        *p = current->next;
    }
    else {
        prev->next = current->next;
    }
    free(current);
    printf("Element %d deleted from the stack.\n", value);
}

// Рекурсивная функция для вывода стека (печать элементов в порядке ввода)
void print_stack(Stack* p) {
    if (p == NULL)
        return;
    print_stack(p->next);
    printf("%d ", p->info);
}

// Рекурсивная функция освобождения памяти стека
void delete_stack(Stack* p) {
    if (p == NULL)
        return;
    delete_stack(p->next);
    free(p);
}

// Функция сортировки стека методом обмена адресами
void Sort_p(Stack** p) {
    if (*p == NULL || (*p)->next == NULL) return;
    Stack* t = NULL;
    int swapped;
    do {
        swapped = 0;
        Stack** current = p;
        while ((*current)->next != t) {
            if ((*current)->info > (*current)->next->info) {
                // Обмен узлов
                Stack* temp = *current;
                *current = (*current)->next;
                temp->next = (*current)->next;
                (*current)->next = temp;
                swapped = 1;
            }
            current = &(*current)->next;
        }
        t = *current;
    } while (swapped);
}

// Функция сортировки стека методом обмена данными
void Sort_info(Stack* p) {
    if (p == NULL || p->next == NULL) return;
    Stack* t = NULL;
    int swapped;
    do {
        swapped = 0;
        Stack* current = p;
        while (current->next != t) {
            if (current->info > current->next->info) {
                int temp = current->info;
                current->info = current->next->info;
                current->next->info = temp;
                swapped = 1;
            }
            current = current->next;
        }
        t = current;
    } while (swapped);
}

void split_stack(Stack* original, Stack** positive, Stack** negative) {
    Stack* current = original;
    while (current != NULL) {
        if (current->info >= 0)
            push(positive, current->info);
        else
            push(negative, current->info);
        current = current->next;
    }
}

void input_stack(Stack** stack) {
    int num_elements;
    printf("Enter the number of elements to push onto the stack: ");
    while (scanf_s("%d", &num_elements) != 1 || num_elements <= 0) {
        printf(RED "Invalid input. Please enter a positive integer: " RESET);
        while (getchar() != '\n'); // Очистка буфера ввода
    }
    for (int i = 0; i < num_elements; i++) {
        int value;
        printf("Enter integer value %d: ", i + 1);
        while (scanf_s("%d", &value) != 1) {
            printf(RED "Invalid input. Please enter an integer: " RESET);
            while (getchar() != '\n');
        }
        push(stack, value);
    }
}

int menu() {
    int choice;
    printf(GREEN "\nMenu:\n" RESET);
    printf("1. Create stack (user input)\n");
    printf("2. Add element to stack\n");
    printf("3. Print stack\n");
    printf("4. Solve the problem (split stack into positive and negative numbers)\n");
    printf("5. Sort stack (address swapping method)\n");
    printf("6. Sort stack (data swapping method)\n");
    printf("7. Delete element from stack (choose element to delete)\n");
    printf("Press any other key to exit.\n");
    printf("Enter operation number: ");
    if (scanf_s("%d", &choice) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    if (choice < 1 || choice > 7) return -1;
    return choice;
}

int main() {
    Stack* stack = NULL;    
    Stack* positive = NULL; 
    Stack* negative = NULL; 

    // Флаги для отслеживания состояния:
    // stack_created = 1, если оригинальный стек создан и готов к операциям
    // stack_split = 1, если стек уже разделён
    int stack_created = 0;
    int stack_split = 0;

    int choice;
    int value;

    do {
        choice = menu();
        switch (choice) {
        case 1:
            // При создании нового стека освобождаем предыдущие стеки 
            if (stack_created || stack_split) {
                if (stack != NULL) {
                    delete_stack(stack);
                    stack = NULL;
                }
                if (positive != NULL) {
                    delete_stack(positive);
                    positive = NULL;
                }
                if (negative != NULL) {
                    delete_stack(negative);
                    negative = NULL;
                }
            }
            input_stack(&stack);
            stack_created = 1;  // Новый стек создан
            stack_split = 0;    // Сброс флага разделения
            printf("Stack created.\n");
            break;
        case 2:
            // Добавление элемента в стек.
            if (stack_split) {
                printf(RED "The original stack has already been split; operation unavailable.\n" RESET);
                break;
            }
            if (!stack_created) {
                printf(RED "The stack has not been created. Please create the stack first.\n" RESET);
                break;
            }
            printf("Enter an integer to add to the stack: ");
            while (scanf_s("%d", &value) != 1) {
                printf(RED "Invalid input. Please enter an integer: " RESET);
                while (getchar() != '\n');
            }
            push(&stack, value);
            printf("Element added to the stack.\n");
            break;
        case 3:
            // Вывод оригинального стека
            if (stack_split) {
                printf(RED "The original stack has already been split; operation unavailable.\n" RESET);
                break;
            }
            if (!stack_created) {
                printf(RED "The stack has not been created. Please create the stack.\n" RESET);
                break;
            }
            if (stack == NULL)
                printf(RED "The stack is empty.\n" RESET);
            else {
                printf("Stack: ");
                print_stack(stack);
                printf("\n");
            }
            break;
        case 4:
            // Разделить стек на положительные и отрицательные числа.
            if (stack_split) {
                printf(RED "The original stack has already been split.\n" RESET);
                break;
            }
            if (!stack_created) {
                printf(RED "The stack has not been created. Please create the stack.\n" RESET);
                break;
            }
            // Освобождаем предыдущие варианты положительного и отрицательного стеков.
            if (positive != NULL) { delete_stack(positive); positive = NULL; }
            if (negative != NULL) { delete_stack(negative); negative = NULL; }
            split_stack(stack, &positive, &negative);
            // После разделения освобождаем оригинальный стек.
            delete_stack(stack);
            stack = NULL;
            stack_created = 0;
            stack_split = 1;
            // Вывод разделенных стеков.
            printf("Positive numbers stack: ");
            if (positive == NULL)
                printf("No positive numbers.\n");
            else {
                print_stack(positive);
                printf("\n");
            }
            printf("Negative numbers stack: ");
            if (negative == NULL)
                printf("No negative numbers.\n");
            else {
                print_stack(negative);
                printf("\n");
            }
            break;
        case 5:
            // Сортировка оригинального стека методом обмена адресами.
            if (stack_split) {
                printf(RED "The original stack has already been split; operation unavailable.\n" RESET);
                break;
            }
            if (!stack_created) {
                printf(RED "The stack has not been created.\n" RESET);
                break;
            }
            if (stack == NULL)
                printf(RED "The stack is empty; nothing to sort.\n" RESET);
            else {
                Sort_p(&stack);
                printf("Stack sorted using address swapping method.\n");
            }
            break;
        case 6:
            // Сортировка оригинального стека методом обмена данными.
            if (stack_split) {
                printf(RED "The original stack has already been split; operation unavailable.\n" RESET);
                break;
            }
            if (!stack_created) {
                printf(RED "The stack has not been created.\n" RESET);
                break;
            }
            if (stack == NULL)
                printf(RED "The stack is empty; nothing to sort.\n" RESET);
            else {
                Sort_info(stack);
                printf("Stack sorted using data swapping method.\n");
            }
            break;
        case 7:
            // Удаление выбранного элемента из оригинального стека.
            if (stack_split) {
                printf(RED "The original stack has already been split; operation unavailable.\n" RESET);
                break;
            }
            if (!stack_created) {
                printf(RED "The stack has not been created.\n" RESET);
                break;
            }
            delete_element_from_stack(&stack);
            break;
        default:
            printf("Exiting program.\n");
            choice = -1;
            break;
        }
    } while (choice != -1);

    if (stack != NULL) delete_stack(stack);
    if (positive != NULL) delete_stack(positive);
    if (negative != NULL) delete_stack(negative);

    return 0;
}
