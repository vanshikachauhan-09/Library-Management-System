# Library Management System (C++)
This is a simple C++ project for managing a library through a menu in the terminal. It allows users to add books, register members, issue and return books, search for books, and view borrowing records. The data is stored in text files, so it is available even after restarting the program.

## Files
| File | Purpose |
|---|---|
| `library.h` | Contains the `Book`, `Member` and `Record` structures and the `Library` class |
| `library.cpp` | Contains the functions used to manage the library |
| `main.cpp` | Displays the menu and handles user input |
| `books.txt` | Contains 10 sample books |
| `members.txt` | Contains 2 sample members |
| `records.txt` | Stores book issue and return details (initially empty) |

 How to Run

Compile the program using:

```bash
g++ -o library main.cpp library.cpp
```

Run it using:

```bash
./library
```

On Windows, run `library.exe` instead.
Make sure you run the program from the folder containing the text files.

## Menu Options
1. Add a book
2. Register a member
3. Issue a book
4. Return a book
5. Search for books by title, author or ISBN
6. Display all books
7. Display all members
8. View borrowing records
9. Exit

How Issuing and Returning Work

Issuing a book
- The book and member must already exist.
- A book cannot be issued if no copies are available.
- A member cannot issue the same book again while they still have it.
- When a book is issued, its available count decreases by one.
- The issue date is set to the current date, and the due date is 14 days later.

Returning a book
- The program checks for an active borrowing record for that member and book.
- After the book is returned, the return date is saved and the available copy count increases by one.
- If there is no active record, the book is not marked as returned.

If a book with an existing ISBN is added again, the number of copies is increased instead of creating a separate entry.

 Data Storage
The program uses `|` to separate fields in the text files.

- `books.txt`: `isbn|title|author|total|available`
- `members.txt`: `id|name`
- `records.txt`: `isbn|memberId|issueDate|dueDate|returnDate`

A `-` in the return date field means that the book has not been returned yet.

The program loads the saved data when it starts and updates the files whenever a change is made.

## Testing
I tested the program using the sample books and members.

| Step | Test | Result |
|---|---|---|
| 1 | Issued book `9780596007126` to member 1 | Book issued successfully with a 14-day due period |
| 2 | Tried issuing the same book to member 2 when no copies were left | Issuing was prevented |
| 3 | Checked the book list | Available copies showed `0/1` |
| 4 | Returned the book for member 1 | Book returned successfully |
| 5 | Checked the book list again | Available copies showed `1/1` |
| 6 | Searched for `clean` | Clean Code and Clean Architecture were found |
| 7 | Restarted the program and checked the books and records | Saved data was still available |

## Author
Vanshika Chauhan

