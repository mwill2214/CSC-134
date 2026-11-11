// M6BONUS1 - Using Data Structures
// Text Adventure: "Server Room Blackout" (menu version)
//
// Based on the instructor's starter program (m6_text_adv.cpp).
// Uses regular arrays only (no vectors), as the assignment requires.
//
// AI assistance: Claude (Anthropic).
// Prompt (summary): "Change the room names/descriptions to a network-admin
// theme, add items, an inventory, and a locked door using plain arrays only,
// then add numbered menus with at least 10 options each, where I can exit
// at any time, and add comments explaining what the code does."
//
// Data structure concepts used:
//   1. Enums            - named constants for directions, rooms, and items
//   2. Parallel arrays  - roomNames[], roomDescriptions[], itemNames[],
//                         itemDescriptions[], itemUseText[], itemLocation[]
//                         all share the same index
//   3. Adjacency list   - connections[from_room][direction] = to_room, or -1
//
// MENUS: every menu has at least 10 numbered options.
//   - Option 0 is ALWAYS "Exit game", so you can quit from any menu.
//   - Item menus list all 10 items plus "Back to main menu".

#include <iostream>
#include <string>
#include <limits>      // numeric_limits, used to clear bad input
using namespace std;

// ---------- Enums: named constants instead of "magic numbers" ----------

// The four compass directions. NUM_DIRECTIONS is automatically 4.
enum Direction { NORTH, EAST, SOUTH, WEST, NUM_DIRECTIONS };
const string DIRECTION_NAMES[NUM_DIRECTIONS] = {"north", "east", "south", "west"};

// The five rooms in the game. NUM_ROOMS is automatically 5.
enum Room { LOBBY, SERVER_ROOM, BREAK_ROOM, ROOF, NETWORK_CLOSET, NUM_ROOMS };

// The ten items in the game. NUM_ITEMS is automatically 10.
enum Item {
    KEYCARD, CABLE, COFFEE, FLASHLIGHT, LAPTOP,
    USB_DRIVE, SCREWDRIVER, NOTEBOOK, CABLE_TESTER, HEADSET,
    NUM_ITEMS
};

// Special values stored in itemLocation[] (any value 0..NUM_ROOMS-1 is a room)
const int IN_INVENTORY = -2;   // the player is carrying the item
const int GONE = -3;           // the item has been used up

// Main menu option numbers (so the code reads like words, not numbers)
const int MENU_EXIT = 0;
const int MENU_NORTH = 1;
const int MENU_EAST = 2;
const int MENU_SOUTH = 3;
const int MENU_WEST = 4;
const int MENU_LOOK = 5;
const int MENU_TAKE = 6;
const int MENU_USE = 7;
const int MENU_INVENTORY = 8;
const int MENU_EXAMINE = 9;
const int MENU_MAP = 10;
const int MENU_HELP = 11;
const int MENU_HINT = 12;
const int MAIN_MENU_MAX = 12;

// Item sub-menu: options 1..10 pick an item, 11 goes back, 0 exits
const int ITEM_MENU_BACK = NUM_ITEMS + 1;   // = 11

// ---------- Helper functions ----------

// Ask the player for a number between 0 and 'high'.
// Keeps asking until the input is valid. Returns 0 (Exit) if input ends,
// so the game can never get stuck in an infinite loop.
int getChoice(int high) {
    int choice;
    while (true) {
        cout << "Enter choice (0 to exit): ";
        if (!(cin >> choice)) {
            if (cin.eof()) return 0;                 // no more input: exit
            cin.clear();                             // reset the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // throw away bad text
            cout << "Please enter a number." << endl;
            continue;
        }
        if (choice >= 0 && choice <= high) return choice;   // valid choice
        cout << "Please choose a number from 0 to " << high << "." << endl;
    }
}

// Print the main menu (13 options including Exit).
void showMainMenu() {
    cout << "\n===== MAIN MENU =====" << endl;
    cout << " 1. Go north" << endl;
    cout << " 2. Go east" << endl;
    cout << " 3. Go south" << endl;
    cout << " 4. Go west" << endl;
    cout << " 5. Look around" << endl;
    cout << " 6. Take an item" << endl;
    cout << " 7. Use an item" << endl;
    cout << " 8. Check inventory" << endl;
    cout << " 9. Examine an item" << endl;
    cout << "10. Show map" << endl;
    cout << "11. Help" << endl;
    cout << "12. Get a hint" << endl;
    cout << " 0. Exit game" << endl;
}

// Print an item sub-menu: all 10 items, then Back (11) and Exit (0).
// 'title' says what the menu is for (Take / Use / Examine).
void showItemMenu(const string title, const string names[]) {
    cout << "\n===== " << title << " WHICH ITEM? =====" << endl;
    for (int i = 0; i < NUM_ITEMS; i++) {
        cout << (i + 1 < 10 ? " " : "") << (i + 1) << ". " << names[i] << endl;
    }
    cout << ITEM_MENU_BACK << ". Back to main menu" << endl;
    cout << " 0. Exit game" << endl;
}

