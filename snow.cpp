/* Jack Timmons, jackt@pdx.edu, 9 September 2024
 *
 * The purpose of this file is to hold the source code for our snow activities ADT and client program.
 */

#include "snow.h"
using namespace std;

/******************** activityData Class: *******************************************/

// This function will serve as the default constructor of the activityData class. The constructor will initialize all
// data members of an activityData object to zero or null.
activityData::activityData()
{
	name = nullptr;
	description = nullptr;
	equipment = nullptr;
	cost = 0.0;

	/* For testing purposes:
	if(!name && !description && !equipment && cost == 0.0)
		cout << "Data pointers set to null and cost set to 0.0" << endl;
	*/
}

// This function is the destructor for the activityData class. The destructor will release all dynamic memory
// allocated to the activityData class object upon the expiry of its lifetime. 
activityData::~activityData()
{
	// Release memory of dynamically allocated char arrays
	if(name)
		delete [] name;
	if(description)
		delete [] description;
	if(equipment)
		delete [] equipment;
	// Set pointers to null
	name = description = equipment = nullptr;
	cost = 0.0;
}

// This function will take in the name of an activity (char array), a description of the activity (char array), any 
// equipment needed for the activity (char array) and the cost of the activity (char array) from the client program
// and will read the data into an activityData class object. The function will dynamically allocate the necessary
// memory for the char arrays name, description, and equipment, and will copy the passed in arrays over.
// If the function successfully loads the passed data to the activityData object, a 1 will be returned- signifying
// success. If the function cannot successfully load the data to the activityData object, a 0 will be returned-
// signiftying failure
int activityData::loadActivity(char passedName[], char passedDescription[], char passedEquipment[], float passedCost)
{
	// Ensure that no null values have been passed in arrays, and that cost is not negative
	if(!passedName || !passedDescription || !passedEquipment || 0 > passedCost)
		return 0; 
	// Allocate memory for name and copy over
	name = new char[strlen(passedName) + 1];
	strcpy(name, passedName);
	// Allocate memory for description and copy over
	description = new char[strlen(passedDescription) + 1];
	strcpy(description, passedDescription);
	// Allocate memory for equipment and copy over
	equipment = new char[strlen(passedEquipment) + 1];
	strcpy(equipment, passedEquipment);
	// Copy over cost
	cost = passedCost;
	return 1;
}

// This function will take in an activityData class object and copy over the private data members to a new
// activityData object. This will come in handy when passing data from the client program to the ADT, as we can
// then package the data into an activityData object, pass the object, and use the loadActivity function to copy
// the data into the new node in the LLL of activities. 
// The function will return a 1 if the loading of the data to the new object was successful, or a 0 if the data
// could not be loaded due to null values or a sub-zero cost.
int activityData::copyActivity(activityData & passedActivity)
{	
	return loadActivity(passedActivity.name, passedActivity.description, passedActivity.equipment, passedActivity.cost);
}

// This function will display the data members of an activityData object, allowing for the client program to display 
// information on a snow activity to the user
int activityData::display()
{
	cout << "Name: " << name << endl << "Description: " << description << endl << "Equipment Needed: " << equipment
	     << endl << "Cost: " << cost << endl << endl;
	return 1;
}


// Compares to see if the name passed in via argument is the same as the name of an activityData object- if the
// names match, return true
bool activityData::isMatch(char * passedName)
{
	if(strcmp(name, passedName) == 0)
		return true;
	else
		return false;
}

// This function compares the name of a passed activity object to the name of the calling activityData object using
// strcmp, and returns the value of the compare function. This allows for direct access to the names of the
// activityData objects, which will make it easier to avoid using getter functions in our ADT. The function will
// return a value greater than zero if the passed activity belongs before the calling activity in the list,
// zero if the two activities have the same name, or a negative value if the passed activity goes after the
// calling activity in the list.
int activityData::compareName(activityData & passedActivity)
{
	return strcmp(name, passedActivity.name);
}

