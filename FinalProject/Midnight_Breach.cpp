// Midnight Breach - Choose Your Own Adventure (CSC 134)
// You are a security analyst. An intruder is stealing data from the company
// network. Find the laptop and the admin token, then lock the intruder out
// from the Server Room before you run out of moves.
//
// Concepts used: arrays, loops (while/for), if/else, functions, enums,
// constants, strings, input validation.

#include <iostream>
#include <string>
#include <limits>
using namespace std;

// ---------- Constants and enums ----------
enum Direction { NORTH, EAST, SOUTH, WEST, NUM_DIRECTIONS };
enum Room { SECURITY_OFFICE, SERVER_ROOM, NETWORK_CLOSET, BREAK_ROOM,
            ARCHIVE, EXEC_OFFICE, NUM_ROOMS };
enum Item { BADGE, LAPTOP, TOKEN, PRINTOUT, USB_DRIVE, FLASHLIGHT, COFFEE,
            NUM_ITEMS };

const int IN_INVENTORY = -2;   // item is being carried
const int NO_EXIT = -1;        // no door in that direction
const int START_MOVES = 25;    // moves before the breach completes

const string DIR_NAMES[NUM_DIRECTIONS] = {"north", "east", "south", "west"};

// ---------- Functions ----------

// Keep asking until the player enters a whole number from 0 to high.
int getChoice(int high) {
    int choice;
    while (true) {
        cout << "Choice: ";
        if (!(cin >> choice)) {
            if (cin.eof()) return 0;             // input ended: quit safely
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number." << endl;
        } else if (choice < 0 || choice > high) {
            cout << "Enter a number from 0 to " << high << "." << endl;
        } else {
            return choice;
        }
    }
}

// Print the item list; returns the chosen item index, or -1 for "back".
int pickItem(const string names[]) {
    cout << "\nWhich item?" << endl;
    for (int i = 0; i < NUM_ITEMS; i++)
        cout << "  " << i + 1 << ". " << names[i] << endl;
    cout << "  0. Back" << endl;
    return getChoice(NUM_ITEMS) - 1;             // -1 means Back
}

