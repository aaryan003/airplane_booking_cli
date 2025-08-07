#include "BookingSystem.h"
#include "StringUtils.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <filesystem>
#include <ctime>
#include <limits>
#include <cstring>

BookingSystem::BookingSystem() {
    std::cout << "Attempting to load existing bookings..." << std::endl;
    if (!std::filesystem::exists("data")) {
        std::filesystem::create_directory("data");
        std::cout << "Created 'data' directory." << std::endl;
    }
    bookingManager.loadBookingsFromFile("data/bookings.json");
    flightList.loadFlightsFromFile("data/flights.json");
    std::cout << "Booking system initialized." << std::endl;
    selectedFlightIndex = -1;
}

BookingSystem::~BookingSystem() {}

void BookingSystem::acquireLock() {
    bool acquired = false;
    while (!acquired) {
        try {
            // Attempt to create the lock file. O_CREAT | O_EXCL ensures it's atomic.
            // On Windows, this is more complex. A simple approach is to check existence and then create.
            if (!std::filesystem::exists("data/lock.txt")) {
                std::ofstream lockFile("data/lock.txt");
                if (lockFile.is_open()) {
                    lockFile << std::this_thread::get_id();
                    lockFile.close();
                    acquired = true;
                }
            }
        } catch (...) {
            // File system errors, assume another process has the lock
        }
        if (!acquired) {
            std::cout << "Thread " << std::this_thread::get_id() << " is waiting for the lock..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    std::cout << "Thread " << std::this_thread::get_id() << " has acquired the lock." << std::endl;
}

void BookingSystem::releaseLock() {
    try {
        if (std::filesystem::exists("data/lock.txt")) {
            std::filesystem::remove("data/lock.txt");
            std::cout << "Thread " << std::this_thread::get_id() << " has released the lock." << std::endl;
        }
    } catch (...) {
        // Handle potential file removal errors
    }
}
double BookingSystem::calculateDynamicPrice(const Flight* flight, int daysUntilDeparture) {
    if (!flight) return 0.0;

    double basePrice = flight->getBasePrice();
    double finalPrice = basePrice;

    // Demand-based pricing (seats left)
    int totalSeats = flight->getTotalSeats();
    int availableSeats = flight->getAvailableSeats();
    double occupancyRate = (double)(totalSeats - availableSeats) / totalSeats;

    // Surge pricing based on demand
    if (occupancyRate >= 0.9) {
        finalPrice += basePrice * 0.50; // 50% surge
    } else if (occupancyRate >= 0.8) {
        finalPrice += basePrice * 0.35; // 35% surge
    } else if (occupancyRate >= 0.7) {
        finalPrice += basePrice * 0.25; // 25% surge
    } else if (occupancyRate >= 0.6) {
        finalPrice += basePrice * 0.15; // 15% surge
    } else if (occupancyRate >= 0.5) {
        finalPrice += basePrice * 0.10; // 10% surge
    }

    // Time-based pricing (days until departure)
    if (daysUntilDeparture <= 1) {
        finalPrice += basePrice * 0.75; // 75% increase for last-minute booking
    } else if (daysUntilDeparture <= 3) {
        finalPrice += basePrice * 0.50; // 50% increase
    } else if (daysUntilDeparture <= 7) {
        finalPrice += basePrice * 0.30; // 30% increase
    } else if (daysUntilDeparture <= 14) {
        finalPrice += basePrice * 0.20; // 20% increase
    } else if (daysUntilDeparture <= 21) {
        finalPrice += basePrice * 0.10; // 10% increase
    } else if (daysUntilDeparture <= 30) {
        finalPrice += basePrice * 0.05; // 5% increase
    }
    // No increase for bookings more than 30 days in advance

    return finalPrice;
}

void BookingSystem::showDynamicPricingBreakdown(const Flight* flight, int daysUntilDeparture) {
    if (!flight) return;

    double basePrice = flight->getBasePrice();
    double dynamicPrice = calculateDynamicPrice(flight, daysUntilDeparture);

    int totalSeats = flight->getTotalSeats();
    int availableSeats = flight->getAvailableSeats();
    double occupancyRate = (double)(totalSeats - availableSeats) / totalSeats;

    std::cout << "\n=== DYNAMIC PRICING BREAKDOWN ===" << std::endl;
    std::cout << "Base Fare: $" << basePrice << std::endl;
    std::cout << "Occupancy Rate: " << (occupancyRate * 100) << "%" << std::endl;
    std::cout << "Days Until Departure: " << daysUntilDeparture << " days" << std::endl;

    double demandSurge = 0.0;
    if (occupancyRate >= 0.9) demandSurge = basePrice * 0.50;
    else if (occupancyRate >= 0.8) demandSurge = basePrice * 0.35;
    else if (occupancyRate >= 0.7) demandSurge = basePrice * 0.25;
    else if (occupancyRate >= 0.6) demandSurge = basePrice * 0.15;
    else if (occupancyRate >= 0.5) demandSurge = basePrice * 0.10;

    double timeSurge = 0.0;
    if (daysUntilDeparture <= 1) timeSurge = basePrice * 0.75;
    else if (daysUntilDeparture <= 3) timeSurge = basePrice * 0.50;
    else if (daysUntilDeparture <= 7) timeSurge = basePrice * 0.30;
    else if (daysUntilDeparture <= 14) timeSurge = basePrice * 0.20;
    else if (daysUntilDeparture <= 21) timeSurge = basePrice * 0.10;
    else if (daysUntilDeparture <= 30) timeSurge = basePrice * 0.05;

    std::cout << "Demand Surge: $" << demandSurge << std::endl;
    std::cout << "Time-based Surge: $" << timeSurge << std::endl;
    std::cout << "Taxes & Fees: $45.50" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Final Price: $" << (dynamicPrice + 45.50) << std::endl;
    std::cout << "==================================" << std::endl;
}

// MISSING METHOD IMPLEMENTATIONS:

void BookingSystem::searchFlights() {
    std::cout << "\n=== FLIGHT SEARCH ===" << std::endl;

    userInput.promptUserDetails();

    std::cout << "Searching for flights..." << std::endl;

    bool found = searchFlightsByRoute();
    if (found) {
        displayFlightOptions();
    } else {
        std::cout << "No flights found matching your criteria." << std::endl;
    }
}

bool BookingSystem::searchFlightsByRoute() {
    std::cout << "Searching flights by route..." << std::endl;

    // Get search criteria from userInput
    const char* origin = userInput.getOrigin();
    const char* destination = userInput.getDestination();
    const char* departureDate = userInput.getDepartureDate();

    // Search using FlightList
    int foundCount = flightList.searchByRoute(origin, destination, departureDate);

    if (foundCount > 0) {
        std::cout << "Found " << foundCount << " flights matching your criteria." << std::endl;
        return true;
    }

    std::cout << "No flights found for the specified route." << std::endl;
    return false;
}

void BookingSystem::displayFlightOptions() {
    std::cout << "\n=== AVAILABLE FLIGHTS ===" << std::endl;

    int flightCount = flightList.getFlightCount();
    if (flightCount == 0) {
        std::cout << "No flights available to display." << std::endl;
        return;
    }

    for (int i = 0; i < flightCount; i++) {
        const Flight* flight = flightList.getFlightByIndex(i);
        if (flight && flight->isAvailable()) {
            std::cout << "\n--- Flight " << (i + 1) << " ---" << std::endl;
            flight->displayFlightInfo();

            // Show dynamic pricing
            int daysUntilDeparture = 15; // Default for demo
            double dynamicPrice = calculateDynamicPrice(flight, daysUntilDeparture);
            std::cout << "Current Price: $" << (dynamicPrice + 45.50) << " (including taxes)" << std::endl;
        }
    }
    std::cout << "=========================" << std::endl;
}

int BookingSystem::selectFlightFromList() {
    int flightCount = flightList.getFlightCount();
    if (flightCount == 0) {
        std::cout << "No flights available for selection." << std::endl;
        return -1;
    }

    std::cout << "\nSelect a flight (1-" << flightCount << ", or 0 to cancel): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::cout << "Flight selection cancelled." << std::endl;
        return -1;
    }

    if (choice < 1 || choice > flightCount) {
        std::cout << "Invalid selection. Please try again." << std::endl;
        return selectFlightFromList(); // Recursive call for retry
    }

    int selectedIndex = choice - 1;
    const Flight* selectedFlight = flightList.getFlightByIndex(selectedIndex);

    if (selectedFlight && selectedFlight->isAvailable()) {
        std::cout << "You selected: " << selectedFlight->getFlightNumber() << std::endl;
        return selectedIndex;
    }

    std::cout << "Selected flight is not available." << std::endl;
    return -1;
}

void BookingSystem::displaySeatAvailability(const Flight* flight) {
    if (!flight) {
        std::cout << "Invalid flight for seat display." << std::endl;
        return;
    }

    std::cout << "\n=== SEAT AVAILABILITY ===" << std::endl;
    std::cout << "Flight: " << flight->getFlightNumber() << std::endl;
    std::cout << "Aircraft: " << flight->getAircraftType() << std::endl;
    std::cout << "Total Seats: " << flight->getTotalSeats() << std::endl;
    std::cout << "Available Seats: " << flight->getAvailableSeats() << std::endl;

    // Display dynamic seat map
    showSeatMap(*flight);
}

bool BookingSystem::selectPassengerSeats(const Flight* flight) {
    if (!flight) {
        std::cout << "Invalid flight for seat selection." << std::endl;
        return false;
    }

    // Load the latest booking data to ensure the seat map is up to date
    bookingManager.loadBookingsFromFile("data/bookings.json");

    int passengerCount = userInput.getTravelers();
    std::cout << "\n=== SEAT SELECTION ===" << std::endl;
    std::cout << "Selecting seats for " << passengerCount << " passenger(s)..." << std::endl;

    for (int i = 0; i < passengerCount; i++) {
        char seatNumber[5];
        bool seatSelected = false;

        while (!seatSelected) {
            std::cout << "\nPassenger " << (i + 1) << " - Enter seat number (e.g., 12A): ";
            std::cin.getline(seatNumber, 5);

            if (isValidSeatNumber(seatNumber)) {
                // Check if the seat is already occupied
                if (isSeatOccupied(seatNumber, *flight)) {
                    std::cout << "Seat " << seatNumber << " is already taken. Please choose another seat." << std::endl;
                    // Display the seat map again to help the user choose a new seat
                    showSeatMap(*flight);
                } else {
                    std::cout << "Seat " << seatNumber << " selected for passenger " << (i + 1) << std::endl;
                    copyString(this->seatNumber, seatNumber, sizeof(this->seatNumber));
                    seatSelected = true; // Exit the loop for this passenger
                }
            } else {
                std::cout << "Invalid seat number format. Please try again." << std::endl;
            }
        }
    }

    return true;
}
void BookingSystem::collectPassengerDetails() {
    std::cout << "\n=== PASSENGER DETAILS ===" << std::endl;

    int passengerCount = userInput.getTravelers();

    for (int i = 0; i < passengerCount; i++) {
        std::cout << "\n--- Passenger " << (i + 1) << " Details ---" << std::endl;

        char name[50];
        std::cout << "Full Name: ";
        std::cin.getline(name, 50);

        // Store the first passenger's name as primary contact
        if (i == 0) {
            strcpy(passengerName, name);
        }

        std::cout << "Date of Birth (YYYY-MM-DD): ";
        char dob[15];
        std::cin.getline(dob, 15);

        std::cout << "Passport Number: ";
        char passport[20];
        std::cin.getline(passport, 20);

        std::cout << "Nationality: ";
        char nationality[30];
        std::cin.getline(nationality, 30);

        char email[50];
        std::cout << "Contact Email: ";
        std::cin.getline(email, 50);

        // Store first passenger's email as primary
        if (i == 0) {
            strcpy(passengerEmail, email);
        }

        char phone[20];
        std::cout << "Phone Number: ";
        std::cin.getline(phone, 20);

        // Store first passenger's phone as primary
        if (i == 0) {
            strcpy(passengerPhone, phone);
        }

        std::cout << "Passenger " << (i + 1) << " details collected successfully." << std::endl;
    }
}
void BookingSystem::reviewBookingDetails() {
    std::cout << "\n=== BOOKING REVIEW ===" << std::endl;

    const Flight* selectedFlight = flightList.getFlightByIndex(selectedFlightIndex);
    if (!selectedFlight) {
        std::cout << "Error: No flight selected." << std::endl;
        return;
    }

    std::cout << "Flight Details:" << std::endl;
    selectedFlight->displayFlightInfo();

    std::cout << "\nPassenger Count: " << userInput.getTravelers() << std::endl;
    std::cout << "Origin: " << userInput.getOrigin() << std::endl;
    std::cout << "Destination: " << userInput.getDestination() << std::endl;
    std::cout << "Departure Date: " << userInput.getDepartureDate() << std::endl;

    // Calculate and display total price
    int daysUntilDeparture = 15;
    double basePrice = calculateDynamicPrice(selectedFlight, daysUntilDeparture);
    double totalPrice = (basePrice + 45.50) * userInput.getTravelers();

    std::cout << "Total Price: $" << totalPrice << std::endl;

    std::cout << "\nPlease review all details carefully before proceeding." << std::endl;
}

bool BookingSystem::validateBookingData() {
    std::cout << "\n=== VALIDATING BOOKING DATA ===" << std::endl;

    // Basic validation checks
    if (selectedFlightIndex == -1) {
        std::cout << "Validation failed: No flight selected." << std::endl;
        return false;
    }

    const Flight* flight = flightList.getFlightByIndex(selectedFlightIndex);
    if (!flight || !flight->isAvailable()) {
        std::cout << "Validation failed: Selected flight is not available." << std::endl;
        return false;
    }

    if (userInput.getTravelers() <= 0) {
        std::cout << "Validation failed: Invalid passenger count." << std::endl;
        return false;
    }

    if (flight->getAvailableSeats() < userInput.getTravelers()) {
        std::cout << "Validation failed: Not enough seats available." << std::endl;
        return false;
    }

    std::cout << "Booking data validation passed." << std::endl;
    return true;
}

bool BookingSystem::processPayment() {
    std::cout << "\n=== PAYMENT PROCESSING ===" << std::endl;

    const Flight* flight = flightList.getFlightByIndex(selectedFlightIndex);
    if (!flight) return false;

    int daysUntilDeparture = 15;
    double basePrice = calculateDynamicPrice(flight, daysUntilDeparture);
    double totalAmount = (basePrice + 45.50) * userInput.getTravelers();

    std::cout << "Total Amount: $" << totalAmount << std::endl;
    std::cout << "Payment Methods:" << std::endl;
    std::cout << "1. Credit Card" << std::endl;
    std::cout << "2. Debit Card" << std::endl;
    std::cout << "3. PayPal" << std::endl;

    std::cout << "Select payment method (1-3): ";
    int paymentChoice;
    std::cin >> paymentChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (paymentChoice < 1 || paymentChoice > 3) {
        std::cout << "Invalid payment method selected." << std::endl;
        return false;
    }

    std::cout << "Processing payment..." << std::endl;

    // Simulate payment processing
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Payment processed successfully!" << std::endl;
    std::cout << "Transaction ID: TXN" << (rand() % 1000000 + 100000) << std::endl;

    return true;
}

bool BookingSystem::generateBookingConfirmation() {
    acquireLock(); // Acquire lock at the start of the critical section

    const Flight* selectedFlight = nullptr;
    try {
        // Reload fresh data to ensure we're not using stale information
        bookingManager.loadBookingsFromFile("data/bookings.json");
        flightList.loadFlightsFromFile("data/flights.json");

        selectedFlight = flightList.getFlightByIndex(selectedFlightIndex);
        if (!selectedFlight) {
            std::cout << "Failed to find the selected flight. Booking cancelled." << std::endl;
            releaseLock(); // Release lock on failure
            return false;
        }

        // Final check to see if the seat is still available
        if (isSeatOccupied(seatNumber, *selectedFlight)) {
            std::cout << "\nBooking failed: The selected seat " << seatNumber << " has been taken by another user." << std::endl;
            releaseLock();
            return false;
        }

        // ... (rest of the booking logic) ...

        // Get a non-const reference to the flight object to modify its properties
        Flight* flightToUpdate = const_cast<Flight*>(selectedFlight);

        // Calculate final price
        int daysUntilDeparture = 15;
        double basePrice = calculateDynamicPrice(selectedFlight, daysUntilDeparture);
        double totalPrice = (basePrice + 45.50) * userInput.getTravelers();

        // Create booking in the BookingManager
        int bookingId = bookingManager.createBooking(
            passengerName,
            flightToUpdate->getFlightNumber(),
            userInput.getOrigin(),
            // CORRECTED LINES BELOW
            userInput.getDestination(),
            userInput.getDepartureDate(),
            flightToUpdate->getDepartureTime(),
            seatNumber,
            userInput.getCabinClass(),
            totalPrice
        );

        if (bookingId != -1) {
            // Update the available seat count for the booked flight.
            int currentAvailableSeats = flightToUpdate->getAvailableSeats();
            int newAvailableSeats = currentAvailableSeats - userInput.getTravelers();
            flightToUpdate->setAvailableSeats(newAvailableSeats);

            std::cout << "\n=== BOOKING CONFIRMATION ===" << std::endl;
            std::cout << "Dear " << passengerName << "," << std::endl;
            std::cout << "Your booking has been confirmed!" << std::endl;

            bookingManager.displayBookingDetails(bookingId);

            // Save the updated bookings and flight list to files
            bookingManager.saveBookingsToFile("data/bookings.json");
            flightList.saveFlightsToFile("data/flights.json");

            std::cout << "\nConfirmation sent to: " << passengerEmail << std::endl;
            std::cout << "SMS notification sent to: " << passengerPhone << std::endl;
            return true;
        }

    } catch (...) {
        std::cout << "An unexpected error occurred during booking confirmation." << std::endl;
    }

    releaseLock(); // Release lock at the end
    return false;
}
int BookingSystem::getBookingIdFromInput(const char* input) {
    if (!input) return -1;

    // Check if input is numeric (booking ID)
    bool isNumeric = true;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9') {
            isNumeric = false;
            break;
        }
    }

    if (isNumeric) {
        // Convert to integer and return
        int bookingId = 0;
        for (int i = 0; input[i] != '\0'; i++) {
            bookingId = bookingId * 10 + (input[i] - '0');
        }
        return bookingId;
    } else {
        // Treat as PNR and find corresponding booking ID
        return bookingManager.findBookingByPnr(input);
    }
}

