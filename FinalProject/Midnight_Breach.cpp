// Midnight Breach - Choose Your Own Adventure (CSC 134)
// You are a security analyst. An intruder is stealing data from the company
// network. Collect the laptop, the admin token, and the firewall disk, then
// lock the intruder out from the Server Room before you run out of moves.
//
// Concepts used: arrays (1D, parallel, 2D), loops (while/for), if/else,
// functions, enums, constants, strings, input validation, ASCII graphics.

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

// ---------- Constants and enums ----------
enum Direction { NORTH, EAST, SOUTH, WEST, NUM_DIRECTIONS };

enum Room { SECURITY_OFFICE, SERVER_ROOM, NETWORK_CLOSET, BREAK_ROOM,
            ARCHIVE, EXEC_OFFICE, LOBBY, GARAGE, ROOF, VAULT,
            CONF_ROOM, HELP_DESK, NUM_ROOMS };                    // 12 rooms

enum Item { BADGE, LAPTOP, TOKEN, FIREWALL_DISK, VAULT_KEY, PRINTOUT,
            USB_DRIVE, FLASHLIGHT, HEADSET, COFFEE, ENERGY_DRINK,
            SKATEBOARD, MAP, STICKY_NOTE, NUM_ITEMS };            // 14 items

const int GRID_ROWS = 3;
const int GRID_COLS = 4;
const int IN_INVENTORY = -2;   // item is being carried
const int GONE = -3;           // item was used up
const int NO_EXIT = -1;        // no door in that direction
const int START_MOVES = 40;    // moves before the breach completes

const string DIR_NAMES[NUM_DIRECTIONS] = {"north", "east", "south", "west"};

// ---------- ASCII pictures (one per room, same order as the Room enum) ----------
const string ROOM_ART[NUM_ROOMS] = {
R"ART(   .-------. .-------. .-------.
   | ALERT | | ALERT | |  OK   |
   '-------' '-------' '-------'
  ___|_________|_________|______
 |_____________ DESK ___________|)ART",

R"ART(  _______  _______  _______
 |:::::::||:::::::||:::::::|
 |o o o o||o o o o||o o o o|
 |:::::::||:::::::||:::::::|
 |_______||_______||_______|  >>> DATA LEAK >>>)ART",

R"ART(  ___________________________
 |==[]==[]==[]==[]==[]==[]==|
 | o  o  o  o  o  o  o  o  |
 |~~~~~~~~~~~~~~~~~~~~~~~~~|
 |_________SWITCHES________|)ART",

R"ART(   .--------.      _______
   | SODA   |     |  ___  |
   | [====] |     | (___) |
   '--------'    _|_______|_
   VENDING      |   TABLE   |)ART",

R"ART(  ______ ______ ______ ______
 |______|______|______|______|
 | BOX  | BOX  | BOX  | BOX  |
 |______|______|______|______|
 |______|______|______|______|)ART",

R"ART(   _________________
  |   [ CEO  PC ]   |
  |  _____________  |
  |_|_____________|_|
       (  chair  ))ART",

R"ART(   _________________________
  |   W E L C O M E        |
  |   _______   _______    |
  |  |   |   | |   |   |   |
  |__|___|___|_|___|___|___|)ART",

R"ART(   __________________________
  |  P      ___      ___     |
  |  .-----'___'----'___'--. |
  |  O--O  (car)    (car)    |
  |__________________________|   (dark))ART",

R"ART(        |            .-.
       -+-          ( ) satellite
        |            '-'
   _____|______________|_____
  |         R O O F          |)ART",

R"ART(   .---------------.
   |    .-----.    |
   |   ( [ + ] )   |
   |    '-----'    |
   '---------------'  V A U L T)ART",

R"ART(   __________________________
  |  o   o   o   o   o   o   |
  |  ======================  |
  |  o   o   o   o   o   o   |
  |__________________________|)ART",

R"ART(    ___________________
   |  H E L P   D E S K |
   |  ( )   ( )   ( )   |
   |____|_____|_____|___|
        headsets)ART"
};

const string TITLE_ART = R"ART(
 =====================================
  __  __ _     _       _       _
 |  \/  (_) __| |_ __ (_) __ _| |__ | |_
 | |\/| | |/ _` | '_ \| |/ _` | '_ \| __|
 | |  | | | (_| | | | | | (_| | | | | |_
 |_|  |_|_|\__,_|_| |_|_|\__, |_| |_|\__|
        B R E A C H      |___/
 =====================================)ART";

const string WIN_ART = R"ART(
    ***************************
    *   S Y S T E M   S A F E  *
    *   INTRUDER LOCKED OUT    *
    ***************************
          \o/  Great work!
           |
          / \)ART";

