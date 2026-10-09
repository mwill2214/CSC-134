// CSC 134
// M4T1 - Basic Loops
// WilliamsM
// 10/5/26

#include <iostream>
using namespace std;

int main() {
    // Part 1: say "Hello" five times (Program 5-3)
    int count = 1;
    while (count <= 5) {
        cout << "Hello #" << count << endl;
        count++; // increment AFTER showing the number
    }
    cout << "That's all!" << endl;

    // Part 2: table of numbers and their squares (Program 5-6)
    const int MIN_NUM = 1;
    const int MAX_NUM = 10;

    cout << endl << "Num\tNum Squared" << endl;
    cout << "--------------------" << endl;
    int num = MIN_NUM; // counter
    while (num <= MAX_NUM) {
        cout << num << "\t" << num * num << endl;
        num++; // increment the counter
    }

    return 0; // done
}