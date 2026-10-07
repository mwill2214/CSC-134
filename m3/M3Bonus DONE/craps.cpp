// CSC 134
// M3BONUS - Craps game (AI assisted development)
// Uses: if statements, while loops, functions, random numbers

#include <iostream>
#include <string>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <limits>    // numeric_limits
using namespace std;

// Function prototypes
int roll_die();
int roll_two_dice();
int get_number(string prompt);
void show_rules();
bool play_round();

int bankroll = 100;   // player's money (global so every function can use it)

int main() {
    int choice;
    bool keep_going = true;

    srand(time(0));   // seed the random number generator ONCE

    while (keep_going) {
        cout << endl;
        cout << "=== CRAPS ===" << endl;
        cout << "Bankroll: $" << bankroll << endl;
        cout << "1. Play a round" << endl;
        cout << "2. Rules" << endl;
        cout << "0. Exit" << endl;

        choice = get_number("Choice: ");

        if (1 == choice) {
            play_round();
            if (bankroll <= 0) {
                cout << "You're out of money. Game over!" << endl;
                keep_going = false;
            }
        }
        else if (2 == choice) {
            show_rules();
        }
        else if (0 == choice) {
            cout << "You leave with $" << bankroll << ". Bye!" << endl;
            keep_going = false;
        }
        else {
            cout << "Not a valid choice." << endl;
        }
    }
    return 0;
}

// Roll one six-sided die (1-6)
int roll_die() {
    return (rand() % 6) + 1;
}

// Roll two dice and show them
int roll_two_dice() {
    int d1 = roll_die();
    int d2 = roll_die();
    int total = d1 + d2;
    cout << "You rolled " << d1 << " + " << d2 << " = " << total << endl;
    return total;
}

// Ask for a number; keep asking until the user types a real number
int get_number(string prompt) {
    int number;
    while (true) {
        cout << prompt;
        if (cin >> number) {
            return number;
        }
        cin.clear();                                          // reset error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // throw away bad input
        cout << "Please enter a number." << endl;
    }
}

void show_rules() {
    cout << endl;
    cout << "RULES" << endl;
    cout << "First roll (the come-out roll):" << endl;
    cout << "  7 or 11    -> you win" << endl;
    cout << "  2, 3, or 12 -> you lose (craps)" << endl;
    cout << "  anything else becomes your POINT" << endl;
    cout << "Then keep rolling:" << endl;
    cout << "  roll your point again -> you win" << endl;
    cout << "  roll a 7              -> you lose" << endl;
    cout << "Win = gain your bet. Lose = lose your bet." << endl;
}

// Plays one full round. Returns true if the player won.
bool play_round() {
    int bet;
    int roll;
    int point;
    bool won;

    // Loop until the bet is valid
    bet = get_number("Enter your bet: $");
    while (bet < 1 || bet > bankroll) {
        cout << "Bet must be between $1 and $" << bankroll << "." << endl;
        bet = get_number("Enter your bet: $");
    }

    cout << "Press Enter to roll..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    // Come-out roll
    roll = roll_two_dice();

    if (7 == roll || 11 == roll) {
        cout << "Natural! You win!" << endl;
        won = true;
    }
    else if (2 == roll || 3 == roll || 12 == roll) {
        cout << "Craps! You lose." << endl;
        won = false;
    }
    else {
        point = roll;
        cout << "Your point is " << point << ". Roll it again before a 7!" << endl;

        // Keep rolling until point (win) or 7 (lose)
        while (true) {
            cout << "Press Enter to roll..." << endl;
            cin.get();
            roll = roll_two_dice();

            if (roll == point) {
                cout << "You made your point! You win!" << endl;
                won = true;
                break;
            }
            else if (7 == roll) {
                cout << "Seven out! You lose." << endl;
                won = false;
                break;
            }
            else {
                cout << "Roll again..." << endl;
            }
        }
    }

    // Update money
    if (won) {
        bankroll = bankroll + bet;
    }
    else {
        bankroll = bankroll - bet;
    }
    return won;
}
