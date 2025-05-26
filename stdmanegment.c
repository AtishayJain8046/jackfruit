#include <stdio.h>
#include <string.h>

struct marks {
    float sgpa;
    int isa;
};

struct student {
    char name[50];
    int roll_no;
    struct marks marks;
};
void displayRecords() {
    struct student s;
    FILE *student = fopen("student.txt", "r");

    if (student == NULL) {
        printf("Error opening file or file doesn't exist.\n");
        return;
    }

    printf("\n--- All Student Records ---\n");
    int record_num = 1;
    while (fscanf(student, "%s %d %f %d", s.name, &s.roll_no, &s.marks.sgpa, &s.marks.isa) != EOF) {
        printf("Record %d:\n", record_num++);
        printf("  Name   : %s\n", s.name);
        printf("  Roll No: %d\n", s.roll_no);
        printf("  SGPA   : %.2f\n", s.marks.sgpa);
        printf("  ISA    : %d\n", s.marks.isa);
        printf("------------------------\n");
    }

    fclose(student);
}
void updateRecord() {
    struct student s[100];
    int count = 0, found = 0, roll;

    FILE *file = fopen("student.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read all records
    while (fscanf(file, "%s %d %f %d", s[count].name, &s[count].roll_no,
                  &s[count].marks.sgpa, &s[count].marks.isa) != EOF) {
        count++;
                  }
    fclose(file);

    printf("Enter roll number to update: ");
    scanf("%d", &roll);

    for (int i = 0; i < count; i++) {
        if (s[i].roll_no == roll) {
            printf("Enter new name: ");
            scanf("%s", s[i].name);
            printf("Enter new SGPA: ");
            scanf("%f", &s[i].marks.sgpa);
            printf("Enter new ISA: ");
            scanf("%d", &s[i].marks.isa);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Record not found.\n");
        return;
    }

    // Write all (including updated) back to file
    file = fopen("student.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %.2f %d\n", s[i].name, s[i].roll_no,
                s[i].marks.sgpa, s[i].marks.isa);
    }
    fclose(file);
    printf("Record updated successfully.\n");
}


int rollExists(int rollToCheck) {
    FILE *fp = fopen("student.txt", "r");
    if (fp == NULL) return 0;

    struct student temp;
    while (fscanf(fp, "%49s %d %f %d", temp.name, &temp.roll_no, &temp.marks.sgpa, &temp.marks.isa) == 4) {
        if (temp.roll_no == rollToCheck) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addRecord() {
    struct student s;
    FILE *fp = fopen("student.txt", "a");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Input name
    while (1) {
        printf("Enter name: ");
        if (fgets(s.name, sizeof(s.name), stdin)) {
            s.name[strcspn(s.name, "\n")] = 0; // remove newline
            if (strlen(s.name) > 0 && strspn(s.name, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == strlen(s.name)) {
                break;
            }
        }
        printf("Invalid name. Use alphabetic characters only.\n");
    }

    // Input roll number
    while (1) {
        printf("Enter roll number: ");
        if (scanf("%d", &s.roll_no) == 1) {
            if (!rollExists(s.roll_no)) {
                break;
            } else {
                printf("Roll number already exists. Enter a unique one.\n");
            }
        } else {
            printf("Invalid input. Enter an integer.\n");
        }
        while (getchar() != '\n'); // clear buffer
    }

    // Input SGPA
    while (1) {
        printf("Enter SGPA: ");
        if (scanf("%f", &s.marks.sgpa) == 1 && s.marks.sgpa >= 0.0 && s.marks.sgpa <= 10.0) {
            break;
        } else {
            printf("Invalid SGPA. Enter a float between 0 and 10.\n");
        }
        while (getchar() != '\n');
    }

    // Input ISA
    while (1) {
        printf("Enter ISA marks: ");
        if (scanf("%d", &s.marks.isa) == 1 && s.marks.isa >= 0 && s.marks.isa <= 100) {
            break;
        } else {
            printf("Invalid ISA. Enter an integer between 0 and 100.\n");
        }
        while (getchar() != '\n');
    }

    fprintf(fp, "%s %d %.2f %d\n", s.name, s.roll_no, s.marks.sgpa, s.marks.isa);
    fclose(fp);

    printf("Record added successfully.\n");

    while (getchar() != '\n'); // clear buffer
}
void search(int key) {
    struct student s;
    FILE *student = fopen("student.txt", "r");
    if (student == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    while (fscanf(student, "%s %d %f %d", s.name, &s.roll_no, &s.marks.sgpa, &s.marks.isa) != EOF) {
        if (s.roll_no == key) {
            printf("Record Found:\n");
            printf("Name: %s\n", s.name);
            printf("Roll No: %d\n", s.roll_no);
            printf("SGPA: %.2f\n", s.marks.sgpa);
            printf("ISA: %d\n", s.marks.isa);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Record with Roll No %d not found.\n", key);
    }

    fclose(student);
}

void sortFileByChoice() {
    struct student s[100];
    int count = 0, choice;

    FILE *fp = fopen("student.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Load records into array
    while (count < 100 && fscanf(fp, "%s %d %f %d", s[count].name, &s[count].roll_no,
                                 &s[count].marks.sgpa, &s[count].marks.isa) == 4) {
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No records found to sort.\n");
        return;
    }

    // Ask user for sort type
    printf("\nChoose sorting field:\n");
    printf("1. Roll Number (ascending)\n");
    printf("2. Name (A-Z)\n");
    printf("3. SGPA (descending)\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid choice.\n");
        while (getchar() != '\n');
        return;
    }

    // Sorting logic (Bubble Sort)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int swap = 0;
            if (choice == 1 && s[j].roll_no > s[j + 1].roll_no) {
                swap = 1;
            } else if (choice == 2 && strcmp(s[j].name, s[j + 1].name) > 0) {
                swap = 1;
            } else if (choice == 3 && s[j].marks.sgpa < s[j + 1].marks.sgpa) {
                swap = 1;
            }

            if (swap) {
                struct student temp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = temp;
            }
        }
    }

    // Write sorted data back to file
    fp = fopen("student.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d %.2f %d\n", s[i].name, s[i].roll_no,
                s[i].marks.sgpa, s[i].marks.isa);
    }
    fclose(fp);

    printf("Records sorted and file updated successfully.\n");
}


    

void deleteRecord() {
    struct student s[100];
    int count = 0, roll, found = 0;

    FILE *file = fopen("student.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read all records
    while (fscanf(file, "%s %d %f %d", s[count].name, &s[count].roll_no,
                  &s[count].marks.sgpa, &s[count].marks.isa) != EOF) {
        count++;
                  }
    fclose(file);

    printf("Enter roll number to delete: ");
    scanf("%d", &roll);

    // Write all except the one to delete
    file = fopen("student.txt", "w");
    for (int i = 0; i < count; i++) {
        if (s[i].roll_no != roll) {
            fprintf(file, "%s %d %.2f %d\n", s[i].name, s[i].roll_no,
                    s[i].marks.sgpa, s[i].marks.isa);
        } else {
            found = 1;
        }
    }
    fclose(file);

    if (found) {
        printf("Record deleted successfully.\n");
    } else {
        printf("Record not found.\n");
    }
}
void generateStatistics() {
    struct student s[100];
    int count = 0;
    float total_sgpa = 0;
    float highest = 0, lowest = 10;
    int above_8 = 0;

    FILE *file = fopen("student.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read records
    while (fscanf(file, "%s %d %f %d", s[count].name, &s[count].roll_no,
                  &s[count].marks.sgpa, &s[count].marks.isa) != EOF) {
        float sgpa = s[count].marks.sgpa;
        total_sgpa += sgpa;

        if (sgpa > highest) highest = sgpa;
        if (sgpa < lowest) lowest = sgpa;
        if (sgpa >= 8.0) above_8++;

        count++;
                  }
    fclose(file);

    if (count == 0) {
        printf("No records to analyze.\n");
        return;
    }

    // Display stats
    printf("\n------ Statistics ------\n");
    printf("Total students       : %d\n", count);
    printf("Average SGPA         : %.2f\n", total_sgpa / count);
    printf("Highest SGPA         : %.2f\n", highest);
    printf("Lowest SGPA          : %.2f\n", lowest);
    printf("Students with SGPA >= 8.0 : %d\n", above_8);
    printf("------------------------\n");
}

int main() {
    int n, key, choice;

    while (1) {
        printf("\nWhat do you want to do?\n");
        printf("1. Add record\n");
        printf("2. Search record\n");
        printf("3. Sort file\n");
        printf("4. Display all records\n");
        printf("5. Update a record\n");
        printf("6. Delete a record\n");
        printf("7. Generate statistics\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  // clear input buffer

        switch (choice) {
            case 1:
                addRecord();
                break;

            case 2:
                printf("Enter roll number to search: ");
                scanf("%d", &key);
                search(key);
                break;

            case 3:
                sortFileByChoice();
                break;
            case 4:
                displayRecords();
                break;
            case 5:
                updateRecord();
                break;
            case 6:
                deleteRecord();
                break;
            case 7:
                generateStatistics();
                break;
            case 8:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
