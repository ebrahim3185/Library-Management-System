#include <iostream>
#include <string>
using namespace std;

// Book class
class Book
{
public:
    int code; // Unique book ID
    string title;
    string author;
    int year;
    bool available; // true: Available, false: Borrowed

    Book(int c = 0, string t = "", string a = "", int y = 0, bool av = true)
        : code(c), title(t), author(a), year(y), available(av) {}

    void display() const
    {
        cout << "Code: " << code << " | Title: " << title
             << " | Author: " << author << " | Year: " << year
             << " | Status: " << (available ? "Available" : "Borrowed") << endl;
    }
};

// Doubly linked list node for books
struct BookNode
{
    Book book;
    BookNode *next;
    BookNode *prev;

    BookNode(Book b) : book(b), next(nullptr), prev(nullptr) {}
};

// Library management class
class Library
{
private:
    BookNode *head;
    BookNode *tail;
    int size;

public:
    Library() : head(nullptr), tail(nullptr), size(0) {}

    ~Library()
    {
        BookNode *current = head;
        while (current)
        {
            BookNode *next = current->next;
            delete current;
            current = next;
        }
    }

    // Add a book (to the end)
    void addBook(Book b)
    {
        BookNode *newNode = new BookNode(b);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
        cout << "Book with code " << b.code << " added successfully.\n";
    }

    // Remove book by code
    bool removeBook(int code)
    {
        BookNode *current = head;
        while (current)
        {
            if (current->book.code == code)
            {
                if (current->prev)
                    current->prev->next = current->next;
                else
                    head = current->next;

                if (current->next)
                    current->next->prev = current->prev;
                else
                    tail = current->prev;

                delete current;
                size--;
                cout << "Book with code " << code << " removed.\n";
                return true;
            }
            current = current->next;
        }
        cout << "Book with code " << code << " not found.\n";
        return false;
    }

    // Search book by code
    Book *searchBook(int code)
    {
        BookNode *current = head;
        while (current)
        {
            if (current->book.code == code)
            {
                return &(current->book);
            }
            current = current->next;
        }
        return nullptr;
    }

    // Display all books
    void displayAll() const
    {
        if (size == 0)
        {
            cout << "Library is empty.\n";
            return;
        }
        BookNode *current = head;
        cout << "List of books:\n";
        while (current)
        {
            current->book.display();
            current = current->next;
        }
    }

