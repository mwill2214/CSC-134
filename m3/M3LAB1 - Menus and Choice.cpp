// CSC 134
// M3LAB1 - Menus and Choice
// WilliamsM
// 28 September 2026


#include <iostream>
using namespace std;

// DECLARE that your functions are coming later, before main()
// after main, DEFINE your functions in full.
void chooseDoor1();
void chooseDoor2();
void chooseDoor3(); // NEW: declares that Door 3's function exists (defined after main)
void chooseDoor4(); // NEW: declares that Door 4's function exists (defined after main)

int main() {
    int choice; // menu choice

    // ask the question
    cout << "Do you choose Door 1 , Door 2 , Door 3 or Door 4?" << endl;
    cout << "1. Choose Door #1" << endl;
    cout << "2. Choose Door #2" << endl;
    cout << "3. Choose Door #3" << endl; // NEW: menu option for Door 3
    cout << "4. Choose Door #4" << endl; // NEW: menu option for Door 4
    cout << "? "; // the prompt
    cin >> choice;
    // can also say (choice == 1)
    if (1 == choice) {
        chooseDoor1();
    }
    else if (2 == choice) {
        chooseDoor2();
    }
    else if (3 == choice) { // NEW: if the user typed 3, run Door 3's function
        chooseDoor3();
    }
    else if (4 == choice) { // NEW: if the user typed 4, run Door 4's function
        chooseDoor4();
    }
    else {
        cout << "I'm sorry, that is not a valid choice." << endl;
        // program ends, or we could loop around again
    }

    cout << "Thank you for playing!" << endl;
    return 0; // tells the computer that we finished without errors

} // end of the main() method

// After main(), we define all our other functions.
// (Declaring means "This function exists", we did that above.)
// (Defining means "This is what the function does".)
void chooseDoor1() {
    // this function is called in main if the user chooses 1.
    cout << "You chose Door 1" << endl;
    cout << "You win ... A NEW CAR!" << endl;
    // NEW: 3 more items to win in Door 1
    cout << "You also win ... a year of free gas!" << endl;
    cout << "You also win ... a set of custom floor mats!" << endl;
    cout << "You also win ... a lifetime car wash pass!" << endl;
    cout << "1. Hop in the car and drive" << endl;
    cout << "2. Donate the car to charity" << endl;
    // NEW: 3 more choices in Door 1 (now 5 total)
    cout << "3. Take your family on a road trip" << endl;
    cout << "4. Sell the car and invest the money" << endl;
    cout << "5. Give the car to your best friend" << endl;
    cout << "? "; // the prompt

    // NEW: read the user's choice and show the result
    int action; // stores the user's choice inside Door 1
    cin >> action;
    if (1 == action) {
        cout << "You cruise down the highway with the windows down!" << endl;
    }
    else if (2 == action) {
        cout << "The charity is thrilled, and you get a nice tax deduction!" << endl;
    }
    else if (3 == action) {
        cout << "Your family makes memories that last a lifetime!" << endl;
    }
    else if (4 == action) {
        cout << "You invest wisely and your money grows over time!" << endl;
    }
    else if (5 == action) {
        cout << "Your best friend cannot stop smiling. Best gift ever!" << endl;
    }
    else {
        cout << "I'm sorry, that is not a valid choice." << endl;
    }
}

