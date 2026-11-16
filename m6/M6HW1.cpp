// =====================================================================
// M6HW1 - Project Prototype (GOLD level)
// Text Adventure: "Server Room Blackout"
//
// Based on the M6BONUS1 text adventure (instructor's starter m6_text_adv.cpp).
// Uses regular arrays only (no vectors).
//
// AI assistance: Claude (Anthropic). Prompts are in M6HW1_lastname.txt.
//
// ---------------------------------------------------------------------
// HOW THE GAME WORKS (read this first)
// ---------------------------------------------------------------------
//   STORY: A storm knocked out the power and the network is down. You are
//          the night-shift network admin. You must plug a network cable
//          into a loose port on the core switch in the Network Closet.
//
//   HOW YOU PLAY: Every turn the game (1) describes the room, (2) shows a
//          numbered menu, (3) reads the number you type, and (4) does that
//          action. Option 0 always exits the game.
//
//   THE 7 ROOMS (each one has exits to its neighbors):
//          Lobby (start), Server Room, Break Room, Roof, Network Closet,
//          Supply Room, Security Office.
//
//   THE 12 ITEMS: keycard, cable, coffee, flashlight, laptop, usb drive,
//          screwdriver, notebook, cable tester, headset, toolkit, radio.
//          Every item can be picked up with Take. You need 3 for Silver.
//
//   THE TWO LOCKS AND KEYS (both must be solved to win):
//     LOCK 1 - Server Room keypad door (north of the Lobby)
//        KEY:  a 4-digit code. The code is written in the NOTEBOOK
//              (in the Lobby). Take the notebook, then Use it.
//        The cable you need is inside the Server Room.
//     LOCK 2 - Network Closet door (south of the Break Room)
//        KEY:  the KEYCARD, which is on the Roof (west of the Lobby).
//        The loose port you must fix is inside the Network Closet.
//
//   WINNING: Take the cable from the Server Room, carry it into the
//          Network Closet, and Use it on the loose port.
//
//   TO EARN ALL THREE LEVELS (menu numbers):
//          1. BRONZE: visit any 3 of the 7 rooms with North(1) East(2)
//             South(3) West(4), and Take(6) 3 items. Then move on to Silver.
//          2. SILVER: read a sign(14) or talk to Priya(13), and Take(6) 3 items.
//          3. GOLD: take the notebook, Use(7) it to read the code, go North and
//             type the code, take the cable, take the keycard on the Roof,
//             walk into the Network Closet, then Use(7) the cable.
//          Menu 15 (Progress) shows a checklist and a NEXT STEP at any time.
//
// ---------------------------------------------------------------------
// LEVEL CHECKLIST (progress bar every turn; menu 15 shows a [X]/[ ] checklist
//                  with HOW-TO steps and a NEXT STEP for each level)
// ---------------------------------------------------------------------
//   BRONZE: At least three different rooms, which have exits between them.
//           (Earned when you have VISITED 3 different rooms AND picked up
//            3 items. The game has 7 rooms in all, but you only need 3 of
//            them to finish Bronze and move forward to Silver.)
//   SILVER: Bronze + dialog that explains the point of the game (wall
//           signs, NPC Priya) + interaction with items (picking up).
//           (Earned when you have read a sign or talked to Priya AND
//            picked up 3 items.)
//   GOLD:   Silver, plus the following:
//             * Some kind of ending ("ENDING: NETWORK RESTORED").
//             * At least one lock and key situation: you find a key in one
//               room and use it to open a door in a different room.
//               This game has TWO of them:
//                 - the CODE written in the notebook (Lobby) opens the
//                   keypad door to the Server Room.
//                 - the KEYCARD found on the Roof opens the door to the
//                   Network Closet.
//           (Earned when you finish the game: Silver is done, both locks
//            were opened with their keys, and you reached the ending.)
//   To change how hard the levels are, edit ROOMS_FOR_BRONZE,
//   ITEMS_FOR_BRONZE, and ITEMS_FOR_SILVER near the top of the program.
//
// ---------------------------------------------------------------------
// DATA STRUCTURE CONCEPTS USED
// ---------------------------------------------------------------------
//   1. Enums            - named constants for directions, rooms, and items
//   2. Parallel arrays  - roomNames[], roomDescriptions[], itemNames[], ...
//                         all use the same index (the enum value)
//   3. Adjacency list   - connections[from_room][direction] = to_room, or -1
//   4. Plain arrays     - visited[] remembers which rooms you have seen
//
// MENUS: every menu has at least 10 options. Option 0 is ALWAYS Exit.
// =====================================================================

#include <iostream>
#include <string>
#include <limits>      // numeric_limits: used to throw away bad input
#include <iomanip>     // setw: used to line up the horizontal menus
using namespace std;

// ---------------------------------------------------------------------
// STEP 1: ENUMS - give names to numbers so the code reads like words
// ---------------------------------------------------------------------

