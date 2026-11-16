// M6HW1 - Project Prototype (GOLD level)
// Text Adventure: "Server Room Blackout"
//
// Based on the M6BONUS1 text adventure (instructor's starter m6_text_adv.cpp).
// Uses regular arrays only (no vectors).
//
// AI assistance: Claude (Anthropic). Prompts are in M6HW1_lastname.txt.
//
// REQUIREMENTS CHECKLIST
//   Bronze: 5 rooms with exits between them (Lobby, Server Room, Break Room,
//           Roof, Network Closet).
//   Silver: Dialog explaining the point of the game (intro, wall signs, and an
//           NPC named Priya) plus item interaction (take, use, examine).
//   Gold:   An ending (win by fixing the network, or quit early) and a
//           lock-and-key situation: the Network Closet door is locked and
//           the keycard (found on the Roof) opens it.
//
// Data structure concepts used:
//   1. Enums            - named constants for directions, rooms, and items
//   2. Parallel arrays  - roomNames[], roomDescriptions[], itemNames[], ...
//   3. Adjacency list   - connections[from_room][direction] = to_room, or -1
//
// MENUS: option 0 is ALWAYS "Exit game". Menus print horizontally.

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

// ---------- Enums ----------
enum Direction { NORTH, EAST, SOUTH, WEST, NUM_DIRECTIONS };
const string DIRECTION_NAMES[NUM_DIRECTIONS] = {"north", "east", "south", "west"};

enum Room { LOBBY, SERVER_ROOM, BREAK_ROOM, ROOF, NETWORK_CLOSET, NUM_ROOMS };

enum Item {
    KEYCARD, CABLE, COFFEE, FLASHLIGHT, LAPTOP,
    USB_DRIVE, SCREWDRIVER, NOTEBOOK, CABLE_TESTER, HEADSET,
    NUM_ITEMS
};

// Special values stored in itemLocation[] (0..NUM_ROOMS-1 means "in that room")
const int IN_INVENTORY = -2;
const int GONE = -3;

// Main menu option numbers
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
const int MENU_TALK = 13;      // NEW: talk to an NPC
const int MENU_READ = 14;      // NEW: read a sign
const int MAIN_MENU_MAX = 14;

const int ITEM_MENU_BACK = NUM_ITEMS + 1;   // = 11

// ---------- Helper functions ----------

// Ask for a number from 0 to 'high'. Returns 0 (Exit) if input ends.
int getChoice(int high) {
    int choice;
    while (true) {
        cout << "Enter choice (0 to exit): ";
        if (!(cin >> choice)) {
            if (cin.eof()) return 0;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number." << endl;
            continue;
        }
        if (choice >= 0 && choice <= high) return choice;
        cout << "Please choose a number from 0 to " << high << "." << endl;
    }
}

// Print one menu option as "number. label", padded so columns line up.
void printOption(int number, const string label, int width) {
    cout << left << setw(width) << (to_string(number) + ". " + label);
}

// Main menu, printed horizontally (15 options including Exit).
void showMainMenu() {
    cout << "\n===== MAIN MENU =====" << endl;
    printOption(1, "North", 16);
    printOption(2, "East", 16);
    printOption(3, "South", 16);
    printOption(4, "West", 16);
    printOption(5, "Look", 16);
    cout << endl;
    printOption(6, "Take", 16);
    printOption(7, "Use", 16);
    printOption(8, "Inventory", 16);
    printOption(9, "Examine", 16);
    printOption(10, "Map", 16);
    cout << endl;
    printOption(11, "Help", 16);
    printOption(12, "Hint", 16);
    printOption(13, "Talk", 16);
    printOption(14, "Read sign", 16);
    printOption(0, "EXIT", 16);
    cout << endl;
}

// Item sub-menu: all 10 items, then Back (11) and Exit (0).
void showItemMenu(const string title, const string names[]) {
    cout << "\n===== " << title << " WHICH ITEM? =====" << endl;
    for (int i = 0; i < NUM_ITEMS; i++) {
        printOption(i + 1, names[i], 20);
        if ((i + 1) % 4 == 0) cout << endl;
    }
    printOption(ITEM_MENU_BACK, "BACK", 20);
    printOption(0, "EXIT", 20);
    cout << endl;
}

// ---------- NEW: story / dialog functions (void functions) ----------

