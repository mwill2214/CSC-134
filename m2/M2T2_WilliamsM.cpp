// CSC 134
// M2T1 - Receipt
// WilliamsM
// 14 September 2026
// Build a receipt that looks like a receipt


#include <iostream>
// iomanip lets you use setw() and setprecision() and fixed
#include <iomanip>
using namespace std;

int main()  {

   // -----------------------------------------
    // Step 1: Declare the variables
    // -----------------------------------------
    
    
    // ex: "Chicken Sandwhich"
     string meal_name;   
     
    // The price of the meal before tax
    double meal_price = 5.99;
    
    // The tax rate is 8%, written as 0.08
    double tax_rate = 0.08;  // double tax_rate; Percent
    
    // This variable will store the dollar amount of tax
    double tax_amount;
    
    // This variable will store the final total // $. meal + tax
    double total;

	// INPUT
	// Right now, nothing. They pick exactly one sandwhich.
	// For now, hard code some values

	meal_name = "chicken Sandwhich"; // pick your own if you want
	meal_price = 5.99;
	tax_rate   = 0.08; // 8%
	
	//PROCESSING
	// tax $ is the meal $ times the tax rate
	tax_amount = meal_price * tax_rate;
	total      = meal_price + tax_amount;

	
	// OUTPUT	
	// TODO: Print like a receipt
	string line = "-----------------------------------------";
	cout << line << endl;
	// Set width of columns and set 2 decimal places
	// requires up top this line: #include <iomanip>
	cout << setprecision(2) << fixed;
	cout << setw(20) << meal_name << setw(10) << meal_price << endl;
	cout << setw(20) << "tax: " << setw(10) << setw(10) << tax_amount << endl;
	cout << line << endl;
	cout << setw(20) << "Total: " << setw(10) << total << endl;
	

	cout << "Thank You Come Again" << endl << endl;
	
	return 0;  // no errors
	
}
	
	