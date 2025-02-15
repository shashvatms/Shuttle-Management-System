#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
using namespace std;

// File to store user credentials
const string USERS_FILE = "users.txt";

// Loads users from file into a map (username -> password)
unordered_map<string, string> loadUsers() {
    unordered_map<string, string> users;
    ifstream infile(USERS_FILE);
    if (!infile) {
        // File might not exist on first run; that's OK.
        return users;
    }
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string username, password;
        if (iss >> username >> password) {
            users[username] = password;
        }
    }
    return users;
}

// Saves a new user to the file (appends to the file)
void saveUser(const string &username, const string &password) {
    ofstream outfile(USERS_FILE, ios::app);
    outfile << username << " " << password << "\n";
}

// Function to sign up a new user
// Returns the new username if successful, or an empty string on failure.
string signup() {
    unordered_map<string, string> users = loadUsers();
    string username, password, confirmPassword;
    cout << "\n=== Sign Up ===" << endl;
    cout << "Enter new username: ";
    cin >> username;
    if (users.find(username) != users.end()) {
        cout << "Username already exists. Please try logging in." << endl;
        return "";
    }
    cout << "Enter password: ";
    cin >> password;
    cout << "Confirm password: ";
    cin >> confirmPassword;
    if (password != confirmPassword) {
        cout << "Passwords do not match. Signup failed." << endl;
        return "";
    }
    saveUser(username, password);
    cout << "Signup successful. You can now log in." << endl;
    return username;
}

// Function to log in an existing user
// Returns the logged-in username if successful, or an empty string if failed.
string login() {
    unordered_map<string, string> users = loadUsers();
    string username, password;
    int attempts = 3;
    
    while (attempts > 0) {
        cout << "\n=== Login ===" << endl;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        if (users.find(username) != users.end() && users[username] == password) {
            cout << "Login successful. Welcome, " << username << "!" << endl;
            return username;
        } else {
            cout << "Invalid credentials. Please try again." << endl;
            attempts--;
        }
    }
    
    return "";
}

// Class representing a shuttle stop
class Stop {
public:
    int id;
    string name;
    Stop(int id, const string &name) : id(id), name(name) {}
};

// Class representing a route, which is a sequence of stops
class Route {
public:
    int id;
    string name;
    vector<Stop> stops;
    Route(int id, const string &name) : id(id), name(name) {}
    
    void addStop(const Stop &stop) {
        stops.push_back(stop);
    }
    
    void displayStops() const {
        for (const auto &stop : stops) {
            cout << stop.id << ". " << stop.name << endl;
        }
    }
};

// Class representing a shuttle operating on a specific route
class Shuttle {
public:
    int id;
    string vehicleNumber;
    int capacity;
    int currentOccupancy;
    Route route;
    
    Shuttle(int id, const string &vehicleNumber, int capacity, const Route &route)
        : id(id), vehicleNumber(vehicleNumber), capacity(capacity), currentOccupancy(0), route(route) {}

    bool canBook() const {
        return currentOccupancy < capacity;
    }

    void bookSeat() {
        if (canBook())
            currentOccupancy++;
    }
    
    void display() const {
        cout << "Shuttle " << vehicleNumber 
             << " (Capacity: " << capacity 
             << ", Occupied: " << currentOccupancy << ")\n";
        cout << "Route: " << route.name << "\n";
    }
};

// Class representing a booking for a shuttle ride
class Booking {
public:
    int id;
    string studentName;
    string shuttleVehicle;
    Stop stopFrom;
    Stop stopTo;
    
    Booking(int id, const string &studentName, const Shuttle &shuttle, const Stop &stopFrom, const Stop &stopTo)
        : id(id), studentName(studentName), shuttleVehicle(shuttle.vehicleNumber), stopFrom(stopFrom), stopTo(stopTo) {}

    void display() const {
        cout << "Booking ID: " << id << "\n";
        cout << "Student: " << studentName << "\n";
        cout << "Shuttle: " << shuttleVehicle << "\n";
        cout << "From: " << stopFrom.name << "  To: " << stopTo.name << "\n";
    }
};

int main() {
    // Display initial menu for authentication
    int authChoice;
    string loggedInUser;
    
    while (true) {
        cout << "\n=== Welcome to the Shuttle Management System ===\n";
        cout << "1. Login\n";
        cout << "2. Signup\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> authChoice;
        
        if (authChoice == 1) {
            loggedInUser = login();
            if (!loggedInUser.empty()) break;
        } else if (authChoice == 2) {
            string newUser = signup();
            if (!newUser.empty()) {
                // Optionally, you can log in automatically after signup.
                loggedInUser = newUser;
                break;
            }
        } else if (authChoice == 3) {
            cout << "Exiting..." << endl;
            return 0;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    if (loggedInUser.empty()) {
        cout << "Authentication failed. Exiting." << endl;
        return 1;
    }
    
    // Create some stops
    Stop stop1(1, "Dormitory");
    Stop stop2(2, "Main Gate");
    Stop stop3(3, "Library");
    Stop stop4(4, "Academic Building");

    // Create a route and add stops
    Route route1(1, "Morning Route");
    route1.addStop(stop1);
    route1.addStop(stop2);
    route1.addStop(stop3);
    route1.addStop(stop4);

    // Create a shuttle with capacity 3
    Shuttle shuttle1(1, "SHUTTLE-001", 3, route1);

    // Vector to store bookings
    vector<Booking> bookings;
    int bookingIdCounter = 1;

    int choice;
    while (true) {
        cout << "\n--- Shuttle Management System ---\n";
        cout << "Logged in as: " << loggedInUser << "\n";
        cout << "1. View Shuttle Details\n";
        cout << "2. Book a Ride\n";
        cout << "3. View Bookings\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            shuttle1.display();
            cout << "\nRoute Stops:\n";
            route1.displayStops();
        }
        else if (choice == 2) {
            if (!shuttle1.canBook()) {
                cout << "Sorry, no seats available on this shuttle.\n";
                continue;
            }
            
            int fromStopId, toStopId;
            cout << "Select your starting stop (enter stop id):\n";
            route1.displayStops();
            cin >> fromStopId;
            
            cout << "Select your destination stop (enter stop id):\n";
            route1.displayStops();
            cin >> toStopId;

            bool foundFrom = false, foundTo = false;
            Stop selectedFrom(0, "");
            Stop selectedTo(0, "");
            for (const auto &s : route1.stops) {
                if (s.id == fromStopId) {
                    selectedFrom = s;
                    foundFrom = true;
                }
                if (s.id == toStopId) {
                    selectedTo = s;
                    foundTo = true;
                }
            }
            if (!foundFrom || !foundTo) {
                cout << "Invalid stop selection. Try again.\n";
                continue;
            }
            
            shuttle1.bookSeat();
            Booking newBooking(bookingIdCounter++, loggedInUser, shuttle1, selectedFrom, selectedTo);
            bookings.push_back(newBooking);
            cout << "Booking successful!\n";
        }
        else if (choice == 3) {
            cout << "\n--- All Bookings ---\n";
            if (bookings.empty()) {
                cout << "No bookings yet.\n";
            } else {
                for (const auto &b : bookings) {
                    b.display();
                    cout << "--------------------\n";
                }
            }
        }
        else if (choice == 4) {
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    
    cout << "Exiting Shuttle Management System.\n";
    return 0;
}
