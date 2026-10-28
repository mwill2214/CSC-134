/*
CSC 134
M5LAB1 - Choose Your Own Adventure
Myra Williams
October 26, 2026

Escape from the Coriolis
Your spaceship, the Coriolis, has landed on a strange planet covered in
glowing, swarming creatures. You need a keycard to unlock the ship's
airlock and fly home. You can talk to the creatures or attack them, and
that choice changes how the story ends.

Each part of the story is its own void function. The player makes a
choice, and each choice calls the next function.
*/

#include <iostream>
#include <limits>
#include <cstdlib>
using namespace std;

// ---------------------------------------------------------------
// STEP 1: GAME STATE
// These variables are global so every story function can read and
// change them. They replace the $variables from the Twine story.
// ---------------------------------------------------------------
int  player_health = 10;     // hit points; at 0 or below the game is lost
bool has_keycard   = false;  // true once the player has the keycard
bool attacked      = false;  // true if the player attacked the creatures

// ---------------------------------------------------------------
// STEP 2: FUNCTION PROTOTYPES
// Every function is listed here, above main(), so the compiler
// knows it exists before it sees the full code below.
// ---------------------------------------------------------------
int  get_choice(int low, int high);  // helper: safe menu input
void main_menu();                    // start: the strange encounter
void choice_front_door();            // talk to the creatures
void choice_back_door();             // the ramp outside the ship
void choice_go_home();               // the airlock of the Coriolis
void attack_swarm();                 // attack the creatures
void enter_nest();                   // the glowing cave (keycard)
void launch_ending();                // ending: escape (2 versions)
void defeat_ending();                // ending: game over

int main() {
  cout << "M5LAB1 - Choose Your Own Adventure" << endl;
  cout << "ESCAPE FROM THE CORIOLIS" << endl << endl;
  // Reset the state so every run starts the same way.
  player_health = 10;
  has_keycard = false;
  attacked = false;
  // Start the story.
  main_menu();
  // When main_menu() returns, the story is over.
  cout << "Thanks for playing!" << endl;
  return 0; // finished with no errors
}

// ---------------------------------------------------------------
// STEP 3: FUNCTION DEFINITIONS
// The full code for each function goes here, below main().
// ---------------------------------------------------------------

// get_choice: asks for a number from low to high.
// If the player types letters or a number out of range, it says so
// and asks again. It only returns a valid number.
int get_choice(int low, int high) {
  int choice = 0;
  while (true) {
    cout << "Choose: ";
    cin >> choice;
    if (cin.eof()) {
      // Input ended (for example Ctrl+D), so stop instead of looping.
      cout << endl << "Input ended, quitting game." << endl;
      exit(0);
    }
    if (cin.fail()) {
      // The player typed something that is not a number.
      cin.clear();       // reset cin so it can read again
      choice = low - 1;  // treat it as an invalid choice
    }
    // Throw away the rest of the line the player typed.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (choice >= low && choice <= high) {
      return choice; // valid, give it back to the story function
    }
    cout << "That's not a valid choice, please try again." << endl;
  }
}

// main_menu: the first choice. Matches the "Strange Encounter" passage.
void main_menu() {
  cout << "Main Menu" << endl;
  cout << "As soon as the hatch of your ship, the Coriolis, opens, you see" << endl;
  cout << "creatures swarming around you. There are more than you can" << endl;
  cout << "easily count, and they are all in constant motion." << endl;
  cout << "Do you:" << endl;
  cout << "1. Try talking to the creatures" << endl;
  cout << "2. Walk down the ramp of your spaceship" << endl;
  cout << "3. Retreat to the airlock" << endl;
  cout << "4. [Quit]" << endl;
  int choice = get_choice(1, 4);

  if (1 == choice) {
    choice_front_door();   // talk
  } else if (2 == choice) {
    choice_back_door();    // the ramp
  } else if (3 == choice) {
    choice_go_home();      // the airlock
  } else {
    cout << "Ok, quitting game" << endl;
    return; // go back to main()
  }
}

// choice_front_door: the "Talk" passage. Safe, friendly path.
void choice_front_door() {
  cout << endl << "You raise a hand and speak slowly. The creatures freeze," << endl;
  cout << "then ripple with soft blue light. A chime-like hum fills the" << endl;
  cout << "air, and several of them drift toward a glowing cave." << endl;
  cout << "Do you:" << endl;
  cout << "1. Follow the creatures to their nest" << endl;
  cout << "2. Walk down the ramp to explore alone" << endl;
  cout << "You have " << player_health << " hp." << endl;
  int choice = get_choice(1, 2);

  if (1 == choice) {
    enter_nest();
  } else {
    choice_back_door();
  }
}

