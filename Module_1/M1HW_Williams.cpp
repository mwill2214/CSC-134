 // CSC 134
 /*
 M1HW - Movie Talk
 Williams, M
 8/31/2026
 Example: Talk about McLintock.
 */

 #include <iostream>
 using namespace std;

 // Strarting point
 int main() {

    // Purpose: Practice using string, int, and double within natural text.
    // Declare variables
    string movie_name; // Film Title as in IMDB
    int    movie_year; // year of release
    double movie_grossm; // millions of USD of the gross

    // Example: Sherk
    movie_name = "McLintock!";
    movie_year = 1963;
    movie_grossm = 8.7 ; // International: $8.7 million, about half a billion, internationally

   
    // Print the movie blurb
    cout << "The movie " << movie_name << " came out in " << movie_year
     << ". It is a Western comedy starring John Wayne and Maureen O'Hara." << endl;

    cout << "It grossed in total $" << movie_grossm << " million." << endl;

    // Print a movie quote
    cout << "QUOTE:" << endl << "\t\"I'm going to give you a little advice: Don't ever hit a man when he's down.\"" << endl;

    return 0; // no errors
 }