// Opening dialog: explains the point of the game.
void showIntro() {
    cout << "=====================================" << endl;
    cout << "        SERVER ROOM BLACKOUT" << endl;
    cout << "=====================================" << endl;
    cout << "A storm knocked out the power at Fayetteville Tech Systems." << endl;
    cout << "You are the night-shift network admin, and the whole network is down." << endl;
    cout << "YOUR GOAL: get a network cable to the Network Closet and plug it" << endl;
    cout << "into the loose switch port. But the closet door is locked..." << endl;
    cout << "Explore, talk to people, read signs, and collect useful items." << endl;
}

// Read the sign in the current room (not every room has one).
void readSign(int currentRoom) {
    if (currentRoom == LOBBY) {
        cout << "SIGN: 'OUTAGE NOTICE - Restore the network by patching the core" << endl;
        cout << "switch in the Network Closet. The closet is LOCKED. Staff keycards" << endl;
        cout << "are NOT kept at the front desk. Check the Roof.'" << endl;
    } else if (currentRoom == NETWORK_CLOSET) {
        cout << "SIGN: 'CORE SWITCH - Port 7 is loose. Use a good Cat6 cable.'" << endl;
    } else if (currentRoom == SERVER_ROOM) {
        cout << "SIGN: 'Spare patch cables are stored in this room.'" << endl;
    } else {
        cout << "There is no sign in this room." << endl;
    }
}

// Talk to the NPC. Priya is in the Break Room; her dialog changes based on
// what the player is carrying. Passing hasKeycard/hasCable as bool arguments.
void talkToNPC(int currentRoom, bool hasKeycard, bool hasCable) {
    if (currentRoom != BREAK_ROOM) {
        cout << "There is no one here to talk to." << endl;
        return;
    }
    cout << "Priya (night-shift tech) looks up from her vending-machine snack." << endl;
    if (!hasCable && !hasKeycard) {
        cout << "Priya: \"Thank goodness, an admin! The network is dead. The core" << endl;
        cout << "switch in the Network Closet has a loose port. You need a" << endl;
        cout << "network cable from the Server Room, and the closet keycard is" << endl;
        cout << "up on the Roof. I'd go myself, but I'm scared of heights.\"" << endl;
    } else if (hasCable && !hasKeycard) {
        cout << "Priya: \"Nice, you have a cable! But the closet is locked." << endl;
        cout << "Try the Roof for the keycard.\"" << endl;
    } else if (!hasCable && hasKeycard) {
        cout << "Priya: \"You got the keycard! Now grab a cable from the Server" << endl;
        cout << "Room (north of the Lobby).\"" << endl;
    } else {
        cout << "Priya: \"You have everything! The closet is just south of this" << endl;
        cout << "room. Go fix the network!\"" << endl;
    }
}

// Ending: shown when the player wins.
void showWinEnding(int moves) {
    cout << "\n*************************************" << endl;
    cout << "  The switch lights blink green. Screens flicker back on." << endl;
    cout << "  Priya cheers from down the hall: \"The network is back online!\"" << endl;
    cout << "  ENDING: NETWORK RESTORED" << endl;
    cout << "  You finished in " << moves << " moves." << endl;
    cout << "*************************************" << endl;
}

