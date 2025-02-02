#include <iostream>
#include "lcms.h"
#include <fstream>
#include <limits>
#include <sstream>
using namespace std;

LCMS::LCMS(string name) {
    libTree = new Tree(name); // initialize the library tree
}

LCMS::~LCMS() {
    delete libTree; //delete the tree
    for (int i = 0; i < borrowers.size(); ++i) { //loop through the borrower vector
        delete borrowers[i]; //delete each memory location of the borrowers
    }
}

int LCMS::import(string path) {
    // Open the file for reading
    ifstream file(path);
    
    // Check if the file is successfully opened
    if (!file.is_open()) {
        cout << 0 << " records have been imported"<<endl;
        return 0; // Return early if file cannot be opened
    }
    
    // Initialize count of imported records
    int count = 0;
    string line;
    
    // Skip the header line
    getline(file, line); 

    // Read each line of the file
    while (getline(file, line)) {
        // Use stringstream to parse each line
        stringstream ss(line);
        
        // Initialize variables to store book details
        string title, author, isbn, publicationYear, category;
        int totalCopies = 0, availableCopies = 0; // Initialize to zero
        
        // Set the delimiter and flag to handle fields enclosed in quotes
        char delimiter = ',';
        bool inQuotes = false;
        string field;

        // Loop through each character in the line
        for (char c : line) {
            // Toggle the inQuotes flag when encountering a double quote
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == delimiter && !inQuotes) {
                // Process the field and reset it
                field = trim(field);

                // Assign the field value to the appropriate variable
                if (title.empty()) {
                    title = field;
                } else if (author.empty()) {
                    author = field;
                } else if (isbn.empty()) {
                    isbn = field;
                } else if (publicationYear.empty()) {
                    publicationYear = field;
                } else if (category.empty()) {
                    category = field;
                } else if (totalCopies == 0) {
                    totalCopies = stoi(field);
                } else if (availableCopies == 0) {
                    availableCopies = stoi(field);
                }
                
                // Clear the field for the next value
                field.clear(); 
            } else {
                // Append characters to the current field
                field += c;
            }
        }

        // Process the last field
        field = trim(field);
        if (title.empty()) {
            title = field;
        } else if (author.empty()) {
            author = field;
        } else if (isbn.empty()) {
            isbn = field;
        } else if (publicationYear.empty()) {
            publicationYear = field;
        } else if (category.empty()) {
            category = field;
        } else if (totalCopies == 0) {
            totalCopies = stoi(field);
        } else if (availableCopies == 0) {
            availableCopies = stoi(field);
        }

        // Convert publication year to integer
        int pubYear = !publicationYear.empty() ? stoi(publicationYear) : 0;        

        // Create or retrieve the category node in the library tree
        Node* categoryNode = libTree->createNode(category);
        if (categoryNode == nullptr) {
            continue; // Skip to the next iteration if category node is not created
        }

        // Create a new book object
        Book* book = new Book(title, author, isbn, pubYear, totalCopies, availableCopies);
        
        // Increment the count of imported records
        count++;

        // Add the book to the category node's book vector
        categoryNode->books.push_back(book);

        // Update the book count in the category node
        libTree->updateBookCount(categoryNode,1);
    }

    // Close the file
    file.close();
    
    // Print the count of imported records
    cout << count << " records have been imported"<< endl;
    
    // Return the count of imported records
    return count;
}

string LCMS::trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");     // Find the index of the first non-whitespace character
    size_t end = str.find_last_not_of(" \t\n\r");     // Find the index of the last non-whitespace character
    // Return an empty string if start or end is not found
    // Otherwise, return the substring between start and end
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}


void LCMS::exportData(string path) {
    ofstream file(path); //write mode on file of the given path
    if (!file.is_open()) { //if file is not open
        cout << "Error: Unable to open file for export" << endl;
        return;
    }
    file << "Title,Author,ISBN,Publication Year,Category,Total Copies,Available Copies" << endl; //add relevant headers
    int totalBooksExported = libTree->exportData(libTree->getRoot(), file); //use export data function with root as an argument to traverse and add data to the file
    file.close();

    cout << totalBooksExported << " records have been exported." << endl;
}