int activityData::write(ofstream & outFile)
{
    if(!outFile) return -1;
    outFile << "Activity Name: " << name << '\n'
            << "Activity Description: " << description << '\n'
            << "Equipment Needed for Activity: " << equipment << '\n'
            << "Activity Cost: " << cost << '\n';
    return 1;
}



//**************** activityList Class: ********************************

// This function serves as the default constructor for our ADT activitiesList class. The constructor will set the
// head pointer to the list of activity nodes to null.
activityList::activityList()
{
	head = nullptr; // constructor is working and sets head to null; 

	/* For testing purposes:
	if(!head)
		cout << "Head is initialized and set to null" << endl;
	*/
}

// This function serves as the destructor for our ADT activitiesList class. The destructor will release all of
// the dynamic memory allocated to the activitiesList class.
activityList::~activityList()
{
	activityNode * temp = nullptr;
	activityNode * current = head;
	locationNode * locTemp = nullptr;
	locationNode * locationCurrent = current -> head;
	while(head)
	{
		// If there are locations at this node, we must deallocate them
		while(locationCurrent)
		{
			// Hold the rest of the list of locations
			locTemp = locationCurrent -> next;
			// Deallocate location dynamic memory
			delete[] locationCurrent -> name;
			delete[] locationCurrent -> description;
			delete[] locationCurrent -> reservation;
			locationCurrent -> numParkingSpots = 0;
			// Re-link list of locations
			locationCurrent = locTemp;
		}
		temp = head -> next; // Save the rest of the list
		delete head; // Deallocate head
		head = temp; // Move head up to next node
	}

}

// This function will allow the client program to add a new activity node to the LLL of activities. The function will
// take in an activityData class object containing all of the relevant information about the activity as an argument,
// and will create a new activity node, placing it correctly in the list based on the alphabetical order of the name
// of the activity. 
int activityList::addActivity(activityData & newActivity)
{
	// Create a temp node with the data passed in, then we can decide where to place it
	activityNode * temp = new activityNode;
	if(!temp)
		return 0;
	temp -> activity.copyActivity(newActivity);
	temp -> next = nullptr;

	// Check to see if the new activity already exists in the list
	if(checkMatch(newActivity))
		return -1;

	// If the list is empty or if temp should be the first node in the list
	if(!head || head -> activity.compareName(temp -> activity) > 0)
	{
		temp -> next = head;
		head = temp;
	}
	// If temp should go in the middle or end of the list
	else
	{	
		// Create current and previous pointers to help us traverse
		activityNode * current = head;
		activityNode * previous = nullptr;

		// Traverse current and previous while the name of the current object is less than the name of
		// the temp object
		while(current && current -> activity.compareName(temp -> activity) < 0)
		{
			previous = current;
			current = current -> next;
		}
		previous -> next = temp;
		temp -> next = current;   
	}
	
	return 1;
}

// This function will check if the name of an activityData object already appears in the list of activities,
// using the compareName function from the activityData class to check each activity in the list. If a match is
// found, a 1 will be returned, signaling to the client program that the activity already appears in the list,
// meaning another activity of that name should not be added to the list.
bool activityList::checkMatch(activityData & passedActivity)
{
	// If the list is empty, there is no way a matching activity can be here
	if(!head) return 0;
	activityNode * current = head;
	while(current)
	{
		// If the compareName function returns 0, a match has been found
		if(!current -> activity.compareName(passedActivity))
			return 1;
		current = current -> next;
	}
	// If we get through the list without finding a match, return 0 indicating no match found
	return 0;
}

// This function will allow the client to display the entire list of activities to the user. The function will use
// a recursive solution to displaying the data contained in the activityData object in each activity node, using
// the display member function from the activityData class. The function will return 0 if the list is empty, and
// will return a positive integer if nodes are displayed.
// Base case: If head is null, there is nothing to display
// Problem breakdown: We can break down the problem, one node at a time, by passing head -> next in each
// 		      recursive call
int activityList::displayAll()
{
	// If the list is empty, there is nothing to display
	if(!head) return 0;
	/* Iterative solution:
	activityNode * current = head;
	while(current)
	{
		current -> activity.display();
		current = current -> next;
	}
	return 1; */
	return displayAll(head);
	
}

