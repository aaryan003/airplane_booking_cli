#include "BookingManager.h"
#include "StringUtils.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstdio>

BookingManager::BookingManager() : bookingCount(0), nextBookingId(1000) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

BookingManager::~BookingManager() {}

int BookingManager::createBooking(const char* passengerName, const char* flightNumber,
                                  const char* origin, const char* destination,
                                  const char* departureDate, const char* departureTime,
                                  const char* seatNumber, const char* cabinClass,
                                  double totalPrice) {
    if (bookingCount >= MAX_BOOKINGS) {
        std::cout << "Error: Maximum bookings limit reached." << std::endl;
        return -1;
    }

    int bookingId = nextBookingId++;

    Booking newBooking(bookingId, passengerName, flightNumber, origin, destination,
                       departureDate, departureTime, seatNumber, cabinClass, totalPrice);

    char pnr[10];
    generatePnr(pnr, bookingId);
    newBooking.setPnr(pnr);

    char currentDate[15], currentTime[10];
    getCurrentDateTime(currentDate, currentTime);
    newBooking.setBookingDate(currentDate);
    newBooking.setBookingTime(currentTime);
    newBooking.setBookingStatus("confirmed");

    bookings[bookingCount] = newBooking;
    bookingCount++;

    std::cout << "Booking created successfully with ID: " << bookingId << std::endl;
    return bookingId;
}

bool BookingManager::modifyBooking(int bookingId) {
    // Load fresh data first
    loadBookingsFromFile("data/bookings.json");

    Booking* booking = findBookingById(bookingId);
    if (!booking) {
        std::cout << "Booking not found with ID: " << bookingId << std::endl;
        return false;
    }

    if (!booking->canBeModified()) {
        std::cout << "Booking cannot be modified." << std::endl;
        return false;
    }

    std::cout << "Current booking details:" << std::endl;
    booking->displayBookingDetails();

    std::cout << "\nWhat would you like to modify?" << std::endl;
    std::cout << "1. Passenger Name" << std::endl;
    std::cout << "2. Seat Number" << std::endl;
    std::cout << "3. Cabin Class" << std::endl;
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(10000, '\n');

    bool modified = false;
    switch (choice) {
        case 1: {
            std::cout << "Enter new passenger name: ";
            char newName[50];
            std::cin.getline(newName, 50);
            booking->setPassengerName(newName);
            modified = true;
            break;
        }
        case 2: {
            std::cout << "Enter new seat number: ";
            char newSeat[5];
            std::cin.getline(newSeat, 5);
            booking->setSeatNumber(newSeat);
            // Add seat change fee
            booking->setTotalPrice(booking->getTotalPrice() + 25.0);
            std::cout << "Seat change fee of $25 applied." << std::endl;
            modified = true;
            break;
        }
        case 3: {
            std::cout << "Enter new cabin class (economy/business/first): ";
            char newClass[15];
            std::cin.getline(newClass, 15);
            booking->setCabinClass(newClass);
            modified = true;
            break;
        }
        default:
            std::cout << "Invalid choice." << std::endl;
            return false;
    }

    if (modified) {
        updateBookingStatus(bookingId, "modified");

        // Save changes immediately
        if (saveBookingsToFile("data/bookings.json")) {
            std::cout << "Booking modified and saved successfully!" << std::endl;
            return true;
        } else {
            std::cout << "Error: Could not save changes to file." << std::endl;
            return false;
        }
    }

    return false;
}