// The four compass directions. NUM_DIRECTIONS is automatically 4.
enum Direction { NORTH, EAST, SOUTH, WEST, NUM_DIRECTIONS };
const string DIRECTION_NAMES[NUM_DIRECTIONS] = {"north", "east", "south", "west"};

// The seven rooms. NUM_ROOMS is automatically 7.
enum Room { LOBBY, SERVER_ROOM, BREAK_ROOM, ROOF, NETWORK_CLOSET,
            SUPPLY_ROOM, SECURITY_OFFICE, NUM_ROOMS };

// The twelve items. NUM_ITEMS is automatically 12.
enum Item {
    KEYCARD, CABLE, COFFEE, FLASHLIGHT, LAPTOP,
    USB_DRIVE, SCREWDRIVER, NOTEBOOK, CABLE_TESTER, HEADSET,
    TOOLKIT, RADIO,
    NUM_ITEMS
};

// Special values stored in itemLocation[]. A value 0..NUM_ROOMS-1 means
// "the item is lying in that room".
const int IN_INVENTORY = -2;   // the player is carrying the item
const int GONE = -3;           // the item has been used up

// The secret keypad code for the Server Room door (KEY for LOCK 1).
// The player learns it by reading the notebook.
const int SERVER_CODE = 4721;

// How much the player must do to earn each level.
const int ROOMS_FOR_BRONZE = 3;           // Bronze: visit any 3 of the 7 rooms
const int ITEMS_FOR_BRONZE = 3;           // Bronze: ALSO pick up 3 items
const int ITEMS_FOR_SILVER = 3;           // Silver: pick up 3 items in total

// Main menu option numbers (named so the code reads like words)
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
const int MENU_TALK = 13;       // talk to an NPC
const int MENU_READ = 14;       // read a sign
const int MENU_PROGRESS = 15;   // show Bronze / Silver / Gold progress
const int MAIN_MENU_MAX = 15;

// Item sub-menu: options 1..12 pick an item, 13 goes back, 0 exits
const int ITEM_MENU_BACK = NUM_ITEMS + 1;   // = 13

// ---------------------------------------------------------------------
// STEP 2: HELPER FUNCTIONS (menus and input)
// ---------------------------------------------------------------------

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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // drop bad text
            cout << "Please enter a number." << endl;
            continue;
        }
        if (choice >= 0 && choice <= high) return choice;   // valid choice
        cout << "Please choose a number from 0 to " << high << "." << endl;
    }
}

// Print one menu option as "number. label", padded to a fixed width so the
// options line up in neat columns.
void printOption(int number, const string label, int width) {
    cout << left << setw(width) << (to_string(number) + ". " + label);
}

// Print the main menu HORIZONTALLY (16 options including Exit).
void showMainMenu() {
    cout << "\n===== MAIN MENU =====" << endl;
    // Row 1: movement and looking
    printOption(1, "North", 16);
    printOption(2, "East", 16);
    printOption(3, "South", 16);
    printOption(4, "West", 16);
    printOption(5, "Look", 16);
    cout << endl;
    // Row 2: item actions
    printOption(6, "Take", 16);
    printOption(7, "Use", 16);
    printOption(8, "Inventory", 16);
    printOption(9, "Examine", 16);
    printOption(10, "Map", 16);
    cout << endl;
    // Row 3: help, dialog, progress, exit
    printOption(11, "Help", 16);
    printOption(12, "Hint", 16);
    printOption(13, "Talk", 16);
    printOption(14, "Read sign", 16);
    printOption(15, "Progress", 16);
    cout << endl;
    printOption(0, "EXIT", 16);
    cout << endl;
}

// Print an item sub-menu: all 12 items, then Back (13) and Exit (0).
// 'title' says what the menu is for (TAKE / USE / EXAMINE).
void showItemMenu(const string title, const string names[]) {
    cout << "\n===== " << title << " WHICH ITEM? =====" << endl;
    for (int i = 0; i < NUM_ITEMS; i++) {
        printOption(i + 1, names[i], 20);
        if ((i + 1) % 4 == 0) cout << endl;      // new row after every 4 options
    }
    printOption(ITEM_MENU_BACK, "BACK", 20);
    printOption(0, "EXIT", 20);
    cout << endl;
}

// ---------------------------------------------------------------------
// STEP 3: LEVEL PROGRESS (Bronze / Silver / Gold shown to the player)
// ---------------------------------------------------------------------

// One-line status shown at the top of every turn.
void showLevelBar(bool bronze, bool silver, bool gold) {
    cout << "LEVEL PROGRESS:  BRONZE [" << (bronze ? "DONE" : " .. ") << "]   "
         << "SILVER [" << (silver ? "DONE" : " .. ") << "]   "
         << "GOLD [" << (gold ? "DONE" : " .. ") << "]" << endl;
}

