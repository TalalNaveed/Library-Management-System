#include <iostream>
#include "book.h"

Book::Book(std::string title, std::string author, std::string isbn, int publication_year, int total_copies, int available_copies) {
    this->title = title; //initializing variable with arguments from constructor
    this->author = author; //initializing variable with arguments from constructor
    this->isbn = isbn; //initializing variable with arguments from constructor
    this->publication_year = publication_year; //initializing variable with arguments from constructor
    this->total_copies = total_copies; //initializing variable with arguments from constructor
    this->available_copies = available_copies; //initializing variable with arguments from constructor
}

void Book::display() {
    std::cout << "Title:            "<< title << std::endl;   //outputing each variable in the desired format
    std::cout << "Author(s):        " << author << std::endl; //outputing each variable in the desired format
    std::cout << "ISBN:             " << isbn << std::endl;   //outputing each variable in the desired format
    std::cout << "Year:             " << publication_year << std::endl; //outputing each variable in the desired format
    std::cout << "Total Copies:     " << total_copies << std::endl;  //outputing each variable in the desired format
    std::cout << "Available Copies: " << available_copies << std::endl; //outputing each variable in the desired format
}