bool BookingManager::cancelBooking(int bookingId) {
    // Load fresh data first
    loadBookingsFromFile("data/bookings.json");

    Booking* booking = findBookingById(bookingId);
    if (!booking) {
        std::cout << "Booking not found with ID: " << bookingId << std::endl;
        return false;
    }

    if (!booking->canBeCancelled()) {
        std::cout << "Booking cannot be cancelled." << std::endl;
        return false;
    }

    std::cout << "\nBooking to be cancelled:" << std::endl;
    booking->displayBookingDetails();

    // Calculate cancellation fee
    int daysUntilDeparture = 10; // Simplified
    double cancellationFee = calculateCancellationFee(bookingId, daysUntilDeparture);
    double refundAmount = booking->getTotalPrice() - cancellationFee;

    std::cout << "\nCancellation Details:" << std::endl;
    std::cout << "Original Amount: $" << booking->getTotalPrice() << std::endl;
    std::cout << "Cancellation Fee: $" << cancellationFee << std::endl;
    std::cout << "Refund Amount: $" << refundAmount << std::endl;

    std::cout << "Confirm cancellation? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(10000, '\n');

    if (confirm == 'y' || confirm == 'Y') {
        // Find the booking index and remove it
        int bookingIndex = -1;
        for (int i = 0; i < bookingCount; i++) {
            if (bookings[i].getBookingId() == bookingId) {
                bookingIndex = i;
                break;
            }
        }

        if (bookingIndex >= 0) {
            // Shift all bookings after this one back
            for (int i = bookingIndex; i < bookingCount - 1; i++) {
                bookings[i] = bookings[i + 1];
            }
            bookingCount--;

            // Save changes
            if (saveBookingsToFile("data/bookings.json")) {
                std::cout << "Booking cancelled successfully!" << std::endl;
                std::cout << "Refund of $" << refundAmount << " will be processed in 5-7 business days." << std::endl;
                return true;
            } else {
                std::cout << "Error: Could not save cancellation to file." << std::endl;
                return false;
            }
        }
    }

    return false;
}

// **Private** helper method
Booking* BookingManager::findBookingById(int bookingId) {
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].getBookingId() == bookingId) {
            return &bookings[i];
        }
    }
    return nullptr;
}

// **Public** getter methods
const Booking* BookingManager::getBooking(int index) const {
    if (index >= 0 && index < bookingCount) {
        return &bookings[index];
    }
    return nullptr;
}

const Booking* BookingManager::getBookingById(int bookingId) const {
    for (int i = 0; i < bookingCount; i++) {
        if (bookings[i].getBookingId() == bookingId) {
            return &bookings[i];
        }
    }
    return nullptr;
}

void BookingManager::displayBookingDetails(int bookingId) {
    // Use the public getter here
    const Booking* booking = getBookingById(bookingId);
    if (booking) {
        booking->displayBookingDetails();
    } else {
        std::cout << "Booking not found with ID: " << bookingId << std::endl;
    }
}

void BookingManager::displayAllBookings() {
    if (bookingCount == 0) {
        std::cout << "No bookings found." << std::endl;
        return;
    }

    std::cout << "\n=== ALL BOOKINGS ===" << std::endl;
    std::cout << "Total bookings: " << bookingCount << std::endl;
    std::cout << "=================" << std::endl;

    for (int i = 0; i < bookingCount; i++) {
        std::cout << "\nBooking " << (i + 1) << ":" << std::endl;
        bookings[i].displayBookingDetails();
        std::cout << "-------------------" << std::endl;
    }
}