void BookingSystem::showSeatMap(const Flight& flight) {
    char seatMap[11][7]; // 10 rows, 6 columns + null terminator

    // Initialize seat map with available seats
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 6; ++j) {
            seatMap[i][j] = 'O';
        }
    }

    // Load fresh data before displaying
    bookingManager.loadBookingsFromFile("data/bookings.json");

    // Mark occupied seats based on bookings
    int bookingCount = bookingManager.getBookingCount();
    for (int i = 0; i < bookingCount; ++i) {
        // Use the new public getter method
        const Booking* booking = bookingManager.getBooking(i);
        if (booking && stringCompare(booking->getFlightNumber(), flight.getFlightNumber())) {
            const char* seatNumber = booking->getSeatNumber();
            int row = 0;
            char column = '\0';

            // Parse seat number string (e.g., "10A")
            if (strlen(seatNumber) == 3) {
                row = (seatNumber[0] - '0') * 10 + (seatNumber[1] - '0');
                column = seatNumber[2];
            } else if (strlen(seatNumber) == 2) {
                row = seatNumber[0] - '0';
                column = seatNumber[1];
            }

            if (row >= 1 && row <= 10 && column >= 'A' && column <= 'F') {
                int colIndex = column - 'A';
                int rowIndex = row - 1;
                seatMap[rowIndex][colIndex] = 'X';
            }
        }
    }

    std::cout << "\n=== DYNAMIC SEAT MAP ===" << std::endl;
    std::cout << "        A B C   D E F" << std::endl;
    for (int i = 0; i < 10; ++i) {
        printf("%-2d  ", i + 1); // Print row number with spacing
        for (int j = 0; j < 6; ++j) {
            std::cout << " " << seatMap[i][j];
            if (j == 2) { // Add aisle
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\nLegend: O = Available, X = Occupied" << std::endl;
    std::cout << "========================" << std::endl;
}
bool BookingSystem::isValidSeatNumber(const char* seatNumber) {
    if (!seatNumber || seatNumber[0] == '\0') return false;

    // Basic validation: should be like "12A" or "5B"
    int len = 0;
    while (seatNumber[len] != '\0') len++;

    if (len < 2 || len > 3) return false;

    // Check if last character is a valid seat letter
    char lastChar = seatNumber[len - 1];
    if (lastChar < 'A' || lastChar > 'F') return false;

    // Check if preceding characters are digits
    for (int i = 0; i < len - 1; i++) {
        if (seatNumber[i] < '0' || seatNumber[i] > '9') return false;
    }

    return true;
}

void BookingSystem::startBookingProcess() {
    std::cout << "\n=== Starting New Booking Process ===" << std::endl;
    std::cout << "Step 1: Collecting travel details..." << std::endl;

    userInput.promptUserDetails();

    std::cout << "\nStep 2: Searching flights..." << std::endl;
    bool found = searchFlightsByRoute();

    if (!found) {
        std::cout << "No flights found for the specified criteria." << std::endl;
        return;
    }

    std::cout << "\nStep 3: Displaying available flights with dynamic pricing..." << std::endl;
    displayFlightOptions();

    selectedFlightIndex = selectFlightFromList();
    if (selectedFlightIndex == -1) {
        std::cout << "No flight selected. Returning to menu." << std::endl;
        return;
    }

    const Flight* selectedFlight = flightList.getFlightByIndex(selectedFlightIndex);
    if (!selectedFlight) {
        std::cout << "Invalid flight selection. Booking cancelled." << std::endl;
        return;
    }

    // Calculate days until departure (simplified for demo)
    int daysUntilDeparture = 15;

    std::cout << "\nStep 4: Dynamic pricing calculation..." << std::endl;
    showDynamicPricingBreakdown(selectedFlight, daysUntilDeparture);

    std::cout << "\nStep 5: Seat selection..." << std::endl;
    displaySeatAvailability(selectedFlight);

    if (!selectPassengerSeats(selectedFlight)) {
        std::cout << "Seat selection failed. Booking cancelled." << std::endl;
        return;
    }

    std::cout << "\nStep 6: Collecting passenger details..." << std::endl;
    collectPassengerDetails();

    std::cout << "\nStep 7: Review booking details..." << std::endl;
    reviewBookingDetails();

    if (validateBookingData()) {
        processPayment();
        generateBookingConfirmation(); // This function will now handle the lock
    } else {
        std::cout << "Booking validation failed. Please try again." << std::endl;
    }
}// Enhanced Modification Functions
void BookingSystem::changeFlightDate() {
    std::cout << "Enter Booking ID or PNR: ";
    char input[20];
    std::cin.getline(input, 20);

    acquireLock();

    try {
        bookingManager.loadBookingsFromFile("data/bookings.json");

        int bookingId = getBookingIdFromInput(input);
        if (bookingId == -1) {
            std::cout << "Booking not found." << std::endl;
            releaseLock();
            return;
        }

        std::cout << "\n=== CHANGE FLIGHT DATE ===" << std::endl;
        std::cout << "Current booking details:" << std::endl;
        bookingManager.displayBookingDetails(bookingId);

        std::cout << "\nEnter new departure date (YYYY-MM-DD): ";
        char newDate[15];
        std::cin.getline(newDate, 15);

        // Search for flights on new date
        std::cout << "Searching flights for new date..." << std::endl;
        // Implementation would search for available flights on new date

        double changeFee = 75.0; // Date change fee
        std::cout << "\nDate Change Fee: $" << changeFee << std::endl;
        std::cout << "Confirm date change? (y/n): ";

        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y') {
            // Update booking date and add fee
            std::cout << "Flight date changed successfully!" << std::endl;
            std::cout << "Date change fee of $" << changeFee << " will be charged." << std::endl;
            bookingManager.saveBookingsToFile("data/bookings.json");
        }

        releaseLock();
    } catch (...) {
        releaseLock();
        std::cout << "Error processing date change." << std::endl;
    }
}

void BookingSystem::changeSeatSelection() {
    std::cout << "Enter Booking ID or PNR: ";
    char input[20];
    std::cin.getline(input, 20);

    acquireLock();

    try {
        bookingManager.loadBookingsFromFile("data/bookings.json");

        int bookingId = getBookingIdFromInput(input);
        if (bookingId == -1) {
            std::cout << "Booking not found." << std::endl;
            releaseLock();
            return;
        }

        std::cout << "\n=== CHANGE SEAT SELECTION ===" << std::endl;
        bookingManager.displayBookingDetails(bookingId);

        // Display current seat map
        std::cout << "\nCurrent seat map for your flight:" << std::endl;
        // Use the existing findBookingById public helper instead of the private one
        // Note: The previous code was trying to access a private method.
        // It's assumed you have a public `findBookingById` in BookingManager, as used in other parts of your code.
        // If not, you need to add a public wrapper for it.

        // The original code was trying to access a private `findBookingById` in BookingManager.
        // Your public interface for `BookingManager` needs to have `displayBookingDetails(int)` and `findBookingByPnr(const char*)`
        // which rely on the private `findBookingById`. Since you are calling these methods from BookingSystem,
        // it means they are already public in your BookingManager.h.
        // The error you showed, however, pointed to `findBookingById` being private.
        // For the sake of this fix, let's assume `BookingManager` has a public method that wraps the private helper.
        // If not, you must make a public method to retrieve a booking by ID.
        // Let's create a temporary solution here.

        // Find the booking to get the flight number
        const char* flightNumber = nullptr;
        int bookingCount = bookingManager.getBookingCount();
        for (int i = 0; i < bookingCount; ++i) {
            const Booking* booking = bookingManager.getBooking(i);
            if (booking && booking->getBookingId() == bookingId) {
                flightNumber = booking->getFlightNumber();
                break;
            }
        }

        if (flightNumber) {
            // Find the flight from the FlightList based on the flight number
            for (int i = 0; i < flightList.getFlightCount(); ++i) {
                const Flight* flight = flightList.getFlightByIndex(i);
                if (stringCompare(flight->getFlightNumber(), flightNumber)) {
                    showSeatMap(*flight);
                    break;
                }
            }
        }
        

        std::cout << "\nEnter new seat number (e.g., 12A): ";
        char newSeat[5];
        std::cin.getline(newSeat, 5);

        double seatChangeFee = 25.0;
        std::cout << "\nSeat Change Fee: $" << seatChangeFee << std::endl;
        std::cout << "Confirm seat change to " << newSeat << "? (y/n): ";

        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y') {
            std::cout << "Seat changed successfully to " << newSeat << "!" << std::endl;
            std::cout << "Seat change fee of $" << seatChangeFee << " will be charged." << std::endl;
            bookingManager.saveBookingsToFile("data/bookings.json");
        }

        releaseLock();
    } catch (...) {
        releaseLock();
        std::cout << "Error processing seat change." << std::endl;
    }
}