void LCMS::findAll(string category) {
    Node* categoryNode = libTree->getNode(category); //using the getNode function, return the node
    if (categoryNode == nullptr) { // if node is null
        cout << "Category not found." << endl;
        return; //return
    }
    libTree->printAll(categoryNode); //else use the the printall function to output all relevant data
}

void LCMS:: findBook(string bookTitle) {
    if (libTree->isEmpty()) { // if the tree is empty
        cout << "Library is empty." << endl; //return tree is empty
        return;
    }

    Book* foundBook = libTree->findBook(libTree->getRoot(), bookTitle); // else get the bookS
    if (foundBook) { // if found
        //output all the relevant details
        cout<< "Book found in Library:"<<endl;
        cout<< "---------------------------------------------------------------------------------------------"<<endl;
        cout << "Title: " << foundBook->title << endl;
        cout << "Author: " << foundBook->author << endl;
        cout << "ISBN: " << foundBook->isbn << endl;
        cout << "Publication Year: " << foundBook->publication_year << endl;
        cout << "Total Copies: " << foundBook->total_copies << endl;
        cout << "Available Copies: " << foundBook->available_copies << endl;
        cout<< "---------------------------------------------------------------------------------------------"<<endl;

    } else {
        cout << "Book not found in Library" << endl; //if not found, output this.
    }

} 

void LCMS::addBook() {
    string title, author, isbn, category, pub, tc, ac;   
    //getting input for each field of the book 
    cout << "Enter title: ";
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    cout << "Enter ISBN: ";
    getline(cin, isbn);
    cout << "Enter publication year: ";
    getline(cin, pub);
    cout << "Enter total copies: ";
    getline(cin, tc);
    cout << "Enter available copies: ";
    getline(cin, ac);
    cout << "Enter category: ";
    getline(cin, category);
    //validation checks to see whether fields are empty
    if (title.empty() || author.empty() || isbn.empty()){
        cout<<"Enter Valid Data"<<endl;
        return;
    }
    //validation checks to see whether the fields are integer or not
    for (char p: pub){ //loop through every digit
        if (!isdigit(p)) { //if not digit, return
            cout << "Enter a valid Year!" << endl;
            return; 
        }
    }
    for (char t: tc){
        if (!isdigit(t)) { //loop through every digit
            cout << "Enter valid total Copies!" << endl;//if not digit, return
            return;
        }
    }
    for (char a: ac){
        if (!isdigit(a)) { //loop through every digit
            cout << "Enter valid available copies!" << endl;//if not digit, return
            return;
        }
    }
    //variables converted to integer
    int publicationYear = stoi(pub);
    int totalCopies = stoi(tc);
    int availableCopies = stoi(ac);
    //if available copies are greater than total, then book can't exist
    if (availableCopies>totalCopies) {
        cout<<"Total Copies cannot be less than available copies. Book cannot be added!"<<endl;
        return;
    }
    Book* newbook = new Book(title, author, isbn, publicationYear, totalCopies, availableCopies); //create a new book of type book
    Node* curr = libTree->getNode(category); //node is get using the category

    if (curr == nullptr && !category.empty()) {//if node not present, then the node is created
        curr = libTree->createNode(category); //create the node
    }
    else if (curr == nullptr && category.empty()) //if category not valid, return
    {
        cout<< "Enter a valid Category"<<endl;
        delete newbook;
        return;
    }
    curr->books.push_back(newbook);
    libTree->updateBookCount(curr,1);
    cout << "Book added successfully." << endl;
}	