void BookingManager::displayBookingsByPassenger(const char* passengerName) {
    if (!passengerName || passengerName[0] == '\0') {
        std::cout << "Invalid passenger name." << std::endl;
        return;
    }

    bool found = false;
    std::cout << "\n=== BOOKINGS FOR: " << passengerName << " ===" << std::endl;

    for (int i = 0; i < bookingCount; i++) {
        // Use case-insensitive comparison and substring matching
        if (stringCompareNoCase(bookings[i].getPassengerName(), passengerName) ||
            containsIgnoreCase(bookings[i].getPassengerName(), passengerName)) {
            bookings[i].displayBookingDetails();
            std::cout << "-------------------" << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No bookings found for passenger: " << passengerName << std::endl;
    }
}

int BookingManager::findBookingByPnr(const char* pnr) {
    for (int i = 0; i < bookingCount; i++) {
        if (stringCompare(bookings[i].getPnr(), pnr)) {
            return bookings[i].getBookingId();
        }
    }
    return -1;
}

void BookingManager::searchBookingsByFlight(const char* flightNumber) {
    bool found = false;
    std::cout << "\n=== BOOKINGS FOR FLIGHT: " << flightNumber << " ===" << std::endl;

    for (int i = 0; i < bookingCount; i++) {
        if (stringCompare(bookings[i].getFlightNumber(), flightNumber)) {
            bookings[i].displayBookingDetails();
            std::cout << "-------------------" << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No bookings found for flight: " << flightNumber << std::endl;
    }
}

bool BookingManager::isValidBookingId(int bookingId) {
    return findBookingById(bookingId) != nullptr;
}

bool BookingManager::isBookingActive(int bookingId) {
    Booking* booking = findBookingById(bookingId);
    if (!booking) return false;

    const char* status = booking->getBookingStatus();
    return stringCompare(status, "confirmed") || stringCompare(status, "modified");
}

bool BookingManager::modifyPassengerName(int bookingId, const char* newName) {
    Booking* booking = findBookingById(bookingId);
    if (!booking) return false;

    booking->setPassengerName(newName);
    updateBookingStatus(bookingId, "modified");
    return true;
}

bool BookingManager::modifySeatNumber(int bookingId, const char* newSeat) {
    Booking* booking = findBookingById(bookingId);
    if (!booking) return false;

    booking->setSeatNumber(newSeat);
    updateBookingStatus(bookingId, "modified");
    return true;
}

bool BookingManager::modifyCabinClass(int bookingId, const char* newClass) {
    Booking* booking = findBookingById(bookingId);
    if (!booking) return false;

    booking->setCabinClass(newClass);
    updateBookingStatus(bookingId, "modified");
    return true;
}

bool BookingManager::modifyDepartureDate(int bookingId, const char* newDate) {
    Booking* booking = findBookingById(bookingId);
    if (!booking) return false;

    booking->setDepartureDate(newDate);
    updateBookingStatus(bookingId, "modified");
    return true;
}

bool BookingManager::saveBookingsToFile(const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file for writing: " << filename << std::endl;
        return false;
    }

    // Write JSON format manually
    file << "{\n";
    file << "  \"bookings\": [\n";

    for (int i = 0; i < bookingCount; i++) {
        file << "    {\n";
        file << "      \"bookingId\": " << bookings[i].getBookingId() << ",\n";
        file << "      \"pnr\": \"" << bookings[i].getPnr() << "\",\n";
        file << "      \"passengerName\": \"" << bookings[i].getPassengerName() << "\",\n";
        file << "      \"flightNumber\": \"" << bookings[i].getFlightNumber() << "\",\n";
        file << "      \"origin\": \"" << bookings[i].getOrigin() << "\",\n";
        file << "      \"destination\": \"" << bookings[i].getDestination() << "\",\n";
        file << "      \"departureDate\": \"" << bookings[i].getDepartureDate() << "\",\n";
        file << "      \"departureTime\": \"" << bookings[i].getDepartureTime() << "\",\n";
        file << "      \"seatNumber\": \"" << bookings[i].getSeatNumber() << "\",\n";
        file << "      \"cabinClass\": \"" << bookings[i].getCabinClass() << "\",\n";
        file << "      \"totalPrice\": " << bookings[i].getTotalPrice() << ",\n";
        file << "      \"bookingStatus\": \"" << bookings[i].getBookingStatus() << "\",\n";
        file << "      \"bookingDate\": \"" << bookings[i].getBookingDate() << "\",\n";
        file << "      \"bookingTime\": \"" << bookings[i].getBookingTime() << "\"\n";
        file << "    }";
        if (i < bookingCount - 1) file << ",";
        file << "\n";
    }

    file << "  ],\n";
    file << "  \"nextBookingId\": " << nextBookingId << "\n";
    file << "}\n";

    file.close();
    std::cout << "Bookings saved successfully to " << filename << std::endl;
    return true;
}

bool BookingManager::loadBookingsFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Warning: Cannot open file for reading: " << filename << std::endl;
        std::cout << "Starting with empty booking list." << std::endl;
        return false;
    }

    // Reset counters
    bookingCount = 0;
    nextBookingId = 1000;

    // Read file content into buffer
    char buffer[50000]; // Large buffer for file content
    char* content = buffer;
    int contentSize = 0;

    // Read entire file
    while (file.good() && contentSize < sizeof(buffer) - 1) {
        file.read(&content[contentSize], 1);
        if (file.gcount() > 0) {
            contentSize++;
        }
    }
    content[contentSize] = '\0';
    file.close();

    if (contentSize == 0) {
        std::cout << "File is empty. Starting with no bookings." << std::endl;
        return false;
    }

    // Parse nextBookingId first
    char* nextIdPos = findStringInBuffer(content, "\"nextBookingId\":");
    if (nextIdPos) {
        char* colonPos = findCharInBuffer(nextIdPos, ':');
        if (colonPos) {
            nextBookingId = parseIntFromBuffer(colonPos + 1);
        }
    }

    // Parse bookings array
    char* bookingsPos = findStringInBuffer(content, "\"bookings\":");
    if (!bookingsPos) {
        std::cout << "No bookings array found in file." << std::endl;
        return false;
    }

    // Find the opening bracket of bookings array
    char* arrayStart = findCharInBuffer(bookingsPos, '[');
    if (!arrayStart) {
        std::cout << "Invalid bookings array format." << std::endl;
        return false;
    }

    // Parse each booking object
    char* pos = arrayStart + 1;
    while (*pos && bookingCount < MAX_BOOKINGS) {
        // Find next booking object
        char* objStart = findCharInBuffer(pos, '{');
        if (!objStart) break;

        char* objEnd = findMatchingBrace(objStart);
        if (!objEnd) break;

        // Extract booking object
        int objLength = objEnd - objStart + 1;
        char bookingObj[2000];
        copyStringN(bookingObj, objStart, objLength);
        bookingObj[objLength] = '\0';

        // Parse booking fields
        Booking booking;

        // Parse bookingId
        int id = parseJsonIntFromObject(bookingObj, "bookingId");
        if (id > 0) booking.setBookingId(id);

        // Parse string fields
        char tempStr[100];
        if (parseJsonStringFromObject(bookingObj, "pnr", tempStr, sizeof(tempStr))) {
            booking.setPnr(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "passengerName", tempStr, sizeof(tempStr))) {
            booking.setPassengerName(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "flightNumber", tempStr, sizeof(tempStr))) {
            booking.setFlightNumber(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "origin", tempStr, sizeof(tempStr))) {
            booking.setOrigin(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "destination", tempStr, sizeof(tempStr))) {
            booking.setDestination(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "departureDate", tempStr, sizeof(tempStr))) {
            booking.setDepartureDate(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "departureTime", tempStr, sizeof(tempStr))) {
            booking.setDepartureTime(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "seatNumber", tempStr, sizeof(tempStr))) {
            booking.setSeatNumber(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "cabinClass", tempStr, sizeof(tempStr))) {
            booking.setCabinClass(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "bookingStatus", tempStr, sizeof(tempStr))) {
            booking.setBookingStatus(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "bookingDate", tempStr, sizeof(tempStr))) {
            booking.setBookingDate(tempStr);
        }
        if (parseJsonStringFromObject(bookingObj, "bookingTime", tempStr, sizeof(tempStr))) {
            booking.setBookingTime(tempStr);
        }

        // Parse totalPrice
        double price = parseJsonDoubleFromObject(bookingObj, "totalPrice");
        if (price > 0.0) booking.setTotalPrice(price);

        // Add to bookings array
        bookings[bookingCount] = booking;
        bookingCount++;

        // Move to next booking
        pos = objEnd + 1;
    }

    std::cout << "Loaded " << bookingCount << " bookings from " << filename << std::endl;
    std::cout << "Next booking ID will be: " << nextBookingId << std::endl;
    return true;
}

