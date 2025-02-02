#include <iostream>
#include "borrower.h"
using namespace std;

Borrower:: Borrower(string name, string id){
	this->name= name; // initializing name with argument
	this->id = id; //initializing id with argument
}

void Borrower::listBooks() {
    if (books_borrowed.empty()) { // if the vector is empty
        cout << "No books borrowed by " <<  name << " (" << id<< ")"<<endl; //return no books borrowed.
        return; //function terminates
    }

    for (int i = 0; i < books_borrowed.size(); ++i) { //loop until end of vector
        cout << i + 1 << ":  " << books_borrowed[i]->title << endl; //give the desired output at each index
    }
}