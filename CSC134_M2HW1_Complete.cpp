/*
CSC 134
M2HW1 - Gold (4 questions max) - All Questions Completed
WiliamsM
9/18/26

PROGRAM DESCRIPTION:
This program demonstrates basic C++ input/output, variables, arithmetic operations,
and string manipulation. It includes four separate functions that solve different
real-world problems using cin and cout.

HOW TO USE:
- Each question is implemented as a separate function
- Uncomment the function calls in main() to run specific questions
- All four questions are implemented for Gold tier
- Comments explain each step of the program logic
*/

#include <iostream>
#include <iomanip>      // For formatting output (setprecision, fixed, setw)
#include <string>       // For string data type and concatenation
#include <cstdlib>      // For random number generation
#include <ctime>        // For seeding random numbers

using namespace std;

// Function prototypes (declarations) - tells compiler these functions exist
// They will be defined below main()
void question1();
void question2();
void question3();
void question4();

int main() {
    // Run all four questions by uncommenting these lines
    question1();
    question2();
    question3();
    question4();
    
    return 0;  // Program ends successfully
}

// ============================================================================
// QUESTION 1: BANKING TRANSACTION SIMULATOR
// ============================================================================
// Purpose: Simulate a simple bank account with deposits and withdrawals
// Concepts: cin/cout, string variables, arithmetic operations, random numbers

void question1() {
    cout << "\n========== QUESTION 1: BANKING TRANSACTIONS ==========" << endl;
    
    // Declare variables to store account information
    string accountName;           // Name on the account
    double startingBalance;       // Initial balance in dollars
    double depositAmount;         // Money being added to account
    double withdrawalAmount;      // Money being removed from account
    double finalBalance;          // Result after all transactions
    int accountNumber;            // Randomly generated account number
    
    // STEP 1: Get the account holder's name
    cout << "Enter the name for this account: ";
    getline(cin, accountName);    // getline() allows spaces in name (improvement feature)
    
    // STEP 2: Get the starting balance
    cout << "Enter the starting account balance ($): ";
    cin >> startingBalance;
    
    // STEP 3: Get the deposit amount
    cout << "Enter the deposit amount ($): ";
    cin >> depositAmount;
    
    // STEP 4: Get the withdrawal amount
    cout << "Enter the withdrawal amount ($): ";
    cin >> withdrawalAmount;
    
    // Clear the input buffer for next question
    cin.ignore();
    
    // STEP 5: Generate a random account number (5-digit number)
    srand(time(0));  // Seed the random number generator with current time
    accountNumber = (rand() % 90000) + 10000;  // Generate number between 10000-99999
    
    // STEP 6: Calculate the final balance
    // Formula: Starting Balance + Deposit - Withdrawal
    finalBalance = startingBalance + depositAmount - withdrawalAmount;
    
    // STEP 7: Display all account information with formatted output
    cout << "\n--- Account Summary ---" << endl;
    cout << "Account Holder: " << accountName << endl;
    cout << "Account Number: " << accountNumber << endl;
    
    // Display money amounts with 2 decimal places (improvement feature)
    // fixed: show decimal point
    // setprecision(2): show exactly 2 digits after decimal
    cout << fixed << setprecision(2);
    cout << "Final Balance: $" << finalBalance << endl;
}

// ============================================================================
// QUESTION 2: GENERAL CRATES STORAGE CALCULATOR
// ============================================================================
// Purpose: Calculate storage costs based on updated pricing
// Concepts: Variables, arithmetic, currency formatting
// NOTE: This is based on M2LAB1 with updated costs

void question2() {
    cout << "\n========== QUESTION 2: GENERAL CRATES STORAGE ==========" << endl;
    
    // Declare variables for storage calculation
    double length;                // Length of crate in feet
    double width;                 // Width of crate in feet
    double height;                // Height of crate in feet
    double volume;                // Cubic footage (length × width × height)
    
    // Updated pricing constants (from problem requirements)
    const double COST_PER_CUBIC_FOOT = 0.3;      // What we charge customers to store
    const double CHARGE_PER_CUBIC_FOOT = 0.52;   // Maximum charge per cubic foot
    
    double storageCost;           // Cost to store the crate
    double chargeToCustomer;      // Amount we charge the customer
    double profit;                // Our profit (charge - cost)
    
    // STEP 1: Get the dimensions of the storage crate
    cout << "Enter the length of the crate (in feet): ";
    cin >> length;
    
    cout << "Enter the width of the crate (in feet): ";
    cin >> width;
    
    cout << "Enter the height of the crate (in feet): ";
    cin >> height;
    
    // STEP 2: Calculate the volume (cubic footage)
    // Formula: Length × Width × Height = Volume
    volume = length * width * height;
    
    // STEP 3: Calculate the storage cost (what it costs us)
    // Formula: Volume × Cost Per Cubic Foot
    storageCost = volume * COST_PER_CUBIC_FOOT;
    
    // STEP 4: Calculate what we charge the customer
    // Formula: Volume × Charge Per Cubic Foot
    chargeToCustomer = volume * CHARGE_PER_CUBIC_FOOT;
    
    // STEP 5: Calculate profit
    // Formula: Customer Charge - Storage Cost
    profit = chargeToCustomer - storageCost;
    
    // STEP 6: Display all calculations with formatted currency (2 decimal places)
    cout << "\n--- Storage Calculation ---" << endl;
    cout << "Crate Dimensions: " << length << " ft × " << width << " ft × " << height << " ft" << endl;
    cout << "Volume: " << volume << " cubic feet" << endl;
    
    // Format all currency with 2 decimal places
    cout << fixed << setprecision(2);
    cout << "Storage Cost (internal): $" << storageCost << endl;
    cout << "Customer Charge: $" << chargeToCustomer << endl;
    cout << "Profit: $" << profit << endl;
}

