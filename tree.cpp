#include <iostream>
#include <fstream>
#include "tree.h"
using namespace std;

Node:: Node(string name) {
	this->name = name; //initializing name with argument
    this->parent = nullptr;  //initializing parent to null pointer
    this->bookCount = 0;  //initializing book count to 0
}

string Node:: getCategory(Node* node){
    string category = node->name; //name of the node stored here
    while (node->parent->parent != nullptr) { // loops until the root is reached
        node = node->parent;  // node now points towards the parent
        category = node->name + "/" + category; //category is formed
    }

    return category; //return category
}
Node::~Node() {
    for (int i = children.size() - 1; i >= 0; --i) { //looping through the children vector
        delete children.at(i); //deleting anything stored at index i
        children.erase(i); //erasing index i
    }

    //implementing the same for the books vector
    for (int i = books.size() - 1; i >= 0; --i) {
        delete books.at(i);
        books.erase(i);
    }
}
Tree::Tree(string rootName) { //defining tree constructor
	this->root = new Node(rootName); //initializing root with a rootname of type Node
}

Tree::~Tree()
{
	delete this->root; //delete Tree's root
}

Node* Tree::getRoot(){
    return root; // return root of the tree
}

bool Tree::isLastChild(Node *ptr) {
    if(ptr!=root and ptr == ptr->parent->children[ptr->parent->children.size()-1]) //if the ptr is the last child
        return true; //return true
    return false; //return false
}

void Tree::insert(Node* node,string name){
    Node* child = new Node(name); // new node with the given name is created
    node->children.push_back(child); // the node is added to the vector
    child->parent = node; // new node points to the parent
    updateBookCount(node,1); //bookcount is updated
}

void Tree::remove(Node* node, string child_name){
    for (int i = 0; i < node->children.size(); ++i) {     // Traverse the tree to find the node to remove
        Node* child = node->children[i];
        if (child->name == child_name) { //if child is found
            remove(child, child_name);             // Recursively remove children of the found node
            updateBookCount(node, -child->bookCount); //update book count
            delete child;// Delete the node 
            node->children.erase(i);            // Erase the child from the children vector
            break;
        }
        else{remove(node->children[i],child_name);} //else check the children
    }
}


bool Tree:: isRoot(Node* node) {
    return (node == root); //checks if node is equal to root
}

Node* Tree::getNode(string path){
    if (path.empty()) // If the path is empty, return nullptr
        return nullptr;

    Node* current = root; // Start from the root node
    while (!path.empty()) {
        size_t next = path.find('/'); // if we find a slash
        string category = path.substr(0, next); // we extract the category
        bool found = false;

        // Loop through children using index-based approach
        for (int i = 0; i < current->children.size(); ++i) {
            Node* child = current->children[i]; // assigning to child a node
            if (child->name == category) { // if the child matches the category
                current = child; // Move to the child node
                found = true; // node found
                break;
            }
        }
        if (!found) 
            return nullptr;

        if (next != string::npos) // if not end of string
            path = path.substr(next + 1); // Move past the delimiter
        else
            path.clear(); // Reached the end of the path
    }
    return current; // Return the last node in the path
}


Node* Tree::createNode(string path) {
    if (path.empty()) // If the path is empty, return nullptr
        return nullptr;

    Node* current = root; // Start from the root node
    while (!path.empty()) {
        size_t next = path.find('/');
        string category = path.substr(0, next);
        bool found = false;

        // Loop through children using index-based approach
        for (int i = 0; i < current->children.size(); ++i) {
            Node* child = current->children[i];
            if (child->name == category) {
                current = child; // Move to the child node
                found = true;
                break;
            }
        }

        if (!found) { // if not found
            Node* newNode = new Node(category); //make a new node
            newNode->parent = current; // assign a parent
            current->children.push_back(newNode); // add it to the vector
            current = newNode; // Move to the newly created node

        }

        if (next != string::npos){
            path = path.substr(next + 1); // Move past the delimiter
        }
        else {
            path.clear(); 
        }
    }
    return current; 
}


Node* Tree::getChild(Node* ptr, string childname) {
    if (ptr == nullptr) { // if the ptr is nullpointer, then return null pointer
        return nullptr;
    }
    for (int i = 0; i < ptr->children.size(); ++i) { // loop through every child
        Node* child = ptr->children[i];
        if (child->name == childname) { // if child is found
            return child; // return child
        }
    }
    return nullptr;
}

