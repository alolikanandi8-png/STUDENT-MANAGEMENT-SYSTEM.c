#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ADMIN_FILE "admin.dat"
#define STUDENT_FILE "students.dat"
#define TEMP_FILE "temp.dat"

typedef struct {
    char username[50];
    char password[50];
} Admin;

typedef struct {
    int id;
    int roll;
    char name[50];
    char course[50];
    char mobile[15];
    int semester;

    float marks[5];
    float percentage;
    char grade[5];
    char result[10];
} Student;


/* ---------- Utility Functions ---------- */

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

int adminExists() {
    FILE *fp = fopen(ADMIN_FILE, "rb");

    if (fp == NULL)
        return 0;

    fclose(fp);
    return 1;
}


/* ---------- Admin Section ---------- */

void createAdmin() {
    Admin a, confirm;
    FILE *fp;

    if (adminExists()) {
        printf("\nAdmin account already exists!\n");
        printf("\n   You can login account or Reset data or Enter Forgot password \n");
        
        pauseScreen();
        return;
    }

    printf("\n========== CREATE ADMIN ACCOUNT ==========\n");
    printf("\nCreate admin account first then you will saw the students data\n");

    printf("Create Username: ");
    scanf("%49s", a.username);

    printf("Create Password: ");
    scanf("%49s", a.password);

    printf("Confirm Password: ");
    scanf("%49s", confirm.password);

    if (strcmp(a.password, confirm.password) != 0) {
        printf("\nPassword does not match!\n");
        pauseScreen();
        return;
    }

    fp = fopen(ADMIN_FILE, "wb");

    if (fp == NULL) {
        printf("\nError creating admin file!\n");
        pauseScreen();
        return;
    }

    fwrite(&a, sizeof(Admin), 1, fp);
    fclose(fp);

    printf("\nAdmin account created successfully!\n");
    pauseScreen();
}


int adminLogin() {
    Admin a, input;
    FILE *fp;

    fp = fopen(ADMIN_FILE, "rb");

    if (fp == NULL) {
        printf("\nNo Admin Account Found!\n");
        printf("Please create an Admin Account first.\n");
        pauseScreen();
        return 0;
    }

    fread(&a, sizeof(Admin), 1, fp);
    fclose(fp);

    printf("\n========== ADMIN LOGIN ==========\n");

    printf("Username: ");
    scanf("%49s", input.username);

    printf("Password: ");
    scanf("%49s", input.password);

    if (strcmp(a.username, input.username) == 0 &&
        strcmp(a.password, input.password) == 0) {

        printf("\nLogin Successful!\n");
        pauseScreen();
        return 1;
    }

    printf("\nInvalid Username or Password!\n");
    printf("\nYou can Enter forgot password or Reset and delete all data's\n");
    pauseScreen();

    return 0;
}


void forgotPassword() {
    Admin a;
    char username[50];
    char newPassword[50];
    char confirmPassword[50];

    FILE *fp;

    fp = fopen(ADMIN_FILE, "rb");

    if (fp == NULL) {
        printf("\nNo Admin Account Found!\n");
        pauseScreen();
        return;
    }

    fread(&a, sizeof(Admin), 1, fp);
    fclose(fp);

    printf("\n========== FORGOT PASSWORD ==========\n");

    printf("Enter Username: ");
    scanf("%49s", username);

    if (strcmp(username, a.username) != 0) {
        printf("\nUsername not found!\n");
        pauseScreen();
        return;
    }

    printf("Enter New Password: ");
    scanf("%49s", newPassword);

    printf("Confirm New Password: ");
    scanf("%49s", confirmPassword);

    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("\nPassword does not match!\n");
        pauseScreen();
        return;
    }

    strcpy(a.password, newPassword);

    fp = fopen(ADMIN_FILE, "wb");

    if (fp == NULL) {
        printf("\nError updating password!\n");
        pauseScreen();
        return;
    }

    fwrite(&a, sizeof(Admin), 1, fp);
    fclose(fp);

    printf("\nPassword changed successfully!\n");
    pauseScreen();
}


