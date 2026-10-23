#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>
using namespace std;

struct Book {
    string isbn;
    string title;
    string author;
    int total;
    int available;
};

struct Member {
    int id;
    string name;
};

struct Record {
    string isbn;
    int memberId;
    string issueDate;
    string dueDate;
    string returnDate;
};

vector<Book> books;
vector<Member> members;
vector<Record> records;

string toLower(string s) {
    for (int i = 0; i < (int)s.size(); i++)
        s[i] = tolower(s[i]);
    return s;
}

vector<string> split(string line) {
    vector<string> parts;
    string word = "";
    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] == '|') {
            parts.push_back(word);
            word = "";
        } else {
            word += line[i];
        }
    }
    parts.push_back(word);
    return parts;
}

string getDate(int extraDays) {
    time_t t = time(0) + extraDays * 86400;
    tm* now = localtime(&t);
    char buf[11];
    strftime(buf, 11, "%Y-%m-%d", now);
    return string(buf);
}

int findBook(string isbn) {
    for (int i = 0; i < (int)books.size(); i++)
        if (books[i].isbn == isbn)
            return i;
    return -1;
}

int findMember(int id) {
    for (int i = 0; i < (int)members.size(); i++)
        if (members[i].id == id)
            return i;
    return -1;
}

void loadData() {
    string line;

    ifstream bookFile("books.txt");
    while (getline(bookFile, line)) {
        vector<string> p = split(line);
        if (p.size() < 5) continue;
        Book b;
        b.isbn = p[0];
        b.title = p[1];
        b.author = p[2];
        b.total = stoi(p[3]);
        b.available = stoi(p[4]);
        books.push_back(b);
    }
    bookFile.close();

    ifstream memberFile("members.txt");
    while (getline(memberFile, line)) {
        vector<string> p = split(line);
        if (p.size() < 2) continue;
        Member m;
        m.id = stoi(p[0]);
        m.name = p[1];
        members.push_back(m);
    }
    memberFile.close();

    ifstream recordFile("records.txt");
    while (getline(recordFile, line)) {
        vector<string> p = split(line);
        if (p.size() < 5) continue;
        Record r;
        r.isbn = p[0];
        r.memberId = stoi(p[1]);
        r.issueDate = p[2];
        r.dueDate = p[3];
        r.returnDate = p[4];
        records.push_back(r);
    }
    recordFile.close();
}

void saveData() {
    ofstream bookFile("books.txt");
    for (int i = 0; i < (int)books.size(); i++)
        bookFile << books[i].isbn << "|" << books[i].title << "|" << books[i].author
                 << "|" << books[i].total << "|" << books[i].available << "\n";
    bookFile.close();

    ofstream memberFile("members.txt");
    for (int i = 0; i < (int)members.size(); i++)
        memberFile << members[i].id << "|" << members[i].name << "\n";
    memberFile.close();

    ofstream recordFile("records.txt");
    for (int i = 0; i < (int)records.size(); i++)
        recordFile << records[i].isbn << "|" << records[i].memberId << "|"
                   << records[i].issueDate << "|" << records[i].dueDate << "|"
                   << records[i].returnDate << "\n";
    recordFile.close();
}

void addBook(string isbn, string title, string author, int copies) {
    if (copies <= 0) {
        cout << "Copies must be at least 1.\n";
        return;
    }
    int index = findBook(isbn);
    if (index != -1) {
        books[index].total += copies;
        books[index].available += copies;
        cout << "Book already exists. Added " << copies << " more copies.\n";
    } else {
        Book b;
        b.isbn = isbn;
        b.title = title;
        b.author = author;
        b.total = copies;
        b.available = copies;
        books.push_back(b);
        cout << "Book added.\n";
    }
    saveData();
}

void registerMember(string name) {
    Member m;
    m.id = 1;
    for (int i = 0; i < (int)members.size(); i++)
        if (members[i].id >= m.id)
            m.id = members[i].id + 1;
    m.name = name;
    members.push_back(m);
    saveData();
    cout << "Member registered with ID " << m.id << ".\n";
}

