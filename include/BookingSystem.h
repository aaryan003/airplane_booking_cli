#ifndef BOOKINGSYSTEM_H
#define BOOKINGSYSTEM_H

#include "BookingManager.h"
#include "FlightList.h"
#include "UserInput.h"
#include "StringUtils.h" // Add this include for StringUtils functions
#include <string>

class BookingSystem {
private:
    BookingManager bookingManager;
    FlightList flightList;
    UserInput userInput;
    char passengerName[50];
    char passengerEmail[50]; // Added missing member
    char passengerPhone[20]; // Added missing member
    char seatNumber[5];
    int selectedFlightIndex;

    // Utility methods
    void acquireLock();
    void releaseLock();
    double calculateTotalPrice();
    void showDynamicPricingBreakdown(const Flight* flight, int daysUntilDeparture);
    double calculateDynamicPrice(const Flight* flight, int daysUntilDeparture);
    int getBookingIdFromInput(const char* input);
    void displaySampleSeatMap();
    bool isValidSeatNumber(const char* seatNumber);

    // Private methods for modification and cancellation
    void processCancellation(int bookingId);
    double calculateCancellationFee(int daysUntilDeparture);
    void displayDetailedBookingInfo(int bookingId);
    void changeFlightDate();
    void changeSeatSelection();
    void upgradeCabinClass();
    void modifyPassengerDetails();
    void showModificationMenu(int bookingId); // Added missing declaration

public:
    BookingSystem();
    ~BookingSystem();
    bool validateBookingData();

    // Main booking operations
    void startBookingProcess();
    void searchFlights();
    void viewBookingDetails();
    void modifyExistingBooking();
    void cancelBooking();

    // Public cancellation and utility methods
    void cancelByPnr();
    void cancelByBookingId();
    void viewCancellationPolicy();
    void checkRefundStatus();
    void viewModificationCharges();

    // Internal booking process steps
    bool searchFlightsByRoute();
    void displayFlightOptions();
    int selectFlightFromList();
    void displaySeatAvailability(const Flight* flight);
    bool selectPassengerSeats(const Flight* flight);
    void collectPassengerDetails();
    void reviewBookingDetails();
    bool processPayment();
    bool generateBookingConfirmation();
    void showSeatMap(const Flight& flight);
};

#endif // BOOKINGSYSTEM_H