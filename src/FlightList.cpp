#include "FlightList.h"
#include "StringUtils.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

FlightList::FlightList() : flightCount(0) {}
FlightList::~FlightList() {}

bool FlightList::addFlight(const Flight& flight) {
    if (flightCount >= MAX_FLIGHTS) {
        return false;
    }
    flights[flightCount] = flight;
    flightCount++;
    return true;
}

Flight* FlightList::getFlightByIndex(int index) {
    if (index < 0 || index >= flightCount) {
        return nullptr;
    }
    return &flights[index];
}

const Flight* FlightList::getFlightByIndex(int index) const {
    if (index < 0 || index >= flightCount) {
        return nullptr;
    }
    return &flights[index];
}

int FlightList::getCount() const {
    return flightCount;
}

int FlightList::getIndexOfFlight(const char* flightNumber) const {
    for (int i = 0; i < flightCount; ++i) {
        if (stringCompare(flights[i].getFlightNumber(), flightNumber)) {
            return i;
        }
    }
    return -1;
}

void FlightList::clearFlights() {
    flightCount = 0;
}

void FlightList::displayFlights() const {
    if (flightCount == 0) {
        std::cout << "No flights available." << std::endl;
        return;
    }
    std::cout << "\n+----+----------+------+------+----------+----------+----------+---------+" << std::endl;
    std::cout << "| No | Flight   | From | To   | Depart   | Arrive   | Aircraft | Price   |" << std::endl;
    std::cout << "+----+----------+------+------+----------+----------+----------+---------+" << std::endl;
    for (int i = 0; i < flightCount; i++) {
        std::cout << "| " << (i + 1) << "  | " << flights[i].getFlightNumber()
                  << " | " << flights[i].getOrigin()
                  << " | " << flights[i].getDestination()
                  << " | " << flights[i].getDepartureTime()
                  << " | " << flights[i].getArrivalTime()
                  << " | " << flights[i].getAircraftType()
                  << " | $" << flights[i].getBasePrice() << " |" << std::endl;
    }
    std::cout << "+----+----------+------+------+----------+----------+----------+---------+" << std::endl;
}

void FlightList::displayFlightDetails(int index) const {
    const Flight* flight = getFlightByIndex(index);
    if (flight) {
        flight->displayDetailedInfo();
    } else {
        std::cout << "Invalid flight index." << std::endl;
    }
}

void FlightList::displayFlightSummary() const {
    std::cout << "\nTotal flights found: " << flightCount << std::endl;
    if (flightCount > 0) {
        double minPrice = flights[0].getBasePrice();
        double maxPrice = flights[0].getBasePrice();
        for (int i = 1; i < flightCount; i++) {
            double price = flights[i].getBasePrice();
            if (price < minPrice) minPrice = price;
            if (price > maxPrice) maxPrice = price;
        }
        std::cout << "Price range: $" << minPrice << " - $" << maxPrice << std::endl;
    }
}

void FlightList::saveFlightsToFile(const char* filename) const {
    nlohmann::json j = nlohmann::json::array();
    for (int i = 0; i < flightCount; ++i) {
        nlohmann::json flightJson;
        flights[i].toJson(flightJson);
        j.push_back(flightJson);
    }
    std::ofstream o(filename);
    if (o.is_open()) {
        o << j.dump(4);
        o.close();
    } else {
        std::cout << "Error: Could not open " << filename << " for writing." << std::endl;
    }
}

