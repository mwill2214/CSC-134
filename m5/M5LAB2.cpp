// M5LAB2
// Program: Rectangle Area Calculator
// Author: Queen B
// Description: Gets a rectangle's length and width from the user,
//              calculates the area, and displays all three values.
//              Includes optional input validation (rejects non-numeric
//              and non-positive input).

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
using namespace std;

// Function prototypes
double getPositiveNumber(const string& prompt);
double getLength();
double getWidth();
double getArea(double length, double width);
void displayData(double length, double width, double area);

int main()
{
   // This program calculates the area of a rectangle.

   double length,    // The rectangle's length
          width,     // The rectangle's width
          area;      // The rectangle's area

   // Get the rectangle's length.
   length = getLength();

   // Get the rectangle's width.
   width = getWidth();

   // Get the rectangle's area.
   area = getArea(length, width);

   // Display the rectangle's data.
   displayData(length, width, area);

   return 0;
}

//***************************************************
// getPositiveNumber                                *
// Optional add-on: prompts until the user enters   *
// a valid number greater than zero, then returns   *
// it as a double. Used by getLength and getWidth.  *
//***************************************************
double getPositiveNumber(const string& prompt)
{
   double value = 0;

   while (value <= 0)
   {
      cout << prompt;
      cin >> value;

      if (cin.fail())
      {
         cin.clear();                                         // Clear the error flag
         cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
         cout << "Invalid input. Please enter a positive number." << endl;
         value = 0;
      }
      else if (value <= 0)
      {
         cout << "Value must be greater than zero." << endl;
      }
   }
   return value;
}

//***************************************************
// getLength                                        *
// Asks the user for the rectangle's length and     *
// returns it as a double.                          *
//***************************************************
double getLength()
{
   return getPositiveNumber("Enter the rectangle's length: ");
}

//***************************************************
// getWidth                                         *
// Asks the user for the rectangle's width and      *
// returns it as a double.                          *
//***************************************************
double getWidth()
{
   return getPositiveNumber("Enter the rectangle's width: ");
}

//***************************************************
// getArea                                          *
// Takes length and width, returns the area.        *
//***************************************************
double getArea(double length, double width)
{
   return length * width;
}

//***************************************************
// displayData                                      *
// Displays the length, width, and area with labels.*
//***************************************************
void displayData(double length, double width, double area)
{
   cout << fixed << setprecision(2);

   cout << "\nRectangle Data\n";
   cout << "------------------------\n";
   cout << left << setw(10) << "Length:" << right << setw(10) << length << endl;
   cout << left << setw(10) << "Width:"  << right << setw(10) << width  << endl;
   cout << left << setw(10) << "Area:"   << right << setw(10) << area   << endl;
}