// Return "[X]" for a finished task and "[ ]" for an unfinished one.
string mark(bool done) {
    return done ? "[X]" : "[ ]";
}

// Detailed checklist (menu option 15).
// For every task it shows [X] / [ ] and, if the task is NOT done yet, a
// "HOW" line that tells the player exactly what to do to finish it.
// At the bottom it prints one NEXT STEP so the player always knows what to do.
void showProgressDetails(const bool visited[], const string roomNames[],
                         const string roomWhere[], int itemsCollected,
                         bool dialogHeard, bool serverUnlocked,
                         bool closetUnlocked, bool won) {
    // Count how many rooms the player has visited
    int roomsSeen = 0;
    for (int i = 0; i < NUM_ROOMS; i++)
        if (visited[i]) roomsSeen++;

    bool roomsDone = (roomsSeen >= ROOMS_FOR_BRONZE);            // Bronze task 1
    bool bronzeItemsDone = (itemsCollected >= ITEMS_FOR_BRONZE); // Bronze task 2
    bool bronze = roomsDone && bronzeItemsDone;
    bool itemsDone = (itemsCollected >= ITEMS_FOR_SILVER);
    bool silver = bronze && dialogHeard && itemsDone;

    cout << "\n===== YOUR PROGRESS =====" << endl;

    // ----- BRONZE: visit 3 rooms + pick up 3 items. Every room is listed by
    //       name so the player knows where they can go. -----
    cout << "BRONZE (" << (bronze ? "COMPLETE - move on to Silver!" : "in progress") << ")" << endl;
    cout << "  " << mark(roomsDone) << " Visit " << ROOMS_FOR_BRONZE << " different rooms ("
         << roomsSeen << " visited, " << ROOMS_FOR_BRONZE << " needed)" << endl;
    cout << "      The " << NUM_ROOMS << " rooms you can visit (any " << ROOMS_FOR_BRONZE << " count):" << endl;
    for (int i = 0; i < NUM_ROOMS; i++) {
        cout << "        " << mark(visited[i]) << " " << left << setw(16) << roomNames[i]
             << roomWhere[i] << endl;
    }
    if (!roomsDone)
        cout << "      HOW: use North/East/South/West (menu 1-4) to walk into the rooms marked [ ]." << endl;
    cout << "  " << mark(bronzeItemsDone) << " Pick up " << ITEMS_FOR_BRONZE << " items ("
         << itemsCollected << " picked up, " << ITEMS_FOR_BRONZE << " needed)" << endl;
    if (!bronzeItemsDone)
        cout << "      HOW: choose Take (menu 6) and pick items that are in your room." << endl;

    // ----- SILVER -----
    cout << "SILVER (" << (silver ? "COMPLETE" : "in progress")
         << (bronze ? "" : " - finish Bronze first") << ")" << endl;
    cout << "  " << mark(dialogHeard) << " Hear dialog that explains the point of the game" << endl;
    if (!dialogHeard)
        cout << "      HOW: Read sign (menu 14) in the Lobby, or Talk (menu 13) to Priya in the Break Room." << endl;
    cout << "  " << mark(itemsDone) << " Pick up " << ITEMS_FOR_SILVER << " items ("
         << itemsCollected << "/" << ITEMS_FOR_SILVER << ")" << endl;
    if (!itemsDone)
        cout << "      HOW: choose Take (menu 6) and pick items that are in your room." << endl;

    // ----- GOLD -----
    cout << "GOLD (" << (won ? "COMPLETE" : "in progress")
         << ") - Silver + a lock and key + an ending" << endl;
    cout << "  " << mark(silver) << " Silver is complete (Gold builds on Silver)" << endl;
    cout << "  LOCK AND KEY: find a key in one room to open a door in a different room." << endl;
    cout << "  " << mark(serverUnlocked) << " Unlock the Server Room keypad door (LOCK 1, code key)" << endl;
    if (!serverUnlocked)
        cout << "      HOW: Take the notebook in the Lobby, Use it (menu 7) to read the code,"
             << "\n           then go North from the Lobby and type the 4-digit code." << endl;
    cout << "  " << mark(closetUnlocked) << " Unlock the Network Closet door (LOCK 2, keycard key)" << endl;
    if (!closetUnlocked)
        cout << "      HOW: Take the keycard on the Roof (west of the Lobby), then walk"
             << "\n           South from the Break Room into the Network Closet." << endl;
    cout << "  " << mark(serverUnlocked || closetUnlocked)
         << " At least one lock and key situation solved" << endl;
    cout << "  " << mark(won) << " Reach the ending (fix the network)" << endl;
    if (!won)
        cout << "      HOW: Take the cable in the Server Room, carry it into the Network"
             << "\n           Closet, and Use it (menu 7) on the loose port."
             << "\n           (Finish Bronze and Silver first, or the game will not count Gold.)" << endl;

    // ----- NEXT STEP: the single most useful thing to do right now -----
    cout << "NEXT STEP: ";
    if (won)                    cout << "You finished the game!" << endl;
    else if (!roomsDone)        cout << "Walk to a room you have not visited yet (menu 1-4)." << endl;
    else if (!bronzeItemsDone)  cout << "Take an item (menu 6). You have " << itemsCollected
                                     << " of " << ITEMS_FOR_BRONZE << "." << endl;
    else if (!dialogHeard)      cout << "Read a sign (menu 14) or talk to Priya (menu 13)." << endl;
    else if (!itemsDone)        cout << "Take more items (menu 6). You have " << itemsCollected
                                     << " of " << ITEMS_FOR_SILVER << "." << endl;
    else if (!serverUnlocked)   cout << "Get the notebook, read the code, and open the Server Room keypad." << endl;
    else if (!closetUnlocked)   cout << "Get the keycard from the Roof and open the Network Closet." << endl;
    else                        cout << "Take the cable and Use it in the Network Closet." << endl;
}