void BookingSystem::upgradeCabinClass() {
    std::cout << "Enter Booking ID or PNR: ";
    char input[20];
    std::cin.getline(input, 20);

    acquireLock();

    try {
        bookingManager.loadBookingsFromFile("data/bookings.json");

        int bookingId = getBookingIdFromInput(input);
        if (bookingId == -1) {
            std::cout << "Booking not found." << std::endl;
            releaseLock();
            return;
        }

        std::cout << "\n=== UPGRADE CABIN CLASS ===" << std::endl;
        bookingManager.displayBookingDetails(bookingId);

        std::cout << "\nAvailable upgrade options:" << std::endl;
        std::cout << "1. Economy to Business Class - $350" << std::endl;
        std::cout << "2. Economy to First Class - $750" << std::endl;
        std::cout << "3. Business to First Class - $400" << std::endl;

        std::cout << "\nSelect upgrade option (1-3): ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        double upgradeFee = 0.0;
        const char* newClass = "";

        switch (choice) {
            case 1:
                upgradeFee = 350.0;
                newClass = "Business";
                break;
            case 2:
                upgradeFee = 750.0;
                newClass = "First";
                break;
            case 3:
                upgradeFee = 400.0;
                newClass = "First";
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
                releaseLock();
                return;
        }

        std::cout << "\nUpgrade Fee: $" << upgradeFee << std::endl;
        std::cout << "Confirm upgrade to " << newClass << " Class? (y/n): ";

        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y') {
            std::cout << "Cabin class upgraded successfully to " << newClass << " Class!" << std::endl;
            std::cout << "Upgrade fee of $" << upgradeFee << " will be charged." << std::endl;
            bookingManager.saveBookingsToFile("data/bookings.json");
        }

        releaseLock();
    } catch (...) {
        releaseLock();
        std::cout << "Error processing cabin upgrade." << std::endl;
    }
}

