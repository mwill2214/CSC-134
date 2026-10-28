// =====================================================================
// CSC 134 - M5HW1
// Menu-based program containing Questions 1-5 as functions (Question 6).
// Includes input validation for every question.
//
// HOW THE PROGRAM IS ORGANIZED:
//   1. Header files and helper functions (safe number input)
//   2. question1() - Average rainfall for three months
//   3. question2() - Volume of a block
//   4. question3() - Roman numeral converter
//   5. question4() - Geometry calculator (circle, rectangle, triangle)
//   6. question5() - Distance traveled table
//   7. main()      - Menu loop that calls the functions above (Question 6)
// =====================================================================
// ---------- Step 1: Include the libraries we need ----------
#include <iostream>   // cin and cout for keyboard input / screen output
#include <iomanip>    // setw() and setprecision() for formatting output
#include <string>     // string variables (month names, Roman numerals)
#include <limits>     // numeric_limits, used to throw away bad input

using namespace std;  // lets us write cout instead of std::cout

// =====================================================================
// HELPER FUNCTIONS
// If the user types letters when we expect a number, cin goes into a
// "failed" state and the program would loop forever. These helpers
// detect that, clean up the stream, and tell us whether the input worked.
// =====================================================================

// Reads a double. Returns true if a number was entered, false if not.
bool readDouble(double &value)
{
    if (cin >> value)       // try to read a number; success means it worked
        return true;

    cin.clear();            // reset cin so it can be used again
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard the bad line
    return false;           // tell the caller the input was not a number
}

// Reads an int. Returns true if a whole number was entered, false if not.
bool readInt(int &value)
{
    if (cin >> value)       // try to read a whole number
        return true;

    cin.clear();            // reset cin after the failure
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard the bad line
    return false;
}

// =====================================================================
// QUESTION 1: Average Rainfall
// Asks for three month names and the rainfall for each, then displays
// the average rounded to 2 decimal places.
// =====================================================================
void question1()
{
    // Step 1: Create storage. Arrays hold three months and three amounts.
    string month[3];        // month names, such as May, June, July
    double rain[3];         // inches of rain for each month

    // Step 2: Loop three times, once for each month.
    for (int i = 0; i < 3; i++)
    {
        // Step 3: Read the month name.
        cout << "Enter month: ";
        cin >> month[i];

        // Step 4: Read the rainfall and keep asking until it is valid.
        bool valid = false;
        while (!valid)
        {
            cout << "Enter rainfall for " << month[i] << ": ";

            if (!readDouble(rain[i]))        // user typed letters/symbols
                cout << "Please enter a number.\n";
            else if (rain[i] < 0)            // rain cannot be negative
                cout << "Rainfall cannot be less than zero.\n";
            else
                valid = true;                // good value, leave the loop
        }
    }

    // Step 5: Add the three amounts and divide by 3.0.
    // Using 3.0 (not 3) makes sure we do decimal division.
    double average = (rain[0] + rain[1] + rain[2]) / 3.0;

    // Step 6: Print the result with exactly 2 decimal places.
    // fixed + setprecision(2) means "show 2 digits after the decimal point".
    cout << fixed << setprecision(2);
    cout << "The average rainfall for " << month[0] << ", " << month[1]
         << ", and " << month[2] << " is " << average << " inches.\n";

    // Step 7: Put the output formatting back to normal so the other
    // questions don't print their numbers with forced 2 decimals.
    cout.unsetf(ios::floatfield);
    cout << setprecision(6);
}

// =====================================================================
// QUESTION 2: Volume of a Block (three dimensional hyperrectangle)
// Asks for width, length, and height, then calculates the volume.
// Input validation: no side can be zero or less.
// =====================================================================
void question2()
{
    // Step 1: Start each side at 0 so the do-while loops run at least once.
    double width = 0, length = 0, height = 0;

    // Step 2: Ask for the width until it is greater than zero.
    do
    {
        cout << "Enter the block's width: ";
        if (!readDouble(width))              // not a number
        {
            cout << "Please enter a number.\n";
            width = 0;                       // force the loop to repeat
        }
        else if (width <= 0)                 // zero or negative
            cout << "No side can be zero or less.\n";
    } while (width <= 0);

    // Step 3: Ask for the length the same way.
    do
    {
        cout << "Enter the block's length: ";
        if (!readDouble(length))
        {
            cout << "Please enter a number.\n";
            length = 0;
        }
        else if (length <= 0)
            cout << "No side can be zero or less.\n";
    } while (length <= 0);

    // Step 4: Ask for the height the same way.
    do
    {
        cout << "Enter the block's height: ";
        if (!readDouble(height))
        {
            cout << "Please enter a number.\n";
            height = 0;
        }
        else if (height <= 0)
            cout << "No side can be zero or less.\n";
    } while (height <= 0);

    // Step 5: Volume = width * length * height.
    double volume = width * length * height;

    // Step 6: Display the answer.
    cout << "The volume of the block is " << volume << ".\n";
}

