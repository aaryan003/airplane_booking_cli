#include "Flight.h"
#include "StringUtils.h"
#include <iostream>

Flight::Flight() {
    airlineName[0] = '\0';
    flightNumber[0] = '\0';
    origin[0] = '\0';
    destination[0] = '\0';
    departureTime[0] = '\0';
    arrivalTime[0] = '\0';
    departureDate[0] = '\0';
    arrivalDate[0] = '\0';
    aircraftType[0] = '\0';
    basePrice = 0.0;
    totalSeats = 0;
    availableSeats = 0;
    duration[0] = '\0';
}

Flight::Flight(const char* airline, const char* flightNum, const char* orig,
               const char* dest, const char* depTime, const char* arrTime,
               const char* depDate, const char* arrDate, const char* aircraft,
               double price, int total, int available) {
    setAirlineName(airline);
    setFlightNumber(flightNum);
    setOrigin(orig);
    setDestination(dest);
    setDepartureTime(depTime);
    setArrivalTime(arrTime);
    setDepartureDate(depDate);
    setArrivalDate(arrDate);
    setAircraftType(aircraft);
    setBasePrice(price);
    setTotalSeats(total);
    setAvailableSeats(available);
    copyString(duration, "3h 30m", 10);
}

Flight::~Flight() {}

void Flight::setAirlineName(const char* airline) { copyString(airlineName, airline, 50); }
void Flight::setFlightNumber(const char* flightNum) { copyString(flightNumber, flightNum, 10); }
void Flight::setOrigin(const char* orig) { copyString(origin, orig, 10); }
void Flight::setDestination(const char* dest) { copyString(destination, dest, 10); }
void Flight::setDepartureTime(const char* depTime) { copyString(departureTime, depTime, 20); }
void Flight::setArrivalTime(const char* arrTime) { copyString(arrivalTime, arrTime, 20); }
void Flight::setDepartureDate(const char* depDate) { copyString(departureDate, depDate, 15); }
void Flight::setArrivalDate(const char* arrDate) { copyString(arrivalDate, arrDate, 15); }
void Flight::setAircraftType(const char* aircraft) { copyString(aircraftType, aircraft, 30); }
void Flight::setBasePrice(double price) { basePrice = price; }
void Flight::setTotalSeats(int total) { totalSeats = total; }
void Flight::setAvailableSeats(int available) { availableSeats = available; }
void Flight::setDuration(const char* dur) { copyString(duration, dur, 10); }

const char* Flight::getAirlineName() const { return airlineName; }
const char* Flight::getFlightNumber() const { return flightNumber; }
const char* Flight::getOrigin() const { return origin; }
const char* Flight::getDestination() const { return destination; }
const char* Flight::getDepartureTime() const { return departureTime; }
const char* Flight::getArrivalTime() const { return arrivalTime; }
const char* Flight::getDepartureDate() const { return departureDate; }
const char* Flight::getArrivalDate() const { return arrivalDate; }
const char* Flight::getAircraftType() const { return aircraftType; }
double Flight::getBasePrice() const { return basePrice; }
int Flight::getTotalSeats() const { return totalSeats; }
int Flight::getAvailableSeats() const { return availableSeats; }
const char* Flight::getDuration() const { return duration; }

double Flight::calculateDynamicPrice(int daysUntilDeparture) const {
    double dynamicPrice = basePrice;
    dynamicPrice += calculateSurgePrice();
    dynamicPrice += calculateTimeBasedPrice(daysUntilDeparture);
    return dynamicPrice;
}

bool Flight::hasAvailableSeats() const { return availableSeats > 0; }

void Flight::displayFlightInfo() const {
    std::cout << "Flight: " << flightNumber << " - " << airlineName << std::endl;
    std::cout << "Route: " << origin << " to " << destination << std::endl;
    std::cout << "Time: " << departureTime << " - " << arrivalTime << std::endl;
    std::cout << "Price: $" << basePrice << std::endl;
}

void Flight::displayDetailedInfo() const {
    std::cout << "\n=== FLIGHT DETAILS ===" << std::endl;
    std::cout << "Airline: " << airlineName << std::endl;
    std::cout << "Flight Number: " << flightNumber << std::endl;
    std::cout << "Aircraft: " << aircraftType << std::endl;
    std::cout << "Route: " << origin << " → " << destination << std::endl;
    std::cout << "Date: " << departureDate << std::endl;
    std::cout << "Departure: " << departureTime << std::endl;
    std::cout << "Arrival: " << arrivalTime << std::endl;
    std::cout << "Duration: " << duration << std::endl;
    std::cout << "Base Price: $" << basePrice << std::endl;
    std::cout << "Available Seats: " << availableSeats << "/" << totalSeats << std::endl;
    std::cout << "=====================" << std::endl;
}

double Flight::calculateSurgePrice() const {
    double occupancyRate = (double)(totalSeats - availableSeats) / totalSeats;
    if (occupancyRate > 0.9) return basePrice * 0.3;
    else if (occupancyRate > 0.7) return basePrice * 0.2;
    else if (occupancyRate > 0.5) return basePrice * 0.1;
    return 0.0;
}

double Flight::calculateTimeBasedPrice(int daysUntilDeparture) const {
    if (daysUntilDeparture <= 1) return basePrice * 0.5;
    else if (daysUntilDeparture <= 7) return basePrice * 0.3;
    else if (daysUntilDeparture <= 14) return basePrice * 0.2;
    else if (daysUntilDeparture <= 30) return basePrice * 0.1;
    return 0.0;
}

void Flight::toJson(nlohmann::json& j) const {
    j["airlineName"] = airlineName;
    j["flightNumber"] = flightNumber;
    j["origin"] = origin;
    j["destination"] = destination;
    j["departureTime"] = departureTime;
    j["arrivalTime"] = arrivalTime;
    j["departureDate"] = departureDate;
    j["arrivalDate"] = arrivalDate;
    j["aircraftType"] = aircraftType;
    j["basePrice"] = basePrice;
    j["totalSeats"] = totalSeats;
    j["availableSeats"] = availableSeats;
    j["duration"] = duration;
}

bool Flight::isAvailable() const {
    return availableSeats > 0;
}

void Flight::fromJson(const nlohmann::json& j) {
    copyString(airlineName, j.at("airlineName").get<std::string>().c_str(), 50);
    copyString(flightNumber, j.at("flightNumber").get<std::string>().c_str(), 10);
    copyString(origin, j.at("origin").get<std::string>().c_str(), 10);
    copyString(destination, j.at("destination").get<std::string>().c_str(), 10);
    copyString(departureTime, j.at("departureTime").get<std::string>().c_str(), 20);
    copyString(arrivalTime, j.at("arrivalTime").get<std::string>().c_str(), 20);
    copyString(departureDate, j.at("departureDate").get<std::string>().c_str(), 15);
    copyString(arrivalDate, j.at("arrivalDate").get<std::string>().c_str(), 15);
    copyString(aircraftType, j.at("aircraftType").get<std::string>().c_str(), 30);
    basePrice = j.at("basePrice").get<double>();
    totalSeats = j.at("totalSeats").get<int>();
    availableSeats = j.at("availableSeats").get<int>();
    copyString(duration, j.at("duration").get<std::string>().c_str(), 10);
}