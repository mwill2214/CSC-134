# CSC 134 – M3 Bonus: Craps Game

**Student:** Myra Williams
**Language:** C++
**Development style:** AI-assisted

A console (text-based) Craps game. The player starts with **$100**, places bets, and rolls two dice until they win, lose, or quit.

**Concepts used:** `if` statements, `while` loops, functions, random numbers, input validation.

---

## How to Compile and Run

```bash
g++ -o craps M3BONUS.cpp
./craps
```

(Replace `M3BONUS.cpp` with whatever your source file is named.)

---

## How to Play

1. **Start the game.** You begin with a bankroll of **$100**.
2. **Choose from the menu:**
   - `1` – Play a round
   - `2` – Show the rules
   - `0` – Exit (you keep your current bankroll)
3. **Enter your bet.** It must be at least $1 and no more than your bankroll.
4. **Press Enter to roll** the two dice.
5. **Come-out roll (first roll):**

   | Roll | Result |
   |------|--------|
   | 7 or 11 | You win (a "natural") |
   | 2, 3, or 12 | You lose ("craps") |
   | 4, 5, 6, 8, 9, 10 | That number becomes your **point** |

6. **If you got a point,** keep pressing Enter to roll:
   - Roll your **point** again → you **win**
   - Roll a **7** → you **lose** ("seven out")
   - Any other number → roll again
7. **Payout:** Win = you gain your bet. Lose = you lose your bet.
8. **The game ends** when you choose `0` to exit, or when your bankroll hits **$0**.

---

## What the Program Does, Step by Step

### 1. Setup
- Includes the libraries needed: `iostream` (input/output), `string`, `cstdlib` (`rand`, `srand`), `ctime` (`time`), and `limits` (used to clear bad input).
- Declares the **function prototypes** so `main()` can call functions defined below it.
- Creates a **global variable** `bankroll = 100` so every function can read and change the player's money.

### 2. `main()` – the menu loop
1. Calls `srand(time(0))` **once** to seed the random number generator so dice rolls differ every run.
2. Starts a `while (keep_going)` loop that repeats until the player quits or goes broke.
3. Each pass of the loop:
   - Prints the menu and the current bankroll.
   - Reads the choice with `get_number()`.
   - Uses `if / else if / else`:
     - **1** → calls `play_round()`. If `bankroll <= 0` afterward, prints "Game over!" and stops the loop.
     - **2** → calls `show_rules()`.
     - **0** → prints the final bankroll and stops the loop.
     - **anything else** → prints "Not a valid choice."
4. Returns `0` when the loop ends.

### 3. `roll_die()`
- Returns a random whole number from **1 to 6** using `(rand() % 6) + 1`.

### 4. `roll_two_dice()`
1. Calls `roll_die()` twice (`d1` and `d2`).
2. Adds them into `total`.
3. Prints the result, e.g. `You rolled 3 + 4 = 7`.
4. Returns the total.

### 5. `get_number(string prompt)` – safe input
1. Prints the prompt and tries to read an integer.
2. If it works, returns the number.
3. If the user typed something that isn't a number (like letters):
   - `cin.clear()` resets the error state.
   - `cin.ignore(...)` throws away the bad input.
   - Prints "Please enter a number." and asks again.
4. This keeps the program from crashing or looping forever on bad input.

### 6. `show_rules()`
- Prints the come-out roll rules, the point rules, and how winning and losing affect the bet.

### 7. `play_round()` – one full round
1. **Get a valid bet:** asks for a bet and loops until it is between $1 and the current bankroll.
2. **Wait for Enter:** `cin.ignore(...)` clears the leftover newline, then `cin.get()` waits for the player to press Enter.
3. **Come-out roll:** calls `roll_two_dice()` and checks:
   - 7 or 11 → `won = true`
   - 2, 3, or 12 → `won = false`
   - otherwise → the roll is saved as `point`
4. **Point phase** (only if a point was set): a `while (true)` loop that
   - waits for Enter, rolls the dice,
   - if the roll equals the point → win, `break`,
   - if the roll is 7 → lose, `break`,
   - otherwise prints "Roll again..." and repeats.
5. **Update money:** if `won`, `bankroll = bankroll + bet`; otherwise `bankroll = bankroll - bet`.
6. **Return** `won` (`true` if the player won).

---

## Sample Run

```
=== CRAPS ===
Bankroll: $100
1. Play a round
2. Rules
0. Exit
Choice: 1
Enter your bet: $20
Press Enter to roll...
You rolled 2 + 4 = 6
Your point is 6. Roll it again before a 7!
Press Enter to roll...
You rolled 3 + 3 = 6
You made your point! You win!
```

Bankroll after this round: **$120**.

---

## Notes

- Dice are simulated with `rand()`, seeded once with the current time.
- A global `bankroll` is used for simplicity so all functions can share it.
- Input validation protects against non-numeric entries and out-of-range bets.
