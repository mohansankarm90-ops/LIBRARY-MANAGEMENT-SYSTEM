#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class Book {
public:
    int id;
    string title, author;
    bool issued;   // false = available

    void create() {
        cout << "Book ID: "; cin >> id;
        cin.ignore();
        cout << "Title: "; getline(cin, title);
        cout << "Author: "; getline(cin, author);
        issued = false;
    }

    void display() {
        cout << "\nID: " << id << "\nTitle: " << title
             << "\nAuthor: " << author
             << "\nStatus: " << (issued ? "Issued" : "Available") << endl;
    }
};

void addBook(Book b) {
    ofstream f("library.dat", ios::binary | ios::app);
    f.write((char*)&b, sizeof(b));
    f.close();
}

// Search by title or author (type: 't' or 'a')
void search(string key, char type) {
    Book b;
    ifstream f("library.dat", ios::binary);
    bool found = false;
    while (f.read((char*)&b, sizeof(b))) {
        string field = (type == 't') ? b.title : b.author;
        if (field == key) { b.display(); found = true; }
    }
    if (!found) cout << "Not found.\n";
    f.close();
}

// Issue or return book (type: 'i' or 'r')
void issueReturn(int id, char type) {
    Book b;
    fstream f("library.dat", ios::binary | ios::in | ios::out);
    while (f.read((char*)&b, sizeof(b))) {
        if (b.id == id) {
            if (type == 'i' && b.issued)  { cout << "Already issued.\n"; f.close(); return; }
            if (type == 'r' && !b.issued) { cout << "Not issued.\n";     f.close(); return; }
            b.issued = (type == 'i');
            f.seekp(-(int)sizeof(b), ios::cur);
            f.write((char*)&b, sizeof(b));
            cout << (type == 'i' ? "Issued.\n" : "Returned.\n");
            f.close();
            return;
        }
    }
    cout << "Book not found.\n";
    f.close();
}

void listAll() {
    Book b;
    ifstream f("library.dat", ios::binary);
    while (f.read((char*)&b, sizeof(b))) b.display();
    f.close();
}

int main() {
    int choice, id;
    string key;
    Book b;

    while (true) {
        cout << "\n1.Add Book  2.Issue  3.Return  4.Search Title  5.Search Author  6.List All  7.Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                b.create();
                addBook(b);
                cout << "Book added.\n";
                break;
            case 2:
                cout << "Book ID: "; cin >> id;
                issueReturn(id, 'i');
                break;
            case 3:
                cout << "Book ID: "; cin >> id;
                issueReturn(id, 'r');
                break;
            case 4:
                cin.ignore();
                cout << "Title: "; getline(cin, key);
                search(key, 't');
                break;
            case 5:
                cin.ignore();
                cout << "Author: "; getline(cin, key);
                search(key, 'a');
                break;
            case 6:
                listAll();
                break;
            case 7:
                return 0;
        }
    }
}