const string LOSE_ART = R"ART(
    !!!!!!!!!!!!!!!!!!!!!!!!!!!
    !   D A T A   S T O L E N  !
    !   THE BREACH IS COMPLETE !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!)ART";

// One-line icon for each item (same order as the Item enum)
const string ITEM_ICON[NUM_ITEMS] = {
    "[ID|badge]", "[=laptop=]", "(token)", "(@ disk @)", "-o|key",
    "[~mail~]", "[usb]=", "(|flashlight>", "(headset)", "c[_]coffee",
    "[energy!]", "o==skate==o", "[~map~]", "[note*]"
};

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

// Print the item list in two columns; returns the chosen item index,
// or -1 for "back".
int pickItem(const string names[]) {
    cout << "\nWhich item?" << endl;
    for (int i = 0; i < NUM_ITEMS; i++) {
        cout << left << setw(4) << (to_string(i + 1) + ".")
             << setw(20) << names[i];
        if (i % 2 == 1) cout << endl;            // two items per row
    }
    cout << "0.  Back" << endl;
    return getChoice(NUM_ITEMS) - 1;             // -1 means Back
}

// Draw the whole building from the grid. * marks the player.
void showMap(const string roomName[], const int grid[GRID_ROWS][GRID_COLS], int current) {
    cout << "\n=== BUILDING MAP (* = you) ===" << endl;
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            int room = grid[r][c];
            cout << (room == current ? "*" : " ") << "["
                 << left << setw(16) << roomName[room] << "]-";
        }
        cout << endl;
        if (r < GRID_ROWS - 1) {                 // vertical connectors
            for (int c = 0; c < GRID_COLS; c++)
                cout << string(10, ' ') << "|" << string(9, ' ');
            cout << endl;
        }
    }
    cout << "(Closet needs the badge. Vault needs the vault key.)" << endl;
}

