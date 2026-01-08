#include <stdio.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>

struct Student {
    char lastName[50];
    char initials[50];
    int birthYear;
    int groupNumber;
    int physicsGrade;
    int mathGrade;
    int csGrade;
    int chemistryGrade;
    double averageGrade;
};

int size = sizeof(Student);
FILE* Fz, * Ft;
char File_Zap[] = "students.dat";
char File_Rez[] = "results.txt";

void calculateAverageGrade(Student& student) {
    student.averageGrade = (student.physicsGrade + student.mathGrade + student.csGrade + student.chemistryGrade) / 4.0;
}

int my_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

void Out(Student student, int index) {
    printf("Student%d: \033[1;32mSurname:\033[0m %s, \033[1;32mInitials:\033[0m %s, \033[1;32mBirth Year:\033[0m %d, \033[1;32mGroup Number:\033[0m %d, \033[1;32mPhysics:\033[0m %d, \033[1;32mMath:\033[0m %d, \033[1;32mCS:\033[0m %d, \033[1;32mChemistry:\033[0m %d, \033[1;32mAverage Grade:\033[0m %.2f\n",
        index, student.lastName, student.initials, student.birthYear, student.groupNumber,
        student.physicsGrade, student.mathGrade, student.csGrade, student.chemistryGrade, student.averageGrade);
    fprintf(Ft, "Student%d: Surname: %s, Initials: %s, Birth Year: %d, Group Number: %d, Physics: %d, Math: %d, CS: %d, Chemistry: %d, Average Grade: %.2f\n",
        index, student.lastName, student.initials, student.birthYear, student.groupNumber,
        student.physicsGrade, student.mathGrade, student.csGrade, student.chemistryGrade, student.averageGrade);
}

void createFile() {
    if (fopen_s(&Fz, File_Zap, "wb") != 0) {
        printf("\033[1;31mError creating file!\033[0m\n");
        return;
    }
    fclose(Fz);
    printf("Created file %s!\n", File_Zap);
}

void addOrEditRecord() {
    if (fopen_s(&Fz, File_Zap, "rb+") != 0) { 
        printf("\033[1;31mError opening file for adding/editing!\033[0m\n");
        return;
    }

    Student student;
    char lastName[50];
    printf("\033[1;32mEnter the surname of the student to add/edit: \033[0m");
    scanf_s("%20s", lastName, (unsigned)_countof(lastName));

    lastName[49] = '\0';

    bool found = false;
    long pos;
    while (fread(&student, size, 1, Fz)) {
        if (my_strcmp(student.lastName, lastName) == 0) {
            found = true;
            pos = ftell(Fz) - size; // Запоминаем позицию записи
            break;
        }
    }

    if (found) {
        printf("\033[1;32mEditing existing record for %s.\033[0m\n", lastName);
        fseek(Fz, pos, SEEK_SET); // Перемещаем указатель файла на начало записи
    }
    else {
        printf("\033[1;32mAdding new record for %s.\033[0m\n", lastName);
        fseek(Fz, 0, SEEK_END); // Перемещаем указатель файла на конец для добавления новой записи
    }

    strcpy_s(student.lastName, sizeof(student.lastName), lastName);

    printf("\033[1;32mInitials: \033[0m");
    scanf_s("%4s", student.initials, (unsigned)_countof(student.initials));
    while (getchar() != '\n'); // Clear input buffer
    printf("\033[1;32mBirth Year (1950-2020): \033[0m");
    while (scanf_s("%d", &student.birthYear) != 1 || student.birthYear < 1950 || student.birthYear > 2020) {
        printf("\033[1;31mInvalid input. Enter a valid birth year (1950-2020): \033[0m");
        while (getchar() != '\n'); 
    }
    printf("\033[1;32mGroup Number: \033[0m");
    while (scanf_s("%d", &student.groupNumber) != 1 || student.groupNumber <= 0) {
        printf("\033[1;31mInvalid input. Enter a valid group number: \033[0m");
        while (getchar() != '\n');
    }
    printf("\033[1;32mPhysics Grade (0-10): \033[0m");
    while (scanf_s("%d", &student.physicsGrade) != 1 || student.physicsGrade < 0 || student.physicsGrade > 10) {
        printf("\033[1;31mInvalid input. Enter a valid grade (0-10): \033[0m");
        while (getchar() != '\n');
    }
    printf("\033[1;32mMath Grade (0-10): \033[0m");
    while (scanf_s("%d", &student.mathGrade) != 1 || student.mathGrade < 0 || student.mathGrade > 10) {
        printf("\033[1;31mInvalid input. Enter a valid grade (0-10): \033[0m");
        while (getchar() != '\n');
    }
    printf("\033[1;32mCS Grade (0-10): \033[0m");
    while (scanf_s("%d", &student.csGrade) != 1 || student.csGrade < 0 || student.csGrade > 10) {
        printf("\033[1;31mInvalid input. Enter a valid grade (0-10): \033[0m");
        while (getchar() != '\n'); 
    }
    printf("\033[1;32mChemistry Grade (0-10): \033[0m");
    while (scanf_s("%d", &student.chemistryGrade) != 1 || student.chemistryGrade < 0 || student.chemistryGrade > 10) {
        printf("\033[1;31mInvalid input. Enter a valid grade (0-10): \033[0m");
        while (getchar() != '\n'); 
    }

    calculateAverageGrade(student);
    fwrite(&student, size, 1, Fz); // Записываем новую или измененную запись
    fclose(Fz);
}