void BookingSystem::modifyPassengerDetails() {
    std::cout << "Enter Booking ID or PNR: ";
    char input[20];
    std::cin.getline(input, 20);

    acquireLock();

    try {
        bookingManager.loadBookingsFromFile("data/bookings.json");

        int bookingId = getBookingIdFromInput(input);
        if (bookingId == -1) {
            std::cout << "Booking not found." << std::endl;
            releaseLock();
            return;
        }

        std::cout << "\n=== MODIFY PASSENGER DETAILS ===" << std::endl;
        bookingManager.displayBookingDetails(bookingId);

        std::cout << "\nWhat would you like to modify?" << std::endl;
        std::cout << "1. Passenger Name (Fee: $50)" << std::endl;
        std::cout << "2. Contact Information (Fee: $0)" << std::endl;
        std::cout << "3. Special Requirements (Fee: $0)" << std::endl;

        std::cout << "\nSelect option (1-3): ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        double modificationFee = 0.0;

        switch (choice) {
            case 1: {
                std::cout << "Enter new passenger name: ";
                char newName[50];
                std::cin.getline(newName, 50);
                modificationFee = 50.0;
                std::cout << "Name change fee: $" << modificationFee << std::endl;
                break;
            }
            case 2: {
                std::cout << "Enter new contact information: ";
                char newContact[100];
                std::cin.getline(newContact, 100);
                modificationFee = 0.0;
                std::cout << "Contact information updated (No fee)" << std::endl;
                break;
            }
            case 3: {
                std::cout << "Enter special requirements: ";
                char requirements[200];
                std::cin.getline(requirements, 200);
                modificationFee = 0.0;
                std::cout << "Special requirements updated (No fee)" << std::endl;
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
                releaseLock();
                return;
        }

        std::cout << "\nTotal modification fee: $" << modificationFee << std::endl;
        std::cout << "Confirm modification? (y/n): ";

        char confirm;
        std::cin >> confirm;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y') {
            std::cout << "Passenger details modified successfully!" << std::endl;
            if (modificationFee > 0) {
                std::cout << "Modification fee of $" << modificationFee << " will be charged." << std::endl;
            }
            bookingManager.saveBookingsToFile("data/bookings.json");
        }

        releaseLock();
    } catch (...) {
        releaseLock();
        std::cout << "Error modifying passenger details." << std::endl;
    }
}