int main() {
    // ---------- Parallel arrays: rooms ----------
    string roomName[NUM_ROOMS] = {
        "Security Office", "Server Room", "Network Closet",
        "Break Room", "Archive", "Executive Office"
    };
    string roomDesc[NUM_ROOMS] = {
        "Monitors flash red: UNAUTHORIZED LOGIN DETECTED.",
        "Rows of servers. One terminal is streaming data to unknown address.",
        "A cramped closet of switches and cables. A locked safe sits open.",
        "A quiet break room. A vending machine hums in the corner.",
        "Dusty shelves of old records and unused equipment.",
        "A plush office. The CEO's computer is still logged in."
    };

    // ---------- Parallel arrays: items ----------
    string itemName[NUM_ITEMS] = {
        "access badge", "laptop", "admin token", "phishing printout",
        "usb drive", "flashlight", "coffee"
    };
    string itemDesc[NUM_ITEMS] = {
        "A badge marked 'NETWORK CLOSET ACCESS'.",
        "A security laptop with the incident-response tools installed.",
        "A hardware token that holds the admin lockout key.",
        "A printed email: 'Urgent! Reset your password at this link.'",
        "A USB drive that holds the server activity logs.",
        "A heavy flashlight with fresh batteries.",
        "A hot coffee. It might buy you some time."
    };
    int itemLoc[NUM_ITEMS] = {
        ARCHIVE, SECURITY_OFFICE, NETWORK_CLOSET, BREAK_ROOM,
        EXEC_OFFICE, ARCHIVE, BREAK_ROOM
    };

    // ---------- 2D array: which room is in each direction ----------
    int exits[NUM_ROOMS][NUM_DIRECTIONS];
    for (int r = 0; r < NUM_ROOMS; r++)
        for (int d = 0; d < NUM_DIRECTIONS; d++)
            exits[r][d] = NO_EXIT;

    exits[SECURITY_OFFICE][NORTH] = SERVER_ROOM;
    exits[SECURITY_OFFICE][EAST]  = BREAK_ROOM;
    exits[SECURITY_OFFICE][WEST]  = ARCHIVE;
    exits[SERVER_ROOM][SOUTH]     = SECURITY_OFFICE;
    exits[SERVER_ROOM][EAST]      = NETWORK_CLOSET;
    exits[NETWORK_CLOSET][WEST]   = SERVER_ROOM;
    exits[BREAK_ROOM][WEST]       = SECURITY_OFFICE;
    exits[BREAK_ROOM][NORTH]      = EXEC_OFFICE;
    exits[ARCHIVE][EAST]          = SECURITY_OFFICE;
    exits[EXEC_OFFICE][SOUTH]     = BREAK_ROOM;

    // ---------- Game state ----------
    int current = SECURITY_OFFICE;
    int movesLeft = START_MOVES;
    int score = 0;
    bool coffeeUsed = false, logsBacked = false;
    bool playing = true, won = false;

    cout << "=== MIDNIGHT BREACH ===" << endl;
    cout << "An intruder is stealing company data. Stop them before time runs out!" << endl;

    // ---------- Main game loop ----------
    while (playing && movesLeft > 0) {
        cout << "\n--- " << roomName[current] << " (moves left: "
             << movesLeft << ") ---" << endl;
        cout << roomDesc[current] << endl;

        for (int i = 0; i < NUM_ITEMS; i++)          // show items in this room
            if (itemLoc[i] == current)
                cout << "You see: " << itemName[i] << endl;

        cout << "Exits:";
        for (int d = 0; d < NUM_DIRECTIONS; d++)     // show available exits
            if (exits[current][d] != NO_EXIT)
                cout << " " << DIR_NAMES[d];
        cout << endl;

        if (movesLeft <= 5) cout << "WARNING: Data transfer almost complete!" << endl;

        cout << "\n1.North 2.East 3.South 4.West 5.Look 6.Take 7.Use" << endl;
        cout << "8.Inventory 9.Examine 10.Hint 0.Quit" << endl;
        int choice = getChoice(10);

        if (choice == 0) {
            playing = false;

        } else if (choice >= 1 && choice <= 4) {             // movement
            int dir = choice - 1;
            int next = exits[current][dir];
            if (next == NO_EXIT) {
                cout << "You can't go that way." << endl;
            } else if (next == NETWORK_CLOSET && itemLoc[BADGE] != IN_INVENTORY) {
                cout << "The closet door is locked. It needs an access badge." << endl;
            } else {
                current = next;
                movesLeft--;                                  // only moving costs time
            }

        } else if (choice == 5) {                             // look
            cout << roomDesc[current] << endl;

        } else if (choice == 6) {                             // take
            int item = pickItem(itemName);
            if (item >= 0) {
                if (itemLoc[item] == current) {
                    itemLoc[item] = IN_INVENTORY;
                    score += 5;
                    cout << "Taken: " << itemName[item] << endl;
                } else {
                    cout << "You don't see that here." << endl;
                }
            }

        } else if (choice == 7) {                             // use
            int item = pickItem(itemName);
            if (item >= 0) {
                if (itemLoc[item] != IN_INVENTORY) {
                    cout << "You aren't carrying that." << endl;
                } else if (item == LAPTOP && current == SERVER_ROOM) {
                    if (itemLoc[TOKEN] == IN_INVENTORY) {
                        cout << "You plug in the token and run the lockout script..." << endl;
                        cout << "The intruder is locked out. Network secured!" << endl;
                        score += 50;
                        won = true;
                        playing = false;
                    } else {
                        cout << "The laptop asks for an admin token." << endl;
                    }
                } else if (item == USB_DRIVE && current == SERVER_ROOM && !logsBacked) {
                    logsBacked = true;
                    score += 20;
                    cout << "You save the server logs as evidence. +20 points." << endl;
                } else if (item == COFFEE && !coffeeUsed) {
                    coffeeUsed = true;
                    movesLeft += 3;
                    cout << "You feel sharper. +3 moves." << endl;
                } else if (item == PRINTOUT) {
                    cout << "The email came from the CEO's account. Check the Network Closet for a token." << endl;
                } else if (item == FLASHLIGHT) {
                    cout << "You click on the flashlight. Nothing hidden here." << endl;
                } else {
                    cout << "Nothing happens." << endl;
                }
            }

        } else if (choice == 8) {                             // inventory
            cout << "You carry:";
            bool any = false;
            for (int i = 0; i < NUM_ITEMS; i++)
                if (itemLoc[i] == IN_INVENTORY) {
                    cout << " [" << itemName[i] << "]";
                    any = true;
                }
            if (!any) cout << " nothing";
            cout << endl;

        } else if (choice == 9) {                             // examine
            int item = pickItem(itemName);
            if (item >= 0) {
                if (itemLoc[item] == IN_INVENTORY || itemLoc[item] == current)
                    cout << itemDesc[item] << endl;
                else
                    cout << "You don't see that here." << endl;
            }

        } else if (choice == 10) {                            // hint
            if (itemLoc[LAPTOP] != IN_INVENTORY)
                cout << "Hint: you need the laptop from the Security Office." << endl;
            else if (itemLoc[TOKEN] != IN_INVENTORY && itemLoc[BADGE] != IN_INVENTORY)
                cout << "Hint: find an access badge in the Archive." << endl;
            else if (itemLoc[TOKEN] != IN_INVENTORY)
                cout << "Hint: the token is in the Network Closet, east of the Server Room." << endl;
            else
                cout << "Hint: use the laptop in the Server Room." << endl;
        }
    }

    // ---------- Ending ----------
    if (!won && movesLeft <= 0 && playing)
        cout << "\nTime's up. The data has been stolen. GAME OVER." << endl;
    cout << "Final score: " << score << endl;
    cout << "Thanks for playing!" << endl;
    return 0;
}