// =====================================================================
// QUESTION 3: Roman Numerals
// Asks for a number from 1 to 10 and displays the Roman numeral.
// Input validation: rejects numbers below 1 or above 10.
// =====================================================================
void question3()
{
    int number = 0;         // the number the user enters
    bool valid = false;     // stays false until we get a good number

    // Step 1: Keep asking until the user enters a whole number from 1 to 10.
    while (!valid)
    {
        cout << "Enter a number (1 - 10): ";

        if (!readInt(number))                    // letters or decimals
            cout << "Please enter a whole number between 1 and 10.\n";
        else if (number < 1 || number > 10)      // out of range
            cout << "The number must be between 1 and 10.\n";
        else
            valid = true;                        // good number
    }

    // Step 2: Use a switch statement to pick the matching Roman numeral.
    // Each case matches one number and stores its Roman numeral in "roman".
    // "break" stops the switch from running into the next case.
    string roman;
    switch (number)
    {
    case 1:  roman = "I";    break;
    case 2:  roman = "II";   break;
    case 3:  roman = "III";  break;
    case 4:  roman = "IV";   break;
    case 5:  roman = "V";    break;
    case 6:  roman = "VI";   break;
    case 7:  roman = "VII";  break;
    case 8:  roman = "VIII"; break;
    case 9:  roman = "IX";   break;
    case 10: roman = "X";    break;
    }

    // Step 3: Display the result.
    cout << "The Roman numeral version of " << number << " is " << roman << ".\n";
}

// =====================================================================
// QUESTION 4: Geometry Calculator
// Shows a menu, then calculates the area of a circle, rectangle, or
// triangle. This function handles ONE selection and then returns to
// the main menu (Question 6 requirement) instead of ending the program.
// =====================================================================
void question4()
{
    const double PI = 3.14159;   // constant: its value can't change
    int choice;                  // the user's menu selection

    // Step 1: Display the Geometry Calculator menu.
    cout << "\nGeometry Calculator\n";
    cout << "1. Calculate the Area of a Circle\n";
    cout << "2. Calculate the Area of a Rectangle\n";
    cout << "3. Calculate the Area of a Triangle\n";
    cout << "4. Quit\n";
    cout << "Enter your choice (1-4): ";

    // Step 2: Validate the menu choice. If it is not a number from 1 to 4,
    // print the error message and return to the main menu.
    if (!readInt(choice) || choice < 1 || choice > 4)
    {
        cout << "The valid choices are 1 through 4. Run the program again and select one of those.\n";
        return;   // leave this function early
    }

    // Step 3: Run the calculation that matches the choice.
    if (choice == 1)
    {
        // ----- Circle: area = PI * radius * radius -----
        double radius;
        cout << "Enter the circle's radius: ";

        if (!readDouble(radius))                 // not a number
            cout << "Please enter a number.\n";
        else if (radius < 0)                     // negative radius is invalid
            cout << "The radius cannot be less than zero.\n";
        else
            cout << "The area is " << PI * radius * radius << "\n";
    }
    else if (choice == 2)
    {
        // ----- Rectangle: area = length * width -----
        double length, width;

        cout << "Enter the rectangle's length: ";
        if (!readDouble(length))
        {
            cout << "Please enter a number.\n";
            return;
        }

        cout << "Enter the rectangle's width: ";
        if (!readDouble(width))
        {
            cout << "Please enter a number.\n";
            return;
        }

        if (length < 0 || width < 0)             // negatives are not allowed
            cout << "Only enter positive values for length and width.\n";
        else
            cout << "The area is " << length * width << "\n";
    }
    else if (choice == 3)
    {
        // ----- Triangle: area = base * height * 0.5 -----
        double base, height;

        cout << "Enter the triangle's base: ";
        if (!readDouble(base))
        {
            cout << "Please enter a number.\n";
            return;
        }

        cout << "Enter the triangle's height: ";
        if (!readDouble(height))
        {
            cout << "Please enter a number.\n";
            return;
        }

        if (base < 0 || height < 0)              // negatives are not allowed
            cout << "Only enter positive values for base and height.\n";
        else
            cout << "The area is " << base * height * 0.5 << "\n";
    }
    else
    {
        // ----- Choice 4: Quit -----
        // Nothing to calculate; just go back to the main menu.
        cout << "Returning to the main menu.\n";
    }
}