void LCMS::editBook(string bookTitle){
    int field; // Declare an integer variable to store the user's choice of field to edit
    Node* curr = libTree->getRoot(); // Get the root node of the library tree
    Book* found = libTree->findBook(curr, bookTitle); // Find the book with the specified title
    if (found == nullptr){ // If the book is not found
        cout<< "Book not found"<<endl; // Display a message indicating that the book was not found
        return; // Return from the function
    }
    bool flag = true; // Declare and initialize a boolean flag variable to control the loop
    while (flag){ // Start a loop that continues until flag is false
        cout<< "1:          Title:"<<endl; // Display a menu option to edit the title
        cout<< "2:          Author:"<<endl; // Display a menu option to edit the author
        cout<< "3:          ISBN:"<<endl; // Display a menu option to edit the ISBN
        cout<< "4:          Publication_Year:"<<endl; // Display a menu option to edit the publication year
        cout<< "5:          Total Copies:"<<endl; // Display a menu option to edit the total copies
        cout<< "6:          Available Copies:"<<endl; // Display a menu option to edit the available copies
        cout<< "7:          exit:"<<endl; // Display a menu option to exit
        cout<< "choose the field you want to edit: "; // Prompt the user to choose a field to edit
        cin>> field; // Read the user's choice into the field variable

        cin.ignore(); // Add this line to clear the newline character from the buffer

        if (field == 1) { // If the user chose to edit the title
            cout << "Enter Title: "; // Prompt the user to enter the new title
            getline(cin, found->title); // Read the new title from the input
        } else if (field == 2) { // If the user chose to edit the author
            cout << "Enter Author(s): "; // Prompt the user to enter the new author
            getline(cin, found->author); // Read the new author from the input
        } else if (field == 3) { // If the user chose to edit the ISBN
            cout << "Enter ISBN: "; // Prompt the user to enter the new ISBN
            getline(cin, found->isbn); // Read the new ISBN from the input
        } else if (field == 4) { // If the user chose to edit the publication year
            int pubYear; // Variable to store the publication year
            cout << "Enter publication year: "; // Prompt the user to enter the new publication year
            while (!(cin >> pubYear)) { // Check if the input is not an integer
                cout << "Invalid input. Please enter an integer for the publication year: "; // Display an error message
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            }
            found->publication_year = pubYear; // Assign the input to the publication year
        } else if (field == 5) { // If the user chose to edit the total copies
            int totalCopies; // Variable to store the total copies
            cout << "Enter total copies: "; // Prompt the user to enter the new total copies
            while (!(cin >> totalCopies)) { // Check if the input is not an integer
                cout << "Invalid input. Please enter an integer for the total copies: "; // Display an error message
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            }
            found->total_copies = totalCopies; // Assign the input to the total copies
        } else if (field == 6) { // If the user chose to edit the available copies
            int availableCopies; // Variable to store the available copies
            cout << "Enter available copies: "; // Prompt the user to enter the new available copies
            while (!(cin >> availableCopies)) { // Check if the input is not an integer
                cout << "Invalid input. Please enter an integer for the available copies: "; // Display an error message
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            }
            found->available_copies = availableCopies; // Assign the input to the available copies
        } else if (field == 7) { // If the user chose to exit
            cout << "Exiting" << endl; // Display a message indicating that the program is exiting
            return; // Return from the function
        } else { // If the user entered an invalid choice
            cout << "Invalid choice. Please enter a number between 1 and 7." << endl; // Display an error message
        }
    }
}

void LCMS::borrowBook(string bookTitle){
    bool exists = false; // Flag to check if the book exists in the library
    bool bexists = false; // Flag to check if the borrower exists in the system
    Book* borrowed = libTree->findBook(libTree->getRoot(),bookTitle); // Find the book in the library
    if (borrowed == nullptr){ // If the book is not found
        cout<<"Book "<<bookTitle <<" does not exist"<<endl; // Print a message and return
        return;
    }
    string bname,bid; // Strings to store borrower's name and ID
    cout<<"Enter Borrower's name : "; // Prompt for borrower's name
    getline(cin,bname); // Get borrower's name from user
    cout<<"Enter Borrower's id   : "; // Prompt for borrower's ID
    getline(cin,bid); // Get borrower's ID from user
    Borrower* customer = nullptr; // Pointer to store the borrower information
    if (borrowed->available_copies > 0) { // If there are available copies of the book
        for (int i = 0; i < borrowers.size(); ++i) { // Iterate through existing borrowers
            if (borrowers[i]->name == bname && borrowers[i]->id == bid) { // If borrower already exists
                customer = borrowers[i]; // Set customer pointer to existing borrower
            }
        }
        if (customer == nullptr){ // If borrower does not exist
            customer = new Borrower(bname, bid); // Create a new borrower
            borrowers.push_back(customer); // Add new borrower to the system
        }
        for (int i=0; i < customer->books_borrowed.size(); ++i){ // Check if the borrower already borrowed this book
            if (customer->books_borrowed[i]->title == borrowed->title){
                exists = true; // Set flag to true if book is already borrowed
                break;
            }
        }
        if (exists == false){ // If the book is not already borrowed by the borrower
            customer->books_borrowed.push_back(borrowed); // Add the book to borrower's list of borrowed books
        }
        for (int i=0; i < borrowed->allBorrowers.size(); ++i){ // Check if borrower already borrowed this book
            if (borrowed->allBorrowers[i]->name == bname && borrowed->allBorrowers[i]->id == bid){
                bexists = true; // Set flag to true if borrower has previously borrowed this book
                break;
            }
        }
        if (bexists == false){ // If the borrower has not previously borrowed this book
            borrowed->allBorrowers.push_back(customer); // Add the borrower to the book's list of all borrowers
        }
        borrowed->currentBorrowers.push_back(customer); // Add the borrower to the book's list of current borrowers
        borrowed->available_copies--; // Decrease the available copies of the book
        cout<<"Book "<<bookTitle<<" has been issued to "<<bname<<endl; // Print a message confirming the issuance of the book
    }
    else{
        cout<<"No copies available for "<< bookTitle<<endl; // Print a message if there are no available copies of the book
    }
}