// ---------------------------------------------------------------------
// STEP 4: STORY / DIALOG FUNCTIONS (void and bool functions)
// ---------------------------------------------------------------------

// Opening dialog: explains the point of the game.
void showIntro() {
    cout << "=====================================" << endl;
    cout << "        SERVER ROOM BLACKOUT" << endl;
    cout << "=====================================" << endl;
    cout << "A storm knocked out the power at Fayetteville Tech Systems." << endl;
    cout << "You are the night-shift network admin, and the whole network is down." << endl;
    cout << "YOUR GOAL: get a network cable and plug it into the loose port" << endl;
    cout << "on the core switch in the Network Closet." << endl;
    cout << "But two doors are locked: you will need a CODE and a KEYCARD." << endl;
    cout << "Explore, talk to people, read signs, and collect useful items." << endl;
    cout << "Watch the LEVEL PROGRESS bar to see Bronze, Silver, and Gold." << endl;
}

// Read the sign in the current room. Returns true if there was a sign
// (reading a sign counts as "dialog" for the Silver level).
bool readSign(int currentRoom) {
    if (currentRoom == LOBBY) {
        cout << "SIGN: 'OUTAGE NOTICE - Restore the network by patching the core" << endl;
        cout << "switch in the Network Closet. The closet is LOCKED: staff keycards" << endl;
        cout << "are NOT kept at the front desk (check the Roof). The Server Room" << endl;
        cout << "has a KEYPAD: the code is written in the staff notebook.'" << endl;
        return true;
    } else if (currentRoom == NETWORK_CLOSET) {
        cout << "SIGN: 'CORE SWITCH - Port 7 is loose. Use a good Cat6 cable.'" << endl;
        return true;
    } else if (currentRoom == SERVER_ROOM) {
        cout << "SIGN: 'Spare patch cables are stored in this room.'" << endl;
        return true;
    } else if (currentRoom == SUPPLY_ROOM) {
        cout << "SIGN: 'SUPPLY ROOM - Cables are kept in the Server Room, not here." << endl;
        cout << "Please return all tools.'" << endl;
        return true;
    } else if (currentRoom == SECURITY_OFFICE) {
        cout << "SIGN: 'SECURITY - Cameras are offline during the outage." << endl;
        cout << "Lost a keycard? Check the Roof.'" << endl;
        return true;
    }
    cout << "There is no sign in this room." << endl;
    return false;
}

// Talk to the NPC. Priya is in the Break Room; what she says depends on
// what the player has already done. Returns true if the player spoke to her.
bool talkToNPC(int currentRoom, bool hasKeycard, bool hasCable, bool serverUnlocked) {
    if (currentRoom != BREAK_ROOM) {
        cout << "There is no one here to talk to." << endl;
        return false;
    }
    cout << "Priya (night-shift tech) looks up from her vending-machine snack." << endl;
    if (!hasCable && !hasKeycard) {
        // First conversation: explains the point of the game
        cout << "Priya: \"Thank goodness, an admin! The network is dead. The core" << endl;
        cout << "switch in the Network Closet has a loose port. You need a" << endl;
        cout << "network cable from the Server Room. The Server Room has a keypad;" << endl;
        cout << "check the notebook in the Lobby for the code. The closet keycard" << endl;
        cout << "is up on the Roof. I'd go myself, but I'm scared of heights.\"" << endl;
    } else if (hasCable && !hasKeycard) {
        cout << "Priya: \"Nice, you have a cable! But the closet is locked." << endl;
        cout << "Try the Roof for the keycard.\"" << endl;
    } else if (!hasCable && hasKeycard) {
        if (serverUnlocked) {
            cout << "Priya: \"You got the keycard! Now grab a cable from the Server" << endl;
            cout << "Room (north of the Lobby).\"" << endl;
        } else {
            cout << "Priya: \"You got the keycard! Now you need a cable, and the Server" << endl;
            cout << "Room keypad code is in the notebook in the Lobby.\"" << endl;
        }
    } else {
        cout << "Priya: \"You have everything! The closet is just south of this" << endl;
        cout << "room. Go fix the network!\"" << endl;
    }
    return true;
}

