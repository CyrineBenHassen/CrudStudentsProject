#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    char first_name[50];
    char last_name[50];
    int student_id;
    int *grades;
};

struct Node {
    struct Student student;
    struct Node *next;
};

struct StudentManager {
    struct Node *head;
};

void add_student(struct StudentManager *manager, struct Student student) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->student = student;
    new_node->next = NULL;

    if (manager->head == NULL) {
        manager->head = new_node;
    } else {
        struct Node *current = manager->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

struct Student find_student(struct StudentManager *manager, int student_id) {
    struct Node *current = manager->head;
    while (current != NULL) {
        if (current->student.student_id == student_id) {
            return current->student;
        }
        current = current->next;
    }

    struct Student not_found_student = {"", "", -1, NULL};
    return not_found_student;
}

void delete_student(struct StudentManager *manager, int student_id) {
    struct Node *current = manager->head;
    struct Node *prev = NULL;

    while (current != NULL && current->student.student_id != student_id) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev != NULL) {
            prev->next = current->next;
        } else {
            manager->head = current->next;
        }
        free(current->student.grades);
        free(current);
    }
}

void display_students(struct StudentManager *manager) {
    struct Node *current = manager->head;
    while (current != NULL) {
        printf("ID: %d, Name: %s %s\n", current->student.student_id, current->student.first_name, current->student.last_name);
        current = current->next;
    }
}

void modify_student(struct StudentManager *manager, int student_id, char new_first_name[50], char new_last_name[50]) {
    struct Node *current = manager->head;
    while (current != NULL) {
        if (current->student.student_id == student_id) {
            strncpy(current->student.first_name, new_first_name, sizeof(current->student.first_name) - 1);
            current->student.first_name[sizeof(current->student.first_name) - 1] = '\0';

            strncpy(current->student.last_name, new_last_name, sizeof(current->student.last_name) - 1);
            current->student.last_name[sizeof(current->student.last_name) - 1] = '\0';

            return;
        }
        current = current->next;
    }
}

void free_students(struct StudentManager *manager) {
    struct Node *current = manager->head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp->student.grades);
        free(temp);
    }
    manager->head = NULL;
}

void save_students(struct StudentManager *manager, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct Node *current = manager->head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%d\n", current->student.first_name, current->student.last_name, current->student.student_id);
        current = current->next;
    }

    fclose(file);
}


void load_students(struct StudentManager *manager, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char first_name[50], last_name[50];
        int student_id;
        sscanf(line, "%49[^,],%49[^,],%d", first_name, last_name, &student_id);

        struct Student new_student;
        strcpy(new_student.first_name, first_name);
        strcpy(new_student.last_name, last_name);
        new_student.student_id = student_id;

        add_student(manager, new_student);
    }

    fclose(file);
}

int main() {
    struct StudentManager manager;
    manager.head = NULL;


    load_students(&manager, "students.txt");

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add Student\n");
        printf("2. Find Student\n");
        printf("3. Delete Student\n");
        printf("4. Modify Student\n");
        printf("5. Display Students\n");
        printf("6. Save Students to File\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:

                {
                    struct Student new_student;
                    printf("Enter first name: ");
                    scanf("%s", new_student.first_name);
                    printf("Enter last name: ");
                    scanf("%s", new_student.last_name);
                    printf("Enter student ID: ");
                    scanf("%d", &new_student.student_id);

                    add_student(&manager, new_student);
                    printf("Student added successfully.\n");
                }
                break;

            case 2:

                {
                    int search_id;
                    printf("Enter student ID to find: ");
                    scanf("%d", &search_id);

                    struct Student found_student = find_student(&manager, search_id);
                    if (found_student.student_id != -1) {
                        printf("Student found (ID %d): %s %s\n", found_student.student_id, found_student.first_name, found_student.last_name);
                    } else {
                        printf("Student not found with ID %d\n", search_id);
                    }
                }
                break;

            case 3:

                {
                    int delete_id;
                    printf("Enter student ID to delete: ");
                    scanf("%d", &delete_id);

                    delete_student(&manager, delete_id);
                    printf("Student deleted successfully.\n");
                }
                break;

            case 4:

                {
                    int modify_id;
                    char new_first_name[50], new_last_name[50];
                    printf("Enter student ID to modify: ");
                    scanf("%d", &modify_id);
                    printf("Enter new first name: ");
                    scanf("%s", new_first_name);
                    printf("Enter new last name: ");
                    scanf("%s", new_last_name);

                    modify_student(&manager, modify_id, new_first_name, new_last_name);
                    printf("Student modified successfully.\n");
                }
                break;

            case 5:

                printf("List of students:\n");
                display_students(&manager);
                break;

            case 6:

                save_students(&manager, "students.txt");
                printf("Students saved to file.\n");
                break;

            case 7:

                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 7);


    free_students(&manager);

    return 0;
}
