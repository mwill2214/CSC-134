// M2Lab1_Williams
// CSC 134
// 9/9/26




#include <iostream>
using namespace std;

int main() {

	// 	set up variables
	string product_name;
	int    product_count;
	double product_price; // $USD, per item
	
	// variables for customer side
	int    purchase_count;
	double purchase_total;
	
	
	
	// GET INPUT -- set up the store 
	cout << "STORE SETUP" << endl;
	cout << "Product Name: ";
	cin  >>  product_name; 
	cout <<  "Item Count: ";
	cin  >> product_count;
    cout << "Price Each: $";
    cin  >> product_price;


    // GET INPUT -- Greet user, get their order
    cout << "---------------------------------" << endl;
    cout << "WELCOME, CUSTOMER." << endl;
	cout << "Welcome to the " << product_name << " shop." << endl;
	cout << "Our " << product_name << "(s) are $" << product_price << " each." << endl;
	 
	cout << "How many would you like to buy today? ";
	cin  >> purchase_count;

	// DO THE PROCESSING
	// NOTE: we don't verify the user input -- they can buy more than we have, or a negative amount.
	// This will be fixed in the Loops module.
	purchase_total = purchase_count * product_price;
	
	// PRINT OUTPUT
	cout << "You have ordered " << purchase_count << " " << product_name << "(s). " << endl;
	cout << "Total price: $" << purchase_total << endl;
	cout << "Thank you for shopping with CSC 134." << endl << endl;
	
	return 0; // no errors
	
}
	 
	