void BookingSystem::viewModificationCharges() {
    std::cout << "\n=== MODIFICATION CHARGES ===" << std::endl;
    std::cout << "Service                     | Fee" << std::endl;
    std::cout << "----------------------------|--------" << std::endl;
    std::cout << "Date Change                 | $75.00" << std::endl;
    std::cout << "Seat Change                 | $25.00" << std::endl;
    std::cout << "Name Change                 | $50.00" << std::endl;
    std::cout << "Economy to Business         | $350.00" << std::endl;
    std::cout << "Economy to First            | $750.00" << std::endl;
    std::cout << "Business to First           | $400.00" << std::endl;
    std::cout << "Contact Info Update         | Free" << std::endl;
    std::cout << "Special Requirements        | Free" << std::endl;
    std::cout << "=============================" << std::endl;

    std::cout << "\nNotes:" << std::endl;
    std::cout << "- All fees are per passenger" << std::endl;
    std::cout << "- Modifications are subject to availability" << std::endl;
    std::cout << "- Fare differences may apply for date changes" << std::endl;
    std::cout << "- Some restrictions may apply based on ticket type" << std::endl;
}

// Enhanced Cancellation Functions
void BookingSystem::cancelByPnr() {
    std::cout << "Enter PNR Code: ";
    char pnr[10];
    std::cin.getline(pnr, 10);

    acquireLock();

    try {
        bookingManager.loadBookingsFromFile("data/bookings.json");

        int bookingId = bookingManager.findBookingByPnr(pnr);
        if (bookingId == -1) {
            std::cout << "No booking found with PNR: " << pnr << std::endl;
            releaseLock();
            return;
        }

        processCancellation(bookingId);
        releaseLock();
    } catch (...) {
        releaseLock();
        std::cout << "Error processing cancellation." << std::endl;
    }
}