void issueBook(string isbn, int memberId) {
    int b = findBook(isbn);
    if (b == -1) {
        cout << "Book not found.\n";
        return;
    }
    if (findMember(memberId) == -1) {
        cout << "Member not found.\n";
        return;
    }
    if (books[b].available == 0) {
        cout << "No copies available. Cannot issue.\n";
        return;
    }
    for (int i = 0; i < (int)records.size(); i++) {
        if (records[i].isbn == isbn && records[i].memberId == memberId && records[i].returnDate == "-") {
            cout << "This member already has this book.\n";
            return;
        }
    }
    Record r;
    r.isbn = isbn;
    r.memberId = memberId;
    r.issueDate = getDate(0);
    r.dueDate = getDate(14);
    r.returnDate = "-";
    records.push_back(r);
    books[b].available--;
    saveData();
    cout << "Book issued. Due date: " << r.dueDate << "\n";
}

void returnBook(string isbn, int memberId) {
    for (int i = 0; i < (int)records.size(); i++) {
        if (records[i].isbn == isbn && records[i].memberId == memberId && records[i].returnDate == "-") {
            records[i].returnDate = getDate(0);
            int b = findBook(isbn);
            if (b != -1)
                books[b].available++;
            saveData();
            cout << "Book returned.\n";
            return;
        }
    }
    cout << "No active borrowing found for this book and member.\n";
}

void searchBooks(string keyword) {
    string key = toLower(keyword);
    bool found = false;
    for (int i = 0; i < (int)books.size(); i++) {
        if (toLower(books[i].title).find(key) != string::npos ||
            toLower(books[i].author).find(key) != string::npos ||
            books[i].isbn.find(keyword) != string::npos) {
            cout << books[i].isbn << " | " << books[i].title << " | " << books[i].author
                 << " | Available: " << books[i].available << "/" << books[i].total << "\n";
            found = true;
        }
    }
    if (!found)
        cout << "No books found.\n";
}

void showBooks() {
    if (books.empty()) {
        cout << "No books in library.\n";
        return;
    }
    for (int i = 0; i < (int)books.size(); i++)
        cout << books[i].isbn << " | " << books[i].title << " | " << books[i].author
             << " | Available: " << books[i].available << "/" << books[i].total << "\n";
}

void showMembers() {
    if (members.empty()) {
        cout << "No members registered.\n";
        return;
    }
    for (int i = 0; i < (int)members.size(); i++)
        cout << members[i].id << " | " << members[i].name << "\n";
}

void showRecords() {
    if (records.empty()) {
        cout << "No borrowing records.\n";
        return;
    }
    for (int i = 0; i < (int)records.size(); i++)
        cout << "ISBN: " << records[i].isbn << " | Member: " << records[i].memberId
             << " | Issued: " << records[i].issueDate << " | Due: " << records[i].dueDate
             << " | Returned: " << records[i].returnDate << "\n";
}

int main() {
    loadData();

    int choice = -1;
    while (choice != 0) {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Add book\n";
        cout << "2. Register member\n";
        cout << "3. Issue book\n";
        cout << "4. Return book\n";
        cout << "5. Search books\n";
        cout << "6. Show all books\n";
        cout << "7. Show all members\n";
        cout << "8. Show borrowing records\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        string isbn, title, author, name, keyword;
        int copies, memberId;

        if (choice == 1) {
            cout << "ISBN: ";
            getline(cin, isbn);
            cout << "Title: ";
            getline(cin, title);
            cout << "Author: ";
            getline(cin, author);
            cout << "Copies: ";
            cin >> copies;
            cin.ignore(1000, '\n');
            addBook(isbn, title, author, copies);
        } else if (choice == 2) {
            cout << "Name: ";
            getline(cin, name);
            registerMember(name);
        } else if (choice == 3) {
            cout << "ISBN: ";
            getline(cin, isbn);
            cout << "Member ID: ";
            cin >> memberId;
            cin.ignore(1000, '\n');
            issueBook(isbn, memberId);
        } else if (choice == 4) {
            cout << "ISBN: ";
            getline(cin, isbn);
            cout << "Member ID: ";
            cin >> memberId;
            cin.ignore(1000, '\n');
            returnBook(isbn, memberId);
        } else if (choice == 5) {
            cout << "Search by title, author or ISBN: ";
            getline(cin, keyword);
            searchBooks(keyword);
        } else if (choice == 6) {
            showBooks();
        } else if (choice == 7) {
            showMembers();
        } else if (choice == 8) {
            showRecords();
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}