// This function will serve as the private, recursive portion of the displayAll function. The 
// function takes in a pointer to an activityNode object, and displays all of the nodes in the 
// list of activities.
int activityList::displayAll(activityNode * head)
{
	if(!head) return 1;
	if(!head -> activity.display())
		return 0;   
	
	return displayAll(head -> next);

}

// This function will allow the client program to add a new location to an activity in our list.
// The function takes in the name of the activity that the user wishes to add a location to, a 
// description of the activity, the reservation requirements at this location, and the number
// of parking spots available at the location. It then searches for an existing activity in the
// list, and if one is found, it allocates the memory for the location data and copies it into a 
// new locationNode. That node is then placed at the front of the list of locationNodes, and a 1
// is returned to signal successfully adding the location. If the list of activities is empty, or
// no matching activities are found, the function returns a 0, indicating that the client program
// should alert the user that adding the location failed.
int activityList::addLocation(char * activityName, char * locationName, char passedDescription[], char passedReservation[], int numSpots)
{
	// If the list is empty, there is nothing to add a location to
	if(!head) return 0;
	activityNode * current = head;
	// Find if the activity exists in the list- if it does, allocate the memory for the location info and
	// copy it in
	while(current)
	{
		// If the name of current matches the passed name, load the location data
		if(current -> activity.isMatch(activityName))
		{
		//	cout << "Match has been found" << endl; // FOR TESTING-REMOVE WHEN POSSIBLE
			locationNode * newLocation = new locationNode;
			newLocation -> name = new char[strlen(locationName) + 1];
			strcpy(newLocation -> name, locationName);
			newLocation -> description = new char[strlen(passedDescription) + 1];
			strcpy(newLocation -> description, passedDescription);
			newLocation -> reservation = new char[strlen(passedReservation) + 1];
			strcpy(newLocation -> reservation, passedReservation);
			newLocation -> numParkingSpots = numSpots;
			// Add the new location to the front of the list of locations
			newLocation -> next = current -> head;
			current -> head = newLocation;
			return 1;
		}
		current = current -> next;
	}
//	cout << "No match found" << endl; // FOR TESTING- REMOVE WHEN POSSIBLE
	return 0; // Activity not found
}

// This function will allow the client program to display all of the locations associated with an
// activity. The function will take in the name of the activity the user wishes to see locations for,
// searches the list of activities for a match, and if one is found, displays all of the locations
// in the list of locationNodes, and returns a 1 signifying success. If the list is empty, or there is
// no activity in the list with a matching name, a 0 will be returned to signify failure.
int activityList::displayLocations(char * activityName)
{
	// If the list is empty, there is nothing to do
	if(!head) return 0;
	activityNode * activityCurrent = head;
	while(activityCurrent)
	{
		if(activityCurrent -> activity.isMatch(activityName))
		{
//			cout << "Matching activity found" << endl; // FOR TESTING, REMOVE WHEN POSSIBLE
			locationNode * locationCurrent = activityCurrent -> head;
			while(locationCurrent)
			{
				cout << "Location Name: " << locationCurrent -> name << endl 
				     << "Location Description: " << locationCurrent -> description << endl
			     	 << "Reservation Requirements: " << locationCurrent -> reservation << endl
		     	     << "Number of Parking Spots: " << locationCurrent -> numParkingSpots 
				     << endl << endl;
				locationCurrent = locationCurrent -> next;
			}
			return 1;
		}
		activityCurrent = activityCurrent -> next;
	}
	// If we have gotten this far, no match has been found
//	cout << "No match found" << endl; // FOR TESTING- REMOVE WHEN POSSIBLE
	return 0;
}

