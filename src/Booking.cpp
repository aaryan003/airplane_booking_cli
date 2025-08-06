#include "Booking.h"
#include "StringUtils.h"
#include <iostream>

Booking::Booking() {
    bookingId = 0;
    pnr[0] = '\0';
    passengerName[0] = '\0';
    flightNumber[0] = '\0';
    origin[0] = '\0';
    destination[0] = '\0';
    departureDate[0] = '\0';
    departureTime[0] = '\0';
    seatNumber[0] = '\0';
    cabinClass[0] = '\0';
    totalPrice = 0.0;
    bookingStatus[0] = '\0';
    bookingDate[0] = '\0';
    bookingTime[0] = '\0';
}

Booking::Booking(int id, const char* passengerN, const char* flightNum,
                 const char* orig, const char* dest, const char* depDate,
                 const char* depTime, const char* seat, const char* cabin, double price) {
    setBookingId(id);
    setPassengerName(passengerN);
    setFlightNumber(flightNum);
    setOrigin(orig);
    setDestination(dest);
    setDepartureDate(depDate);
    setDepartureTime(depTime);
    setSeatNumber(seat);
    setCabinClass(cabin);
    setTotalPrice(price);
    setBookingStatus("confirmed");
    pnr[0] = '\0';
    bookingDate[0] = '\0';
    bookingTime[0] = '\0';
}

Booking::~Booking() {}

void Booking::copyString(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (i < maxLen - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void Booking::getCurrentDateTime(char* date, char* time) {
    // This is a simple mock implementation.
    // In a real application, you'd use a library like <chrono> or <ctime>.
    copyString(date, "2025-08-06", 15);
    copyString(time, "22:37", 10);
}

bool Booking::stringCompare(const char* str1, const char* str2) const {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == str2[i];
}

void Booking::setBookingId(int id) { bookingId = id; }
void Booking::setPnr(const char* pnrCode) { copyString(pnr, pnrCode, 10); }
void Booking::setPassengerName(const char* name) { copyString(passengerName, name, 50); }
void Booking::setFlightNumber(const char* flightNum) { copyString(flightNumber, flightNum, 10); }
void Booking::setOrigin(const char* orig) { copyString(origin, orig, 10); }
void Booking::setDestination(const char* dest) { copyString(destination, dest, 10); }
void Booking::setDepartureDate(const char* date) { copyString(departureDate, date, 15); }
void Booking::setDepartureTime(const char* time) { copyString(departureTime, time, 10); }
void Booking::setSeatNumber(const char* seat) { copyString(seatNumber, seat, 5); }
void Booking::setCabinClass(const char* cabin) { copyString(cabinClass, cabin, 15); }
void Booking::setTotalPrice(double price) { totalPrice = price; }
void Booking::setBookingStatus(const char* status) { copyString(bookingStatus, status, 20); }
void Booking::setBookingDate(const char* date) { copyString(bookingDate, date, 15); }
void Booking::setBookingTime(const char* time) { copyString(bookingTime, time, 10); }

int Booking::getBookingId() const { return bookingId; }
const char* Booking::getPnr() const { return pnr; }
const char* Booking::getPassengerName() const { return passengerName; }
const char* Booking::getFlightNumber() const { return flightNumber; }
const char* Booking::getOrigin() const { return origin; }
const char* Booking::getDestination() const { return destination; }
const char* Booking::getDepartureDate() const { return departureDate; }
const char* Booking::getDepartureTime() const { return departureTime; }
const char* Booking::getSeatNumber() const { return seatNumber; }
const char* Booking::getCabinClass() const { return cabinClass; }
double Booking::getTotalPrice() const { return totalPrice; }
const char* Booking::getBookingStatus() const { return bookingStatus; }
const char* Booking::getBookingDate() const { return bookingDate; }
const char* Booking::getBookingTime() const { return bookingTime; }

void Booking::displayBookingDetails() const {
    std::cout << "\n=== BOOKING DETAILS ===" << std::endl;
    std::cout << "Booking ID: " << getBookingId() << std::endl;
    std::cout << "PNR: " << getPnr() << std::endl;
    std::cout << "Passenger: " << getPassengerName() << std::endl;
    std::cout << "Flight: " << getFlightNumber() << std::endl;
    std::cout << "Route: " << getOrigin() << " -> " << getDestination() << std::endl;
    std::cout << "Departure: " << getDepartureDate() << " at " << getDepartureTime() << std::endl;
    std::cout << "Seat: " << getSeatNumber() << std::endl;
    std::cout << "Class: " << getCabinClass() << std::endl;
    std::cout << "Total Price: $" << getTotalPrice() << std::endl;
    std::cout << "Status: " << getBookingStatus() << std::endl;
    std::cout << "Booked On: " << getBookingDate() << " at " << getBookingTime() << std::endl;
    std::cout << "=======================" << std::endl;
}

void Booking::displayBookingSummary() const {
    std::cout << "Booking ID: " << getBookingId() << " | PNR: " << getPnr() << std::endl;
    std::cout << "Passenger: " << getPassengerName() << " | Flight: " << getFlightNumber() << std::endl;
    std::cout << "Route: " << getOrigin() << " -> " << getDestination() << std::endl;
    std::cout << "Status: " << getBookingStatus() << std::endl;
    std::cout << "Price: $" << getTotalPrice() << std::endl;
}

bool Booking::isActive() const {
    return stringCompare(bookingStatus, "confirmed") || stringCompare(bookingStatus, "modified");
}

bool Booking::canBeModified() const {
    return isActive();
}

bool Booking::canBeCancelled() const {
    return isActive();
}