// =====================================================================
// QUESTION 5: Distance Traveled
// Asks for speed (mph) and hours traveled, then uses a loop to display
// the distance traveled at the end of each hour.
// Input validation: speed cannot be negative, hours must be at least 1.
// =====================================================================
void question5()
{
    double speed = -1;   // starts invalid so the loop below runs
    int hours = 0;       // starts invalid so the loop below runs

    // Step 1: Ask for the speed until it is zero or higher.
    do
    {
        cout << "What is the speed of the vehicle in mph? ";
        if (!readDouble(speed))                  // not a number
        {
            cout << "Please enter a number.\n";
            speed = -1;                          // force the loop to repeat
        }
        else if (speed < 0)                      // negative speed
            cout << "Speed cannot be negative.\n";
    } while (speed < 0);

    // Step 2: Ask for the hours until it is 1 or higher.
    do
    {
        cout << "How many hours has it traveled? ";
        if (!readInt(hours))                     // not a whole number
        {
            cout << "Please enter a whole number.\n";
            hours = 0;                           // force the loop to repeat
        }
        else if (hours < 1)                      // less than 1 hour
            cout << "Hours traveled must be at least 1.\n";
    } while (hours < 1);

    // Step 3: Print the table headings.
    cout << "\nHour" << setw(20) << "Distance Traveled\n";
    cout << "--------------------------------\n";

    // Step 4: Loop from hour 1 up to the total hours.
    // Each pass prints: distance = speed * hour.
    // setw() lines the columns up neatly.
    for (int hour = 1; hour <= hours; hour++)
    {
        cout << setw(2) << hour << setw(18) << speed * hour << "\n";
    }
}

// =====================================================================
// QUESTION 6: Main Menu
// main() shows a menu, calls the function the user picks, and repeats
// until the user chooses the exit option.
// =====================================================================
int main()
{
    int choice = 0;   // the user's menu selection

    // Step 1: A do-while loop shows the menu at least once and keeps
    // repeating until the user picks 6 (Exit).
    do
    {
        // Step 2: Display the main menu.
        cout << "\n===== M5HW1 Main Menu =====\n";
        cout << "1. Question 1 - Average Rainfall\n";
        cout << "2. Question 2 - Volume of a Block\n";
        cout << "3. Question 3 - Roman Numerals\n";
        cout << "4. Question 4 - Geometry Calculator\n";
        cout << "5. Question 5 - Distance Traveled\n";
        cout << "6. Exit\n";
        cout << "Enter your choice (1-6): ";

        // Step 3: Read the choice. If the user typed letters instead of a
        // number, set choice to 0 so it falls into the "invalid" case below.
        if (!readInt(choice))
            choice = 0;

        cout << "\n";

        // Step 4: Call the function that matches the choice.
        switch (choice)
        {
        case 1: question1(); break;   // run Question 1
        case 2: question2(); break;   // run Question 2
        case 3: question3(); break;   // run Question 3
        case 4: question4(); break;   // run Question 4
        case 5: question5(); break;   // run Question 5
        case 6: cout << "Goodbye!\n"; break;   // exit message
        default:                               // anything else is an error
            cout << "Invalid choice. Please enter a number from 1 to 6.\n";
        }

        // Step 5: Go back to the top and show the menu again,
        // unless the user chose 6.
    } while (choice != 6);

    // Step 6: Return 0 to tell the operating system the program ended normally.
    return 0;
}