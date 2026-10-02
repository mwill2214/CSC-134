// CSC 134
// M3Lab2 - Letter Grades
// WilliamsM
// 30 September 2026
// Convert number grades to letter grades

#include <iostream>
using namespace std;

int main() {
    // Step 1: Print welcome message and user prompt
    cout << "Welcome to the number grade to letter grade conversion program." <<
    endl << endl;
    cout << "Enter a number grade (0-100): ";

    // Step 2: Declare variables to hold inputs and results
    int num_grade;
    char letter_grade = '-'; // only one letter long; uses single quotes like 'A',
                             // not "A"
                             // - is only printed if no grade is found

    // Step 3: Get the numerical grade input from the user
    cin >> num_grade;
    //cout << "You entered: " << num_grade << endl;

    // Step 4: Calculate and find the letter grade using conditional statements
    // We check ranges sequentially from highest to lowest score
    if (num_grade >= 90) {
        letter_grade = 'A'; // Range: 90 - 100
    }
    else if (num_grade >= 80) {
        letter_grade = 'B'; // Range: 80 - 89
    }
    else if (num_grade >= 70) {
        letter_grade = 'C'; // Range: 70 - 79
    }
    else if (num_grade >= 60) {
        letter_grade = 'D'; // Range: 60 - 69
    }
    else if (num_grade >= 0) {
        letter_grade = 'F'; // Range: 0 - 59
    }
    
    else {
        // Optional handling for invalid negative inputs
        cout << "Invalid grade entered." << endl;
    }

    // Step 5: Output the final results to the console
    cout << "Number Grade: " << num_grade << endl;
    cout << "Letter Grade: " << letter_grade << endl;

    return 0;
}