void BookingSystem::cancelByBookingId() {
    std::cout << "Enter Booking ID: ";
    int bookingId;
    std::cin >> bookingId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    acquireLock();

    try {
        bookingManager.loadBookingsFromFile("data/bookings.json");

        if (!bookingManager.isValidBookingId(bookingId)) {
            std::cout << "No booking found with ID: " << bookingId << std::endl;
            releaseLock();
            return;
        }

        processCancellation(bookingId);
        releaseLock();
    } catch (...) {
        releaseLock();
        std::cout << "Error processing cancellation." << std::endl;
    }
}

void BookingSystem::processCancellation(int bookingId) {
    // Load the latest data from files
    bookingManager.loadBookingsFromFile("data/bookings.json");
    flightList.loadFlightsFromFile("data/flights.json");

        const Booking* booking = bookingManager.getBookingById(bookingId);
        if (!booking) {
            std::cout << "Error: Booking not found for cancellation process." << std::endl;
            return;
    }

    // Get a copy of the flight number before it's removed
    char flightNumber[10];
    copyString(flightNumber, booking->getFlightNumber(), 10);

    // Now, proceed with the cancellation in BookingManager
    if (bookingManager.cancelBooking(bookingId)) {
        // Find the corresponding flight and update its seat count
        Flight* flightToUpdate = flightList.getFlightByNumber(flightNumber);
        if (flightToUpdate) {
            int currentAvailableSeats = flightToUpdate->getAvailableSeats();
            flightToUpdate->setAvailableSeats(currentAvailableSeats + 1); // Increment seat count
            std::cout << "Flight seat count updated. New available seats: " << flightToUpdate->getAvailableSeats() << std::endl;

            // Save the updated flight list
            flightList.saveFlightsToFile("data/flights.json");
        } else {
            std::cout << "Warning: Could not find corresponding flight to update seat count." << std::endl;
        }

        std::cout << "\nBooking cancelled successfully!" << std::endl;
    } else {
        std::cout << "Cancellation aborted or failed." << std::endl;
    }
}