// LOCK 1: the keypad on the Server Room door.
// Asks for a number. Returns true if the code is correct.
// Sets quit = true if the input ended (so the game can stop).
bool tryKeypad(bool &quit) {
    int code;
    cout << "The Server Room door has a KEYPAD. Enter the 4-digit code (0 to walk away): ";
    if (!(cin >> code)) {
        if (cin.eof()) {                 // no more input: leave the game
            quit = true;
            return false;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "The keypad only accepts numbers." << endl;
        return false;
    }
    if (code == 0) {                     // 0 = cancel without a wrong-code message
        cout << "You step away from the keypad." << endl;
        return false;
    }
    if (code == SERVER_CODE) {
        cout << "BEEP BEEP. The keypad turns green and the door unlocks!" << endl;
        return true;
    }
    cout << "BZZT. Wrong code. The keypad resets." << endl;
    return false;
}

// The ending shown when the player wins.
void showWinEnding(int moves) {
    cout << "\n*************************************" << endl;
    cout << "  The switch lights blink green. Screens flicker back on." << endl;
    cout << "  Priya cheers from down the hall: \"The network is back online!\"" << endl;
    cout << "  ENDING: NETWORK RESTORED" << endl;
    cout << "  You finished in " << moves << " moves." << endl;
    cout << "*************************************" << endl;
}

// ---------------------------------------------------------------------
// STEP 5: MAIN - builds the world, then runs the game loop
// ---------------------------------------------------------------------
int main() {

    // ---------- 5a. Parallel arrays for rooms (index = Room enum) ----------
    string roomNames[NUM_ROOMS] = {
        "Lobby", "Server Room", "Break Room", "Roof", "Network Closet",
        "Supply Room", "Security Office"
    };
    string roomDescriptions[NUM_ROOMS] = {
        "A dim office lobby. The power is out and the clock is blinking 12:00. A sign hangs on the wall.",
        "Racks of silent servers. Every status light is dark. A sign is taped to a rack.",
        "A cramped break room with a humming vending machine. Priya is here.",
        "A windy rooftop covered in antennas and satellite dishes.",
        "A cold closet full of patch panels. One switch has a loose port. A sign is on the switch.",
        "A narrow supply room with shelves of boxes, spare labels, and a dusty toolbox. A sign is on the door.",
        "A tiny security office. The camera monitors are dark. A sign is pinned to the corkboard."
    };
    // Where to find each room (shown in the Progress checklist)
    string roomWhere[NUM_ROOMS] = {
        "(start room)",
        "(north of the Lobby, keypad door)",
        "(east of the Lobby)",
        "(west of the Lobby)",
        "(south of the Break Room, keycard door)",
        "(east of the Break Room)",
        "(south of the Lobby)"
    };

    // ---------- 5b. Parallel arrays for items (index = Item enum) ----------
    string itemNames[NUM_ITEMS] = {
        "keycard", "cable", "coffee", "flashlight", "laptop",
        "usb drive", "screwdriver", "notebook", "cable tester", "headset",
        "toolkit", "radio"
    };
    string itemDescriptions[NUM_ITEMS] = {
        "A keycard labeled 'NETWORK CLOSET'.",
        "A blue Cat6 patch cable.",
        "A lukewarm coffee. Not useful, but morale matters.",
        "A heavy flashlight with fresh batteries.",
        "A laptop with 3% battery left.",
        "A USB drive labeled 'BACKUPS - DO NOT LOSE'.",
        "A small Phillips screwdriver.",
        "A notebook full of IP addresses and scribbled passwords. (Try USING it.)",
        "A handheld tester that checks Ethernet cables.",
        "A call-center headset. Static hisses in the earpiece.",
        "A canvas toolkit with a ratchet, a crimper, and a bag of zip ties.",
        "A walkie-talkie tuned to the security channel."
    };
    // What happens when you Use each item (the cable and keycard have
    // special cases handled in the game loop).
    string itemUseText[NUM_ITEMS] = {
        "You wave the keycard around. It opens the Network Closet door when you walk into it.",
        "You need a loose network port to plug this into.",
        "You sip the coffee. Slightly better.",
        "You click the flashlight on. The shadows retreat.",
        "The laptop shows 'No network connection'. Of course.",
        "You turn the USB drive over. Nothing to plug it into yet.",
        "You tighten a random screw. Satisfying, but pointless.",
        "You flip through the notebook. Page 3: 'Server Room keypad code: 4721.' Page 12: 'Port 7 is the loose one.'",
        "The tester beeps. Your cable is good!",
        "You put on the headset. You hear only static.",
        "You open the toolkit. Plenty of tools, but the port only needs a cable.",
        "You press the button. A guard mumbles: 'Storm outage... the closet keycard is on the roof.'"
    };
    // Where each item starts: a room number, IN_INVENTORY, or GONE
    int itemLocation[NUM_ITEMS];
    itemLocation[KEYCARD]      = ROOF;            // KEY for LOCK 2
    itemLocation[CABLE]        = SERVER_ROOM;     // behind LOCK 1 (needed to win)
    itemLocation[COFFEE]       = BREAK_ROOM;
    itemLocation[FLASHLIGHT]   = LOBBY;
    itemLocation[LAPTOP]       = SERVER_ROOM;
    itemLocation[USB_DRIVE]    = ROOF;
    itemLocation[SCREWDRIVER]  = BREAK_ROOM;
    itemLocation[NOTEBOOK]     = LOBBY;           // holds the KEY for LOCK 1
    itemLocation[CABLE_TESTER] = NETWORK_CLOSET;
    itemLocation[HEADSET]      = BREAK_ROOM;
    itemLocation[TOOLKIT]      = SUPPLY_ROOM;     // new room, new item
    itemLocation[RADIO]        = SECURITY_OFFICE; // new room, new item

    // ---------- 5c. Adjacency list (2D array) ----------
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
    connections[BREAK_ROOM][EAST]      = SUPPLY_ROOM;       // new room
    connections[SUPPLY_ROOM][WEST]     = BREAK_ROOM;
    connections[LOBBY][SOUTH]          = SECURITY_OFFICE;   // new room
    connections[SECURITY_OFFICE][NORTH] = LOBBY;

    // ---------- 5d. Game state variables ----------
    int currentRoom = LOBBY;        // the player starts in the Lobby
    bool gameRunning = true;        // set to false to end the game
    bool won = false;               // true only if the player reaches the good ending
    int moves = 0;                  // counts successful room changes

    // Lock state: both doors start locked
    bool serverUnlocked = false;    // LOCK 1 (keypad)
    bool closetUnlocked = false;    // LOCK 2 (keycard)

    // Level tracking
    bool visited[NUM_ROOMS];        // which rooms the player has been in
    for (int i = 0; i < NUM_ROOMS; i++) visited[i] = false;
    bool dialogHeard = false;       // read a sign or talked to Priya (Silver)
    int itemsCollected = 0;         // how many items the player has picked up (Silver)
    bool bronzeDone = false;
    bool silverDone = false;
    bool goldDone = false;

    // ---------- 5e. Start the game ----------
    showIntro();                    // the intro explains the story; Silver still needs a sign or Priya

    // ---------- 5f. MAIN GAME LOOP: one pass = one turn ----------
    while (gameRunning) {

        // STEP A: remember that the player has now seen this room
        visited[currentRoom] = true;

        // STEP B: count how many different rooms have been visited
        int roomsSeen = 0;
        for (int i = 0; i < NUM_ROOMS; i++)
            if (visited[i]) roomsSeen++;

        // STEP C: check whether Bronze or Silver was just earned
        if (!bronzeDone && roomsSeen >= ROOMS_FOR_BRONZE && itemsCollected >= ITEMS_FOR_BRONZE) {
            bronzeDone = true;
            cout << "\n*** BRONZE LEVEL COMPLETE: you visited " << ROOMS_FOR_BRONZE
                 << " rooms and picked up " << ITEMS_FOR_BRONZE << " items! ***" << endl;
            cout << "*** You can now move on to SILVER (menu 15 shows what to do). ***" << endl;
        }
        if (!silverDone && bronzeDone && dialogHeard && itemsCollected >= ITEMS_FOR_SILVER) {
            silverDone = true;
            cout << "\n*** SILVER LEVEL COMPLETE: you heard dialog and picked up "
                 << ITEMS_FOR_SILVER << " items! ***" << endl;
        }

        // STEP D: describe the current room and show progress
        cout << "\n";
        showLevelBar(bronzeDone, silverDone, goldDone);
        cout << "Rooms visited: " << roomsSeen << " of " << NUM_ROOMS
             << " (Bronze needs " << ROOMS_FOR_BRONZE << ")   Items picked up: " << itemsCollected
             << " (Bronze needs " << ITEMS_FOR_BRONZE << ", Silver needs " << ITEMS_FOR_SILVER << ")" << endl;
        cout << "You are in the " << roomNames[currentRoom] << endl;
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

        // STEP E: show the menu and read the player's choice
        showMainMenu();
        int choice = getChoice(MAIN_MENU_MAX);

        // STEP F: do what the player chose

        // ----- Menus 1-4: MOVEMENT (includes both lock-and-key checks) -----
        if (choice >= MENU_NORTH && choice <= MENU_WEST) {
            int dir = choice - MENU_NORTH;                 // 1->NORTH(0) ... 4->WEST(3)
            int next = connections[currentRoom][dir];      // look up the adjacency list
            bool canEnter = true;                          // may the player walk through?

            if (next == -1) {
                // No exit that way
                cout << "You can't go that way." << endl;
                canEnter = false;

            } else if (next == NETWORK_CLOSET && !closetUnlocked) {
                // LOCK 2: needs the KEYCARD in the inventory
                if (itemLocation[KEYCARD] == IN_INVENTORY) {
                    closetUnlocked = true;                 // the key opens the lock for good
                    cout << "You swipe the keycard. The Network Closet lock clicks open!" << endl;
                    cout << "  [LOCK AND KEY] The keycard from the Roof opened a door in a different room!" << endl;
                } else {
                    cout << "The door is locked. It needs a keycard." << endl;
                    canEnter = false;
                }

            } else if (next == SERVER_ROOM && !serverUnlocked) {
                // LOCK 1: needs the 4-digit CODE typed on the keypad
                bool quit = false;
                if (tryKeypad(quit)) {
                    serverUnlocked = true;                 // the code opens the lock for good
                    cout << "  [LOCK AND KEY] The code from the notebook opened a door in a different room!" << endl;
                } else {
                    canEnter = false;
                    if (quit) gameRunning = false;         // input ended: stop the game
                }
            }

            if (canEnter) {
                currentRoom = next;                        // move to the new room
                moves++;
            }

        // ----- 0: EXIT -----
        } else if (choice == MENU_EXIT) {
            gameRunning = false;

        // ----- 5: LOOK -----
        } else if (choice == MENU_LOOK) {
            cout << roomDescriptions[currentRoom] << endl;

        // ----- 6: TAKE an item (it must be in this room) -----
        } else if (choice == MENU_TAKE) {
            showItemMenu("TAKE", itemNames);
            int pick = getChoice(ITEM_MENU_BACK);
            if (pick == 0) {
                gameRunning = false;                       // exit from the sub-menu
            } else if (pick != ITEM_MENU_BACK) {
                int item = pick - 1;                       // menu number -> array index
                if (itemLocation[item] == currentRoom) {
                    itemLocation[item] = IN_INVENTORY;     // move it into the inventory
                    itemsCollected++;                      // counts toward Silver
                    cout << "Taken: " << itemDescriptions[item] << endl;
                    cout << "(Items picked up so far: " << itemsCollected << ")" << endl;
                } else {
                    cout << "You don't see that here." << endl;
                }
            }

        // ----- 7: USE an item (it must be in the inventory) -----
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
                    // WINNING MOVE: cable used on the loose port in the closet.
                    // Plugging in ENDS the game, so if Bronze or Silver is not
                    // finished yet, warn the player and let them decide.
                    bool readyForGold = (roomsSeen >= ROOMS_FOR_BRONZE
                                         && itemsCollected >= ITEMS_FOR_BRONZE
                                         && dialogHeard
                                         && itemsCollected >= ITEMS_FOR_SILVER);
                    int confirm = 1;                       // 1 = plug it in
                    if (!readyForGold) {
                        cout << "Careful: plugging in ENDS the game, and Bronze/Silver are not finished." << endl;
                        cout << "(Menu 15 shows what is left.)" << endl;
                        printOption(1, "Plug it in anyway", 22);
                        printOption(2, "Not yet", 22);
                        printOption(0, "EXIT", 22);
                        cout << endl;
                        confirm = getChoice(2);
                    }
                    if (confirm == 1) {
                        itemLocation[CABLE] = GONE;
                        cout << "You plug the cable into the loose port. Lights blink on..." << endl;
                        won = true;
                        gameRunning = false;
                    } else if (confirm == 0) {
                        gameRunning = false;               // exit from the sub-menu
                    } else {
                        cout << "You hold off and keep exploring." << endl;
                    }
                } else {
                    cout << itemUseText[item] << endl;     // normal use message
                }
            }

        // ----- 8: INVENTORY -----
        } else if (choice == MENU_INVENTORY) {
            cout << "You are carrying: ";
            bool any = false;
            for (int i = 0; i < NUM_ITEMS; i++) {
                if (itemLocation[i] == IN_INVENTORY) {
                    if (any) cout << ", ";         // comma only BETWEEN items
                    cout << itemNames[i];
                    any = true;
                }
            }
            if (!any) cout << "nothing";
            cout << endl;

        // ----- 9: EXAMINE an item (carried, or in this room) -----
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

        // ----- 10: MAP (matches the adjacency list) -----
        } else if (choice == MENU_MAP) {
            cout << "\n             [Server Room] (keypad)" << endl;
            cout << "                    |" << endl;
            cout << "   [Roof] ----- [Lobby] ----- [Break Room] ----- [Supply Room]" << endl;
            cout << "                   |               |" << endl;
            cout << "           [Security Office]  [Network Closet] (keycard)" << endl;

        // ----- 11: HELP -----
        } else if (choice == MENU_HELP) {
            cout << "Type the number of a menu option and press Enter." << endl;
            cout << "Option 0 exits the game from any menu." << endl;
            cout << "Goal: get the network cable to the Network Closet and use it." << endl;
            cout << "LOCK 1: Server Room keypad. The code is in the notebook." << endl;
            cout << "LOCK 2: Network Closet door. The key is the keycard." << endl;
            cout << "Talk to Priya in the Break Room and read signs for clues." << endl;
            cout << "Bronze = visit " << ROOMS_FOR_BRONZE << " rooms + pick up " << ITEMS_FOR_BRONZE
                 << " items. Silver = Bronze + dialog + pick up " << ITEMS_FOR_SILVER << " items." << endl;
            cout << "Gold = Silver + a lock and key (find a key in one room to open a door in another)"
                 << " + an ending." << endl;
            cout << "Option 15 shows your Bronze, Silver, and Gold progress." << endl;

        // ----- 12: HINT (changes depending on what the player has done) -----
        } else if (choice == MENU_HINT) {
            if (!serverUnlocked && itemLocation[CABLE] != IN_INVENTORY) {
                if (itemLocation[NOTEBOOK] != IN_INVENTORY)
                    cout << "Hint: the Server Room has the cable. Take the notebook in the Lobby for the keypad code." << endl;
                else
                    cout << "Hint: use the notebook to read the code, then go north from the Lobby." << endl;
            } else if (itemLocation[CABLE] != IN_INVENTORY) {
                cout << "Hint: take the cable from the Server Room." << endl;
            } else if (itemLocation[KEYCARD] != IN_INVENTORY && !closetUnlocked) {
                cout << "Hint: the Network Closet is locked. Find the keycard on the Roof." << endl;
            } else {
                cout << "Hint: take the cable to the Network Closet (south of the Break Room)." << endl;
            }

        // ----- 13: TALK (NPC dialog) -----
        } else if (choice == MENU_TALK) {
            if (talkToNPC(currentRoom,
                          itemLocation[KEYCARD] == IN_INVENTORY,
                          itemLocation[CABLE] == IN_INVENTORY,
                          serverUnlocked)) {
                dialogHeard = true;                        // counts toward Silver
            }

        // ----- 14: READ SIGN -----
        } else if (choice == MENU_READ) {
            if (readSign(currentRoom)) {
                dialogHeard = true;                        // counts toward Silver
            }

        // ----- 15: PROGRESS -----
        } else if (choice == MENU_PROGRESS) {
            showProgressDetails(visited, roomNames, roomWhere, itemsCollected,
                                dialogHeard, serverUnlocked, closetUnlocked, won);
        }
    }

    // ---------- 5g. The game loop is over: show the ending ----------
    // Re-check Bronze and Silver one last time (the last turn may have
    // earned them before the loop ended).
    int roomsSeenEnd = 0;
    for (int i = 0; i < NUM_ROOMS; i++)
        if (visited[i]) roomsSeenEnd++;
    bronzeDone = (roomsSeenEnd >= ROOMS_FOR_BRONZE) && (itemsCollected >= ITEMS_FOR_BRONZE);
    silverDone = bronzeDone && dialogHeard && (itemsCollected >= ITEMS_FOR_SILVER);

    if (won && serverUnlocked && closetUnlocked) {
        showWinEnding(moves);
        if (silverDone) {
            goldDone = true;   // Silver + ending + both lock-and-key puzzles solved
            cout << "\n*** GOLD LEVEL COMPLETE! ***" << endl;
            cout << "  Silver: done" << endl;
            cout << "  Lock and key: the notebook code opened the Server Room, and the" << endl;
            cout << "                keycard from the Roof opened the Network Closet" << endl;
            cout << "  Ending: NETWORK RESTORED" << endl;
        } else {
            // Gold is built on top of Silver, so tell the player what is missing
            cout << "\nYou fixed the network, but GOLD also needs BRONZE and SILVER first." << endl;
            if (roomsSeenEnd < ROOMS_FOR_BRONZE)
                cout << "  Missing: visit " << ROOMS_FOR_BRONZE << " different rooms ("
                     << roomsSeenEnd << "/" << ROOMS_FOR_BRONZE << " visited)." << endl;
            if (!dialogHeard)
                cout << "  Missing: read a sign (menu 14) or talk to Priya (menu 13)." << endl;
            if (itemsCollected < ITEMS_FOR_SILVER)
                cout << "  Missing: pick up " << ITEMS_FOR_SILVER << " items ("
                     << itemsCollected << "/" << ITEMS_FOR_SILVER << " picked up)." << endl;
        }
    } else {
        cout << "\nYou stepped away. The network is still down..." << endl;
    }

    // Final level summary
    cout << "\nFINAL ";
    showLevelBar(bronzeDone, silverDone, goldDone);
    cout << "Thanks for playing!" << endl;
    return 0;
}
