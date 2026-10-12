// CSC 134
// M4LAB1 - Two-D Loops
// WilliamsM
// 12 October 2026
// Draw boxes with text


#include <iostream>
using namespace std;

int main() {

    string token = " ✅";  // use any character you want
    // const int WIDTH = 5;
    // const int HEIGHT = 5;
    int width, height;
    // Get width and height
    cout << "Enter rectangle width: ";
    cin >> width;
    cout << "Enter rectangle height: ";
    cin >> height;


    cout << "Part 1 " << endl;
    // First part: Printing one row
    for (int i = 0; i < width; i++) {
        cout << token << " "; // adding a space
    }
    cout << endl; // end the row

    cout << "Part 2 " << endl;
    // Next part: printing one column
    for (int j =0; j < height; j++) {
        cout << token << endl;
    }

    cout << "Here is your rectangle " << width << "by " << height << endl;
    // Full rectangle: print height number of rows.
    // each row is width long.
    for (int j = 0; j < height; j++) {
        // draw one full row
        for (int i = 0; i < width; i++) {
            cout << token << " ";
        } // (repeat width times)
        cout << endl;  // end of row

    } // (repeat height times)

}