void viewRecords() {
    if (fopen_s(&Fz, File_Zap, "rb") != 0) {
        printf("\033[1;31mError opening file for viewing!\033[0m\n");
        return;
    }
    printf("\n\t--------- Informations ---------\n");
    fprintf(Ft, "\n\t--------- Informations ---------\n");
    Student student;
    int index = 1;
    while (fread(&student, size, 1, Fz)) {
        Out(student, index++);
    }
    fclose(Fz);
}

void solveIndividualTask() {
    if (fopen_s(&Fz, File_Zap, "rb") != 0) {
        printf("\033[1;31mError opening file for solving task!\033[0m\n");
        return;
    }
    long len = _filelength(_fileno(Fz));
    if (len == -1L) {
        printf("\033[1;31mError getting file length!\033[0m\n");
        fclose(Fz);
        return;
    }
    int kol = len / size;
    Student* students = (Student*)malloc(kol * size);
    if (students == NULL) {
        printf("\033[1;31mMemory allocation error!\033[0m\n");
        fclose(Fz);
        return;
    }
    if (fread(students, size, kol, Fz) != kol) {
        printf("\033[1;31mError reading file!\033[0m\n");
        free(students);
        fclose(Fz);
        return;
    }
    fclose(Fz);

    int groupNumber;
    printf("\033[1;32mEnter group number: \033[0m");
    while (scanf_s("%d", &groupNumber) != 1 || groupNumber <= 0) {
        printf("\033[1;31mInvalid input. Enter a valid group number: \033[0m");
        while (getchar() != '\n'); 
    }

    double totalAverage = 0;
    int count = 0;
    for (int i = 0; i < kol; i++) {
        if (students[i].groupNumber == groupNumber) {
            totalAverage += students[i].averageGrade;
            count++;
        }
    }

    if (count > 0) {
        totalAverage /= count;
        printf("Overall average grade for group %d: %.2f\n", groupNumber, totalAverage);
        fprintf(Ft, "Overall average grade for group %d: %.2f\n", groupNumber, totalAverage);

        int allSame = 1;
        for (int i = 1; i < count; i++) {
            if (students[i].averageGrade != students[0].averageGrade) {
                allSame = 0;
                break;
            }
        }

        if (allSame && count!=1) {
            printf("All students have the same average grade.\n");
            fprintf(Ft, "All students have the same average grade.\n");
        }
        else if (count == 1) {
            printf("Only one student in the group.\n");
            fprintf(Ft, "Only one student in the group.\n");
        }
        else {
            printf("Students with average grade above overall average:\n");
            fprintf(Ft, "Students with average grade above overall average:\n");
            int index = 1;
            for (int i = 0; i < kol; i++) {
                if (students[i].groupNumber == groupNumber && students[i].averageGrade > totalAverage) {
                    Out(students[i], index++);
                }
            }
        }
    }
    else {
        printf("No students found in group %d\n", groupNumber);
        fprintf(Ft, "No students found in group %d\n", groupNumber);
    }

    free(students);
}

int main() {
    if (fopen_s(&Ft, File_Rez, "w") != 0) {
        printf("\033[1;31mError opening result file!\033[0m\n");
        return 1;
    }

    int kod;
    while (1) {
        puts("\n1. Create\n2. View\n3. Add/Edit\n4. Solve Individual Task\n0. Exit");
        while (scanf_s("%d", &kod) != 1 || kod < 0 || kod > 4) {
            printf("\033[1;31mInvalid input. Enter a valid option (0-4): \033[0m");
            while (getchar() != '\n'); 
        }

        switch (kod) {
        case 1:
            createFile();
            break;
        case 2:
            viewRecords();
            break;
        case 3:
            addOrEditRecord();
            break;
        case 4:
            solveIndividualTask();
            break;
        case 0:
            fclose(Ft);
            return 0;
        default:
            printf("\033[1;31mInvalid choice. Please try again.\033[0m\n");
        }
    }
}