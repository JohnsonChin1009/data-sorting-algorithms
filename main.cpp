#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // for std::remove

using namespace std;

// Define the Property structure
struct Property {
    string adsId;
    string propertyName;
    int completionYear;
    int monthlyRent;
    string location;
    string propertyType;
    int numberOfRooms;
    int parkingSpaces;
    int numberOfBathrooms;
    int sizeInSqFt;
    string furnishingStatus;
};

// Function to remove non-digit characters from a string
string removeNonDigit(const string& str) {
    string result;
    for (char c : str) {
        if (isdigit(c)) {
            result += c;
        }
    }
    return result;
}

// Function to import data from a CSV file
int importData(const string& filename, Property*& properties) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open the file: " << filename << endl;
        return 0; // Return 0 if file opening fails
    }

    string line;
    getline(file, line); // Skip the header line

    // Use a vector to store properties temporarily
    vector<Property> tempProperties;

    while (getline(file, line)) {
        stringstream ss(line);
        Property property;
        string value;

        // Read each value separated by comma
        getline(ss, property.adsId, ','); // ads_id
        getline(ss, property.propertyName, ','); // prop_name

        getline(ss, value, ','); // completion_year
        property.completionYear = stoi(value); // Convert string to int

        getline(ss, value, ','); // monthly_rent
        value = removeNonDigit(value); // Remove "RM" and spaces
        property.monthlyRent = stoi(value); // Convert string to int

        getline(ss, property.location, ','); // location
        getline(ss, property.propertyType, ','); // property_type

        getline(ss, value, ','); // rooms
        property.numberOfRooms = stoi(value); // Convert string to int

        getline(ss, value, ','); // parking
        property.parkingSpaces = stoi(value); // Convert string to int

        getline(ss, value, ','); // bathrooms
        property.numberOfBathrooms = stoi(value); // Convert string to int

        getline(ss, value, ','); // size
        property.sizeInSqFt = stoi(removeNonDigit(value)); // Remove non-digit characters and convert to int

        getline(ss, property.furnishingStatus, ','); // furnished

        // Add the constructed Property object to the vector
        tempProperties.push_back(property);
    }

    file.close();

    // Allocate memory for the properties array based on the number of properties read
    int propertyCount = tempProperties.size();
    properties = new Property[propertyCount];

    // Copy the properties from the vector to the dynamically allocated array
    for (int i = 0; i < propertyCount; ++i) {
        properties[i] = tempProperties[i];
    }

    return propertyCount;
}

// Function to test the importData function
int main() {
    Property* properties = nullptr;
    string filename = "mudah-apartment-kl-selangor.csv";
    int propertyCount = importData(filename, properties);

    // Print out imported data for verification
    for (int i = 0; i < propertyCount; ++i) {
        const Property& property = properties[i];
        cout << "Ads ID: " << property.adsId << endl;
        cout << "Property Name: " << property.propertyName << endl;
        cout << "Completion Year: " << property.completionYear << endl;
        cout << "Monthly Rent: " << property.monthlyRent << endl;
        cout << "Location: " << property.location << endl;
        cout << "Property Type: " << property.propertyType << endl;
        cout << "Rooms: " << property.numberOfRooms << endl;
        cout << "Parking: " << property.parkingSpaces << endl;
        cout << "Bathrooms: " << property.numberOfBathrooms << endl;
        cout << "Size: " << property.sizeInSqFt << endl;
        cout << "Furnished: " << property.furnishingStatus << endl;
        cout << endl;
    }

    // Free the dynamically allocated memory
    delete[] properties;

    return 0;
}