int main() {
    // ---------- Parallel arrays for rooms ----------
    string roomNames[NUM_ROOMS] = {
        "Lobby", "Server Room", "Break Room", "Roof", "Network Closet"
    };
    string roomDescriptions[NUM_ROOMS] = {
        "A dim office lobby. The power is out and the clock is blinking 12:00. A sign hangs on the wall.",
        "Racks of silent servers. Every status light is dark. A sign is taped to a rack.",
        "A cramped break room with a humming vending machine. Priya is here.",
        "A windy rooftop covered in antennas and satellite dishes.",
        "A cold closet full of patch panels. One switch has a loose port. A sign is on the switch."
    };

    // ---------- Parallel arrays for items ----------
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

    // ---------- Adjacency list ----------
    int connections[NUM_ROOMS][NUM_DIRECTIONS];
    for (int i = 0; i < NUM_ROOMS; i++)
        for (int j = 0; j < NUM_DIRECTIONS; j++)
            connections[i][j] = -1;

    connections[LOBBY][NORTH]          = SERVER_ROOM;
    connections[LOBBY][EAST]           = BREAK_ROOM;
    connections[LOBBY][WEST]           = ROOF;
    connections[SERVER_ROOM][SOUTH]    = LOBBY;
    connections[BREAK_ROOM][WEST]      = LOBBY;
    connections[BREAK_ROOM][SOUTH]     = NETWORK_CLOSET;
    connections[ROOF][EAST]            = LOBBY;
    connections[NETWORK_CLOSET][NORTH] = BREAK_ROOM;

    // ---------- Game state ----------
    int currentRoom = LOBBY;
    bool gameRunning = true;
    bool won = false;       // NEW: true only if the player reaches the good ending
    int moves = 0;          // NEW: counts successful room changes

    showIntro();

    // ---------- Main game loop ----------
    while (gameRunning) {

        cout << "\nYou are in the " << roomNames[currentRoom] << endl;
        cout << roomDescriptions[currentRoom] << endl;

        for (int i = 0; i < NUM_ITEMS; i++) {
            if (itemLocation[i] == currentRoom) {
                cout << "You see: " << itemNames[i] << endl;
            }
        }

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

        showMainMenu();
        int choice = getChoice(MAIN_MENU_MAX);

        // Movement (menu 1-4)
        if (choice >= MENU_NORTH && choice <= MENU_WEST) {
            int dir = choice - MENU_NORTH;
            int next = connections[currentRoom][dir];

            if (next == -1) {
                cout << "You can't go that way." << endl;
            } else if (next == NETWORK_CLOSET && itemLocation[KEYCARD] != IN_INVENTORY) {
                // LOCK: the closet door needs the keycard (the KEY)
                cout << "The door is locked. It needs a keycard." << endl;
            } else {
                if (next == NETWORK_CLOSET) {
                    cout << "You swipe the keycard. The lock clicks open!" << endl;
                }
                currentRoom = next;
                moves++;
            }

        } else if (choice == MENU_EXIT) {
            gameRunning = false;

        } else if (choice == MENU_LOOK) {
            cout << roomDescriptions[currentRoom] << endl;

        } else if (choice == MENU_TAKE) {
            showItemMenu("TAKE", itemNames);
            int pick = getChoice(ITEM_MENU_BACK);
            if (pick == 0) {
                gameRunning = false;
            } else if (pick != ITEM_MENU_BACK) {
                int item = pick - 1;
                if (itemLocation[item] == currentRoom) {
                    itemLocation[item] = IN_INVENTORY;
                    cout << "Taken: " << itemDescriptions[item] << endl;
                } else {
                    cout << "You don't see that here." << endl;
                }
            }

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
                    // Winning move
                    itemLocation[CABLE] = GONE;
                    cout << "You plug the cable into the loose port. Lights blink on..." << endl;
                    won = true;
                    gameRunning = false;
                } else {
                    cout << itemUseText[item] << endl;
                }
            }

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

        } else if (choice == MENU_MAP) {
            // Map matches the adjacency list exactly
            cout << "\n                [Server Room]" << endl;
            cout << "                      |" << endl;
            cout << "        [Roof] - [Lobby] - [Break Room]" << endl;
            cout << "                                |" << endl;
            cout << "                        [Network Closet] (locked)" << endl;

        } else if (choice == MENU_HELP) {
            cout << "Type the number of a menu option and press Enter." << endl;
            cout << "Option 0 exits the game from any menu." << endl;
            cout << "Goal: get the network cable to the Network Closet and use it." << endl;
            cout << "Talk to Priya in the Break Room and read signs for clues." << endl;

        } else if (choice == MENU_HINT) {
            if (itemLocation[CABLE] != IN_INVENTORY) {
                cout << "Hint: you need a network cable. Try the Server Room." << endl;
            } else if (itemLocation[KEYCARD] != IN_INVENTORY) {
                cout << "Hint: the Network Closet is locked. Find a keycard." << endl;
            } else {
                cout << "Hint: take the cable to the Network Closet (south of the Break Room)." << endl;
            }

        } else if (choice == MENU_TALK) {
            talkToNPC(currentRoom,
                      itemLocation[KEYCARD] == IN_INVENTORY,
                      itemLocation[CABLE] == IN_INVENTORY);

        } else if (choice == MENU_READ) {
            readSign(currentRoom);
        }
    }

    // ---------- Ending ----------
    if (won) {
        showWinEnding(moves);
    } else {
        cout << "\nYou stepped away. The network is still down..." << endl;
    }
    cout << "Thanks for playing!" << endl;
    return 0;
}