// This function will allow the client program to display the reservation requirements
// for a given activity and location. The function takes in the name of the activity and
// the name of the location as arguments, then works to find a matching activity in the
// list. If the list of activities is empty, or the function does not find a matching 
// activity, the function will return a 0. If the function finds a matching activity, 
// it then checks for a matching location. If a matching location is found, the function
// then displays the reservation requirements associated with that location. If there is
// not a matching location in the list of locations, a -1 will be returned.
int activityList::displayReservation(char * activityName, char * locationName)
{
	// If the list is empty, there is nothing to display
	if(!head) return 0;
	activityNode * activityCurrent = head;
	while(activityCurrent)
	{
		if(activityCurrent -> activity.isMatch(activityName))
		{
			// If the list of locations is empty, there is nothing to display
			if(!activityCurrent -> head) return 0;
			locationNode * locationCurrent = activityCurrent -> head;
			while(locationCurrent)
			{
				if(strcmp(locationCurrent -> name, locationName) == 0) 
				{
					cout << "Reservation Requirements: " 
					     << locationCurrent -> reservation << endl;
					return 1;
				}
				locationCurrent = locationCurrent -> next;
			}
			// If we reach this point, no matching locations have been found
			return -1;
		}
		activityCurrent = activityCurrent -> next;
	}
	// If we reach this point, no matching activities have been found
	return 0;
}


// This function will allow the client program to remove a paticular activity from the list of
// activities. The function takes in the name of the activity that the user wishes to remove,
// searches the list for an activity with the same name, and if one is found, it deletes the node
// associated with that activity, deallocating all of the dynamic memory associated with the 
// activity and the list of locations associated with that activity, and returns a 1 to signify
// that the removal of the activity was successful. If the list of activities is empty, the
// function returns a 0. If the list of activities is not empty, but no matching activity was 
// found, the function returns a -1. 
int activityList::removeActivity(char * activityName)
{
	// If the list of activities is empty, there are no locations to remove
	if(!head) return 0;
	activityNode * activityCurrent = head;
	activityNode * activityPrevious = nullptr;
	while(activityCurrent)
	{
		// If a matching activity is found, deallocate all dynamic memory associated
		if(activityCurrent -> activity.isMatch(activityName))
		{
			// Deallocate locations
			if(activityCurrent -> head)
			{
				locationNode * locCurrent = activityCurrent -> head;
				while(locCurrent)
				{
					// Hold onto the rest of the locations
					locationNode * temp = locCurrent -> next;
					// Deallocate dynamic memory
					if(locCurrent -> name)
						delete [] locCurrent -> name;
					if(locCurrent -> description)
						delete [] locCurrent -> description;
					if(locCurrent -> reservation)
						delete [] locCurrent -> reservation;
					locCurrent -> name = nullptr;
					locCurrent -> description = nullptr;
					locCurrent -> reservation = nullptr;
					locCurrent -> numParkingSpots = 0;
					delete locCurrent;
					locCurrent = temp;
				}
			}
			if(activityPrevious) // No need to change head
				activityPrevious -> next = activityCurrent -> next;
			else // Remove the first node
				head = activityCurrent -> next;	
			delete activityCurrent;
			// Reconnect the list around the deleted node
			return 1;
		}
		// Traverse current and previous
		activityPrevious = activityCurrent;
		activityCurrent = activityCurrent -> next;
	}
	// If we have reached this point, no matching activity is in the list
	return -1;
}

int activityList::writeToFile(char * fileName)
{
    // Open the file passed by the client function
    ofstream outFile(fileName);
    if(!outFile)
        return -1;

	activityNode * activityCurrent = head;
    while(activityCurrent)
    {
        // Write the activity data to the file
        int writeActivityData = activityCurrent -> activity.write(outFile);
        // If the file could not be written to by the member function, return an
        // error
        if(!writeActivityData) return -1;
        outFile << "\nLocations: \n";
        // Write the locations associated with this activity to the file
        locationNode * locationCurrent = activityCurrent -> head;
        while(locationCurrent)
        {
             outFile << "\tLocation Name: " << locationCurrent -> name << '\n'
                     << "\tLocation Description: " << locationCurrent -> description << '\n'
                     << "\tReservation Requirements: " << locationCurrent -> reservation << '\n'
                     << "\tNumber of Parking Spots: " << locationCurrent -> numParkingSpots 
                     << '\n' << '\n';
            locationCurrent = locationCurrent -> next;
        }
        activityCurrent = activityCurrent -> next;
    }
    return 1;
}

