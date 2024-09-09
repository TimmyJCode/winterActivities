/* Jack Timmons, jackt@pdx.edu, 9 September 2024
 
 The purpose of this program is to create an ADT for managing a LLL of snow activities, and a menu interface for 
 testing the function of the ADT. The data structure that we will be using for our ADT is a linear linked list of
 node structs containting the necessary data regarding the snow activity in a struct object, a head pointer to 
 another linked list of locations that the activity is available in, and a next pointer to the next snow activity.
 The greater list of activities will be managed by the activityList class, and the data for each activity will 
 be managed by the activityData struct. The list of activities will be sorted alphabetically by name. 
 The purpose of this file is to hold the function prototypes and struct / class interfaces for our ADT and client
 program. 
 */

#include <iostream>
#include <cctype>
#include <cstring>
#include <fstream>

using namespace std;
// Struct / class interfaces:

// This class will combine all of the data necessary for each snow activity into one passable package.
// Each node in the LLL of snow activities will contain one of these class objects to hold the relevant data for the
// activity. 
class activityData
{
	public:
		activityData();
		~activityData();
		// Load activity from the client:
		int loadActivity(char passedName[], char passedDescription[], char passedEquipment[], float passedCost);
		// Load activity from another activityData object:
		int copyActivity(activityData & passedActivity);
		// Display activity data:
		int display();
		// Determine if a name matches the name of an activity:
		bool isMatch(char * passedName);
		// Compare the name of an activity with another
		int compareName(activityData & passedActivity);
        // Write the data for the activity to a file
        int write(ofstream & outFile);
		
	private:
		char * name; // The name of the activity
		char * description; // A description of the activity
		char * equipment; // The equipment needed for the activity
		float cost; // The cost of the activity
};


// This struct will be a node in a list of locations that provide a certain activity. Each location
// node will contain a description of the location, whether or not the location requires reservations, the number 
// of parking spots available at the location, and a pointer to the next location node.
struct locationNode
{
	char * name; // The name of the location
	char * description; // A description of the location
	char * reservation; // Whether or not reservations are required at this location
	int numParkingSpots; // The number of parking spots at this location
	locationNode * next; // Pointer to the next location in the list of locations offering an activity
};

// This struct will be used to represent each node in our LLL of snow activities. Each node will consist of an 
// activityData class object holding the information about the activity, a head pointer to the location LLL for that
// activity, and a pointer to the next activity node in the list.
struct activityNode
{
	activityData activity; // Struct containing data on activity
	locationNode * head; // Pointer to list of locations associated with this activity
	activityNode * next; // Pointer to the next activity in the list
};

// This class will represent our ADT and manage our LLL of snow activities. The member functions of this class will
// give the client program the ability to add new activities, display all the activities in the list, add location
// information to an activity, display all of the locations associated with an activity, dsiplay the reservation 
// requirements at a location, and remove a particular activity. Each node in the list of activities will represent
// one activity- containing an activityData class object that holds the relavant information about the activity, a
// head pointer to the LLL of locations associated with the activity, and a next pointer to the next activity node
// in the list.
class activityList
{
	public:
		// Default constructor: initialize head to null
		activityList(); 
		// Destuctor: release all dynamic memory for all activities / locations
		~activityList(); 
		// Add a new activity to the list
		int addActivity(activityData & newActivity);
		// Check if the name of an activity match any of the activities in the list
		bool checkMatch(activityData & newActivity);
		// Display all of the activities in the list
		int displayAll();
		// Add a location to an activity with this name
		int addLocation(char * activityName, char * locationName, char passedDescription[], char passedReservation[], int numSpots);
		// Display all of the locations associated with an activitiy with this name
		int displayLocations(char * activityName);
		// Display the reservation requirements for this activity at this location
		int displayReservation(char * activityName, char * locationName); 
		// Remove an activity with this name from the list
		int removeActivity(char * activityName);
        // Write the list to a file
        int writeToFile(char * fileName);



	private:
		activityNode * head; // Pointer to the first node in the list of activities
		// Recursive portion of displayAll
		int displayAll(activityNode * head);
};	



// Client Function prototypes:
void displayMenu();
void mainMenu(activityList & myList);
void passActivityInfo(activityData & anActivity);
void passLocationInfo(activityList & myList);
void exportToFile(activityList & myList);
