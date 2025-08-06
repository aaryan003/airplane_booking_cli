#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <iostream>

class Booking {
private:
    int bookingId;
    char pnr[10];
    char passengerName[50];
    char flightNumber[10];
    char origin[10];
    char destination[10];
    char departureDate[15];
    char departureTime[10];
    char seatNumber[5];
    char cabinClass[15];
    double totalPrice;
    char bookingStatus[20];
    char bookingDate[15];
    char bookingTime[10];

    // Private helper methods
    void copyString(char* dest, const char* src, int maxLen);
    void getCurrentDateTime(char* date, char* time);
    bool stringCompare(const char* str1, const char* str2) const;

public:
    // Constructors & Destructor
    Booking();
    Booking(int id, const char* passengerN, const char* flightNum,
            const char* orig, const char* dest, const char* depDate,
            const char* depTime, const char* seat, const char* cabin, double price);
    ~Booking();

    // Setters
    void setBookingId(int id);
    void setPnr(const char* pnrCode);
    void setPassengerName(const char* name);
    void setFlightNumber(const char* flightNum);
    void setOrigin(const char* orig);
    void setDestination(const char* dest);
    void setDepartureDate(const char* date);
    void setDepartureTime(const char* time);
    void setSeatNumber(const char* seat);
    void setCabinClass(const char* cabin);
    void setTotalPrice(double price);
    void setBookingStatus(const char* status);
    void setBookingDate(const char* date);
    void setBookingTime(const char* time);

    // Getters
    int getBookingId() const;
    const char* getPnr() const;
    const char* getPassengerName() const;
    const char* getFlightNumber() const;
    const char* getOrigin() const;
    const char* getDestination() const;
    const char* getDepartureDate() const;
    const char* getDepartureTime() const;
    const char* getSeatNumber() const;
    const char* getCabinClass() const;
    double getTotalPrice() const;
    const char* getBookingStatus() const;
    const char* getBookingDate() const;
    const char* getBookingTime() const;

    // Display booking information
    void displayBookingDetails() const;
    void displayBookingSummary() const;

    // Status checks
    bool isActive() const;
    bool canBeModified() const;
    bool canBeCancelled() const;
};

#endif // BOOKING_H