#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip> // For std::hex

using namespace std;

const int PAGE_SIZE = 256;  // Page size in bytes
const int FRAME_SIZE = 1024; // Frame size in bytes
const int NUM_PAGES = 1024;    // Number of pages in logical memory
const int NUM_FRAMES = 8;    // Number of frames in physical memory

// Function to generate a random logical address
int generateLogicalAddress() {
    return rand() % (NUM_PAGES * PAGE_SIZE); // Generates a random logical address within the range of logical memory
}

// Function to simulate loading a page from secondary storage into memory
void loadPageIntoMemory(int pageNumber, vector<int>& pageFrameTable, vector<int>& physicalMemory) {
    // Find an available frame in physical memory
    for (int i = 0; i < NUM_FRAMES; ++i) {
        if (physicalMemory[i] == -1) { // Found an empty frame
            physicalMemory[i] = pageNumber; // Load the page into the frame
            pageFrameTable[pageNumber] = i; // Update the page/frame table
            cout << "Page " << pageNumber << " loaded into frame " << i << endl;
            return;
        }
    }
    // If no empty frame is available, perform replacement (e.g., LRU or FIFO)
    // For simplicity, let's assume FIFO (first-in, first-out) replacement
    int frameToReplace = pageFrameTable[pageNumber]; // Get the frame associated with the page
    cout << "Page fault occurred. Evicting page " << physicalMemory[frameToReplace] << " from frame " << frameToReplace << endl;
    physicalMemory[frameToReplace] = pageNumber; // Load the new page into the frame
    pageFrameTable[pageNumber] = frameToReplace; // Update the page/frame table
}

int main() {
    srand(time(nullptr)); // Seed random number generator with current time

    // Initialize page/frame table
    vector<int> pageFrameTable(NUM_PAGES, -1); // Page/frame table initialized with -1 indicating no mapping initially

    // Allocate frames in physical memory
    vector<int> physicalMemory(NUM_FRAMES, -1); // Physical memory initialized with -1 indicating empty frames initially

    // Fill page/frame table and physical memory
    for (int i = 0; i < NUM_PAGES; ++i) {
        pageFrameTable[i] = i % NUM_FRAMES; // Circular mapping of pages to frames ensures each page has a corresponding frame
    }

    // Generate logical addresses and translate them into physical addresses
    // This loop translates 10 random logical addresses to physical addresses using the page/frame table
    for (int i = 0; i < 10; ++i) {
        int logicalAddress = generateLogicalAddress(); // Generates a random logical address
        int pageNumber = logicalAddress / PAGE_SIZE; // Calculates the page number from the logical address
        int offset = logicalAddress % PAGE_SIZE; // Calculates the offset within the page from the logical address

        if (pageNumber < NUM_PAGES) { // Check if the page number is within the valid range
            int frameNumber = pageFrameTable[pageNumber]; // Retrieves the frame number corresponding to the page from the page/frame table
            if (frameNumber == -1) { // Page fault occurred, load page into memory
                loadPageIntoMemory(pageNumber, pageFrameTable, physicalMemory);
                frameNumber = pageFrameTable[pageNumber]; // Update frame number after loading
            }
            int physicalAddress = frameNumber * FRAME_SIZE + offset; // Calculates the physical address by adding offset to the base address of the frame

            if (logicalAddress != physicalAddress) {
                cout << "Logical Address: 0x" << hex << setw(4) << setfill('0') << logicalAddress << " => Page Number: 0x" << setw(2) << setfill('0') << pageNumber << "  Offset: 0x" << setw(2) << setfill('0') << offset << endl; // Prints the logical address, page number, and offset in hexadecimal format
            }
        }
        else {
            cout << "Invalid Page Number: " << pageNumber << endl; // Prints an error message for invalid page number
        }
    }

    return 0;
}