// choice_back_door: the "Ramp" passage. A hub with three exits.
// The attack option only shows up if the player has not attacked yet.
void choice_back_door() {
  cout << endl << "You walk down the ramp. The creatures part around you" << endl;
  cout << "without ever touching you. Ahead is the mouth of a glowing" << endl;
  cout << "cave. Behind you is the airlock of the Coriolis." << endl;
  cout << "Do you:" << endl;
  cout << "1. Enter the glowing cave" << endl;
  cout << "2. Return to the airlock" << endl;
  int max_choice = 2;
  if (!attacked) {
    cout << "3. Attack the creatures" << endl;
    max_choice = 3;
  }
  cout << "You have " << player_health << " hp." << endl;
  int choice = get_choice(1, max_choice);

  if (1 == choice) {
    enter_nest();
  } else if (2 == choice) {
    choice_go_home();
  } else {
    attack_swarm();
  }
}

// choice_go_home: the "Airlock" passage. Needs the keycard to open.
void choice_go_home() {
  cout << endl << "The Coriolis has sealed its airlock behind you. A panel" << endl;
  cout << "beside the door blinks, waiting for a keycard." << endl;

  if (has_keycard) {
    // The player has the keycard, so the door opens.
    cout << "The keycard flashes green and the door slides open!" << endl;
    cout << "Do you:" << endl;
    cout << "1. Launch the Coriolis" << endl;
    cout << "2. Go back outside" << endl;
    int choice = get_choice(1, 2);
    if (1 == choice) {
      launch_ending();
    } else {
      choice_back_door();
    }
  } else {
    // No keycard: the panel shocks the player.
    cout << "The panel flashes red: ACCESS DENIED. A jolt of electricity" << endl;
    cout << "shoots through your hand." << endl;
    player_health = player_health - 4;
    cout << "You have " << player_health << " hp." << endl;
    if (player_health < 1) {
      defeat_ending();   // path 1 to the defeat ending
    } else {
      cout << "Do you:" << endl;
      cout << "1. Go back outside and look for a keycard" << endl;
      get_choice(1, 1);  // only one option, so just wait for a 1
      choice_back_door();
    }
  }
}

// attack_swarm: the "Attack" passage. Costs 5 hp and sets attacked.
void attack_swarm() {
  attacked = true;
  player_health = player_health - 5;
  cout << endl << "You swing at the nearest creature. The whole swarm erupts," << endl;
  cout << "stinging you with sparks of static before scattering toward" << endl;
  cout << "a glowing cave." << endl;
  cout << "You have " << player_health << " hp." << endl;

  if (player_health < 1) {
    defeat_ending();     // path 2 to the defeat ending
  } else {
    cout << "Do you:" << endl;
    cout << "1. Chase the swarm to their nest" << endl;
    cout << "2. Limp back to the airlock" << endl;
    int choice = get_choice(1, 2);
    if (1 == choice) {
      enter_nest();
    } else {
      choice_go_home();
    }
  }
}

// enter_nest: the "Nest" passage. The keycard is here.
// If the player attacked, the keycard costs 3 hp. If not, it is a
// gift and gives 2 hp. After that the player is sent to the airlock.
void enter_nest() {
  cout << endl << "The cave is lit by thousands of tiny glowing eggs." << endl;

  if (!has_keycard) {
    if (attacked) {
      cout << "The creatures guard their nest and zap you as you grab a" << endl;
      cout << "glowing keycard from the floor." << endl;
      player_health = player_health - 3;
    } else {
      cout << "The creatures gently nudge a glowing keycard toward you." << endl;
      cout << "It is a gift." << endl;
      player_health = player_health + 2;
    }
    has_keycard = true;
  } else {
    cout << "The nest is quiet now. There is nothing more to find here." << endl;
  }

  cout << "You have " << player_health << " hp." << endl;

  if (player_health < 1) {
    defeat_ending();     // path 3 to the defeat ending
  } else {
    cout << "Do you:" << endl;
    cout << "1. Return to the airlock" << endl;
    get_choice(1, 1);    // only one option, so just wait for a 1
    choice_go_home();
  }
}

// launch_ending: the "Launch" passage. Ending 1 (two versions).
// The last line depends on whether the player attacked.
void launch_ending() {
  cout << endl << "The engines roar to life and the Coriolis lifts off. Below" << endl;
  cout << "you, the creatures swirl into one glowing spiral, like they" << endl;
  cout << "are waving goodbye." << endl;
  cout << "You escaped with " << player_health << " hp." << endl;
  if (attacked) {
    cout << "You will never forget the sting of that swarm." << endl;
  } else {
    cout << "You made friends on a strange planet today." << endl;
  }
  cout << "THE END" << endl;
}

// defeat_ending: the "Defeat" passage. Ending 2.
// Called from three places: the airlock, the attack, and the nest.
void defeat_ending() {
  cout << endl << "Your vision fades as the glowing creatures drift past," << endl;
  cout << "humming softly." << endl;
  cout << "GAME OVER" << endl;
}
