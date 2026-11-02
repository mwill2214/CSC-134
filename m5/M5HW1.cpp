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
//
// NEW: Question 1 now has a menu for each section: a month menu and a
//      rainfall menu. The user can pick from the list or choose the last
//      option to type their own value like before.
//
// NEW: In Question 1 the user can now EXIT at any time and go back to the
//      main menu: choose 0 in the month menu or the rainfall menu, or
//      type the word exit at any typing prompt.
// =====================================================================
// ---------- Step 1: Include the libraries we need ----------
#include <iostream>   // cin and cout for keyboard input / screen output
#include <iomanip>    // setw() and setprecision() for formatting output
#include <string>     // string variables (month names, Roman numerals)
#include <limits>     // numeric_limits, used to throw away bad input
#include <cctype>     // NEW: tolower(), used to read "exit" in any letter case

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

// ---------------------------------------------------------------------
// NEW HELPER: typedExit
// Call this right BEFORE readDouble() at a typing prompt. It checks
// whether the user typed the word "exit" (any letter case).
// Returns true  -> the user typed exit, so the caller should leave.
// Returns false -> the user typed something else. If it was a word,
//                  cin is put in a failed state so the normal
//                  "Please enter a number." message runs next.
// ---------------------------------------------------------------------
bool typedExit()
{
    cin >> ws;                       // skip spaces and the leftover newline
    int next = cin.peek();           // look at the next character without removing it

    if (next == 'e' || next == 'E')  // only words starting with e can be "exit"
    {
        string word;
        cin >> word;                 // read the whole word
        for (size_t k = 0; k < word.size(); k++)
            word[k] = (char)tolower((unsigned char)word[k]);   // make it lowercase

        if (word == "exit")
            return true;             // the user wants to leave

        cin.setstate(ios::failbit);  // not exit: let the caller's number check fail
    }

    return false;                    // not the exit word
}

