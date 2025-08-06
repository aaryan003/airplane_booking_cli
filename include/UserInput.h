#pragma once
#ifndef USERINPUT_H
#define USERINPUT_H

#include "StringUtils.h"
#include <iostream>

class UserInput {
private:
    char origin[10];
    char destination[10];
    char tripType[20];
    char departureDate[15];
    char returnDate[15];
    int travelers;
    char cabinClass[15];

public:
    UserInput();
    ~UserInput();

    // User input prompts
    void promptUserDetails();
    void promptOrigin();
    void promptDestination();
    void promptTripType();
    void promptDepartureDate();
    void promptReturnDate();
    void promptTravelers();
    void promptCabinClass();

    // Getters for search criteria
    const char* getOrigin() const;
    const char* getDestination() const;
    const char* getTripType() const;
    const char* getDepartureDate() const;
    const char* getReturnDate() const;
    int getTravelers() const;
    const char* getCabinClass() const;

    // Validation and display
    bool isValidDate(const char* date) const;
    bool isValidAirportCode(const char* code) const;
    void displaySearchSummary() const;
    void clearInputBuffer();
};

#endif // USERINPUT_H