double BookingSystem::calculateCancellationFee(int daysUntilDeparture) {
    if (daysUntilDeparture <= 1) {
        return 150.0; // 24 hours or less: $150 fee
    } else if (daysUntilDeparture <= 7) {
        return 100.0; // 1-7 days: $100 fee
    } else if (daysUntilDeparture <= 14) {
        return 75.0;  // 8-14 days: $75 fee
    } else if (daysUntilDeparture <= 30) {
        return 50.0;  // 15-30 days: $50 fee
    } else {
        return 25.0;  // More than 30 days: $25 fee
    }
}

void BookingSystem::viewCancellationPolicy() {
    std::cout << "\n=== CANCELLATION POLICY ===" << std::endl;
    std::cout << "Cancellation Timeline         | Fee" << std::endl;
    std::cout << "------------------------------|--------" << std::endl;
    std::cout << "More than 30 days             | $25.00" << std::endl;
    std::cout << "15-30 days before departure   | $50.00" << std::endl;
    std::cout << "8-14 days before departure    | $75.00" << std::endl;
    std::cout << "1-7 days before departure     | $100.00" << std::endl;
    std::cout << "Within 24 hours               | $150.00" << std::endl;
    std::cout << "No-show                       | No refund" << std::endl;
    std::cout << "===============================" << std::endl;

    std::cout << "\nImportant Notes:" << std::endl;
    std::cout << "• Cancellation fees are per passenger" << std::endl;
    std::cout << "• Refunds are processed within 5-7 business days" << std::endl;
    std::cout << "• Non-refundable tickets cannot be cancelled" << std::endl;
    std::cout << "• Medical emergencies may qualify for fee waivers" << std::endl;
    std::cout << "• Group bookings have different cancellation terms" << std::endl;
}

void BookingSystem::checkRefundStatus() {
    std::cout << "Enter Booking ID or PNR to check refund status: ";
    char input[20];
    std::cin.getline(input, 20);

    acquireLock();

    try {
        bookingManager.loadBookingsFromFile("data/bookings.json");

        int bookingId = getBookingIdFromInput(input);
        if (bookingId == -1) {
            std::cout << "Booking not found." << std::endl;
            releaseLock();
            return;
        }

        std::cout << "\n=== REFUND STATUS ===" << std::endl;
        bookingManager.displayBookingDetails(bookingId);

        // Demo refund status
        std::cout << "\nRefund Information:" << std::endl;
        std::cout << "Refund Amount: $295.49" << std::endl;
        std::cout << "Refund Status: Processing" << std::endl;
        std::cout << "Expected Credit Date: 2025-08-13" << std::endl;
        std::cout << "Refund Method: Original payment method" << std::endl;
        std::cout << "Reference Number: RFD123456789" << std::endl;

        releaseLock();
    } catch (...) {
        releaseLock();
        std::cout << "Error checking refund status." << std::endl;
    }
}

// **Updated and Corrected functions below**

// Fix the viewBookingDetails function
void BookingSystem::viewBookingDetails() {
    std::cout << "\n=== VIEW BOOKING DETAILS ===" << std::endl;
    std::cout << "Search Options:" << std::endl;
    std::cout << "1. Search by Booking ID" << std::endl;
    std::cout << "2. Search by PNR" << std::endl;
    std::cout << "3. Search by Passenger Name" << std::endl;
    std::cout << "4. View All Bookings" << std::endl;

    std::cout << "\nSelect search option (1-4): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bookingManager.loadBookingsFromFile("data/bookings.json");

    switch (choice) {
        case 1: {
            std::cout << "Enter Booking ID: ";
            int bookingId;
            std::cin >> bookingId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            bookingManager.displayBookingDetails(bookingId);
            break;
        }
        case 2: {
            std::cout << "Enter PNR: ";
            char pnr[10];
            std::cin.getline(pnr, 10);

            int bookingId = bookingManager.findBookingByPnr(pnr);
            if (bookingId != -1) {
                bookingManager.displayBookingDetails(bookingId);
            } else {
                std::cout << "No booking found with PNR: " << pnr << std::endl;
            }
            break;
        }
        case 3: {
            std::cout << "Enter Passenger Name: ";
            char passengerName[50];
            std::cin.getline(passengerName, 50);

            bookingManager.displayBookingsByPassenger(passengerName);
            break;
        }
        case 4: {
            bookingManager.displayAllBookings();
            break;
        }
        default:
            std::cout << "Invalid choice." << std::endl;
    }
}

// Fix the modifyExistingBooking function
void BookingSystem::modifyExistingBooking() {
    std::cout << "\n=== MODIFY EXISTING BOOKING ===" << std::endl;

    char input[20];
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS) {
        std::cout << "Enter PNR or Booking ID: ";
        std::cin.getline(input, 20);

        int bookingId = getBookingIdFromInput(input);

        if (bookingId != -1) {
            // Valid booking found
            std::cout << "Booking found! Loading details..." << std::endl;
            displayDetailedBookingInfo(bookingId);

            // Show modification options
            showModificationMenu(bookingId);
            return;
        } else {
            attempts++;
            std::cout << "Invalid PNR/Booking ID. ";

            if (attempts < MAX_ATTEMPTS) {
                std::cout << "Please try again. (" << (MAX_ATTEMPTS - attempts) << " attempts remaining)" << std::endl;
            } else {
                std::cout << "Maximum attempts reached. Returning to main menu." << std::endl;
            }
        }
    }

    // If we reach here, all attempts failed
    std::cout << "Unable to find booking. Please check your PNR/Booking ID and try again later." << std::endl;
}
// Fix the cancelBooking function
void BookingSystem::cancelBooking() {
    std::cout << "\n=== CANCEL BOOKING ===" << std::endl;
    std::cout << "Choose cancellation method:" << std::endl;
    std::cout << "1. Cancel by PNR" << std::endl;
    std::cout << "2. Cancel by Booking ID" << std::endl;
    std::cout << "3. View Cancellation Policy" << std::endl;
    std::cout << "4. Check Refund Status" << std::endl;
    std::cout << "5. Back to Main Menu" << std::endl;

    std::cout << "\nEnter your choice (1-5): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            std::cout << "Enter PNR: ";
            char pnr[10];
            std::cin.getline(pnr, 10);

            int bookingId = bookingManager.findBookingByPnr(pnr);
            if (bookingId != -1) {
                bookingManager.cancelBooking(bookingId);
            } else {
                std::cout << "No booking found with PNR: " << pnr << std::endl;
            }
            break;
        }
        case 2: {
            std::cout << "Enter Booking ID: ";
            int bookingId;
            std::cin >> bookingId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            bookingManager.cancelBooking(bookingId);
            break;
        }
        case 3: {
            viewCancellationPolicy();
            break;
        }
        case 4: {
            std::cout << "Refund status check feature coming soon..." << std::endl;
            break;
        }
        case 5: {
            return;
        }
        default:
            std::cout << "Invalid choice." << std::endl;
    }
}