void LCMS::returnBook(string bookTitle){
    Book* exist = libTree->findBook(libTree->getRoot(),bookTitle); // Find the book in the library tree
    if (exist==nullptr) { // If the book does not exist
        cout<<"Book Does Not Exist"<<endl; // Display a message indicating that the book does not exist
        return; // Return from the function
    }
    string name,id; // Declare variables to store borrower's name and ID
    cout<<"Enter Borrower's name : "; // Prompt the user to enter borrower's name
    getline(cin,name); // Read the borrower's name from the input
    cout<<"Enter Borrower's id   : "; // Prompt the user to enter borrower's ID
    getline(cin,id); // Read the borrower's ID from the input
    Borrower* borrower = nullptr; // Initialize a pointer to Borrower object
    for (int i = 0; i < borrowers.size(); ++i) { // Loop through existing borrowers
        if (borrowers[i]->name == name && borrowers[i]->id == id) { // Check if the borrower exists
            borrower = borrowers[i]; // Assign the existing borrower to the borrower pointer
        }
    }
    if (borrower == nullptr) { // If the borrower is not found
        cout << "Borrower not found." << endl; // Display a message indicating that the borrower is not found
        return; // Return from the function
    }
    Book* borrowedBook = nullptr; // Initialize a pointer to Book object
    for (int i = 0; i < borrower->books_borrowed.size(); ++i) { // Loop through books borrowed by the borrower
        if (borrower->books_borrowed[i]->title == bookTitle) { // Check if the book is borrowed by the borrower
            borrowedBook = borrower->books_borrowed[i]; // Assign the borrowed book to the borrowedBook pointer
            break; // Exit the loop
        }
    }
    if (borrowedBook == nullptr) { // If the borrowed book is not found
        cout << "Book not found in borrower's list." << endl; // Display a message indicating that the book is not found in borrower's list
        return; // Return from the function
    }

    for (int i = 0; i < borrowedBook->currentBorrowers.size(); ++i) { // Loop through current borrowers of the book
        if (borrowedBook->currentBorrowers[i]->name == name && borrowedBook->currentBorrowers[i]->id == id) { // Check if the borrower matches
            borrowedBook->currentBorrowers.erase(i); // Remove the borrower from the list of current borrowers
            break; // Exit the loop
        }
    }
    borrowedBook->available_copies++; // Increase the available copies of the book by 1

    cout << "Book has been successfully returned"<<endl; // Display a message indicating successful book return
}


void LCMS::listCurrentBorrowers(string bookTitle){
    Book* curr = libTree->findBook(libTree->getRoot(),bookTitle); // Find the book in the library tree
    if (curr == nullptr) { // If the book does not exist
        cout<< "Book does not exist"<<endl; // Display a message indicating that the book does not exist
        return; // Return from the function
    }
    if (curr->currentBorrowers.size()==0){ // If there are no current borrowers for the book
        cout<<"No current borrowers for this book"<<endl; // Display a message indicating no current borrowers for the book
        return; // Return from the function
    }
    for (int i=0;i<curr->currentBorrowers.size();++i){ // Loop through the current borrowers of the book
        cout<<i+1<<" "<< curr->currentBorrowers[i]->name<<" (" << curr->currentBorrowers[i]->id<< ") "<< ":"<<endl; // Display the current borrower's name and ID
    }
}


