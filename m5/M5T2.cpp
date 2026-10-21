/*
// CSC 134
M5T2 - More Practice with Functions
Name: WilliamsM
Partner: AI navigator - Claude
Date: October 21, 2026

With a partner, write a C++ program that prints the numbers 1 to 10
and their squares, using one value-returning function and one void function
*/

#include <iostream>
using namespace std;

// Prototypes: Declare functions here, define them under main.
int square(int num);                               // Does the math
void printAnswerLine(int number, int squared);     // Prints one line of the table

int main() {
    // Print the numbers 1 to 10 and their squares.
    for (int i = 1; i <= 10; i++) {
        int result = square(i);        // store the value square() returns
        printAnswerLine(i, result);    // pass number first, then its square
    }
    return 0;
}

// Define functions here.
int square(int num) {
    // Does the math -- input: an integer, output: that integer squared.
    return num * num;
}

void printAnswerLine(int number, int squared) {
    // Print one line, for example: 3 squared is 9
    cout << number << " squared is " << squared << endl;
}