void FlightList::loadFlightsFromFile(const char* filename) {
    clearFlights();
    std::ifstream i(filename);
    if (!i.is_open()) {
        std::cout << "Error: Could not open " << filename << " for reading. File may not exist." << std::endl;
        return;
    }
    try {
        nlohmann::json j;
        i >> j;
        for (const auto& flightJson : j) {
            Flight f;
            f.fromJson(flightJson);
            addFlight(f);
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cout << "JSON parse error: " << e.what() << std::endl;
    }
    i.close();
}

void FlightList::loadMockData(const UserInput& userInput) {
    clearFlights();
    std::cout << "=== CREATING MOCK FLIGHT DATA ===" << std::endl;
    const char* searchOrigin = userInput.getOrigin();
    const char* searchDestination = userInput.getDestination();
    const char* searchDate = userInput.getDepartureDate();
    std::cout << "\nCreating flights that match search criteria..." << std::endl;

    Flight flight1;
    flight1.setAirlineName("Air India");
    flight1.setFlightNumber("AI101");
    flight1.setOrigin(searchOrigin);
    flight1.setDestination(searchDestination);
    flight1.setDepartureDate(searchDate);
    flight1.setArrivalDate(searchDate);
    flight1.setDepartureTime("08:30");
    flight1.setArrivalTime("22:45");
    flight1.setAircraftType("Boeing 787-8");
    flight1.setTotalSeats(248);
    flight1.setAvailableSeats(45);
    flight1.setBasePrice(299.99);
    flight1.setDuration("14h 15m");
    if (addFlight(flight1)) {
        std::cout << "✓ Added flight AI101: " << searchOrigin << " → " << searchDestination << " on " << searchDate << std::endl;
    } else {
        std::cout << "✗ Failed to add flight AI101" << std::endl;
    }

    Flight flight2;
    flight2.setAirlineName("Air India");
    flight2.setFlightNumber("AI103");
    flight2.setOrigin(searchOrigin);
    flight2.setDestination(searchDestination);
    flight2.setDepartureDate(searchDate);
    flight2.setArrivalDate(searchDate);
    flight2.setDepartureTime("14:15");
    flight2.setArrivalTime("04:30");
    flight2.setAircraftType("Boeing 777-300ER");
    flight2.setTotalSeats(342);
    flight2.setAvailableSeats(78);
    flight2.setBasePrice(349.99);
    flight2.setDuration("14h 15m");
    if (addFlight(flight2)) {
        std::cout << "✓ Added flight AI103: " << searchOrigin << " → " << searchDestination << " on " << searchDate << std::endl;
    } else {
        std::cout << "✗ Failed to add flight AI103" << std::endl;
    }

    Flight flight3;
    flight3.setAirlineName("Air India");
    flight3.setFlightNumber("AI105");
    flight3.setOrigin(searchOrigin);
    flight3.setDestination(searchDestination);
    flight3.setDepartureDate(searchDate);
    flight3.setArrivalDate(searchDate);
    flight3.setDepartureTime("23:45");
    flight3.setArrivalTime("19:30");
    flight3.setAircraftType("Airbus A350-900");
    flight3.setTotalSeats(314);
    flight3.setAvailableSeats(92);
    flight3.setBasePrice(279.99);
    flight3.setDuration("19h 45m");
    if (addFlight(flight3)) {
        std::cout << "✓ Added flight AI105: " << searchOrigin << " → " << searchDestination << " on " << searchDate << std::endl;
    } else {
        std::cout << "✗ Failed to add flight AI105" << std::endl;
    }

    // Adding non-matching flights for testing
    addFlight(Flight("Air India", "AI201", "JFK", "DEL", "10:30", "12:45", "2025-09-08", "2025-09-08", "Boeing 787-9", 320.99, 296, 67));
    addFlight(Flight("United Airlines", "UA301", "LAX", "BOM", "16:20", "22:15", "2025-09-08", "2025-09-08", "Boeing 777-200LR", 389.99, 317, 123));
    addFlight(Flight("Air India", "AI107", "NYC", "BOM", "11:45", "03:30", "2025-09-09", "2025-09-09", "Boeing 787-8", 310.99, 248, 34));

    std::cout << "\n=== MOCK DATA CREATION COMPLETE ===" << std::endl;
    std::cout << "Total flights created: " << flightCount << std::endl;
    std::cout << "=====================================" << std::endl;
}

int FlightList::getFlightCount() const {
    return flightCount;
}

int FlightList::searchByRoute(const char* origin, const char* destination, const char* departureDate) {
    if (!origin || !destination || !departureDate) {
        return 0;
    }

    // First, try to load flights from JSON file
    loadFlightsFromFile("data/flights.json");

    // If no flights loaded from JSON, use mock data as backup
    if (flightCount == 0) {
        std::cout << "flights.json not found or empty. Loading mock data as backup..." << std::endl;

        // Create a temporary UserInput object with search criteria
        UserInput tempUserInput;
        // Note: Since UserInput doesn't have setters, we'll create mock data directly
        loadMockDataForRoute(origin, destination, departureDate);
    }

    // Count matching flights
    int foundCount = 0;
    for (int i = 0; i < flightCount; i++) {
        bool originMatch = stringCompare(flights[i].getOrigin(), origin);
        bool destMatch = stringCompare(flights[i].getDestination(), destination);
        bool dateMatch = stringCompare(flights[i].getDepartureDate(), departureDate);

        if (originMatch && destMatch && dateMatch) {
            foundCount++;
        }
    }

    return foundCount;
}

// Helper method to load mock data for specific route (as backup)
void FlightList::loadMockDataForRoute(const char* origin, const char* destination, const char* departureDate) {
    clearFlights();

    std::cout << "=== LOADING BACKUP MOCK DATA FOR ROUTE ===" << std::endl;
    std::cout << "Route: " << origin << " → " << destination << " on " << departureDate << std::endl;

    // Create flights for the specific route
    Flight flight1;
    flight1.setAirlineName("Air India");
    flight1.setFlightNumber("AI101");
    flight1.setOrigin(origin);
    flight1.setDestination(destination);
    flight1.setDepartureDate(departureDate);
    flight1.setArrivalDate(departureDate);
    flight1.setDepartureTime("08:30");
    flight1.setArrivalTime("22:45");
    flight1.setAircraftType("Boeing 787-8");
    flight1.setTotalSeats(248);
    flight1.setAvailableSeats(45);
    flight1.setBasePrice(299.99);
    flight1.setDuration("14h 15m");
    addFlight(flight1);

    Flight flight2;
    flight2.setAirlineName("IndiGo");
    flight2.setFlightNumber("6E201");
    flight2.setOrigin(origin);
    flight2.setDestination(destination);
    flight2.setDepartureDate(departureDate);
    flight2.setArrivalDate(departureDate);
    flight2.setDepartureTime("14:15");
    flight2.setArrivalTime("04:30");
    flight2.setAircraftType("Airbus A320");
    flight2.setTotalSeats(180);
    flight2.setAvailableSeats(67);
    flight2.setBasePrice(199.99);
    flight2.setDuration("12h 15m");
    addFlight(flight2);

    Flight flight3;
    flight3.setAirlineName("Vistara");
    flight3.setFlightNumber("UK301");
    flight3.setOrigin(origin);
    flight3.setDestination(destination);
    flight3.setDepartureDate(departureDate);
    flight3.setArrivalDate(departureDate);
    flight3.setDepartureTime("19:30");
    flight3.setArrivalTime("09:45");
    flight3.setAircraftType("Airbus A321");
    flight3.setTotalSeats(158);
    flight3.setAvailableSeats(23);
    flight3.setBasePrice(279.99);
    flight3.setDuration("14h 15m");
    addFlight(flight3);

    std::cout << "Loaded " << flightCount << " backup flights for route." << std::endl;
    std::cout << "===========================================" << std::endl;
}

// Add this new public getter method to FlightList.h
Flight* FlightList::getFlightByNumber(const char* flightNumber) {
    for (int i = 0; i < flightCount; ++i) {
        if (stringCompare(flights[i].getFlightNumber(), flightNumber)) {
            return &flights[i];
        }
    }
    return nullptr;
}