double BookingManager::calculateTotalRevenue() const {
    double totalRevenue = 0.0;
    for (int i = 0; i < bookingCount; i++) {
        if (stringCompare(bookings[i].getBookingStatus(), "confirmed") ||
            stringCompare(bookings[i].getBookingStatus(), "modified")) {
            totalRevenue += bookings[i].getTotalPrice();
        }
    }
    return totalRevenue;
}

// Private helper methods implementation
void BookingManager::generatePnr(char* pnr, int bookingId) {
    // Generate a 6-character PNR based on booking ID and random letters
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    pnr[0] = letters[rand() % 26];
    pnr[1] = letters[rand() % 26];

    // Add some digits based on booking ID
    int id = bookingId % 10000;
    pnr[2] = '0' + (id / 1000);
    pnr[3] = '0' + ((id / 100) % 10);
    pnr[4] = '0' + ((id / 10) % 10);
    pnr[5] = '0' + (id % 10);
    pnr[6] = '\0';
}

void BookingManager::updateBookingStatus(int bookingId, const char* status) {
    Booking* booking = findBookingById(bookingId);
    if (booking) {
        booking->setBookingStatus(status);
    }
}

double BookingManager::calculateCancellationFee(int bookingId, int daysUntilDeparture) {
    Booking* booking = findBookingById(bookingId);
    if (!booking) return 0.0;

    double basePrice = booking->getTotalPrice();
    double fee = 0.0;

    // Calculate fee based on days until departure
    if (daysUntilDeparture <= 1) {
        fee = basePrice * 0.50; // 50% fee for same day cancellation
    } else if (daysUntilDeparture <= 7) {
        fee = basePrice * 0.30; // 30% fee for 1-7 days
    } else if (daysUntilDeparture <= 14) {
        fee = basePrice * 0.20; // 20% fee for 8-14 days
    } else if (daysUntilDeparture <= 30) {
        fee = basePrice * 0.10; // 10% fee for 15-30 days
    } else {
        fee = 50.0; // Fixed $50 fee for more than 30 days
    }

    // Minimum fee of $25
    if (fee < 25.0) fee = 25.0;

    return fee;
}