// Client program functions:

// This function will be used by the client program to read in the data members for each activity from the user. The 
// function will then call the loadActivity member function of the activityData class to load the data members to
// the activityData object passed in the arguments of the function call.
// If the loadActivity function returns a 1, the this function will report success to the user. If the loadActivity
// function returns a zero, this function reports that the program failed to load the activity information to the 
// user.
void passActivityInfo(activityData & anActivity)
{
	char name[26];
	char description[101];
	char equipment[101];
	float cost;

	// Gather the data needed to pass to member function to create activity:
	cout << "Please enter the name of the activity you wish to add: " << endl;
	cin.get(name, 26, '\n');
	cin.ignore(100, '\n');

	cout << "Please enter a description of the activity: " << endl;
	cin.get(description, 101, '\n');
	cin.ignore(100, '\n');

	cout << "Please enter the equipment needed for this activity: " << endl;
	cin.get(equipment, 101, '\n');
	cin.ignore(100, '\n');

	cout << "Please enter the cost of the activity: " << endl;
	cin >> cost;
	cin.ignore(100, '\n');	     
	

	// Now that we have all the information, we can pass it to the loadActivity member function
	int success = anActivity.loadActivity(name, description, equipment, cost);
	if(success)
		cout << "Activity information saved" << endl;
	else
		cout << "Error loading activity information" << endl;
}

// This client program function will allow the user to add new locations to an activity in the list.
// The function will take in the list of activities by reference, prompt the user for the relevant data,
// read in the data, and pass that information to the addLocation member function of the activityList 
// class. If the location is successfully added, the addLocation function will return a 1, and if there
// is not a matching activity in the list, the addActivity function will return a 0. That return will be 
// used by this function to output a message alerting the user to the success or failure to add the location.
void passLocationInfo(activityList & myList)
{
	char activityName[100];
	char locationName[100];
	char userDescription[100];
	char userReservation[100];
	int numSpots;
	cout << "Please enter the name of the activity you'd like to add a location to:" << endl;
	cin.get(activityName, 100, '\n');
	cin.ignore(100, '\n');
	// Here is where I want to add a check to see if the name is in the list- need to modify the checkMatch
	// function to accept char * names so I can pre-check that condition
	
	cout << "Please enter the name of the location you would like to add:" << endl;
	cin.get(locationName, 100, '\n');
	cin.ignore(100, '\n');

	cout << "Please enter a description of the location you would like to add:" << endl;
	cin.get(userDescription, 100, '\n');
	cin.ignore(100, '\n');

	cout << "Please enter whether or not reservations are required, and if so, the requirements:" << endl;
	cin.get(userReservation, 100, '\n');
	cin.ignore(100, '\n');

	cout << "Please enter the number of parking spots available at this location:" << endl;
	cin >> numSpots;
	cin.ignore(100, '\n');

	// Now we can load the information to the appropriate node
	int success = myList.addLocation(activityName, locationName, userDescription, userReservation, numSpots);
	if(success)
		cout << "Location added successfully!" << endl;
	else
		cout << "Error adding location- no matching activity found" << endl;
}
// This function will get the name of the file that the user would like to open,
// and will pass it to the activityList class writeToFile function, so that the
// user's activities and locations can be saved externally
void exportToFile(activityList & myList)
{
    char fileName[50];
    cout << "Enter the name of the file you wish to save your activites to: ";
    cin.get(fileName, 50, '\n');
    cin.ignore(100, '\n');
    int success = myList.writeToFile(fileName);
    if(success == -1)
        cout << "Error saving activities- could not open file" << endl;
    else if(success)
        cout << "Activities saved to " << fileName << " successfully!" << endl;
}