void LCMS::listAllBorrowers(string bookTitle){
    Book* curr = libTree->findBook(libTree->getRoot(),bookTitle); // Find the book in the library tree
    if (curr == nullptr) { // If the book does not exist
        cout<< "Book does not exist"<<endl; // Display a message indicating that the book does not exist
        return; // Return from the function
    }
    for (int i=0;i<curr->allBorrowers.size();++i){ // Loop through all borrowers of the book
        cout<<i+1<<" "<< curr->allBorrowers[i]->name<<" (" << curr->allBorrowers[i]->id<< ") "<< ":"<<endl; // Display the borrower's name and ID
    }
}

void LCMS::listBooks(string borrower_name_id){
    stringstream extract(borrower_name_id); // Create a stringstream to extract the borrower's name and ID from the input string
    string name, id; // Declare variables to store the borrower's name and ID
    getline(extract, name, ','); // Extract the name from the stringstream, delimited by a comma
    getline(extract, id); // Extract the ID from the stringstream
    
    Borrower* borrower = nullptr; // Initialize a pointer to Borrower object to nullptr
    for (int i = 0; i < borrowers.size(); ++i) { // Iterate through the borrowers vector
        if (borrowers[i]->name == name && borrowers[i]->id == id) { // Check if the borrower's name and ID match
            borrower = borrowers[i]; // If found, assign the borrower object to the pointer
        }
    }
    if (borrower == nullptr) { // If borrower is not found
        cout << "Borrower not found." << endl; // Display a message indicating that the borrower is not found
        return; // Return from the function
    }
    cout << "Books borrowed by " << borrower->name << " (" << borrower->id<< ") "<< ":"<<endl; // Display borrower's name and ID
    borrower->listBooks(); // Call the listBooks() function of the borrower object to list the books borrowed by the borrower
}


void LCMS::removeBook(string bookTitle) { // Function to remove a book from the library management system
    if (libTree->isEmpty()) { // Check if the library tree is empty
        cout << "Library is empty." << endl; // Display a message indicating that the library is empty
        return; // Return from the function
    }

    Book* foundBook = libTree->findBook(libTree->getRoot(), bookTitle); // Find the book in the library tree
    if (foundBook == nullptr) { // If the book is not found
        cout<< "Book not found"<<endl; // Display a message indicating that the book is not found
        return; // Return from the function
    }

    for (int i = 0; i < borrowers.size(); ++i) { // Loop through the borrowers vector
        for (int j = 0; j < borrowers[i]->books_borrowed.size(); ++j) { // Loop through the books borrowed by each borrower
            if (borrowers[i]->books_borrowed[j] == foundBook) { // Check if the found book matches any book borrowed by the borrower
                borrowers[i]->books_borrowed.erase(j); // Remove the book from the borrower's list of borrowed books
                break;  // No need to continue searching once book is found and removed
            }
        }
    }
    if (foundBook) { // If the book is found
        string option; // Declare a string variable to store the user's choice
        cout<< "Are you sure you want to delete the book "<<bookTitle<<"(yes/no): "; // Prompt the user to confirm book deletion
        cin>> option; // Read the user's input
        cin.ignore(); // Ignore any remaining characters in the input buffer
        if (option == "yes" || option == "Yes") { // If the user confirms deletion
            bool removed = libTree->removeBook(libTree->getRoot(), bookTitle); // Remove the book from the library tree
            cout<< "deleting Book "<< bookTitle<< endl; // Display a message indicating book deletion process
            if (removed) { // If the book is successfully removed
                    cout << "Book " << bookTitle << " has been successfully removed"<< endl; // Display a success message
            } 
            else { // If the book removal fails
                cout << "Failed to remove book " << bookTitle << " from the library." << endl; // Display a failure message
            }
            
        }        
        else if(option =="no" || option== "No") { // If the user cancels deletion
            cout<<"Book not deleted"; // Display a message indicating that the book deletion is canceled
            return; // Return from the function
        }
    } 
    else { // If the book is not found in the library tree
        cout << "Book " << bookTitle << " not found in the library." << endl; // Display a message indicating that the book is not found
    }
}