    // Sort by code (Bubble Sort)
    void sortByCode()
    {
        if (size <= 1)
            return;
        bool swapped;
        do
        {
            swapped = false;
            BookNode *current = head;
            while (current->next)
            {
                if (current->book.code > current->next->book.code)
                {
                    swap(current->book, current->next->book);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
        cout << "Books sorted by code.\n";
    }

    // Sort by publication year
    void sortByYear()
    {
        if (size <= 1)
            return;
        bool swapped;
        do
        {
            swapped = false;
            BookNode *current = head;
            while (current->next)
            {
                if (current->book.year > current->next->book.year)
                {
                    swap(current->book, current->next->book);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
        cout << "Books sorted by year.\n";
    }

    // Change availability status
    bool setAvailability(int code, bool avail)
    {
        Book *b = searchBook(code);
        if (b)
        {
            b->available = avail;
            return true;
        }
        return false;
    }
};

// Loan request structure
struct LoanRequest
{
    int bookCode;
    string borrowerName;

    LoanRequest(int c, string name) : bookCode(c), borrowerName(name) {}
};

// Queue node for loan requests
struct RequestNode
{
    LoanRequest request;
    RequestNode *next;

    RequestNode(LoanRequest r) : request(r), next(nullptr) {}
};

// Queue for loan requests (FIFO)
class LoanQueue
{
private:
    RequestNode *front;
    RequestNode *rear;

public:
    LoanQueue() : front(nullptr), rear(nullptr) {}

    ~LoanQueue()
    {
        while (front)
        {
            RequestNode *temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(int code, string name)
    {
        RequestNode *newNode = new RequestNode(LoanRequest(code, name));
        if (!rear)
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Loan request from " << name << " for book code " << code << " added to queue.\n";
    }

    LoanRequest dequeue()
    {
        if (!front)
        {
            throw runtime_error("Queue is empty!");
        }
        RequestNode *temp = front;
        LoanRequest req = temp->request;
        front = front->next;
        if (!front)
            rear = nullptr;
        delete temp;
        return req;
    }

    bool isEmpty() const { return front == nullptr; }

    void display() const
    {
        if (isEmpty())
        {
            cout << "No pending requests.\n";
            return;
        }
        RequestNode *current = front;
        cout << "Pending loan requests:\n";
        while (current)
        {
            cout << "Book code: " << current->request.bookCode
                 << " | Borrower: " << current->request.borrowerName << endl;
            current = current->next;
        }
    }
};

// Stack for operation history (last 5 operations)
class HistoryStack
{
private:
    string history[5];
    int top;

public:
    HistoryStack() : top(-1) {}

    void push(string operation)
    {
        if (top >= 4)
        {
            // Shift to remove oldest
            for (int i = 0; i < 4; i++)
            {
                history[i] = history[i + 1];
            }
            history[4] = operation;
        }
        else
        {
            top++;
            history[top] = operation;
        }
    }

    void display() const
    {
        if (top < 0)
        {
            cout << "History is empty.\n";
            return;
        }
        cout << "Operation history (newest first):\n";
        for (int i = top; i >= 0; i--)
        {
            cout << "- " << history[i] << endl;
        }
    }
};

int main()
{
    Library library;
    LoanQueue loanQueue;
    HistoryStack history;

    int choice;
    do
    {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Add Book\n";
        cout << "2. Remove Book\n";
        cout << "3. Search Book\n";
        cout << "4. Display All Books\n";
        cout << "5. Sort by Code\n";
        cout << "6. Sort by Year\n";
        cout << "7. Request Loan\n";
        cout << "8. Process Next Loan Request\n";
        cout << "9. Return Book\n";
        cout << "10. Show Loan Queue\n";
        cout << "11. Show Operation History\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            int code, year;
            string title, author;
            cout << "Book code: ";
            cin >> code;
            cin.ignore();
            cout << "Title: ";
            getline(cin, title);
            cout << "Author: ";
            getline(cin, author);
            cout << "Publication year: ";
            cin >> year;
            Book b(code, title, author, year);
            library.addBook(b);
            history.push("Added book with code " + to_string(code));
        }
        else if (choice == 2)
        {
            int code;
            cout << "Book code to remove: ";
            cin >> code;
            if (library.removeBook(code))
            {
                history.push("Removed book with code " + to_string(code));
            }
        }
        else if (choice == 3)
        {
            int code;
            cout << "Book code to search: ";
            cin >> code;
            Book *b = library.searchBook(code);
            if (b)
                b->display();
            else
                cout << "Book not found.\n";
        }
        else if (choice == 4)
        {
            library.displayAll();
        }
        else if (choice == 5)
        {
            library.sortByCode();
            history.push("Sorted by code");
        }
        else if (choice == 6)
        {
            library.sortByYear();
            history.push("Sorted by year");
        }
        else if (choice == 7)
        {
            int code;
            string name;
            cout << "Book code: ";
            cin >> code;
            cin.ignore();
            cout << "Borrower name: ";
            getline(cin, name);
            loanQueue.enqueue(code, name);
            history.push("Loan request for code " + to_string(code) + " by " + name);
        }
        else if (choice == 8)
        {
            try
            {
                LoanRequest req = loanQueue.dequeue();
                Book *b = library.searchBook(req.bookCode);
                if (b && b->available)
                {
                    b->available = false;
                    cout << "Book code " << req.bookCode << " loaned to " << req.borrowerName << ".\n";
                    history.push("Loaned book code " + to_string(req.bookCode) + " to " + req.borrowerName);
                }
                else
                {
                    cout << "Book is not available. Request returned to queue.\n";
                    loanQueue.enqueue(req.bookCode, req.borrowerName);
                }
            }
            catch (...)
            {
                cout << "No pending requests.\n";
            }
        }
        else if (choice == 9)
        {
            int code;
            cout << "Returned book code: ";
            cin >> code;
            if (library.setAvailability(code, true))
            {
                cout << "Book returned and now available.\n";
                history.push("Returned book code " + to_string(code));
            }
        }
        else if (choice == 10)
        {

            loanQueue.display();
        }
        else if (choice == 11)
        {
            history.display();
        }

    } while (choice != 0);

    cout << "Goodbye!\n";
    return 0;
}