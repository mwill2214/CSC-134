// CSC 134
// M3HW1 - Gold
// Name: WilliamsM
// Date: October 5, 2026
//
// All four questions are answered in main(), separated by a cout header.
// Every variable has a unique name so nothing is declared twice.

#include <iostream>   // cin and cout
#include <iomanip>    // setprecision and fixed for dollar amounts
#include <string>     // string variables
#include <cstdlib>    // rand() and srand()
#include <ctime>      // time() to seed the random numbers
using namespace std;

int main()
{
    // ==================================================
    // Question 1 - Talking Computer- Chat Bot
    // ==================================================
    cout << "Question 1" << endl;

    // Step 1: make a string variable to hold the user's reply
    string chatAnswer;

    // Step 2: greet the user and ask the question
    cout << "Hello, I'm a C++ program!" << endl;
    cout << "Do you like me? Please type yes or no." << endl;

    // Step 3: read the user's reply
    cin >> chatAnswer;

    // Step 4: if / else if / else gives three possible responses
    if (chatAnswer == "yes")
    {
        cout << "That's great! I'm sure we'll get along." << endl;
    }
    else if (chatAnswer == "no")
    {
        cout << "Well, maybe you'll learn to like me later." << endl;
    }
    else
    {
        // Runs for anything that is not "yes" or "no"
        cout << "If you're not sure... that's OK." << endl;
    }

    cout << endl;

    // ==================================================
    // Question 2 - Receipt calculator with tip
    // ==================================================
    cout << "Question 2" << endl;

    // Step 1: constants for the tax and tip rates (easy to change later)
    const double TAX_RATE = 0.075;   // 7.5% sales tax
    const double TIP_RATE = 0.15;    // 15% tip for dine in

    // Step 2: variables for the input and the calculated amounts
    double mealPrice;
    int orderType;
    double taxAmount;
    double tipAmount = 0.0;          // starts at 0 so takeaway has no tip
    double totalDue;

    // Step 3: ask for the meal price
    cout << "Please enter the price of the meal: $";
    cin >> mealPrice;

    // Step 4: ask whether the order is dine in or takeaway
    cout << "Please enter 1 if the order is dine in, 2 if it is to go: ";
    cin >> orderType;

    // Step 5: calculate the tax (meal price times tax rate)
    taxAmount = mealPrice * TAX_RATE;

    // Step 6: only dine in orders get a tip
    if (orderType == 1)
    {
        tipAmount = mealPrice * TIP_RATE;
    }

    // Step 7: total = meal + tax + tip
    totalDue = mealPrice + taxAmount + tipAmount;

    // Step 8: print the receipt, always showing 2 decimal places
    cout << fixed << setprecision(2);
    cout << endl;
    cout << "------- RECEIPT -------" << endl;
    cout << "Meal price:  $" << mealPrice << endl;
    cout << "Tax:         $" << taxAmount << endl;
    if (orderType == 1)
    {
        cout << "Tip (15%):   $" << tipAmount << endl;
    }
    else
    {
        cout << "Tip:         $" << tipAmount << " (takeaway)" << endl;
    }
    cout << "-----------------------" << endl;
    cout << "Total due:   $" << totalDue << endl;

    cout << endl;

    // ==================================================
    // Question 3 - Choose Your Own Adventure
    // ==================================================
    cout << "Question 3" << endl;

    // Step 1: variables to store the player's two choices
    int firstChoice;
    int secondChoice;

    // Step 2: set the scene and give the first choice
    cout << "You wake up alone in a dark cave. You hear growling nearby." << endl;
    cout << "1. Walk toward the growling" << endl;
    cout << "2. Sneak toward the faint light" << endl;
    cout << "Enter 1 or 2: ";
    cin >> firstChoice;

    // Step 3: nested if - the second choice only happens on option 2
    if (firstChoice == 1)
    {
        // Choice 1 leads straight to game over
        cout << "A giant bear was guarding the cave. GAME OVER." << endl;
    }
    else if (firstChoice == 2)
    {
        // Choice 2 leads to another decision
        cout << "You reach a fork in the tunnel near the light." << endl;
        cout << "1. Climb the rope toward the sunlight" << endl;
        cout << "2. Jump into the dark water below" << endl;
        cout << "Enter 1 or 2: ";
        cin >> secondChoice;

        // One answer is victory, the other is defeat
        if (secondChoice == 1)
        {
            cout << "You climb out into the sunshine. YOU WIN!" << endl;
        }
        else if (secondChoice == 2)
        {
            cout << "The water is freezing and the current drags you under. YOU LOSE." << endl;
        }
        else
        {
            cout << "You hesitate too long and the cave collapses. GAME OVER." << endl;
        }
    }
    else
    {
        // Handles any number other than 1 or 2
        cout << "You froze in fear and never moved. GAME OVER." << endl;
    }

    cout << endl;

    // ==================================================
    // Question 4 - Math practice
    // ==================================================
    cout << "Question 4" << endl;

    // Step 1: seed the random number generator with the current time
    // so the numbers are different every run
    srand(time(0));

    // Step 2: generate two single digit numbers (0 through 9)
    // rand() % 10 gives the remainder after dividing by 10
    int firstNumber = rand() % 10;
    int secondNumber = rand() % 10;

    // Step 3: variable for the user's answer, and the correct answer
    int userAnswer;
    int correctAnswer = firstNumber + secondNumber;

    // Step 4: show the addition problem
    cout << "What is " << firstNumber << " plus " << secondNumber << "?" << endl;

    // Step 5: read the user's answer
    cin >> userAnswer;

    // Step 6: compare and report whether it was correct
    if (userAnswer == correctAnswer)
    {
        cout << "Correct!" << endl;
    }
    else
    {
        cout << "Incorrect." << endl;
    }

    return 0;
}
