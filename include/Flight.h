#ifndef FLIGHT_H
#define FLIGHT_H

#pragma once
#include <nlohmann/json.hpp>
#include "StringUtils.h"

class Flight {
private:
    char airlineName[50];
    char flightNumber[10];
    char origin[10];
    char destination[10];
    char departureTime[20];
    char arrivalTime[20];
    char departureDate[15];
    char arrivalDate[15];
    char aircraftType[30];
    double basePrice;
    int totalSeats;
    int availableSeats;
    char duration[10];

public:
    // Constructors and Destructor
    Flight();
    Flight(const char* airline, const char* flightNum, const char* orig,
           const char* dest, const char* depTime, const char* arrTime,
           const char* depDate, const char* arrDate, const char* aircraft,
           double price, int total, int available);
    ~Flight();

    // Setters
    void setAirlineName(const char* airline);
    void setFlightNumber(const char* flightNum);
    void setOrigin(const char* orig);
    void setDestination(const char* dest);
    void setDepartureTime(const char* depTime);
    void setArrivalTime(const char* arrTime);
    void setDepartureDate(const char* depDate);
    void setArrivalDate(const char* arrDate);
    void setAircraftType(const char* aircraft);
    void setBasePrice(double price);
    void setTotalSeats(int total);
    void setAvailableSeats(int available);
    void setDuration(const char* dur);

    // Getters
    const char* getAirlineName() const;
    const char* getFlightNumber() const;
    const char* getOrigin() const;
    const char* getDestination() const;
    const char* getDepartureTime() const;
    const char* getArrivalTime() const;
    const char* getDepartureDate() const;
    const char* getArrivalDate() const;
    const char* getAircraftType() const;
    double getBasePrice() const;
    int getTotalSeats() const;
    int getAvailableSeats() const;
    const char* getDuration() const;

    // Pricing and Availability Methods
    double calculateDynamicPrice(int daysUntilDeparture) const;
    bool hasAvailableSeats() const;
    bool isAvailable() const;  // Added missing method

    // Display Methods
    void displayFlightInfo() const;
    void displayDetailedInfo() const;

    // JSON serialization
    void toJson(nlohmann::json& j) const;
    void fromJson(const nlohmann::json& j);

private:
    // Helper methods for internal calculations
    double calculateSurgePrice() const;
    double calculateTimeBasedPrice(int daysUntilDeparture) const;
};


#endif // FLIGHT_H