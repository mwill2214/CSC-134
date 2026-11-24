# Midnight Breach: Project Proposal

CSC 134 Choose Your Own Adventure | Language: C++ | Author: Myra Williams | AI assistance: Claude (Anthropic)

This proposal follows the software development lifecycle (SDLC): Gather Requirements, Design, Implement, Test, Release. This submission contains the Requirements document, a preliminary Design document, and the prompts used so far.

## 1. Project Summary

Midnight Breach is a text-based choose-your-own-adventure game. The player is a security analyst. An intruder is stealing data from the company network. The player must collect a laptop, an admin token, and a firewall disk, then use the laptop in the Server Room to lock the intruder out before the move counter reaches zero.

The program combines the Midnight Breach adventure with the numbered-menu program from M6BONUS1.

## 2. Requirements Document

### 2.1 Goal

Build a replayable console adventure in C++ that demonstrates loops, arrays, functions, and input validation, with a win condition, a lose condition, and a scoring system.

### 2.2 Functional Requirements

| ID | Requirement |
| --- | --- |
| FR-1 | The game shall contain 12 rooms laid out on a 3 x 4 grid. |
| FR-2 | The game shall contain 14 items, each with a name, description, icon, use message, and starting location. |
| FR-3 | The player shall move North, East, South, or West. Moving into a wall shall show an error and not cost a move. |
| FR-4 | The Network Closet shall require the access badge. The Vault shall require the vault key. The Parking Garage shall hide its items unless the player carries the flashlight. |
| FR-5 | The player shall be able to Look, Take, Use, Examine, view Inventory, view the Map, get Help, and get a Hint. |
| FR-6 | Every menu shall have at least 10 numbered options, shall be printed horizontally, and shall use option 0 as EXIT. |
| FR-7 | Item menus shall list all 14 items plus BACK and EXIT. |
| FR-8 | The player starts with 40 moves. Each room change costs 1 move. |
| FR-9 | Coffee shall add 3 moves and the energy drink shall add 5 moves. Each is used up after one use. |
| FR-10 | While carried, the skateboard shall make every second move free. |
| FR-11 | The player wins by using the laptop in the Server Room while carrying the admin token and the firewall disk. |
| FR-12 | The player loses if moves reach 0 before winning. |
| FR-13 | Score: +5 per item taken, +20 for saving server logs with the USB drive, +50 for the winning move, and a bonus of 2 points per move left. |
| FR-14 | The win screen shall show a rank: Elite Analyst (20+ moves left), Senior Analyst (10+), or Junior Analyst. |
| FR-15 | Warnings shall appear when moves are low (12 or fewer, and 5 or fewer). |
| FR-16 | The game shall show ASCII art for the title, each room, and the win and lose screens. |
| FR-17 | The Hint option shall change based on the player's progress. |

### 2.3 Non-Functional Requirements

| ID | Requirement |
| --- | --- |
| NFR-1 | Use regular arrays only (no vectors). |
| NFR-2 | Use only the standard library, and compile with any C++11 or newer compiler. |
| NFR-3 | Invalid input (letters, out-of-range numbers, end of input) shall never crash the game or cause an infinite loop. |
| NFR-4 | Code shall use named constants and enums instead of unexplained numbers, and shall be commented for a student reader. |
| NFR-5 | The game shall be winnable and shall be losable. |

### 2.4 Required Programming Concepts

Arrays (1D, parallel, and 2D), while and for loops, if/else, functions, enums, constants, strings, input validation, ASCII graphics, and an adjacency list.

### 2.5 Deliverables

- midnight\_breach.cpp (source code)
- README.md (what the game is, how to compile and play)
- HOW\_IT\_WORKS.md (how the code is organized)
- This proposal, with the prompt log

### 2.6 Assumptions and Constraints

- Single player, single file, console only.
- No saving or loading of games.
- Game state lives in memory only while the program runs.

## 3. Design Document (Preliminary)

### 3.1 Architecture

The program is one file with three layers: constants and data at the top, helper functions in the middle, and main() holding the game state and game loop.

### 3.2 Data Design

| Structure | Type | Purpose |
| --- | --- | --- |
| roomName, roomDesc | Parallel 1D string arrays | Room text, indexed by the Room enum |
| itemName, itemDesc, itemUseText | Parallel 1D string arrays | Item text, indexed by the Item enum |
| itemLoc | 1D int array | Where each item is: a room number, IN\_INVENTORY (-2), or GONE (-3) |
| grid\[3\]\[4\] | 2D int array | Building layout |
| exits\[12\]\[4\] | 2D int array (adjacency list) | exits\[room\]\[direction\] gives the next room or NO\_EXIT (-1), built from the grid at startup |
| ROOM\_ART, ITEM\_ICON | Constant string arrays | ASCII pictures and one-line icons |

