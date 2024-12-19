#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm> // For find_if

using namespace std;

// Enum for genres
enum class Genre { Fiction, Nonfiction, Periodical, Biography, Children };

// Convert Genre to string
string genre_to_string(Genre g) {
    switch (g) {
        case Genre::Fiction: return "Fiction";
        case Genre::Nonfiction: return "Nonfiction";
        case Genre::Periodical: return "Periodical";
        case Genre::Biography: return "Biography";
        case Genre::Children: return "Children";
        default: return "Unknown";
    }
}

// Book class
class Book {
    string isbn;
    string title;
    string author;
    int copyright_date;
    bool checked_out;
    Genre genre;

public:
    Book(string isbn, string title, string author, int copyright_date, Genre genre)
        : isbn(isbn), title(title), author(author), copyright_date(copyright_date), checked_out(false), genre(genre) {}

    string get_isbn() const { return isbn; }
    string get_title() const { return title; }
    string get_author() const { return author; }
    int get_copyright_date() const { return copyright_date; }
    Genre get_genre() const { return genre; }
    bool is_checked_out() const { return checked_out; }

    void check_out() { checked_out = true; }
    void check_in() { checked_out = false; }

    bool operator==(const Book& other) const { return isbn == other.isbn; }
    bool operator!=(const Book& other) const { return !(*this == other); }

    friend ostream& operator<<(ostream& os, const Book& book) {
        os << "Title: " << book.title << "\n"
           << "Author: " << book.author << "\n"
           << "ISBN: " << book.isbn << "\n"
           << "Genre: " << genre_to_string(book.genre) << "\n";
        return os;
    }
};

// Patron class
class Patron {
    string user_name;
    string card_number;
    int owed_fees;

public:
    Patron(string user_name, string card_number, int owed_fees = 0)
        : user_name(user_name), card_number(card_number), owed_fees(owed_fees) {}

    string get_user_name() const { return user_name; }
    string get_card_number() const { return card_number; }
    int get_owed_fees() const { return owed_fees; }

    bool owes_fees() const { return owed_fees > 0; }
    void set_fees(int fees) { owed_fees = fees; }
};

// Transaction class
class Transaction {
    Book book;
    Patron patron;
    string activity;
    string date;

public:
    Transaction(Book book, Patron patron, string activity, string date)
        : book(book), patron(patron), activity(activity), date(date) {}

    string get_description() const {
        return "Transaction: " + activity + " | Book: " + book.get_title() + " | Patron: " + patron.get_user_name() +
               " | Date: " + date;
    }
};

// Library class
class Library {
    vector<Book> books;
    vector<Patron> patrons;
    vector<Transaction> transactions;
    vector<string> errors;

public:
    void add_book(const Book& book) { books.push_back(book); }
    void add_patron(const Patron& patron) { patrons.push_back(patron); }

    const vector<Book>& get_books() const { return books; }
    const vector<Transaction>& get_transactions() const { return transactions; }
    const vector<string>& get_errors() const { return errors; }

    void check_out_book(const string& isbn, const string& card_number, const string& date) {
        auto book = find_if(books.begin(), books.end(), [&](const Book& b) { return b.get_isbn() == isbn; });
        auto patron = find_if(patrons.begin(), patrons.end(), [&](const Patron& p) { return p.get_card_number() == card_number; });

        if (book == books.end()) {
            errors.push_back("Error: Book with ISBN " + isbn + " not found.");
            return;
        }

        if (patron == patrons.end()) {
            errors.push_back("Error: Patron with card number " + card_number + " not found.");
            return;
        }

        if (patron->owes_fees()) {
            errors.push_back("Error: Patron owes fees.");
            return;
        }

        if (book->is_checked_out()) {
            errors.push_back("Error: Book is already checked out.");
            return;
        }

        book->check_out();
        transactions.emplace_back(*book, *patron, "Check Out", date);
    }

