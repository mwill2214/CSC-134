// M4HW1_WilliamsM
// Name: Williams, M
// Course: (enter course info here)
// Date: (enter date here)
//
// Gold version: asks the user for a number from 1 to 12, keeps asking
// until the input is valid, then prints that number's times table.

#include <iostream>
using namespace std;

int main() {

    // declare variables
    int firstNum, secondNum, answer;

    // Gold: input validation loop - does not exit until 1 to 12 is entered
    cout << "Enter a number from 1 to 12? ";
    cin >> firstNum;

    while (firstNum < 1 || firstNum > 12) {
        cout << "Invalid number. Please enter a number from 1 to 12? ";
        cin >> firstNum;
    }

    // Bronze/Silver: print the times table (1 through 12) using a while loop
    secondNum = 1;

    while (secondNum <= 12) {
        answer = firstNum * secondNum;
        cout << firstNum << " times " << secondNum << " is " << answer << "." << endl;
        secondNum++;
    }

    return 0;
}