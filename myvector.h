//============================================================================
// Name         : myvector.h
// Author       : Muhammad Talal Naveed
// Version      : 1.0
// Date Created : 
// Date Modified:
// Description  : Vector implementation in C++
//============================================================================
#ifndef MYVECTOR_H
#define MYVECTOR_H
#include<iostream>
#include<cstdlib>
#include<iomanip>
#include <stdexcept>
#include<sstream>

using namespace std;
template <typename T>
class MyVector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		MyVector();						//No argument constructor
		MyVector(int cap);				//One Argument Constructor
		MyVector(const MyVector& other);		//Copy Constructor
		~MyVector();					//Destructor
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element); //Add an element at the index 
		void erase(int index);			//Removes an element from the index
		T& operator[](int index);		//return reference of the element at index
		T& at(int index); 				//return reference of the element at index
		const T& front();				//Returns reference of the first element in the vector
		const T& back();				//Returns reference of the Last element in the vector
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Return true if the vector is empty, False otherwise
		void shrink_to_fit();			//Reduce vector capacity to fit its size
};
//============================================================================
class VectorEmpty:public exception // inherits exception
{
	public:
		VectorEmpty(const string& err) : errMsg(err) { } // assigns the error message to the the attribute
		virtual const char* what() const throw() {
			return errMsg.c_str(); // returns the attribute containing the error message
		}
	private:
		string errMsg; // String attribute to store the message
};
//============================================================================
template <typename T>
MyVector<T>::MyVector() 
{
    v_size = 0; // initializing number of elements to zero
    v_capacity = 0; // initializing capacity to zero
    data = nullptr; // initializing data pointer to nullptr
}
//========================================
template <typename T>
MyVector<T>::MyVector(int cap) 
{
    v_size = 0; // initializing number of elements to zero
    v_capacity = cap; // initializing capacity to the given capacity
    data = new T[cap]; // making an array of the new capacity
}
//========================================
template <typename T>
MyVector<T>::MyVector(const MyVector& other) {
    v_size = other.v_size; //copies the vector size
    v_capacity = other.v_capacity; //copies the vector capacity
    data = new T[v_capacity]; //copy vector made of the same capcity 
    for (int i = 0; i < v_size; ++i)
        data[i] = other.data[i]; //data copied to the new vector
}

//========================================
template <typename T>
MyVector<T>::~MyVector() 
{
    delete[] data; // deallocating the memory for the array
}
//========================================
template <typename T>
int MyVector<T>::size() const 
{
    return v_size; // number of elements are returned
}
//========================================
template <typename T>
int MyVector<T>::capacity() const 
{
    return v_capacity; // vector capacity is returned
}
//========================================
template <typename T>
bool MyVector<T>::empty() const 
{
    return v_size == 0; // boolean value equivalent to this expression is returned
}
//========================================
template <typename T>
void MyVector<T>::push_back(T element) 
{
    int newcap; // variable to store new capacity
    if (v_size == v_capacity) { // if the number of elements equals the spaces
        if (v_capacity == 0){ // if there are no elements
            newcap = 1; // allocate one space
        }
        else {newcap = v_capacity * 2;} // else double the size of the array
        T *newarray = new T[newcap]; // creating a new array of new capacity
        for (int i = 0; i < v_size; ++i) {  // transfering all elements from old array to the new array
            newarray[i] = data[i];
        }
        delete[] data; // deallocate memory for the array
        data = newarray; // the data pointer points to the new array
        v_capacity = newcap; // new capacity is made

    }

    data[v_size] = element; // element is added at the end
    ++v_size; // size is incremented
}
//===============================================================================
template <typename T>
void MyVector<T>::insert(int index, T element) 
{
	int newcap;
    if (index < 0 || index > v_size) { 
        throw out_of_range("Invalid Index"); // if index out of range, exception is thrown
    }
    if (v_size == v_capacity) { // if the number of elements equals the spaces
        if (v_capacity == 0){ // if there are no elements
            newcap = 1; // allocate one space
        }
        else {newcap = v_capacity * 2;} // else double the size of the array
        T *newarray = new T[newcap]; // creating a new array of new capacity
        for (int i = 0; i < v_size; ++i) { // transfering all elements from old array to the new array
            newarray[i] = data[i];
        }
        delete[] data; // deallocate memory for the array
        data = newarray; // the data pointer points to the new array
        v_capacity = newcap; // new capacity is made

    }
    for (int i = v_size-1; i >= index; i--){ // all elements are shifted one place forward to empty the index for insertion
        data[i+1] = data[i];
    }
    data[index] = element; // element inserted
    v_size++; // size incremented
}
//================================================================================
template <typename T>
void MyVector<T>::erase(int index) 
{
    if (index < 0 || index >= v_size) { // if index is out of range
        throw out_of_range("Invalid Index"); // exception is thrown
    }
    for (int i = index; i < v_size - 1; i++) { // elements are shifted backwards and the element at the index is removed
        data[i] = data[i + 1];
    }
    v_size--; // size is decremented
}
//==================================================================================
template <typename T>
T& MyVector<T>::operator[](int index) 
{
    return data[index]; // overloads the operator and returns the element at that index
}
//========================================
template <typename T>
T& MyVector<T>::at(int index) 
{
    if (index < 0 || index >= v_size) {
        throw out_of_range("Invalid Index"); // if index is out of range, exception is thrown
    }
    return data[index]; // else the element at that index is returned
}
//========================================
template <typename T>
const T& MyVector<T>::front() 
{
    if (v_size == 0) { // if vector is empty
        throw VectorEmpty("Vector is empty"); // exception is thrown
    }
    return data[0]; // else element at the first index is returned
}
//========================================
template <typename T>
const T& MyVector<T>::back() 
{
    if (v_size == 0) { // if vector is empty
        throw VectorEmpty("Vector is empty"); // exception is thrown
    }
    return data[v_size - 1]; // else element at the last index is returned
}
//======================================
template <typename T>
void MyVector<T>::shrink_to_fit() {
    T *newarray = new T[v_size]; // new array of size equal to number of elements is created
    for (int i = 0; i < v_size; i++) {  // elements from old array are copied to the new array
        newarray[i] = data[i];
    }
    delete[] data; // old array is deleted
    data = newarray; // data points to the new array
    v_capacity = v_size;  // capacity is assigned a new size
}
#endif