void chooseDoor2() {
    // this function is called in main if the user chooses 1.
    cout << "You chose Door 2" << endl;
    cout << "You win ... a bottle of floor wax." << endl;
    // NEW: 3 more items to win in Door 2
    cout << "You also win ... a brand new mop!" << endl;
    cout << "You also win ... a big bucket!" << endl;
    cout << "You also win ... a pack of sponges!" << endl;
    // NEW: 3 choices in Door 2
    cout << "1. Wax the floors in your house" << endl;
    cout << "2. Start a floor cleaning business" << endl;
    cout << "3. Give the cleaning kit to a neighbor" << endl;
    // NEW: 2 more choices in Door 2 (now 5 total)
    cout << "4. Sell the cleaning kit online" << endl;
    cout << "5. Use the wax to make your garage floor shine" << endl;
    cout << "? "; // the prompt

    // NEW: read the user's choice and show the result
    int action; // stores the user's choice inside Door 2
    cin >> action;
    if (1 == action) {
        cout << "Your floors are so shiny you can see your reflection!" << endl;
    }
    else if (2 == action) {
        cout << "Your first customer books you for the whole week!" << endl;
    }
    else if (3 == action) {
        cout << "Your neighbor is grateful and bakes you cookies!" << endl;
    }
    else if (4 == action) { // NEW: response for choice 4
        cout << "You sell the kit online and make $50 in one day!" << endl;
    }
    else if (5 == action) { // NEW: response for choice 5
        cout << "Your garage floor looks brand new. Everyone is jealous!" << endl;
    }
    else {
        cout << "I'm sorry, that is not a valid choice." << endl;
    }
}

// NEW: Door 3 is defined here, after main(), like the other doors.
void chooseDoor3() {
    // this function is called in main if the user chooses 3.
    int action; // STEP 1: a variable to store the user's choice inside Door 3

    // STEP 2: tell the user which door they picked and what they won
    cout << "You chose Door 3" << endl;
    cout << "You win ... A TRIP TO HAWAII!" << endl;

    // STEP 3: show 5 things the user can do with the prize
    cout << "1. Book the flight and leave tomorrow" << endl;
    cout << "2. Invite your best friend to come along" << endl;
    cout << "3. Save the trip for your honeymoon" << endl;
    cout << "4. Sell the trip for cash" << endl;
    cout << "5. Give the trip to your parents" << endl;
    cout << "? "; // the prompt

    // STEP 4: read the user's choice from the keyboard
    cin >> action;

    // STEP 5: check which choice was entered and show the result
    if (1 == action) {
        cout << "You pack your bags and soak up the sun on the beach!" << endl;
    }
    else if (2 == action) {
        cout << "You and your best friend have the vacation of a lifetime!" << endl;
    }
    else if (3 == action) {
        cout << "A very romantic plan. Your future spouse will be thrilled!" << endl;
    }
    else if (4 == action) {
        cout << "You sell the trip and pocket $2,000. Cha-ching!" << endl;
    }
    else if (5 == action) {
        cout << "Your parents are so happy they cry tears of joy!" << endl;
    }
    else {
        // STEP 6: if the number was not 1-5, tell the user it is invalid
        cout << "I'm sorry, that is not a valid choice." << endl;
    }
}

// NEW: Door 4 is defined here, after main(), like the other doors.
void chooseDoor4() {
    // this function is called in main if the user chooses 4.
    int action; // STEP 1: a variable to store the user's choice inside Door 4

    // STEP 2: tell the user which door they picked and what they won
    cout << "You chose Door 4" << endl;
    cout << "You win ... A BRAND NEW GAMING COMPUTER!" << endl;

    // STEP 3: show 5 things the user can do with the prize
    cout << "1. Set it up and play video games all night" << endl;
    cout << "2. Use it to learn how to code" << endl;
    cout << "3. Start a streaming channel" << endl;
    cout << "4. Donate it to a local school" << endl;
    cout << "5. Give it to your little brother or sister" << endl;
    cout << "? "; // the prompt

    // STEP 4: read the user's choice from the keyboard
    cin >> action;

    // STEP 5: check which choice was entered and show the result
    if (1 == action) {
        cout << "You are the champion of the leaderboard by sunrise!" << endl;
    }
    else if (2 == action) {
        cout << "You write your first C++ program and land a great career!" << endl;
    }
    else if (3 == action) {
        cout << "Your channel gets 1,000 followers in the first week!" << endl;
    }
    else if (4 == action) {
        cout << "The students are thrilled with their new computer lab!" << endl;
    }
    else if (5 == action) {
        cout << "You are officially the best sibling ever!" << endl;
    }
    else {
        // STEP 6: if the number was not 1-5, tell the user it is invalid
        cout << "I'm sorry, that is not a valid choice." << endl;
    }
}

// If we had a Door #3, or 4, we would add another else if to our
// main(), and then declare and define chooseDoor3() and so on.