    vector<string> get_fee_owing_patrons() const {
        vector<string> owing_patrons;
        for (const auto& patron : patrons) {
            if (patron.owes_fees()) {
                owing_patrons.push_back(patron.get_user_name());
            }
        }
        return owing_patrons;
    }
};

// Main function
int main() {
    Library library;

    // Sample data
    library.add_book(Book("123", "C++ Programming", "Bjarne Stroustrup", 2013, Genre::Nonfiction));
    library.add_book(Book("124", "Python", "Bjarne Stroustrup", 2013, Genre::Nonfiction));
    library.add_book(Book("456", "Harry Potter", "J.K. Rowling", 1997, Genre::Fiction));
    library.add_patron(Patron("Alice", "P001", 0));
    library.add_patron(Patron("Bob", "P002", 10));

    // Simulate a checkout
    library.check_out_book("123", "P001", "2024-11-27");
    library.check_out_book("124", "P001", "2024-11-27");

    while (true) {
        cout << "\nLibrary Menu:\n";
        cout << "1. List all books\n";
        cout << "2. List patrons who owe fees\n";
        cout << "3. List all transactions\n";
        cout << "4. List all errors in transactions\n";
        cout << "5. Add new book\n";
        cout << "6. Add new Patron\n";
        cout << "7. Add new transaction\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input. Please enter a number between 1 and 8.\n";
            continue;
        }

        if (choice == 1) {
            cout << "Books in Library:\n";
            for (const auto& book : library.get_books()) {
                cout << book << endl; // Uses overloaded << operator
            }
        } else if (choice == 2) {
            auto owing_patrons = library.get_fee_owing_patrons();
            if (owing_patrons.empty()) {
                cout << "No patrons owe fees.\n";
            } else {
                cout << "Patrons who owe fees:\n";
                for (const auto& name : owing_patrons) {
                    cout << name << endl;
                }
            }
        } else if (choice == 3) {
            cout << "List of Transactions:\n";
            for (const auto& transaction : library.get_transactions()) {
                cout << transaction.get_description() << endl;
            }
        } else if (choice == 4) {
            cout << "Failed Transactions (Errors):\n";
            const auto& errors = library.get_errors();
            if (errors.empty()) {
                cout << "No errors recorded.\n";
            } else {
                for (const auto& error : errors) {
                    cout << error << endl;
                }
            }
        } else if (choice == 5) {
            string isbn, title, author;
            int year, genre_choice;
            cout << "Enter ISBN: ";
            cin >> isbn;
            cin.ignore(); // Ignore leftover newline
            cout << "Enter Title: ";
            getline(cin, title);
            cout << "Enter Author: ";
            getline(cin, author);
            cout << "Enter Year: ";
            cin >> year;
            cout << "Select Genre: (0: Fiction, 1: Nonfiction, 2: Periodical, 3: Biography, 4: Children): ";
            cin >> genre_choice;
            if (genre_choice >= 0 && genre_choice <= 4) {
                library.add_book(Book(isbn, title, author, year, static_cast<Genre>(genre_choice)));
                cout << "Book added successfully!\n";
            } else {
                cout << "Invalid genre choice. Book not added.\n";
            }
        } else if (choice == 6) {
            string name, card;
            int fees;
            cout << "Enter Patron Name: ";
            cin.ignore(); // Ignore leftover newline
            getline(cin, name);
            cout << "Enter Card Number: ";
            cin >> card;
            cout << "Enter Fees Owed: ";
            cin >> fees;
            library.add_patron(Patron(name, card, fees));
            cout << "Patron added successfully!\n";
        } else if (choice == 7) {
            string isbn, card_number, date;
            cout << "Enter ISBN of the book: ";
            cin >> isbn;
            cout << "Enter Patron Card Number: ";
            cin >> card_number;
            cout << "Enter Date (YYYY-MM-DD): ";
            cin >> date;
            library.check_out_book(isbn, card_number, date);
        } else if (choice == 8) {
            cout << "Exiting the program. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
