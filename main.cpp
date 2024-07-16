#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono> // For timing
using namespace std;
using namespace std::chrono;

// Defining the structure of the importing data
struct Property {
    string adsId;
    string propertyName;
    int completionYear;
    string monthlyRent;
    string location;
    string propertyType;
    int numberOfRooms;
    int parkingSpaces;
    int numberOfBathrooms;
    int sizeInSqFt;
    string furnishingStatus;
};

// Defining the structure for containing adsId and monthlyRent
struct AdsRent {
    string adsId;
    int monthlyRent;
};

// Function to count the total number of lines in the .csv file
int countLines(const string& filename) {
    ifstream file(filename);
    string line;
    int lineCount = 0;

    while (getline(file, line)) {
        lineCount++;
    }

    return lineCount - 1; // Subtract 1 for the header line
}

// Function to import data from the .csv file into the system memory
Property* importDataFromCSV(const string& filename, int& numberOfProperties) {
    ifstream file(filename);
    string line;

    // Step 1: Count the lines to determine the number of properties
    numberOfProperties = countLines(filename);
    cout << "Number of lines in the file (excluding header): " << numberOfProperties << endl;

    // Step 2: Allocate system memory for the properties
    Property* properties = new Property[numberOfProperties];

    // Step 3: Reset file stream to beginning and skip the header line
    file.clear();
    file.seekg(0, ios::beg);
    getline(file, line); // Skipping the header line

    // Step 4: Read the property data
    int index = 0;
    while (getline(file, line) && index < numberOfProperties) {
        stringstream ss(line);
        string token;

        getline(ss, properties[index].adsId, ',');
        getline(ss, properties[index].propertyName, ',');
        
        getline(ss, token, ',');
        try {
            properties[index].completionYear = token.empty() ? 0 : stoi(token);
        } catch (...) {
            properties[index].completionYear = 0;
        }
        
        getline(ss, properties[index].monthlyRent, ',');
        getline(ss, properties[index].location, ',');
        getline(ss, properties[index].propertyType, ',');

        getline(ss, token, ',');
        try {
            properties[index].numberOfRooms = token.empty() ? 0 : stoi(token);
        } catch (...) {
            properties[index].numberOfRooms = 0;
        }

        getline(ss, token, ',');
        try {
            properties[index].parkingSpaces = token.empty() ? 0 : stoi(token);
        } catch (...) {
            properties[index].parkingSpaces = 0;
        }

        getline(ss, token, ',');
        try {
            properties[index].numberOfBathrooms = token.empty() ? 0 : stoi(token);
        } catch (...) {
            properties[index].numberOfBathrooms = 0;
        }

        getline(ss, token, ',');
        try {
            properties[index].sizeInSqFt = token.empty() ? 0 : stoi(token);
        } catch (...) {
            properties[index].sizeInSqFt = 0;
        }

        getline(ss, properties[index].furnishingStatus, ',');

        index++;
    }
    return properties;
}


// Function to extract the numeric value from the monthly rent string (This is because the monthly rent is in string format)
int extractRent(const string& rentStr) {
    string numericStr;
    for (char c : rentStr) {
        if (isdigit(c)) {
            numericStr += c;
        }
    }
    return numericStr.empty() ? 0 : stoi(numericStr); // Converts the numeric string to an integer OR set it to 0 if empty.
}

// Function to extract the ads_id and monthlyRent from the properties array.
AdsRent* extractAdsRent(Property* properties, int numberOfProperties) {
    AdsRent* adsRents = new AdsRent[numberOfProperties];

    for (int i = 0; i < numberOfProperties; ++i) {
        adsRents[i].adsId = properties[i].adsId;
        adsRents[i].monthlyRent = extractRent(properties[i].monthlyRent);
    }

    return adsRents;
}

// Function to write any data into a text file.
void writeToFile(const string& filename, AdsRent* adsRents, int numAdsRents) {
    ofstream writeFile(filename); // Receive filename as parameter

    // Write header line for better understanding
    writeFile << "adsId,monthlyRent\n";

    // Write data
    for (int i = 0; i < numAdsRents; ++i) {
        writeFile << adsRents[i].adsId << ","
                << adsRents[i].monthlyRent << "\n";
    }

    writeFile.close();
}

// To turn the array into a heap
void heapify(AdsRent arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].monthlyRent > arr[largest].monthlyRent) {
        largest = left;
    }

    if (right < n && arr[right].monthlyRent > arr[largest].monthlyRent) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Function: Sorting the heapified array (heapified-hehe get it?)
void heapSort(AdsRent arr[], int n) {
    // Build the heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Extracts an element from the heap one by one
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Merge Sort function
void merge(AdsRent arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays dynamically
    AdsRent* L = new AdsRent[n1];
    AdsRent* R = new AdsRent[n2];

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    // Merge the temp arrays back into arr[l..r]
    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].monthlyRent <= R[j].monthlyRent) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Free dynamically allocated memory
    delete[] L;
    delete[] R;
}

void mergeSort(AdsRent arr[], int l, int r) {
    if (l >= r) {
        return; // Base case: Array with 0 or 1 element
    }

    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Main function
int main() {
    // Step 1: Declare the filename to use:
    string inputFilename = "mudah-apartment-kl-selangor.csv";
    string outputFilename = "ads-rent.txt";

    // Step 2: Call the importDataFromCSV function:
    cout << "Importing data from: " << inputFilename << endl;
    int numberOfProperties = 0;
    Property* properties = importDataFromCSV(inputFilename, numberOfProperties);

    // Step 3: Extract adsId and monthlyRent into AdsRent structure
    AdsRent* adsRents = extractAdsRent(properties, numberOfProperties);

    // Step 4: Write the extracted data to a new file
    cout << "Writing data to: " << outputFilename << endl;
    writeToFile(outputFilename, adsRents, numberOfProperties);

    // Step 5: Heap Sort the adsRents array based on monthlyRent
    auto startHeap = high_resolution_clock::now(); // Start measuring heap sort time
    heapSort(adsRents, numberOfProperties);
    auto stopHeap = high_resolution_clock::now();  // Stop measuring heap sort time
    auto durationHeap = duration_cast<milliseconds>(stopHeap - startHeap);
    cout << "Heap Sort Time: " << durationHeap.count() << " milliseconds" << endl;

    // Step 6: Write sorted data to result-heap-sort.txt
    string sortedFilename1 = "result-heap-sort.txt";
    cout << "Writing heap sorted data to: " << sortedFilename1 << endl;
    writeToFile(sortedFilename1, adsRents, numberOfProperties);

    // Step 7: Merge Sort the adsRents array based on monthlyRent
    auto startMerge = high_resolution_clock::now(); // Start measuring merge sort time
    mergeSort(adsRents, 0, numberOfProperties - 1);
    auto stopMerge = high_resolution_clock::now();  // Stop measuring merge sort time
    auto durationMerge = duration_cast<milliseconds>(stopMerge - startMerge);
    cout << "Merge Sort Time: " << durationMerge.count() << " milliseconds" << endl;

    // Step 8: Write sorted data to result-merge-sort.txt
    string sortedFilename2 = "result-merge-sort.txt";
    cout << "Writing merge sorted data to: " << sortedFilename2 << endl;
    writeToFile(sortedFilename2, adsRents, numberOfProperties);

    // Deallocate memory
    delete[] properties;
    delete[] adsRents;

    return 0;
}