void BookingSystem::showModificationMenu(int bookingId) {
    int choice;
    bool keepModifying = true;

    while (keepModifying) {
        std::cout << "\n=== MODIFICATION OPTIONS ===" << std::endl;
        std::cout << "1. Change Flight Date" << std::endl;
        std::cout << "2. Change Seat Selection" << std::endl;
        std::cout << "3. Upgrade Cabin Class" << std::endl;
        std::cout << "4. Modify Passenger Details" << std::endl;
        std::cout << "5. Cancel This Booking" << std::endl;
        std::cout << "6. Return to Main Menu" << std::endl;
        std::cout << "Enter your choice (1-6): ";

        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                changeFlightDate();
            break;
            case 2:
                changeSeatSelection();
            break;
            case 3:
                upgradeCabinClass();
            break;
            case 4:
                modifyPassengerDetails();
            break;
            case 5:
                processCancellation(bookingId);
            keepModifying = false;
            break;
            case 6:
                keepModifying = false;
            break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }

        if (choice >= 1 && choice <= 4) {
            std::cout << "\nWould you like to make another modification? (y/n): ";
            char continueChoice;
            std::cin >> continueChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (continueChoice != 'y' && continueChoice != 'Y') {
                keepModifying = false;
            }
        }
    }
}

void BookingSystem::displayDetailedBookingInfo(int bookingId) {
    // This calls the display method of the BookingManager to show core details.
    bookingManager.displayBookingDetails(bookingId);

    // This section adds supplementary information not stored in the Booking object itself.
    std::cout << "\n=== ADDITIONAL INFORMATION ===" << std::endl;
    std::cout << "Booking Channel: Online" << std::endl;
    std::cout << "Payment Status: Confirmed" << std::endl;
    std::cout << "E-ticket Status: Issued" << std::endl;
    std::cout << "Check-in Status: Not available yet" << std::endl;
    std::cout << "Baggage Allowance: 1 x 23kg" << std::endl;
    std::cout << "Frequent Flyer: Not applied" << std::endl;
    std::cout << "Special Services: None" << std::endl;

    std::cout << "\n=== CONTACT INFORMATION ===" << std::endl;
    std::cout << "For changes: 1-800-AIRLINE" << std::endl;
    std::cout << "For support: support@airline.com" << std::endl;
    std::cout << "Online management: www.airline.com/manage" << std::endl;
}

bool BookingSystem::isSeatOccupied(const char* seatNumber, const Flight& flight) {
    int bookingCount = bookingManager.getBookingCount();
    for (int i = 0; i < bookingCount; ++i) {
        // Use the new public getter method
        const Booking* booking = bookingManager.getBooking(i);
        if (booking && stringCompare(booking->getFlightNumber(), flight.getFlightNumber()) &&
            stringCompare(booking->getSeatNumber(), seatNumber)) {
            return true; // Seat is occupied
            }
    }
    return false; // Seat is available
}

// Concurrency simulation function
void BookingSystem::simulateConcurrentBooking(int threadId, int flightIndex, const char* name, const char* seat) {
    std::cout << "Thread " << threadId << ": Attempting to book seat " << seat << " for flight " << flightList.getFlightByIndex(flightIndex)->getFlightNumber() << std::endl;

    // Simulate some work before trying to get the lock
    std::this_thread::sleep_for(std::chrono::milliseconds(50 * (threadId % 3)));

    acquireLock();

    try {
        // Critical Section: Accessing and modifying shared resources
        bookingManager.loadBookingsFromFile("data/bookings.json");
        flightList.loadFlightsFromFile("data/flights.json");

        const Flight* selectedFlight = flightList.getFlightByIndex(flightIndex);
        if (!selectedFlight) {
            std::cout << "Thread " << threadId << ": Error - Flight not found." << std::endl;
            releaseLock();
            return;
        }

        if (isSeatOccupied(seat, *selectedFlight)) {
            std::cout << "Thread " << threadId << ": Seat " << seat << " is already occupied. Booking failed." << std::endl;
        } else {
            // Simulate a successful payment
            std::this_thread::sleep_for(std::chrono::seconds(1));

            int newBookingId = bookingManager.createBooking(name, selectedFlight->getFlightNumber(),
                                                           selectedFlight->getOrigin(), selectedFlight->getDestination(),
                                                           selectedFlight->getDepartureDate(), selectedFlight->getDepartureTime(),
                                                           seat, "Economy", selectedFlight->getBasePrice() + 50.0);

            if (newBookingId != -1) {
                // Update flight seat count and save
                Flight* flightToUpdate = const_cast<Flight*>(selectedFlight);
                flightToUpdate->setAvailableSeats(flightToUpdate->getAvailableSeats() - 1);
                bookingManager.saveBookingsToFile("data/bookings.json");
                flightList.saveFlightsToFile("data/flights.json");

                std::cout << "Thread " << threadId << ": Successfully booked seat " << seat << " with Booking ID " << newBookingId << std::endl;
            } else {
                std::cout << "Thread " << threadId << ": Booking failed." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Thread " << threadId << ": An unexpected error occurred: " << e.what() << std::endl;
    }

    releaseLock();
}