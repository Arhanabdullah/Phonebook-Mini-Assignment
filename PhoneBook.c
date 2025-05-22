#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 5000
#define NAME_LENGTH 50
#define PHONE_LENGTH 25 // extended to support ISD code

// Contact structure
typedef struct {
    char name[NAME_LENGTH];
    char phone[PHONE_LENGTH];
} Contact;

Contact phonebook[MAX_CONTACTS];
int contactCount = 0;

// Function to validate 10-digit local phone number
int isValidPhoneNumber(char* number) {
    if (strlen(number) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (number[i] < '0' || number[i] > '9') return 0;
    }
    return 1;
}

// Function to select ISD code
char* selectISDCode() {
    static char isd[5];
    int choice;
    printf("\n--- Select Country for ISD Code ---\n");
    printf("1. India (+91)\n");
    printf("2. USA (+1)\n");
    printf("3. UK (+44)\n");
    printf("4. Canada (+1)\n");
    printf("5. Australia (+61)\n");
    printf("6. Germany (+49)\n");
    printf("7. France (+33)\n");
    printf("8. China (+86)\n");
    printf("9. Japan (+81)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: strcpy(isd, "+91"); break;
        case 2: strcpy(isd, "+1"); break;
        case 3: strcpy(isd, "+44"); break;
        case 4: strcpy(isd, "+1"); break;
        case 5: strcpy(isd, "+61"); break;
        case 6: strcpy(isd, "+49"); break;
        case 7: strcpy(isd, "+33"); break;
        case 8: strcpy(isd, "+86"); break;
        case 9: strcpy(isd, "+81"); break;
        default: printf("Invalid choice! Defaulting to +91\n"); strcpy(isd, "+91"); break;
    }
    return isd;
}

// Function to save contacts to file
void saveToFile() {
    FILE *file = fopen("contacts.txt", "w");
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }
    for (int i = 0; i < contactCount; i++) {
        fprintf(file, "%s %s\n", phonebook[i].name, phonebook[i].phone);
    }
    fclose(file);
}

// Function to load contacts from file
void loadFromFile() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%s %s", phonebook[contactCount].name, phonebook[contactCount].phone) != EOF) {
        contactCount++;
    }
    fclose(file);
}

// Function to sort contacts alphabetically
void sortContacts() {
    for (int i = 0; i < contactCount - 1; i++) {
        for (int j = i + 1; j < contactCount; j++) {
            if (strcmp(phonebook[i].name, phonebook[j].name) > 0) {
                Contact temp = phonebook[i];
                phonebook[i] = phonebook[j];
                phonebook[j] = temp;
            }
        }
    }
}

// Function to add a contact
void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf("Phonebook is full!\n");
        return;
    }
    printf("Enter name: ");
    scanf("%s", phonebook[contactCount].name);

    char localNumber[20];
    while (1) {
        printf("Enter 10-digit phone number: ");
        scanf("%s", localNumber);
        if (isValidPhoneNumber(localNumber)) break;
        else printf("❌ Invalid! Phone number must be exactly 10 digits.\n");
    }

    char* isd = selectISDCode();
    strcpy(phonebook[contactCount].phone, isd);
    strcat(phonebook[contactCount].phone, localNumber);

    contactCount++;
    sortContacts();
    saveToFile();
    printf("Contact added successfully.\n");
}

// Function to view all contacts
void viewContacts() {
    printf("\nContact List:\n");
    for (int i = 0; i < contactCount; i++) {
        printf("%d. Name: %s, Phone: %s\n", i + 1, phonebook[i].name, phonebook[i].phone);
    }
}

// Function to search a contact by name
void searchContact() {
    char searchName[NAME_LENGTH];
    printf("Enter name to search: ");
    scanf("%s", searchName);
    for (int i = 0; i < contactCount; i++) {
        if (strcmp(phonebook[i].name, searchName) == 0) {
            printf("Found: %s - %s\n", phonebook[i].name, phonebook[i].phone);
            return;
        }
    }
    printf("Contact not found.\n");
}

// Function to delete a contact by name
void deleteContact() {
    char deleteName[NAME_LENGTH];
    printf("Enter name to delete: ");
    scanf("%s", deleteName);
    for (int i = 0; i < contactCount; i++) {
        if (strcmp(phonebook[i].name, deleteName) == 0) {
            for (int j = i; j < contactCount - 1; j++) {
                phonebook[j] = phonebook[j + 1];
            }
            contactCount--;
            sortContacts();
            saveToFile();
            printf("Contact deleted successfully.\n");
            return;
        }
    }
    printf("Contact not found.\n");
}

// Function to edit a contact
void editContact() {
    char editName[NAME_LENGTH];
    printf("Enter name to edit: ");
    scanf("%s", editName);
    for (int i = 0; i < contactCount; i++) {
        if (strcmp(phonebook[i].name, editName) == 0) {
            printf("Enter new name: ");
            scanf("%s", phonebook[i].name);

            char localNumber[20];
            while (1) {
                printf("Enter new 10-digit phone number: ");
                scanf("%s", localNumber);
                if (isValidPhoneNumber(localNumber)) break;
                else printf("❌ Invalid! Phone number must be exactly 10 digits.\n");
            }

            char* isd = selectISDCode();
            strcpy(phonebook[i].phone, isd);
            strcat(phonebook[i].phone, localNumber);

            sortContacts();
            saveToFile();
            printf("Contact updated.\n");
            return;
        }
    }
    printf("Contact not found.\n");
}

int main() {
    int choice;
    loadFromFile();
    while (1) {
        printf("\nPhonebook Menu:\n");
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Edit Contact\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5: editContact(); break;
            case 6: saveToFile(); exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}