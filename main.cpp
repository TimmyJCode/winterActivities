/* Jack Timmons, jackt@pdx.edu, CS163 Program 1, 9 September 2024
 *
 * The purpose of this file is to serve as the main function of our snow activities program. Here we will call the 
 * necessary functions to manage our client program and ADT to specification. 
 */

#include "snow.h"
using namespace std;

int main()
{
	activityList myList;
	displayMenu();
	mainMenu(myList);
	return 0;
}