int main() {
    // ---------- Parallel arrays: rooms ----------
    string roomName[NUM_ROOMS] = {
        "Security Office", "Server Room", "Network Closet", "Break Room",
        "Archive", "Executive Office", "Lobby", "Parking Garage",
        "Roof", "Vault", "Conference Room", "Help Desk"
    };
    string roomDesc[NUM_ROOMS] = {
        "Monitors flash red: UNAUTHORIZED LOGIN DETECTED.",
        "Rows of servers. One terminal is streaming data to an unknown address.",
        "A cramped closet of switches and cables. A safe sits open.",
        "A quiet break room. A vending machine hums in the corner.",
        "Dusty shelves of old records and unused equipment.",
        "A plush office. The CEO's computer is still logged in.",
        "A marble lobby. The front desk is empty.",
        "A concrete garage with flickering lights.",
        "A windy rooftop covered in antennas and satellite dishes.",
        "A steel vault. Backup disks sit in numbered slots.",
        "A long table with chairs pushed back in a hurry.",
        "Headsets and half-finished tickets cover the desks."
    };

    // ---------- Parallel arrays: items ----------
    string itemName[NUM_ITEMS] = {
        "access badge", "laptop", "admin token", "firewall disk",
        "vault key", "phishing printout", "usb drive", "flashlight",
        "headset", "coffee", "energy drink", "skateboard", "building map",
        "sticky note"
    };
    string itemDesc[NUM_ITEMS] = {
        "A badge marked 'NETWORK CLOSET ACCESS'.",
        "A security laptop with incident-response tools installed.",
        "A hardware token that holds the admin lockout key.",
        "A disk labeled 'FIREWALL RULES - RESTORE'.",
        "A heavy steel key stamped 'VAULT'.",
        "A printed email: 'Urgent! Reset your password at this link.'",
        "A USB drive that can save the server activity logs.",
        "A heavy flashlight with fresh batteries.",
        "A headset. Faint IT chatter plays in the earpiece.",
        "A hot coffee. It might buy you some time.",
        "A cold energy drink. Big boost, short crash.",
        "A skateboard. Every other move is free while you carry it.",
        "A folded map of the whole building.",
        "A sticky note in messy handwriting."
    };
    int itemLoc[NUM_ITEMS] = {
        ARCHIVE, SECURITY_OFFICE, NETWORK_CLOSET, VAULT,
        GARAGE, BREAK_ROOM, EXEC_OFFICE, HELP_DESK,
        HELP_DESK, BREAK_ROOM, ROOF, LOBBY, CONF_ROOM,
        CONF_ROOM
    };

    // ---------- 2D array: the building layout (3 rows x 4 columns) ----------
    const int grid[GRID_ROWS][GRID_COLS] = {
        { ARCHIVE,   EXEC_OFFICE,     CONF_ROOM,   ROOF           },
        { HELP_DESK, SECURITY_OFFICE, SERVER_ROOM, NETWORK_CLOSET },
        { GARAGE,    LOBBY,           BREAK_ROOM,  VAULT          }
    };

    // ---------- 2D array: exits[room][direction] built from the grid ----------
    int exits[NUM_ROOMS][NUM_DIRECTIONS];
    for (int r = 0; r < NUM_ROOMS; r++)
        for (int d = 0; d < NUM_DIRECTIONS; d++)
            exits[r][d] = NO_EXIT;

    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            int room = grid[r][c];
            if (r > 0)             exits[room][NORTH] = grid[r - 1][c];
            if (c < GRID_COLS - 1) exits[room][EAST]  = grid[r][c + 1];
            if (r < GRID_ROWS - 1) exits[room][SOUTH] = grid[r + 1][c];
            if (c > 0)             exits[room][WEST]  = grid[r][c - 1];
        }
    }

    // ---------- Game state ----------
    int current = SECURITY_OFFICE;
    int movesLeft = START_MOVES;
    int stepCount = 0;                // counts moves (used by the skateboard)
    int score = 0;
    bool logsSaved = false;
    bool playing = true, won = false, newRoom = true;

    cout << TITLE_ART << endl;
    cout << "\nAn intruder is stealing company data. Stop them before time runs out!" << endl;
    cout << "Goal: get the laptop, admin token, and firewall disk to the Server Room." << endl;

    // ---------- Main game loop ----------
    while (playing && movesLeft > 0) {
        bool hasLight = (itemLoc[FLASHLIGHT] == IN_INVENTORY);

        cout << "\n--- " << roomName[current] << " (moves left: "
             << movesLeft << " | score: " << score << ") ---" << endl;
        if (newRoom) {                               // draw the picture on entry
            cout << ROOM_ART[current] << endl;
            newRoom = false;
        }
        cout << roomDesc[current] << endl;

        if (current == GARAGE && !hasLight) {
            cout << "It is pitch dark. You can't see any items without a light." << endl;
        } else {
            for (int i = 0; i < NUM_ITEMS; i++)      // show items in this room
                if (itemLoc[i] == current)
                    cout << "You see: " << itemName[i] << "  " << ITEM_ICON[i] << endl;
        }

        cout << "Exits:";
        for (int d = 0; d < NUM_DIRECTIONS; d++)     // show available exits
            if (exits[current][d] != NO_EXIT)
                cout << " " << DIR_NAMES[d];
        cout << endl;

        if (movesLeft <= 5)       cout << "!! CRITICAL: transfer 95% complete !!" << endl;
        else if (movesLeft <= 12) cout << "WARNING: intruder is copying more data..." << endl;

        cout << "\n1.North 2.East 3.South 4.West 5.Look 6.Take 7.Use" << endl;
        cout << "8.Inventory 9.Examine 10.Hint 0.Quit" << endl;
        int choice = getChoice(10);

        if (choice == 0) {
            playing = false;

        } else if (choice >= 1 && choice <= 4) {             // movement
            int next = exits[current][choice - 1];
            if (next == NO_EXIT) {
                cout << "You can't go that way." << endl;
            } else if (next == NETWORK_CLOSET && itemLoc[BADGE] != IN_INVENTORY) {
                cout << "The closet door is locked. It needs an access badge." << endl;
            } else if (next == VAULT && itemLoc[VAULT_KEY] != IN_INVENTORY) {
                cout << "The vault is locked. It needs a vault key." << endl;
            } else {
                current = next;
                newRoom = true;
                stepCount++;
                if (itemLoc[SKATEBOARD] == IN_INVENTORY && stepCount % 2 == 0)
                    cout << "You glide on the skateboard. That move was free!" << endl;
                else
                    movesLeft--;
            }

        } else if (choice == 5) {                             // look
            cout << ROOM_ART[current] << endl;

        } else if (choice == 6) {                             // take
            int item = pickItem(itemName);
            if (item >= 0) {
                if (current == GARAGE && !hasLight) {
                    cout << "It's too dark to find anything." << endl;
                } else if (itemLoc[item] == current) {
                    itemLoc[item] = IN_INVENTORY;
                    score += 5;
                    cout << "Taken: " << itemName[item] << "  " << ITEM_ICON[item] << endl;
                } else {
                    cout << "You don't see that here." << endl;
                }
            }

        } else if (choice == 7) {                             // use
            int item = pickItem(itemName);
            if (item >= 0) {
                if (itemLoc[item] != IN_INVENTORY) {
                    cout << "You aren't carrying that." << endl;
                } else if (item == LAPTOP) {
                    if (current != SERVER_ROOM) {
                        cout << "The laptop shows 'No secure connection'. Try the Server Room." << endl;
                    } else if (itemLoc[TOKEN] != IN_INVENTORY) {
                        cout << "The laptop asks for an admin token." << endl;
                    } else if (itemLoc[FIREWALL_DISK] != IN_INVENTORY) {
                        cout << "The laptop asks for the firewall disk." << endl;
                    } else {
                        cout << "You plug in the token, load the firewall disk," << endl;
                        cout << "and run the lockout script..." << endl;
                        score += 50;
                        won = true;
                        playing = false;
                    }
                } else if (item == USB_DRIVE) {
                    if (current == SERVER_ROOM && !logsSaved) {
                        logsSaved = true;
                        score += 20;
                        cout << "You save the server logs as evidence. +20 points." << endl;
                    } else {
                        cout << "Nothing to copy here. Try the Server Room." << endl;
                    }
                } else if (item == COFFEE) {
                    itemLoc[COFFEE] = GONE;
                    movesLeft += 3;
                    cout << "You feel sharper. +3 moves." << endl;
                } else if (item == ENERGY_DRINK) {
                    itemLoc[ENERGY_DRINK] = GONE;
                    movesLeft += 5;
                    cout << "Zoom! +5 moves." << endl;
                } else if (item == MAP) {
                    showMap(roomName, grid, current);
                } else if (item == PRINTOUT) {
                    cout << "The email came from the CEO's account. The attacker has CEO credentials." << endl;
                } else if (item == HEADSET) {
                    cout << "IT chatter: 'The firewall disk is locked in the Vault.'" << endl;
                } else if (item == STICKY_NOTE) {
                    cout << "Note: 'Vault key is in the Parking Garage. Bring a light!'" << endl;
                } else if (item == FLASHLIGHT) {
                    cout << "You click on the flashlight. The shadows retreat." << endl;
                } else if (item == SKATEBOARD) {
                    cout << "Just keep it with you. Every other move will be free." << endl;
                } else {
                    cout << "Nothing happens. Maybe it works somewhere else." << endl;
                }
            }

        } else if (choice == 8) {                             // inventory
            cout << "You carry:" << endl;
            bool any = false;
            for (int i = 0; i < NUM_ITEMS; i++)
                if (itemLoc[i] == IN_INVENTORY) {
                    cout << "  " << left << setw(20) << itemName[i] << ITEM_ICON[i] << endl;
                    any = true;
                }
            if (!any) cout << "  nothing" << endl;

        } else if (choice == 9) {                             // examine
            int item = pickItem(itemName);
            if (item >= 0) {
                if (itemLoc[item] == IN_INVENTORY || itemLoc[item] == current)
                    cout << ITEM_ICON[item] << "  " << itemDesc[item] << endl;
                else
                    cout << "You don't see that here." << endl;
            }

        } else if (choice == 10) {                            // hint
            if (itemLoc[LAPTOP] != IN_INVENTORY)
                cout << "Hint: you need the laptop from the Security Office." << endl;
            else if (itemLoc[TOKEN] != IN_INVENTORY && itemLoc[BADGE] != IN_INVENTORY)
                cout << "Hint: find an access badge in the Archive (northwest corner)." << endl;
            else if (itemLoc[TOKEN] != IN_INVENTORY)
                cout << "Hint: the token is in the Network Closet, east of the Server Room." << endl;
            else if (itemLoc[FIREWALL_DISK] != IN_INVENTORY && itemLoc[VAULT_KEY] != IN_INVENTORY)
                cout << "Hint: the firewall disk is in the Vault (southeast corner)." << endl;
            else if (itemLoc[FIREWALL_DISK] != IN_INVENTORY && itemLoc[FLASHLIGHT] != IN_INVENTORY)
                cout << "Hint: the Garage is dark. Get a flashlight from the Help Desk." << endl;
            else if (itemLoc[FIREWALL_DISK] != IN_INVENTORY)
                cout << "Hint: the vault key is in the Parking Garage (southwest corner)." << endl;
            else
                cout << "Hint: use the laptop in the Server Room." << endl;
        }
    }

    // ---------- Ending ----------
    if (won) {
        cout << WIN_ART << endl;
        int bonus = movesLeft * 2;                   // reward saving moves
        score += bonus;
        cout << "\nMoves left: " << movesLeft << " (bonus +" << bonus << ")" << endl;
        string rank = "Junior Analyst";
        if (movesLeft >= 20)      rank = "Elite Analyst";
        else if (movesLeft >= 10) rank = "Senior Analyst";
        cout << "Rank: " << rank << endl;
    } else if (movesLeft <= 0 && playing) {
        cout << LOSE_ART << endl;
    }
    cout << "Final score: " << score << endl;
    cout << "Thanks for playing!" << endl;
    return 0;
}