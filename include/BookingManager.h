#ifndef BOOKINGMANAGER_H
#define BOOKINGMANAGER_H

#include "Booking.h"

class BookingManager {
private:
    static const int MAX_BOOKINGS = 100;
    Booking bookings[MAX_BOOKINGS];
    int bookingCount;
    int nextBookingId;

    // Private helper methods (Existing)
    void generatePnr(char* pnr, int bookingId);
    Booking* findBookingById(int bookingId);
    void updateBookingStatus(int bookingId, const char* status);
    double calculateCancellationFee(int bookingId, int daysUntilDeparture);
    void getCurrentDateTime(char* date, char* time);

    // Private helper methods (New) - No STL dependencies
    bool parseJsonStringBasic(const char* json, const char* key, char* result, int maxLen);
    bool parseJsonIntBasic(const char* json, const char* key, int& result);
    bool parseJsonDoubleBasic(const char* json, const char* key, double& result);
    bool containsIgnoreCase(const char* str, const char* substr);

    // Custom JSON parsing helper functions
    char* findStringInBuffer(const char* buffer, const char* searchStr);
    char* findCharInBuffer(const char* buffer, char ch);
    char* findMatchingBrace(const char* start);
    int parseIntFromBuffer(const char* buffer);
    double parseDoubleFromBuffer(const char* buffer);
    void copyStringN(char* dest, const char* src, int n);
    bool parseJsonStringFromObject(const char* json, const char* key, char* result, int maxLen);
    int parseJsonIntFromObject(const char* json, const char* key);
    double parseJsonDoubleFromObject(const char* json, const char* key);

public:
    BookingManager();
    ~BookingManager();

    // Core booking operations
    int createBooking(const char* passengerName, const char* flightNumber,
                      const char* origin, const char* destination,
                      const char* departureDate, const char* departureTime,
                      const char* seatNumber, const char* cabinClass, double totalPrice);

    bool modifyBooking(int bookingId);
    bool cancelBooking(int bookingId);

    // Display methods
    void displayBookingDetails(int bookingId);
    void displayAllBookings();
    void displayBookingsByPassenger(const char* passengerName);

    // Search methods
    int findBookingByPnr(const char* pnr);
    void searchBookingsByFlight(const char* flightNumber);

    // Utility methods
    bool isValidBookingId(int bookingId);
    bool isBookingActive(int bookingId);
    bool modifyPassengerName(int bookingId, const char* newName);
    bool modifySeatNumber(int bookingId, const char* newSeat);
    bool modifyCabinClass(int bookingId, const char* newClass);
    bool modifyDepartureDate(int bookingId, const char* newDate);

    // File operations
    bool saveBookingsToFile(const char* filename);
    bool loadBookingsFromFile(const char* filename);

    // Statistics methods
    int getBookingCount() const { return bookingCount; }
    double calculateTotalRevenue() const;
};

#endif // BOOKINGMANAGER_H