// Menu Functions:

// This client function will serve to display the menu of options to the user, allowing them to view the 
// available options to manage their list of activities.
void displayMenu()
{
	cout << "Welcome to the snow activity management system. Please select from a menu option below to begin:"
	     << endl << "1: Add a new snow activity" << endl 
	     << "2: Display all activities" << endl 
	     << "3: Add a location to an activity" << endl
	     << "4: Display all locations for an activity" << endl
	     << "5: Display reservation requirements for an activity/location:" << endl 
	     << "6: Remove an activity from the list:" << endl
	     << "7: Export Activities to File:" << endl
	     << "8: Quit:" << endl;
}

// This client function will serve as the control for the main menu. The function takes in the list by reference, and
// prompts the user for their menu selection, based on the options presented by the displayMenu function.
// Based on user selection, the menu will call the appropriate client and member functions to manage the list of
// activities. 
void mainMenu(activityList & myList)
{ 
	int menuSelection;
	cout << "Please enter your menu selection now: ";
	cin >> menuSelection;
	cin.ignore(100, '\n');
	// Include check to make sure correct input here
	switch(menuSelection)
	{
		case 1:
		{
			char response {'Y'};
			while(toupper(response) == 'Y')
			{
				activityData newActivity;
				passActivityInfo(newActivity);
				int success = myList.addActivity(newActivity);
				if(!success)
					cout << "Error loading new activity- unable to create new activity" << endl;
				if(success < 0)
					cout << "Error adding new activity- activity already exists" << endl;
				else if(success > 0)
					cout << "Activity added to list!" << endl;
				cout << "Would you like to add another activity? Enter Y for yes or N for no" << endl;
				cin >> response;
				cin.ignore(100, '\n');
			}
		}
			break;
		case 2:
		{
			int display = myList.displayAll();	
			if(!display)
				cout << "Error displaying list- no entries found" << endl;
		}
			break;
		case 3:
		{
			passLocationInfo(myList);	
		}
			break;
		case 4:
		{
			char activityName[100];
			cout << "Please enter the name of the activity you wish to view locations for:" << endl;
			cin.get(activityName, 100, '\n');
			cin.ignore(100, '\n');
			if(!myList.displayLocations(activityName))
				cout << "Error displaying locations- no matching activity found" << endl;
		}
			break;
		case 5:
		{
			char activityName[100];
			char locationName[100];
			cout << "Please enter the name of the activity you wish to view reservation requirements for:" << endl;
			cin.get(activityName, 100, '\n');
			cin.ignore(100, '\n');
			cout << "Please enter the name of the location you wish to view reservation requirements for:" << endl;
			cin.get(locationName, 100, '\n');
			cin.ignore(100, '\n');
			int success = myList.displayReservation(activityName, locationName);
			if(!success)
				cout << "Error- no matching activities found" << endl;
			if(success < 0)
				cout << "Error- no matching locations found" << endl;
		}
			break;
		case 6:
		{
			char activityName[100];
			cout << "Please enter the name of the activity you wish to delete:" << endl;
			cin.get(activityName, 100, '\n');
			cin.ignore(100, '\n');
			int success = myList.removeActivity(activityName);
			if(!success)
				cout << "Error: no activities currently saved" << endl;
			if(success < 0)
				cout << "Error: no matching activity found" << endl;
			else
				cout << "Activity successfully removed" << endl;
		}
			break;
        case 7:
        {
            exportToFile(myList);
        }
            break;
		case 8: 
		{
			cout << "Goodbye, see you next time!" << endl;
		}
		
	}		
	if(menuSelection != 8)
	{
		char response;
		cout << "Would you like to return to the main menu? Enter Y for yes or N for no" << endl;
		cin >> response;
		cin.ignore(100, '\n');
		if(toupper(response) == 'Y')
		{
			displayMenu();
			mainMenu(myList);
		}
		else
			cout << "See you next time!" << endl;
	}
}