void LCMS::addCategory(string category) { // Function to add a new category to the library management system
    Node* curr = libTree->getNode(category); // Get the node corresponding to the provided category
    if (curr != nullptr) { // If the category already exists
        cout<<"Category already exists"<<endl; // Display a message indicating that the category already exists
        return; // Return from the function
    }
    if (curr == nullptr && category.empty()) { // If the category is empty
        cout<<"Enter a valid Category"<<endl; // Display a message indicating that a valid category should be entered
        return; // Return from the function
    }
    curr = libTree->createNode(category); // Create a new node for the category
    if (curr != nullptr) { // If the node creation is successful
        size_t pos = category.find_last_of('/'); // Find the position of the last occurrence of '/' character
        string categoryName;
        if (pos != string::npos) { // If the '/' character is found
            categoryName = category.substr(pos + 1); // Extract the category name after the last '/'
        } else { // If the '/' character is not found
            categoryName = category; // Set the category name to the entire category string
        }
        cout << "Category " << categoryName <<" has been successfully created" <<endl; // Display a message indicating successful creation of the category
    }
}


void LCMS::findCategory(string category) { // Function to find a category in the library management system
    Node* foundcat = libTree->getNode(category); // Get the node corresponding to the provided category

    if (foundcat != nullptr) { // If the category node is found
        size_t pos = category.find_last_of('/'); // Find the position of the last occurrence of '/' character
        string categoryName;
        if (pos != string::npos) { // If the '/' character is found
            categoryName = category.substr(pos + 1); // Extract the category name after the last '/'
        } else { // If the '/' character is not found
            categoryName = category; // Set the category name to the entire category string
        }

        cout << "Category " << categoryName <<" was found in the catalog" <<endl; // Display a message indicating that the category was found
    } 
    else { // If the category node is not found
        cout << "No such Category/Sub-Category found in the Catalog" << endl; // Display a message indicating that the category was not found
    }
}


void LCMS::removeCategory(string category){ // Function to remove a category from the library management system
    Node*curr = libTree->getNode(category); // Get the node corresponding to the provided category
    if (curr== nullptr) { // If the category node does not exist
        cout<<"Category does not exists"<<endl; // Display a message indicating that the category does not exist
        return;
    }
    removeBooksFromBorrowers(curr); // Remove books associated with the category from borrowers
    libTree->remove(libTree->getRoot(),curr->name); // Remove the category node from the library tree
    curr = libTree->getNode(category); // Attempt to get the node again after removal
    if (curr == nullptr) { // If the category node does not exist after removal
        size_t pos = category.find_last_of('/'); // Find the position of the last occurrence of '/' character
        string categoryName;
        if (pos != string::npos) { // If the '/' character is found
            categoryName = category.substr(pos + 1); // Extract the category name after the last '/'
        } else { // If the '/' character is not found
            categoryName = category; // Set the category name to the entire category string
        }
        cout<<categoryName<< " has been successfully deleted"<<endl; // Display a message indicating that the category was successfully deleted
    }
    else { // If the category node still exists after removal
        cout<<"Category not deleted"; // Display a message indicating that the category was not deleted
    }
}


void LCMS::removeBooksFromBorrowers(Node* node){ // Function to remove books associated with a node from borrowers
    if (node == nullptr) { // If the node is null, return early
        return;
    }

    for (int bookIndex = 0; bookIndex < node->books.size(); ++bookIndex) { // Iterate through the books of the node
        Book* book = node->books[bookIndex]; // Get the current book
        // Remove the book from borrowers' books_borrowed vector
        for (int borrowerIndex = 0; borrowerIndex < borrowers.size(); ++borrowerIndex) { // Iterate through borrowers
            Borrower* borrower = borrowers[borrowerIndex]; // Get the current borrower
            int borrowedCount = borrower->books_borrowed.size(); // Get the size of the books borrowed by the borrower
            for (int i = 0; i < borrowedCount; ++i) { // Iterate through the borrowed books of the borrower
                if (borrower->books_borrowed[i] == book) { // If the borrower has borrowed the current book
                    borrower->books_borrowed.erase(i); // Remove the book from the borrower's list of borrowed books
                    break; // Book found, no need to continue searching
                }
            }
        }
    }

    for (int childIndex = 0; childIndex < node->children.size(); ++childIndex) { // Iterate through the children of the node
        Node* child = node->children[childIndex]; // Get the current child node
        removeBooksFromBorrowers(child); // Recursively remove books from borrowers for the child node
    }
}
