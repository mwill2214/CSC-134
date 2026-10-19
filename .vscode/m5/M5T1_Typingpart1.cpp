// CSC 134
// M5T1 - Functions
// WilliamsM
// 19 October 2026
// Goal: Practice basic functions
// void, and value returning

#include <iostream>
using namespace std;

// DECLARE FUNCTIONS HERE
//Void functions
void greet();
void speak(string message);

int main()
{
    cout << "Void Function Example" << endl;
    greet();
    speak("Hello world");

    return 0;
}
// DEFINE FUNCTIONS HERE
void greet() {
    // greet the user: no arguments in, no return out 
    cout << "What's up?" << endl;
}

void speaking (string message) {
    // talk to user:
    // arguments: string, the message to speak
    // return: none
    // (this would be useful if I wanted speak text to look a certain way, etc.)
    cout << message << endl;
}