/* ---------- Student ID ---------- */

int getNextID() {
    FILE *fp;
    Student s;
    int maxID = 0;

    fp = fopen(STUDENT_FILE, "rb");

    if (fp == NULL)
        return 1;

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id > maxID)
            maxID = s.id;
    }

    fclose(fp);

    return maxID + 1;
}


/* ---------- Duplicate Roll ---------- */

int rollExists(int roll) {
    FILE *fp;
    Student s;

    fp = fopen(STUDENT_FILE, "rb");

    if (fp == NULL)
        return 0;

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.roll == roll) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


/* ---------- Grade ---------- */

void calculateResult(Student *s) {

    int i;
    int fail = 0;

    float total = 0;

    for (i = 0; i < 5; i++) {

        total += s->marks[i];

        if (s->marks[i] < 40)
            fail = 1;
    }

    s->percentage = total / 5;

    if (fail) {

        strcpy(s->grade, "F");
        strcpy(s->result, "FAIL");

    } else {

        strcpy(s->result, "PASS");

        if (s->percentage >= 90)
            strcpy(s->grade, "A+");

        else if (s->percentage >= 80)
            strcpy(s->grade, "A");

        else if (s->percentage >= 70)
            strcpy(s->grade, "B+");

        else if (s->percentage >= 60)
            strcpy(s->grade, "B");

        else if (s->percentage >= 50)
            strcpy(s->grade, "C");

        else
            strcpy(s->grade, "D");
    }
}


/* ---------- Add Student ---------- */

