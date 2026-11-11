/*
CSC 134
M6T1 - Basic Arrays
Myra Williams
11/11/26

Counts cars passing a construction site, Monday through Friday.
Part 1: loop, no arrays.
Part 2: loop plus arrays, with a table and an ASCII bar chart.
*/

#include <iostream>
#include <string>
using namespace std;

// Function Declarations
void part1();
void part2();

// main
int main() {
    part1();
    cout << endl;
    part2();

    return 0;
}

// function definitions
void part1() {
    // Part 1 - no arrays
    // Count 5 days of cars, get total and average
    cout << "=== Part 1: Loop, no arrays ===" << endl;
    cout << "Enter the cars counted each day." << endl;
    cout << "Day 0 = Monday, Day 4 = Friday" << endl;

    const int SIZE = 5;
    int count = 0;
    int cars_today;      // current value, to add
    int cars_total = 0;  // running total
    double cars_avg = 0; // average per day

    while (count < SIZE) {
        cout << "Day " << count << ": ";
        cin >> cars_today;
        cars_total += cars_today;
        count++; // Move to next day
    }

    cars_avg = (double) cars_total / SIZE;
    cout << "Total = " << cars_total << endl;
    cout << "Average = " << cars_avg << endl;
}

void part2() {
    // Part 2 - uses two arrays:
    // Names of the days
    // Cars counted on each day
    cout << "=== Part 2: Loop with arrays ===" << endl;

    const int SIZE = 5;
    string days[SIZE] = {"Mon", "Tue", "Wed", "Thu", "Fri"}; // initialized
    int cars[SIZE];                                          // not initialized
    int cars_total = 0;
    double cars_avg = 0.0;

    // get the data
    for (int i = 0; i < SIZE; i++) {
        cout << "Cars on " << days[i] << ": ";
        cin >> cars[i];
        cars_total += cars[i];
    }
    cars_avg = (double) cars_total / SIZE;

    // print the full week in table format
    cout << endl;
    cout << "Day\tCars" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << days[i] << "\t" << cars[i] << endl;
    }
    cout << "Total = " << cars_total << endl;
    cout << "Average = " << cars_avg << endl;

    // ASCII bar chart: one * for every 5 cars (rounded up)
    cout << endl;
    cout << "Cars per day (each * = 5 cars)" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << days[i] << " | ";
        int stars = (cars[i] + 4) / 5;
        for (int j = 0; j < stars; j++) {
            cout << "*";
        }
        cout << " " << cars[i] << endl;
    }
}
