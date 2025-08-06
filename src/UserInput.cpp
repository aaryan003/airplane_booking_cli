#include "UserInput.h"
#include "StringUtils.h"
#include <iostream>
#include <limits>

UserInput::UserInput() {
    origin[0] = '\0';
    destination[0] = '\0';
    tripType[0] = '\0';
    departureDate[0] = '\0';
    returnDate[0] = '\0';
    travelers = 0;
    cabinClass[0] = '\0';
}

UserInput::~UserInput() {}

void UserInput::promptUserDetails() {
    std::cout << "\n=== Flight Search Details ===" << std::endl;
    promptOrigin();
    promptDestination();
    promptTripType();
    promptDepartureDate();
    if (stringCompare(tripType, "round-trip")) {
        promptReturnDate();
    }
    promptTravelers();
    promptCabinClass();
    displaySearchSummary();
}

void UserInput::promptOrigin() {
    std::cout << "Enter departure city/airport code (e.g., NYC, DEL): ";
    std::cin.getline(origin, 10);
    toUpperCase(origin);
}

void UserInput::promptDestination() {
    std::cout << "Enter destination city/airport code (e.g., BOM, LON): ";
    std::cin.getline(destination, 10);
    toUpperCase(destination);
}

void UserInput::promptTripType() {
    std::cout << "Trip type (1: One-way, 2: Round-trip): ";
    int choice;
    std::cin >> choice;
    clearInputBuffer();
    if (choice == 1) {
        copyString(tripType, "one-way", 20);
    } else if (choice == 2) {
        copyString(tripType, "round-trip", 20);
    } else {
        std::cout << "Invalid choice. Defaulting to one-way." << std::endl;
        copyString(tripType, "one-way", 20);
    }
}

void UserInput::promptDepartureDate() {
    std::cout << "Enter departure date (YYYY-MM-DD): ";
    std::cin.getline(departureDate, 15);
    if (!isValidDate(departureDate)) {
        std::cout << "Invalid date format. Please use YYYY-MM-DD." << std::endl;
        promptDepartureDate();
    }
}

void UserInput::promptReturnDate() {
    std::cout << "Enter return date (YYYY-MM-DD): ";
    std::cin.getline(returnDate, 15);
    if (!isValidDate(returnDate)) {
        std::cout << "Invalid date format. Please use YYYY-MM-DD." << std::endl;
        promptReturnDate();
    }
}

void UserInput::promptTravelers() {
    std::cout << "Number of travelers (1-9): ";
    int choice = 0;
    std::cin >> choice;
    clearInputBuffer();
    travelers = choice;
    if (travelers < 1 || travelers > 9) {
        std::cout << "Invalid number. Please enter 1-9." << std::endl;
        promptTravelers();
    }
}

void UserInput::promptCabinClass() {
    std::cout << "Cabin class (1: Economy, 2: Business, 3: First): ";
    int choice;
    std::cin >> choice;
    clearInputBuffer();
    switch (choice) {
        case 1:
            copyString(cabinClass, "economy", 15);
            break;
        case 2:
            copyString(cabinClass, "business", 15);
            break;
        case 3:
            copyString(cabinClass, "first", 15);
            break;
        default:
            std::cout << "Invalid choice. Defaulting to economy." << std::endl;
            copyString(cabinClass, "economy", 15);
    }
}

const char* UserInput::getOrigin() const { return origin; }
const char* UserInput::getDestination() const { return destination; }
const char* UserInput::getTripType() const { return tripType; }
const char* UserInput::getDepartureDate() const { return departureDate; }
const char* UserInput::getReturnDate() const { return returnDate; }
int UserInput::getTravelers() const { return travelers; }
const char* UserInput::getCabinClass() const { return cabinClass; }

bool UserInput::isValidDate(const char* date) const {
    if (stringLength(date) != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }
    return true;
}

bool UserInput::isValidAirportCode(const char* code) const {
    int len = stringLength(code);
    return (len >= 3 && len <= 4);
}

void UserInput::displaySearchSummary() const {
    std::cout << "\n--- Search Summary ---" << std::endl;
    std::cout << "From: " << origin << std::endl;
    std::cout << "To: " << destination << std::endl;
    std::cout << "Trip Type: " << tripType << std::endl;
    std::cout << "Departure: " << departureDate << std::endl;
    if (stringCompare(tripType, "round-trip")) {
        std::cout << "Return: " << returnDate << std::endl;
    }
    std::cout << "Travelers: " << travelers << std::endl;
    std::cout << "Class: " << cabinClass << std::endl;
    std::cout << "----------------------" << std::endl;
}

void UserInput::clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}