Enums: Direction (NORTH, EAST, SOUTH, WEST), Room (12 values), Item (14 values).

### 3.3 Building Layout

|  | Col 1 | Col 2 | Col 3 | Col 4 |
| --- | --- | --- | --- | --- |
| Row 1 | Archive | Executive Office | Conference Room | Roof |
| Row 2 | Help Desk | Security Office (start) | Server Room | Network Closet |
| Row 3 | Parking Garage | Lobby | Break Room | Vault |

### 3.4 Item Placement

Archive: badge. Security Office: laptop. Network Closet: admin token. Vault: firewall disk. Parking Garage: vault key. Break Room: phishing printout, coffee. Executive Office: USB drive. Help Desk: flashlight, headset. Roof: energy drink. Lobby: skateboard. Conference Room: map, sticky note.

### 3.5 Function Design

| Function | Job |
| --- | --- |
| getChoice(high) | Ask for a number from 0 to high, repeat until valid, return 0 if input ends |
| printOption(number, label, width) | Print one menu entry padded to a fixed width |
| showMainMenu() | Print the main menu horizontally |
| showItemMenu(title, names) | Print the item menu, 4 per row, plus BACK and EXIT |
| pickItem(title, names) | Show the item menu and return an item index, PICK\_BACK, or PICK\_EXIT |
| showMap(roomName, grid, current) | Draw the building with \* on the player's room |

### 3.6 Game Loop (Pseudocode)

```
show title and goal
while playing and movesLeft > 0:
    show room name, moves, score
    if first visit: show room art
    show description, visible items, exits, warnings
    show main menu; choice = getChoice(12)
    if EXIT:        playing = false
    if direction:   check exit, lock, then move and spend a move
    if LOOK:        show art
    if TAKE:        pickItem; move item into inventory; +5
    if USE:         special cases for laptop, USB, coffee, drink, map
    if INVENTORY:   list carried items
    if EXAMINE:     show item description
    if MAP:         showMap
    if HELP/HINT:   show text
if won: win art, bonus, rank
else if out of moves: lose art
show final score
```

### 3.7 Winning Path (Shortest Solution)

Security Office: take laptop. Archive: take badge. Network Closet: take admin token. Garage: take flashlight first from the Help Desk, then take the vault key. Vault: take firewall disk. Server Room: use laptop. The 40-move budget allows this route with moves to spare, and the extras (coffee, drink, skateboard) improve the rank.

## 4. Test Plan (Preview)

| Test | Expected result |
| --- | --- |
| Enter letters at a menu | Message shown, game keeps running |
| Enter 99 or -1 | Range message shown |
| Walk into a wall | "You can't go that way", no move spent |
| Enter Closet without badge | Door locked message |
| Enter Vault without key | Vault locked message |
| Take an item in the dark Garage | Too dark message |
| Use laptop outside the Server Room | No secure connection message |
| Use laptop missing token or disk | Asks for the missing item |
| Use coffee or energy drink | Moves increase and item is gone |
| Carry skateboard and move twice | Second move is free |
| Run out of moves | Lose screen |
| Complete the winning path | Win screen, bonus, rank |
| End input (Ctrl+D) | Game exits cleanly |

## 5. Release Plan (Preview)

Compile with g++ -std=c++11 midnight\_breach.cpp -o midnight\_breach, run ./midnight\_breach, and submit the .cpp, README.md, HOW\_IT\_WORKS.md, and this document.

## 6. Prompt Log

| # | Prompt | Purpose |
| --- | --- | --- |
| 1 | "Help me build a C++ choose-your-own-adventure for CSC 134 using loops and arrays. It should be a different adventure from my earlier modules." | Gather requirements and pick the theme |
| 2 | "Expand Midnight Breach to 12 rooms and 14 items, add extra moves and ASCII graphics, and keep it in C++." | Grow the content |
| 3 | "Combine Midnight Breach with the M6BONUS1 menu program: every menu needs at least 10 numbered options, option 0 is always EXIT, and print menus horizontally." | Merge menu requirements |
| 4 | "Add the project proposal: the SDLC (Gather Requirements, Design, Implement, Test, Release), with requirements and design documents plus the relevant prompts." | This document |
| 5 | "Write an updated README.md and a HOW\_IT\_WORKS.md to go with the code." | Release documentation |

Note: edit the wording of prompts 1 to 3 and 5 to match exactly what you typed, since your instructor may compare them to your chat history.