int main() {
    // ---------- Parallel arrays for rooms (index with the Room enum) ----------
    string roomNames[NUM_ROOMS] = {
        "Lobby", "Server Room", "Break Room", "Roof", "Network Closet"
    };
    string roomDescriptions[NUM_ROOMS] = {
        "A dim office lobby. The power is out and the clock is blinking 12:00.",
        "Racks of silent servers. Every status light is dark.",
        "A cramped break room with a humming vending machine.",
        "A windy rooftop covered in antennas and satellite dishes.",
        "A cold closet full of patch panels. One switch has a loose port."
    };

    // ---------- Parallel arrays for items (index with the Item enum) ----------
    string itemNames[NUM_ITEMS] = {
        "keycard", "cable", "coffee", "flashlight", "laptop",
        "usb drive", "screwdriver", "notebook", "cable tester", "headset"
    };
    string itemDescriptions[NUM_ITEMS] = {
        "A keycard labeled 'NETWORK CLOSET'.",
        "A blue Cat6 patch cable.",
        "A lukewarm coffee. Not useful, but morale matters.",
        "A heavy flashlight with fresh batteries.",
        "A laptop with 3% battery left.",
        "A USB drive labeled 'BACKUPS - DO NOT LOSE'.",
        "A small Phillips screwdriver.",
        "A notebook full of IP addresses and scribbled passwords.",
        "A handheld tester that checks Ethernet cables.",
        "A call-center headset. Static hisses in the earpiece."
    };
    // What happens when you use each item (the cable is a special case below)
    string itemUseText[NUM_ITEMS] = {
        "You wave the keycard around. It only works on the closet door.",
        "You need a loose network port to plug this into.",
        "You sip the coffee. Slightly better.",
        "You click the flashlight on. The shadows retreat.",
        "The laptop shows 'No network connection'. Of course.",
        "You turn the USB drive over. Nothing to plug it into yet.",
        "You tighten a random screw. Satisfying, but pointless.",
        "You flip through the notebook. Page 12: 'Port 7 is the loose one.'",
        "The tester beeps. Your cable is good!",
        "You put on the headset. You hear only static."
    };
    // Where each item starts: a room number, IN_INVENTORY, or GONE
    int itemLocation[NUM_ITEMS];
    itemLocation[KEYCARD]      = ROOF;
    itemLocation[CABLE]        = SERVER_ROOM;
    itemLocation[COFFEE]       = BREAK_ROOM;
    itemLocation[FLASHLIGHT]   = LOBBY;
    itemLocation[LAPTOP]       = SERVER_ROOM;
    itemLocation[USB_DRIVE]    = ROOF;
    itemLocation[SCREWDRIVER]  = BREAK_ROOM;
    itemLocation[NOTEBOOK]     = LOBBY;
    itemLocation[CABLE_TESTER] = NETWORK_CLOSET;
    itemLocation[HEADSET]      = BREAK_ROOM;

    // ---------- Adjacency list (2D array) ----------
    // connections[from_room][direction] = to_room, or -1 if there is no exit
    int connections[NUM_ROOMS][NUM_DIRECTIONS];

    // Start with no exits anywhere...
    for (int i = 0; i < NUM_ROOMS; i++)
        for (int j = 0; j < NUM_DIRECTIONS; j++)
            connections[i][j] = -1;

    // ...then add each exit (every exit has a matching way back)
    connections[LOBBY][NORTH]          = SERVER_ROOM;
    connections[LOBBY][EAST]           = BREAK_ROOM;
    connections[LOBBY][WEST]           = ROOF;
    connections[SERVER_ROOM][SOUTH]    = LOBBY;
    connections[BREAK_ROOM][WEST]      = LOBBY;
    connections[BREAK_ROOM][SOUTH]     = NETWORK_CLOSET;
    connections[ROOF][EAST]            = LOBBY;
    connections[NETWORK_CLOSET][NORTH] = BREAK_ROOM;

    // ---------- Game state ----------
    int currentRoom = LOBBY;     // the player starts in the Lobby
    bool gameRunning = true;     // set to false to end the game

    cout << "SERVER ROOM BLACKOUT" << endl;
    cout << "The network is down. Find what you need and fix it!" << endl;

    // ---------- Main game loop ----------
    while (gameRunning) {

        // Describe the current room
        cout << "\nYou are in the " << roomNames[currentRoom] << endl;
        cout << roomDescriptions[currentRoom] << endl;

        // List every item whose location equals the current room
        for (int i = 0; i < NUM_ITEMS; i++) {
            if (itemLocation[i] == currentRoom) {
                cout << "You see: " << itemNames[i] << endl;
            }
        }

        // List every direction that has an exit (not -1)
        cout << "Exits: ";
        bool hasExits = false;
        for (int dir = 0; dir < NUM_DIRECTIONS; dir++) {
            if (connections[currentRoom][dir] != -1) {
                cout << DIRECTION_NAMES[dir] << " ";
                hasExits = true;
            }
        }
        if (!hasExits) cout << "none";
        cout << endl;

        // Show the menu and get the player's choice
        showMainMenu();
        int choice = getChoice(MAIN_MENU_MAX);

        // Menus 1-4: movement. Convert the menu number to a Direction.
        if (choice >= MENU_NORTH && choice <= MENU_WEST) {
            int dir = choice - MENU_NORTH;        // 1->NORTH(0) ... 4->WEST(3)
            int next = connections[currentRoom][dir];   // look up the adjacency list

            if (next == -1) {
                cout << "You can't go that way." << endl;
            } else if (next == NETWORK_CLOSET && itemLocation[KEYCARD] != IN_INVENTORY) {
                // Locked door: only open if the keycard is in the inventory
                cout << "The door is locked. It needs a keycard." << endl;
            } else {
                currentRoom = next;               // move to the new room
            }

        // Exit: stop the game loop
        } else if (choice == MENU_EXIT) {
            gameRunning = false;

        // Look: print the room description again
        } else if (choice == MENU_LOOK) {
            cout << roomDescriptions[currentRoom] << endl;

        // Take: choose an item from the item menu
        } else if (choice == MENU_TAKE) {
            showItemMenu("TAKE", itemNames);
            int pick = getChoice(ITEM_MENU_BACK);
            if (pick == 0) {
                gameRunning = false;                       // exit from sub-menu
            } else if (pick != ITEM_MENU_BACK) {
                int item = pick - 1;                       // menu number -> array index
                if (itemLocation[item] == currentRoom) {   // item must be in this room
                    itemLocation[item] = IN_INVENTORY;     // move it to the inventory
                    cout << "Taken: " << itemDescriptions[item] << endl;
                } else {
                    cout << "You don't see that here." << endl;
                }
            }

        // Use: choose an item from the item menu
        } else if (choice == MENU_USE) {
            showItemMenu("USE", itemNames);
            int pick = getChoice(ITEM_MENU_BACK);
            if (pick == 0) {
                gameRunning = false;
            } else if (pick != ITEM_MENU_BACK) {
                int item = pick - 1;
                if (itemLocation[item] != IN_INVENTORY) {
                    cout << "You aren't carrying that." << endl;
                } else if (item == CABLE && currentRoom == NETWORK_CLOSET) {
                    // Winning move: cable used in the Network Closet
                    itemLocation[CABLE] = GONE;
                    cout << "You plug the cable into the loose port. Lights blink on..." << endl;
                    cout << "The network is back online. You win!" << endl;
                    gameRunning = false;
                } else {
                    cout << itemUseText[item] << endl;     // normal use message
                }
            }

        // Inventory: list every item whose location is IN_INVENTORY
        } else if (choice == MENU_INVENTORY) {
            cout << "You are carrying: ";
            bool any = false;
            for (int i = 0; i < NUM_ITEMS; i++) {
                if (itemLocation[i] == IN_INVENTORY) {
                    cout << itemNames[i] << ", ";
                    any = true;
                }
            }
            if (!any) cout << "nothing";
            cout << endl;

        // Examine: read an item's description (must be carried or in this room)
        } else if (choice == MENU_EXAMINE) {
            showItemMenu("EXAMINE", itemNames);
            int pick = getChoice(ITEM_MENU_BACK);
            if (pick == 0) {
                gameRunning = false;
            } else if (pick != ITEM_MENU_BACK) {
                int item = pick - 1;
                if (itemLocation[item] == IN_INVENTORY || itemLocation[item] == currentRoom) {
                    cout << itemDescriptions[item] << endl;
                } else {
                    cout << "You don't see that here." << endl;
                }
            }

        // Map: a simple picture of how the rooms connect
        } else if (choice == MENU_MAP) {
            cout << "\n        [Roof] - [Lobby] - [Break Room]" << endl;
            cout << "                    |            |" << endl;
            cout << "              [Server Room]  [Network Closet]" << endl;
            cout << "(Server Room is north of the Lobby; Closet is south of the Break Room.)" << endl;

        // Help: explain the menus
        } else if (choice == MENU_HELP) {
            cout << "Type the number of a menu option and press Enter." << endl;
            cout << "Option 0 exits the game from any menu." << endl;
            cout << "Goal: get the network cable to the Network Closet and use it." << endl;

        // Hint: the hint changes depending on what the player has done
        } else if (choice == MENU_HINT) {
            if (itemLocation[CABLE] != IN_INVENTORY) {
                cout << "Hint: you need a network cable. Try the Server Room." << endl;
            } else if (itemLocation[KEYCARD] != IN_INVENTORY) {
                cout << "Hint: the Network Closet is locked. Find a keycard." << endl;
            } else {
                cout << "Hint: take the cable to the Network Closet (south of the Break Room)." << endl;
            }
        }
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}