void BookingManager::getCurrentDateTime(char* date, char* time) {
    time_t now = std::time(0);
    struct tm* timeInfo = std::localtime(&now);

    // Format: YYYY-MM-DD
    sprintf(date, "%04d-%02d-%02d",
            timeInfo->tm_year + 1900,
            timeInfo->tm_mon + 1,
            timeInfo->tm_mday);

    // Format: HH:MM
    sprintf(time, "%02d:%02d",
            timeInfo->tm_hour,
            timeInfo->tm_min);
}

// Custom JSON parsing functions without STL
char* BookingManager::findStringInBuffer(const char* buffer, const char* searchStr) {
    if (!buffer || !searchStr) return nullptr;

    int searchLen = 0;
    while (searchStr[searchLen]) searchLen++; // Calculate length

    const char* pos = buffer;
    while (*pos) {
        bool match = true;
        for (int i = 0; i < searchLen; i++) {
            if (pos[i] != searchStr[i]) {
                match = false;
                break;
            }
        }
        if (match) return const_cast<char*>(pos);
        pos++;
    }
    return nullptr;
}

char* BookingManager::findCharInBuffer(const char* buffer, char ch) {
    if (!buffer) return nullptr;

    const char* pos = buffer;
    while (*pos) {
        if (*pos == ch) return const_cast<char*>(pos);
        pos++;
    }
    return nullptr;
}