// ============================================================================
// QUESTION 3: PIZZA PARTY CALCULATOR
// ============================================================================
// Purpose: Calculate leftover pizza slices after distributing to visitors
// Concepts: Arithmetic operations, integer division, modulo operator

void question3() {
    cout << "\n========== QUESTION 3: PIZZA PARTY CALCULATOR ==========" << endl;
    
    // Declare variables for pizza party calculation
    int numberOfPizzas;           // How many pizzas to order
    int slicesPerPizza;           // Slices in each pizza
    int numberOfVisitors;         // How many people coming to party
    
    int totalSlices;              // Total number of slices available
    int slicesPerVisitor = 3;     // Each visitor gets 3 slices
    int slicesNeeded;             // Total slices needed for all visitors
    int leftoverSlices;           // Slices remaining after distribution
    
    // STEP 1: Get the number of pizzas
    cout << "How many pizzas are you ordering? ";
    cin >> numberOfPizzas;
    
    // STEP 2: Get slices per pizza
    cout << "How many slices per pizza? ";
    cin >> slicesPerPizza;
    
    // STEP 3: Get number of visitors
    cout << "How many visitors are coming? ";
    cin >> numberOfVisitors;
    
    // STEP 4: Calculate total number of slices available
    // Formula: Number of Pizzas × Slices Per Pizza
    totalSlices = numberOfPizzas * slicesPerPizza;
    
    // STEP 5: Calculate total slices needed
    // Formula: Number of Visitors × Slices Per Visitor (3)
    slicesNeeded = numberOfVisitors * slicesPerVisitor;
    
    // STEP 6: Calculate leftover slices
    // Formula: Total Slices - Slices Needed
    leftoverSlices = totalSlices - slicesNeeded;
    
    // STEP 7: Display the results
    cout << "\n--- Pizza Party Summary ---" << endl;
    cout << "Total Pizzas: " << numberOfPizzas << endl;
    cout << "Slices Per Pizza: " << slicesPerPizza << endl;
    cout << "Total Slices Available: " << totalSlices << endl;
    cout << "Number of Visitors: " << numberOfVisitors << endl;
    cout << "Slices Per Visitor: " << slicesPerVisitor << endl;
    cout << "Slices Needed: " << slicesNeeded << endl;
    
    // Display leftover slices (could be negative if not enough pizza!)
    cout << "Leftover Slices: " << leftoverSlices << endl;
    
    // BONUS: Add helpful feedback
    if (leftoverSlices >= 0) {
        cout << "You have enough pizza! ✓" << endl;
    } else {
        cout << "WARNING: You need " << abs(leftoverSlices) << " more slices!" << endl;
    }
}

// ============================================================================
// QUESTION 4: FTCC CHEERING PROGRAM (GOLD BONUS - STRING CONCATENATION)
// ============================================================================
// Purpose: Display school spirit message using ONLY string variables
// Concepts: String variables, string concatenation with + operator
// GOLD BONUS REQUIREMENTS:
//   A. No raw strings - only string variables
//   B. Use only: letsGo, school, team, cheerOne, cheerTwo
//   C. Use string concatenation with the + operator

void question4() {
    cout << "\n========== QUESTION 4: FTCC CHEERING PROGRAM ==========" << endl;
    
    // STEP 1: Declare string variables (GOLD requirement B)
    // These are the ONLY string variables allowed for this bonus
    string letsGo = "Let's go ";     // Base phrase for cheer
    string school = "FTCC";          // School name
    string team = "Trojans";         // Team name
    string cheerOne;                 // Will hold "Let's go FTCC" (built with concatenation)
    string cheerTwo;                 // Will hold "Let's go Trojans" (built with concatenation)
    
    // STEP 2: Build cheerOne using string concatenation
    // The + operator joins strings together
    // Formula: cheerOne = "Let's go " + "FTCC"
    cheerOne = letsGo + school;
    
    // STEP 3: Build cheerTwo using string concatenation
    // Formula: cheerTwo = "Let's go " + "Trojans"
    cheerTwo = letsGo + team;
    
    // STEP 4: Display the cheering message
    // Output "Let's go FTCC" three times (using cheerOne variable)
    cout << cheerOne << endl;
    cout << cheerOne << endl;
    cout << cheerOne << endl;
    
    // Output "Let's go Trojans" once (using cheerTwo variable)
    cout << cheerTwo << endl;
}

// ============================================================================
// END OF PROGRAM
// ============================================================================
// SUMMARY OF CONCEPTS DEMONSTRATED:
//
// 1. INPUT/OUTPUT (I/O):
//    - cin: Read data from keyboard
//    - cout: Display data to screen
//    - getline(): Read entire line including spaces
//
// 2. VARIABLES:
//    - int: Whole numbers (no decimals)
//    - double: Numbers with decimals (money, measurements)
//    - string: Text data
//    - const: Values that don't change
//
// 3. ARITHMETIC OPERATIONS:
//    - + (addition)
//    - - (subtraction)
//    - * (multiplication)
//    - / (division)
//
// 4. OUTPUT FORMATTING:
//    - fixed: Show decimal point
//    - setprecision(2): Show exactly 2 decimal places
//    - setw(): Set field width for alignment
//
// 5. STRING OPERATIONS:
//    - Concatenation: Joining strings with + operator
//    - getline(): Reading strings with spaces
//
// 6. FUNCTIONS:
//    - Function prototypes: Declare before using
//    - Function definitions: Full code below main()
//    - return statement: Exit function and return to caller
