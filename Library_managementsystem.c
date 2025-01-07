#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100

// Structure to store book details
struct Book {
    int bookID;
    char title[100];
    int totalBooks;
    int borrowedBooks;
};

// Structure to store borrower details
struct Borrower {
    int bookID;
    char borrowerName[50];
    struct tm issueDate;
    struct tm dueDate;
    int isReturned; // 0 = not returned, 1 = returned
};

// Library structure containing books and borrower data
struct Library {
    struct Book books[MAX_BOOKS];
    struct Borrower borrowers[MAX_BOOKS];
    int bookCount;
    int borrowerCount;
};

// Function to initialize the library
void initializeLibrary(struct Library *lib) {
    lib->bookCount = 0;
    lib->borrowerCount = 0;
}

// Function to add a book to the library
void addBook(struct Library *lib, int bookID, const char *title, int totalBooks) {
    struct Book newBook = {bookID, "", totalBooks, 0};
    strcpy(newBook.title, title);
    lib->books[lib->bookCount++] = newBook;
}

// Function to borrow a book
void borrowBook(struct Library *lib) {
    int bookID, day, month, year;
    char borrowerName[50];

    printf("Enter Book ID to borrow: ");
    scanf("%d", &bookID);

    // Find the book in the library
    for (int i = 0; i < lib->bookCount; i++) {
        if (lib->books[i].bookID == bookID) {
            if (lib->books[i].borrowedBooks < lib->books[i].totalBooks) {
                printf("Enter Borrower's Name: ");
                scanf(" %[^\n]%*c", borrowerName);  // Read a full line (with spaces)

                printf("Enter Issue Date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);

                // Set the issue date and calculate the due date (14 days from issue)
                struct tm issueDate = {0};
                issueDate.tm_mday = day;
                issueDate.tm_mon = month - 1; // Months are 0-based
                issueDate.tm_year = year - 1900; // tm_year is years since 1900

                struct tm dueDate = issueDate;
                dueDate.tm_mday += 14; // Setting due date to 14 days from the issue date
                mktime(&dueDate); // Normalize the due date

                // Add borrower information
                struct Borrower newBorrower = {bookID, "", issueDate, dueDate, 0};
                strcpy(newBorrower.borrowerName, borrowerName);

                lib->borrowers[lib->borrowerCount++] = newBorrower;
                lib->books[i].borrowedBooks++;
                printf("Book '%s' borrowed successfully by %s.\n", lib->books[i].title, borrowerName);
                return;
            } else {
                printf("No copies of this book are available for borrowing.\n");
                return;
            }
        }
    }
    printf("Book with ID %d not found in the library.\n", bookID);
}

/*Yeh function returnBook ek book return karne aur late fee calculate karne ke liye banaye hai. 
Yeh function Library structure ka pointer lib leta hai jo library ke data ko access karne mein help karta hai*/
void returnBook(struct Library *lib) {
    int bookID, day, month, year;
    char borrowerName[50];

    printf("Enter Book ID to return: ");
    scanf("%d", &bookID);
    printf("Enter Borrower's Name: ");
    scanf(" %[^\n]%*c", borrowerName); /*Yahaan user se bookID aur borrower ka naam liya ja raha hai. 
    scanf(" %[^\n]%*c", borrowerName); syntax kause pura line (jo spaces ke saath ho) read karne ke 
    liye gya hai,*/

    for (int i = 0; i < lib->borrowerCount; i++) {
        if (lib->borrowers[i].bookID == bookID && strcmp(lib->borrowers[i].borrowerName, borrowerName) == 0 && lib->borrowers[i].isReturned == 0) {
            printf("Enter Return Date (DD MM YYYY): ");
            scanf("%d %d %d", &day, &month, &year);
            /*Yahaan ek for loop hai jo library ke borrowerCount tak iterate karta hai aur check karta hai ki bookID,
             borrowerName, aur isReturned condition match ho. Iska matlab hai ki agar book ID aur borrower ka 
             naam match karte hain aur woh book ab tak return nahi hui hai (isReturned == 0), toh yeh code 
             block execute hoga.*/

            struct tm due = lib->borrowers[i].dueDate;   /*due mein book ki due date hai 
            jo borrower ke structure mein pehle se stored hai.*/
            struct tm returned = {0};
            returned.tm_mday = day;
            returned.tm_mon = month - 1; //returned ek nayi tm structure hai jo user ke input ko store karti hai (day, month, aur year).
            returned.tm_year = year - 1900;
            time_t dueTime = mktime(&due);
            time_t returnTime = mktime(&returned);

            
            // Calculate the difference in days
            double diffInSeconds = difftime(returnTime, dueTime);
            int diffInDays = diffInSeconds / (60 * 60 * 24);

            if (diffInDays > 0) {
                int lateFee = diffInDays * 5;
                printf("Book '%s' returned late. Late fee: %d rupees.\n", lib->books[bookID - 1].title, lateFee);
            } else {
                printf("Book '%s' returned on time.\n", lib->books[bookID - 1].title);
            }

            // Mark the book as returned
            lib->borrowers[i].isReturned = 1;
            lib->books[bookID - 1].borrowedBooks--;
            return;
        }
    }
    printf("No record found for this book borrowing.\n");
}

// Function to display available books
void displayBooks(struct Library *lib) {
    printf("\nBooks in the Library:\n");
    for (int i = 0; i < lib->bookCount; i++) {
        printf("ID: %d, Title: %s, Total Books: %d, Borrowed: %d, Available: %d\n", lib->books[i].bookID, lib->books[i].title, lib->books[i].totalBooks, lib->books[i].borrowedBooks, lib->books[i].totalBooks - lib->books[i].borrowedBooks);
    }
}

int main() {
    struct Library lib;
    initializeLibrary(&lib);

    // Adding books to the library
    addBook(&lib, 1, "The C Programming Language", 5);
    addBook(&lib, 2, "Data Structures and Algorithms", 3);
    addBook(&lib, 3, "Operating Systems Concepts", 4);

    int choice;
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Display Available Books\n");
        printf("2. Borrow Book\n");
        printf("3. Return Book\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBooks(&lib);
                break;
            case 2:
                borrowBook(&lib);
                break;
            case 3:
                returnBook(&lib);
                break;
            case 4:
                printf("Exiting the Library Management System.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}