// ---------------------------------------------------------------------
// NEW HELPER: pickFromMenu (used by the rainfall menu in Question 1)
// Shows a numbered menu of ready-made values plus one extra last option
// "Type my own value". The user's choice is validated (must be a number
// from 1 to count + 1).
//   label   - what we are asking for (used in the menu title)
//   options - the array of ready-made values to show in the menu
//   count   - how many values are in the options array
//   value   - receives the chosen value if the user picks from the list
//   exitRequested - NEW: optional. If given, it is set to true when the
//                   user chooses 0 (Exit to main menu)
// Returns true  -> the user picked a value from the list (value is set)
// Returns false -> the user chose "Type my own value" (or chose Exit),
//                  so the caller should check exitRequested first
// ---------------------------------------------------------------------
bool pickFromMenu(const string &label, const double options[], int count, double &value,
                  bool *exitRequested = nullptr)
{
    int choice = 0;         // the menu number the user enters
    bool ok = false;        // stays false until the choice is valid

    // Keep showing the menu until the choice is a whole number from 1 to count + 1.
    while (!ok)
    {
        cout << "\nChoose the " << label << ":\n";

        // Print one menu line for each ready-made value.
        for (int i = 0; i < count; i++)
            cout << setw(2) << (i + 1) << ". " << options[i] << "\n";

        // The last menu line lets the user type their own value instead.
        cout << setw(2) << (count + 1) << ". Type my own value\n";

        // NEW: Option 0 lets the user exit back to the main menu.
        cout << " 0. Exit to main menu\n";
        cout << "Enter your choice (0-" << (count + 1) << "): ";

        if (!readInt(choice))                          // letters/symbols
            cout << "Please enter a number from 0 to " << (count + 1) << ".\n";
        else if (choice < 0 || choice > count + 1)     // out of range (0 is now allowed)
            cout << "The valid choices are 0 through " << (count + 1) << ".\n";
        else
            ok = true;                                 // good choice
    }

    // NEW: Choice 0 means the user wants to exit. Tell the caller and stop.
    if (choice == 0)
    {
        if (exitRequested != nullptr)
            *exitRequested = true;
        return false;
    }

    // If the choice was one of the ready-made values, hand it back.
    if (choice <= count)
    {
        value = options[choice - 1];   // menu number 1 is position 0, and so on
        return true;                   // a value from the list was picked
    }

    return false;                      // user wants to type their own value
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

    // NEW: The month menu list. Position 0 is January, position 11 is
    // December. Menu choice 1-12 matches position 0-11 (choice minus 1).
    const string MONTH_NAMES[12] = {"January", "February", "March", "April",
                                    "May", "June", "July", "August",
                                    "September", "October", "November", "December"};

    // NEW: Ready-made rainfall amounts (in inches) for the rainfall menu.
    const double RAIN_OPTIONS[6] = {0, 1, 2.5, 5, 8.33333, 15};

    // Step 2: Loop three times, once for each month.
    for (int i = 0; i < 3; i++)
    {
        // NEW: Month menu. Shows the 12 months plus a 13th option to type
        // a month name. Keeps asking until the choice is a number 1-13.
        int monthChoice = 0;
        bool monthMenuValid = false;
        while (!monthMenuValid)
        {
            cout << "\nSelect month " << (i + 1) << " of 3:\n";
            for (int m = 0; m < 12; m++)         // print the 12 month lines
                cout << setw(2) << (m + 1) << ". " << MONTH_NAMES[m] << "\n";
            cout << "13. Type my own month\n";   // the extra "type it" option
            cout << " 0. Exit to main menu\n";   // NEW: exit option
            cout << "Enter your choice (0-13): ";

            if (!readInt(monthChoice))                       // letters/symbols
                cout << "Please enter a number from 0 to 13.\n";
            else if (monthChoice < 0 || monthChoice > 13)    // out of range (0 is now allowed)
                cout << "The valid choices are 0 through 13.\n";
            else
                monthMenuValid = true;                       // good choice
        }

        // NEW: Choice 0 means exit. Leave Question 1 and go back to the main menu.
        if (monthChoice == 0)
        {
            cout << "Exiting Question 1. Returning to the main menu.\n";
            return;
        }

        // NEW: If a month from the list was chosen (1-12), save its name
        // (choice 5 -> "May"). Choice 13 means the user types the month.
        bool monthPicked = (monthChoice <= 12);
        if (monthPicked)
            month[i] = MONTH_NAMES[monthChoice - 1];

        // Step 3: Read the month name.
        // NEW: This only runs if the user chose "Type my own month".
        if (!monthPicked)
        {
            cout << "(Type exit to return to the main menu)\n";   // NEW: reminder
            cout << "Enter month: ";
            cin >> month[i];

            // NEW: If the user typed exit (any letter case), leave Question 1.
            string typedMonth = month[i];
            for (size_t k = 0; k < typedMonth.size(); k++)
                typedMonth[k] = (char)tolower((unsigned char)typedMonth[k]);
            if (typedMonth == "exit")
            {
                cout << "Exiting Question 1. Returning to the main menu.\n";
                return;
            }
        }

        // NEW: Rainfall menu. rainPicked is true if the user chose a
        // ready-made amount from the list.
        bool exitFromRain = false;   // NEW: becomes true if the user chooses 0 (Exit)
        bool rainPicked = pickFromMenu("rainfall for " + month[i] + " (inches)",
                                       RAIN_OPTIONS, 6, rain[i], &exitFromRain);

        // NEW: If the user chose Exit in the rainfall menu, leave Question 1.
        if (exitFromRain)
        {
            cout << "Exiting Question 1. Returning to the main menu.\n";
            return;
        }

        // Step 4: Read the rainfall and keep asking until it is valid.
        bool valid = false;

        // NEW: A value picked from the list is already valid, so the
        // typing loop below is skipped.
        if (rainPicked)
            valid = true;

        while (!valid)
        {
            cout << "(Type exit to return to the main menu)\n";   // NEW: reminder
            cout << "Enter rainfall for " << month[i] << ": ";

            // NEW: If the user typed exit, leave Question 1.
            if (typedExit())
            {
                cout << "Exiting Question 1. Returning to the main menu.\n";
                return;
            }

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