void Tree:: updateBookCount(Node *ptr, int offset){
    if (ptr == nullptr) // If the given node is nullptr, return
        return;
    ptr->bookCount += offset; // Update the book count of the given node
    while (ptr->parent != nullptr) { // update bookcount until ptr is root
        ptr = ptr->parent;
        ptr->bookCount += offset; // bookcount updated
    }
}

Book* Tree::findBook(Node* node, string bookTitle) {
    if (node == nullptr) {
        return nullptr;
    }
    
    for (int i = 0; i < node->books.size(); ++i) { // looping through the book vector
        Book* book = node->books[i];
        if (book->title == bookTitle) { // if book is found
            return book; // return book
        }
    }

    for (int i = 0; i < node->children.size(); ++i) { // else loop through the children of the node
        Book* foundBook = findBook(node->children[i], bookTitle); //recurively find the book
        if (foundBook != nullptr) {
            return foundBook; // Book found in one of the children nodes
        }
    }
    return nullptr;
}

bool Tree::removeBook(Node* node, string bookTitle) {
    if (node == nullptr) {
        return false;
    }
    for (int i = 0; i < node->books.size(); ++i) { //loop through the boko vector
        if (node->books[i]->title == bookTitle) {
            delete node->books[i]; // Delete the book object from memory
            node->books.erase(i); // Erase the book from the list
            updateBookCount(node, -1); // Update the book count
            return true;
        }
    }
    for (int i = 0; i < node->children.size(); ++i) { //else loop through the children vector
        // Recursively search in each child node
        if (removeBook(node->children[i], bookTitle)) { //recursively find the book
            return true; // Book found and removed in child node
        }
    }
    return false;
}

void Tree::printAll(Node* node) {
    if (node == nullptr) {
        return; 
    }
    for (int i = 0; i < node->books.size(); ++i) { //looping through each book and formatting and printing the data
        cout << "Title:            "<<  node->books[i]->title << endl; 
        cout << "Author(s):        " << node->books[i]->author << endl;
        cout << "ISBN:             " << node->books[i]->isbn << endl;
        cout << "Year:             " << node->books[i]->publication_year << endl;
        cout<< "---------------------------------------------------------------------------------"<<endl;

    }

    for (int j = 0; j < node->children.size(); ++j) {
        printAll(node->children[j]); //recurively move to the children
    }
}

void Tree::print()  
{
    print_helper("","",root);
}

void Tree::print_helper(string padding, string pointer,Node *node) //helper method for the print method
{
    if (node != nullptr) 
    {

        cout <<padding<<pointer<<node->name<<"("<<node->bookCount<<")"<<endl;

        if(node!=root)  padding+=(isLastChild(node)) ? "   " : "│  ";

        for(int i=0; i<node->children.size(); i++)  //remove the file/folder from original path
        {
            string marker = isLastChild(node->children[i]) ? "└──" : "├──";
            print_helper(padding,marker, node->children[i]);
        }
    }
}


int Tree::exportData(Node *node, ofstream& file) {
    if (node == nullptr) { // if node empty
        return 0;
    }

    int totalBooksExported = 0;

    // Export book details for each book in the node
    for (int i = 0; i < node->books.size(); ++i) { 
        Book* book = node->books[i];
        bool hastcomma = (book->title.find(",") != std::string::npos); //if title has comma
        bool hasacomma = (book->author.find(",") != std::string::npos); // if author has comma

        if (hastcomma) {
            file << "\"" << book->title << "\","; // add double quotes
        } else {
            file << book->title << ",";
        }
        if (hasacomma) {
            file << "\"" << book->author << "\","; // add double quotes
        } else {
            file << book->author << ",";
        }
        // Then add all other relevant fields to the file
        file << book->isbn << "," 
             << book->publication_year << ","
             << node->getCategory(node) << ","
             << book->total_copies << ","
             << book->available_copies << endl;
        totalBooksExported++;
    }

    // Recursively export data for each child node
    for (int i = 0; i < node->children.size(); ++i) {
        totalBooksExported += exportData(node->children[i], file);
    }

    return totalBooksExported;
}



bool Tree::isEmpty() {
    return root == nullptr; // if root is nullpointer return true
}