void addStudent() {

    Student s;
    FILE *fp;

    printf("\n========== ADD STUDENT ==========\n");

    s.id = getNextID();

    printf("Student ID: %d\n", s.id);

    while (1) {

        printf("Enter Roll Number: ");
        scanf("%d", &s.roll);

        if (rollExists(s.roll)) {
            printf("Roll number already exists! Enter another.\n");
        } else {
            break;
        }
    }

    clearInputBuffer();

    printf("Enter Student Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Course: ");
    fgets(s.course, sizeof(s.course), stdin);
    s.course[strcspn(s.course, "\n")] = '\0';

    printf("Enter Mobile Number: ");
    scanf("%14s", s.mobile);

    printf("Enter Semester: ");
    scanf("%d", &s.semester);

    printf("\nEnter Marks of 5 Subjects:\n");

    for (int i = 0; i < 5; i++) {

        do {
            printf("Subject %d: ", i + 1);
            scanf("%f", &s.marks[i]);

            if (s.marks[i] < 0 || s.marks[i] > 100)
                printf("Marks must be between 0 and 100!\n");

        } while (s.marks[i] < 0 || s.marks[i] > 100);
    }

    calculateResult(&s);

    fp = fopen(STUDENT_FILE, "ab");

    if (fp == NULL) {
        printf("\nError opening student file!\n");
        pauseScreen();
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("\nStudent added successfully!\n");
    printf("Student ID: %d\n", s.id);
    printf("Percentage: %.2f%%\n", s.percentage);
    printf("Grade: %s\n", s.grade);
    printf("Result: %s\n", s.result);

    pauseScreen();
}


/* ---------- Display Student ---------- */

void displayStudent(Student s) {

    printf("\n----------------------------------------\n");

    printf("ID          : %d\n", s.id);
    printf("Roll        : %d\n", s.roll);
    printf("Name        : %s\n", s.name);
    printf("Course      : %s\n", s.course);
    printf("Mobile      : %s\n", s.mobile);
    printf("Semester    : %d\n", s.semester);

    printf("Marks       : ");

    for (int i = 0; i < 5; i++)
        printf("%.0f ", s.marks[i]);

    printf("\nPercentage  : %.2f%%\n", s.percentage);
    printf("Grade       : %s\n", s.grade);
    printf("Result      : %s\n", s.result);
}


/* ---------- Display All ---------- */

void displayAllStudents() {

    FILE *fp;
    Student s;
    int count = 0;

    fp = fopen(STUDENT_FILE, "rb");

    if (fp == NULL) {
        printf("\nNo student data available.\n");
        pauseScreen();
        return;
    }

    printf("\n========== ALL STUDENTS ==========\n");

    while (fread(&s, sizeof(Student), 1, fp)) {

        displayStudent(s);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("\nNo students found.\n");

    else
        printf("\nTotal Students: %d\n", count);

    pauseScreen();
}


/* ---------- Search Student ---------- */

void searchStudent() {

    FILE *fp;
    Student s;

    int choice;
    int roll;
    char name[50];
    int found = 0;

    fp = fopen(STUDENT_FILE, "rb");

    if (fp == NULL) {
        printf("\nNo student data available.\n");
        pauseScreen();
        return;
    }

    printf("\n========== SEARCH STUDENT ==========\n");

    printf("1. Search by Roll\n");
    printf("2. Search by Name\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);

    if (choice == 1) {

        printf("Enter Roll Number: ");
        scanf("%d", &roll);

        while (fread(&s, sizeof(Student), 1, fp)) {

            if (s.roll == roll) {

                displayStudent(s);
                found = 1;
                break;
            }
        }

    } else if (choice == 2) {

        clearInputBuffer();

        printf("Enter Name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        while (fread(&s, sizeof(Student), 1, fp)) {

            if (strcasecmp(s.name, name) == 0) {

                displayStudent(s);
                found = 1;
                break;
            }
        }

    } else {

        printf("\nInvalid choice!\n");
        fclose(fp);
        pauseScreen();
        return;
    }

    fclose(fp);

    if (!found)
        printf("\nStudent not found!\n");

    pauseScreen();
}


/* ---------- Update Student ---------- */

void updateStudent() {

    FILE *fp;
    Student s;

    int roll;
    int found = 0;

    fp = fopen(STUDENT_FILE, "rb+");

    if (fp == NULL) {
        printf("\nNo student data available.\n");
        pauseScreen();
        return;
    }

    printf("\n========== UPDATE STUDENT ==========\n");

    printf("Enter Roll Number: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(Student), 1, fp)) {

        if (s.roll == roll) {

            found = 1;

            clearInputBuffer();

            printf("\nCurrent Student Data:");
            displayStudent(s);

            printf("\nEnter New Name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';

            printf("Enter New Course: ");
            fgets(s.course, sizeof(s.course), stdin);
            s.course[strcspn(s.course, "\n")] = '\0';

            printf("Enter New Mobile: ");
            scanf("%14s", s.mobile);

            printf("Enter New Semester: ");
            scanf("%d", &s.semester);

            printf("\nEnter New Marks:\n");

            for (int i = 0; i < 5; i++) {

                do {
                    printf("Subject %d: ", i + 1);
                    scanf("%f", &s.marks[i]);

                    if (s.marks[i] < 0 || s.marks[i] > 100)
                        printf("Enter marks between 0 and 100!\n");

                } while (s.marks[i] < 0 || s.marks[i] > 100);
            }

            calculateResult(&s);

            fseek(fp, -(long)sizeof(Student), SEEK_CUR);

            fwrite(&s, sizeof(Student), 1, fp);

            printf("\nStudent updated successfully!\n");

            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nStudent not found!\n");

    pauseScreen();
}


/* ---------- Delete Student ---------- */

void deleteStudent() {

    FILE *fp, *temp;

    Student s;

    int roll;
    int found = 0;
    char confirm;

    fp = fopen(STUDENT_FILE, "rb");

    if (fp == NULL) {
        printf("\nNo student data available.\n");
        pauseScreen();
        return;
    }

    temp = fopen(TEMP_FILE, "wb");

    if (temp == NULL) {
        fclose(fp);
        printf("\nError creating temporary file!\n");
        pauseScreen();
        return;
    }

    printf("\n========== DELETE STUDENT ==========\n");

    printf("Enter Roll Number: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(Student), 1, fp)) {

        if (s.roll == roll) {

            found = 1;

            printf("\nStudent Found:");
            displayStudent(s);

            printf("\nDelete this student? (Y/N): ");
            scanf(" %c", &confirm);

            if (toupper(confirm) != 'Y')
                fwrite(&s, sizeof(Student), 1, temp);

        } else {

            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename(TEMP_FILE, STUDENT_FILE);

    if (found)
        printf("\nStudent deleted successfully!\n");
    else
        printf("\nStudent not found!\n");

    pauseScreen();
}


/* ---------- Statistics ---------- */

void statistics() {

    FILE *fp;
    Student s;

    int total = 0;
    int pass = 0;
    int fail = 0;

    float highest = -1;
    char topper[50] = "";

    fp = fopen(STUDENT_FILE, "rb");

    if (fp == NULL) {
        printf("\nNo student data available.\n");
        pauseScreen();
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp)) {

        total++;

        if (strcmp(s.result, "PASS") == 0)
            pass++;
        else
            fail++;

        if (s.percentage > highest) {

            highest = s.percentage;
            strcpy(topper, s.name);
        }
    }

    fclose(fp);

    printf("\n========== STATISTICS & TOPPER ==========\n");

    printf("Total Students : %d\n", total);
    printf("Passed         : %d\n", pass);
    printf("Failed         : %d\n", fail);

    if (total > 0) {

        printf("Pass Percentage: %.2f%%\n",
               (pass * 100.0) / total);

        printf("Topper         : %s\n", topper);
        printf("Highest Marks  : %.2f%%\n", highest);

    } else {

        printf("No student records found.\n");
    }

    pauseScreen();
}


/* ---------- Reset Everything ---------- */

void resetSystem() {

    char confirm;

    printf("\n========================================\n");
    printf("       RESET / START FROM FIRST\n");
    printf("========================================\n");

    printf("\nWARNING!\n");
    printf("This will delete ALL admin and student data.\n");
    printf("The system will start completely fresh.\n");

    printf("\nAre you sure? (Y/N): ");
    scanf(" %c", &confirm);

    if (toupper(confirm) != 'Y') {

        printf("\nReset cancelled.\n");
        pauseScreen();
        return;
    }

    remove(ADMIN_FILE);
    remove(STUDENT_FILE);
    remove(TEMP_FILE);

    printf("\nAll data has been cleared successfully!\n");
    printf("Now you can start again from Create Admin Account.\n");

    pauseScreen();
}


/* ---------- Student Menu ---------- */

void studentMenu() {

    int choice;

    while (1) {

        printf("\n\n========================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM\n");
        printf("========================================\n");

        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Statistics & Topper\n");
        printf("7. Logout\n");

        printf("----------------------------------------\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                addStudent();
                break;

            case 2:
                displayAllStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                statistics();
                break;

            case 7:
                printf("\nLogging out...\n");
                return;

            default:
                printf("\nInvalid choice! Please try again.\n");
                pauseScreen();
        }
    }
}


/* ---------- Main ---------- */

int main() {

    int choice;

    while (1) {

        printf("\n\n========================================\n");
        printf("      STUDENT MANAGEMENT SYSTEM\n");
        printf("========================================\n");

        printf("1. Admin Login\n");
        printf("2. Forgot Password\n");
        printf("3.Create Admin Account***\n");
        printf("4. Reset / Start From First\n");
        printf("5. Exit\n");
        printf("Create Admin Account First Then You will be shown the students data\n");

        printf("----------------------------------------\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:

                if (adminLogin())
                    studentMenu();

                break;

            case 2:
                forgotPassword();
                break;

            case 3:
                createAdmin();
                break;

            case 4:
                resetSystem();
                break;

            case 5:

                printf("\nThank you for using Student Management System!\n");
                return 0;

            default:

                printf("\nInvalid choice! Please enter 1-5.\n");
                pauseScreen();
        }
    }

    return 0;
}