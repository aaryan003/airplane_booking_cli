#pragma once
#ifndef FLIGHTLIST_H
#define FLIGHTLIST_H

#include "Flight.h"
#include "UserInput.h"
#include <nlohmann/json.hpp>
#include "StringUtils.h"
#include <string>

class FlightList {
private:
    static const int MAX_FLIGHTS = 50;
    Flight flights[MAX_FLIGHTS];
    int flightCount;

public:
    FlightList();
    ~FlightList();

    // Flight management methods
    bool addFlight(const Flight& flight);
    Flight* getFlightByIndex(int index);
    const Flight* getFlightByIndex(int index) const;
    int getCount() const;
    int getFlightCount() const;  // Added missing method
    void clearFlights();
    int getIndexOfFlight(const char* flightNumber) const;

    // Search methods - Added missing method
    int searchByRoute(const char* origin, const char* destination, const char* departureDate);

    // Display methods
    void displayFlights() const;
    void displayFlightDetails(int index) const;
    void displayFlightSummary() const;

    // Data persistence using JSON
    void saveFlightsToFile(const char* filename) const;
    void loadFlightsFromFile(const char* filename);

    // Mock data loading
    void loadMockData(const UserInput& userInput);

private:
    // Helper method for loading mock flights for specific route (as backup)
    void loadMockDataForRoute(const char* origin, const char* destination, const char* departureDate);
};

#endif // FLIGHTLIST_H