char* BookingManager::findMatchingBrace(const char* start) {
    if (!start || *start != '{') return nullptr;

    int braceCount = 1;
    const char* pos = start + 1;

    while (*pos && braceCount > 0) {
        if (*pos == '{') braceCount++;
        else if (*pos == '}') braceCount--;
        if (braceCount == 0) return const_cast<char*>(pos);
        pos++;
    }
    return nullptr;
}

int BookingManager::parseIntFromBuffer(const char* buffer) {
    if (!buffer) return 0;

    // Skip whitespace
    while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n') buffer++;

    int result = 0;
    bool negative = false;

    if (*buffer == '-') {
        negative = true;
        buffer++;
    }

    while (*buffer >= '0' && *buffer <= '9') {
        result = result * 10 + (*buffer - '0');
        buffer++;
    }

    return negative ? -result : result;
}

double BookingManager::parseDoubleFromBuffer(const char* buffer) {
    if (!buffer) return 0.0;

    // Skip whitespace
    while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n') buffer++;

    double result = 0.0;
    double decimal = 0.0;
    double decimalDiv = 1.0;
    bool negative = false;
    bool afterDecimal = false;

    if (*buffer == '-') {
        negative = true;
        buffer++;
    }

    while ((*buffer >= '0' && *buffer <= '9') || *buffer == '.') {
        if (*buffer == '.') {
            afterDecimal = true;
        } else if (afterDecimal) {
            decimalDiv *= 10.0;
            decimal = decimal * 10.0 + (*buffer - '0');
        } else {
            result = result * 10.0 + (*buffer - '0');
        }
        buffer++;
    }

    result += decimal / decimalDiv;
    return negative ? -result : result;
}

void BookingManager::copyStringN(char* dest, const char* src, int n) {
    for (int i = 0; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
}

bool BookingManager::parseJsonStringFromObject(const char* json, const char* key, char* result, int maxLen) {
    // Find key in JSON object
    char searchKey[100];
    sprintf(searchKey, "\"%s\":", key);

    char* keyPos = findStringInBuffer(json, searchKey);
    if (!keyPos) return false;

    char* valueStart = findCharInBuffer(keyPos + stringLength(searchKey), '"');
    if (!valueStart) return false;

    valueStart++; // Skip opening quote
    char* valueEnd = findCharInBuffer(valueStart, '"');
    if (!valueEnd) return false;

    int valueLength = valueEnd - valueStart;
    if (valueLength >= maxLen) valueLength = maxLen - 1;

    copyStringN(result, valueStart, valueLength);
    result[valueLength] = '\0';

    return true;
}

int BookingManager::parseJsonIntFromObject(const char* json, const char* key) {
    // Find key in JSON object
    char searchKey[100];
    sprintf(searchKey, "\"%s\":", key);

    char* keyPos = findStringInBuffer(json, searchKey);
    if (!keyPos) return 0;

    char* valueStart = keyPos + stringLength(searchKey);
    return parseIntFromBuffer(valueStart);
}

double BookingManager::parseJsonDoubleFromObject(const char* json, const char* key) {
    // Find key in JSON object
    char searchKey[100];
    sprintf(searchKey, "\"%s\":", key);

    char* keyPos = findStringInBuffer(json, searchKey);
    if (!keyPos) return 0.0;

    char* valueStart = keyPos + stringLength(searchKey);
    return parseDoubleFromBuffer(valueStart);
}

bool BookingManager::containsIgnoreCase(const char* str, const char* substr) {
    if (!str || !substr) return false;

    char lowerStr[200], lowerSubstr[200];
    copyString(lowerStr, str, sizeof(lowerStr));
    copyString(lowerSubstr, substr, sizeof(lowerSubstr));

    toLowerCase(lowerStr);
    toLowerCase(lowerSubstr);

    return contains(lowerStr, lowerSubstr);
}
