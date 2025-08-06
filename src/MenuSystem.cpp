#include "MenuSystem.h"
#include <iostream>
#include <limits>
#include <cstdlib> // for system("cls") or system("clear")
#include <cstring> // For strlen

MenuSystem::MenuSystem() : isRunning(true) {}

MenuSystem::~MenuSystem() {}

void MenuSystem::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MenuSystem::pressEnterToContinue() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void MenuSystem::showWelcomeBanner() {
    clearScreen();
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "║                    ✈️  SKYWAY AIRLINES  ✈️                   ║" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "║              Your Journey Begins With Us                     ║" << std::endl;
    std::cout << "║                                                              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
}

void MenuSystem::showMainMenu() {
    std::cout << "┌────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                      🌟 MAIN MENU 🌟                       │" << std::endl;
    std::cout << "├────────────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│  1. 🛫 Book New Flight                                     │" << std::endl;
    std::cout << "│  2. 📋 View My Bookings                                    │" << std::endl;
    std::cout << "│  3. ✏️  Modify Existing Booking                            │" << std::endl;
    std::cout << "│  4. ❌ Cancel Booking                                      │" << std::endl;
    std::cout << "│  5. 📞 Customer Support                                    │" << std::endl;
    std::cout << "│  6. 🚪 Exit                                                │" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────────┘" << std::endl;
    std::cout << std::endl;
    std::cout << "Please enter your choice (1-6): ";
}

void MenuSystem::showBookingMenu() {
    std::cout << "┌────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                   🛫 FLIGHT BOOKING 🛫                     │" << std::endl;
    std::cout << "├────────────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│  1. 🔍 Search Flights                                      │" << std::endl;
    std::cout << "│  2. 📊 View Flight Options                                 │" << std::endl;
    std::cout << "│  3. 🎯 Select Flight                                       │" << std::endl;
    std::cout << "│  4. 💺 Choose Seats                                        │" << std::endl;
    std::cout << "│  5. 👤 Enter Passenger Details                             │" << std::endl;
    std::cout << "│  6. 📝 Review Booking                                      │" << std::endl;
    std::cout << "│  7. 💳 Proceed to Payment                                  │" << std::endl;
    std::cout << "│  8. 🏠 Back to Main Menu                                   │" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────────┘" << std::endl;
    std::cout << std::endl;
    std::cout << "Please enter your choice (1-8): ";
}

void MenuSystem::showCancellationMenu() {
    std::cout << "┌────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                   ❌ BOOKING CANCELLATION                   │" << std::endl;
    std::cout << "├────────────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│  1. 🎫 Cancel by PNR                                       │" << std::endl;
    std::cout << "│  2. 🆔 Cancel by Booking ID                                │" << std::endl;
    std::cout << "│  3. 📋 View Cancellation Policy                           │" << std::endl;
    std::cout << "│  4. 💰 Check Refund Status                                │" << std::endl;
    std::cout << "│  5. 🏠 Back to Main Menu                                   │" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────────┘" << std::endl;
    std::cout << std::endl;
    std::cout << "Please enter your choice (1-5): ";
}

void MenuSystem::showLoadingAnimation(const char* message) {
    std::cout << message;
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << " Done!" << std::endl;
}

void MenuSystem::showSuccessMessage(const char* message) {
    std::cout << "┌────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                        ✅ SUCCESS                           │" << std::endl;
    std::cout << "├────────────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│  " << message;
    int messageLen = strlen(message);
    for (int i = messageLen; i < 58; i++) {
        std::cout << " ";
    }
    std::cout << "│" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────────┘" << std::endl;
}

void MenuSystem::showErrorMessage(const char* message) {
    std::cout << "┌────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                         ❌ ERROR                            │" << std::endl;
    std::cout << "├────────────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│  " << message;
    int messageLen = strlen(message);
    for (int i = messageLen; i < 58; i++) {
        std::cout << " ";
    }
    std::cout << "│" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────────┘" << std::endl;
}

void MenuSystem::showCustomerSupport() {
    clearScreen();
    std::cout << "┌────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                    📞 CUSTOMER SUPPORT                     │" << std::endl;
    std::cout << "├────────────────────────────────────────────────────────────┤" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│  📧 Email: support@skyway-airlines.com                    │" << std::endl;
    std::cout << "│  📞 Phone: +1-800-SKYWAY (1-800-759-9297)                 │" << std::endl;
    std::cout << "│  🌐 Website: www.skyway-airlines.com                      │" << std::endl;
    std::cout << "│  💬 Live Chat: Available 24/7 on our website              │" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│  🕒 Support Hours:                                         │" << std::endl;
    std::cout << "│     Monday - Friday: 6:00 AM - 10:00 PM EST               │" << std::endl;
    std::cout << "│     Saturday - Sunday: 8:00 AM - 8:00 PM EST              │" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────────┘" << std::endl;
}

void MenuSystem::showExitMessage() {
    clearScreen();
    std::cout << "┌────────────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│                   Thank you for choosing                   │" << std::endl;
    std::cout << "│                    SKYWAY AIRLINES! ✈️                     │" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "│                 Have a wonderful journey!                  │" << std::endl;
    std::cout << "│                                                            │" << std::endl;
    std::cout << "└────────────────────────────────────────────────────────────┘" << std::endl;
    std::cout << std::endl;
}

void MenuSystem::run() {
    showWelcomeBanner();
    pressEnterToContinue();

    int choice;
    bool running = true;

    while (running) {
        clearScreen();
        showWelcomeBanner();
        showMainMenu();

        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: // Book New Flight
                handleNewBooking();
                break;

            case 2: // View My Bookings
                clearScreen();
                showLoadingAnimation("Loading your bookings");
                bookingSystem.viewBookingDetails();
                pressEnterToContinue();
                break;

            case 3: // Modify Existing Booking
                clearScreen();
                bookingSystem.modifyExistingBooking();
                pressEnterToContinue();
                break;

            case 4: // Cancel Booking
                handleCancellation();
                break;

            case 5: // Customer Support
                showCustomerSupport();
                pressEnterToContinue();
                break;

            case 6: // Exit
                showExitMessage();
                running = false;
                break;

            default:
                showErrorMessage("Invalid choice. Please select 1-6.");
                pressEnterToContinue();
        }
    }
}

void MenuSystem::handleNewBooking() {
    int bookingChoice;
    bool bookingInProgress = true;

    while (bookingInProgress) {
        clearScreen();
        showBookingMenu();

        std::cin >> bookingChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (bookingChoice) {
            case 1: // Search Flights
                clearScreen();
                showLoadingAnimation("Searching flights");
                bookingSystem.searchFlights();
                pressEnterToContinue();
                break;

            case 2: // View Flight Options
                clearScreen();
                bookingSystem.displayFlightOptions();
                pressEnterToContinue();
                break;

            case 3: { // Select Flight
                clearScreen();
                // We don't want to call startBookingProcess here as it duplicates the entire workflow.
                // We should only call selectFlightFromList.
                int selectedIndex = bookingSystem.selectFlightFromList();
                if (selectedIndex != -1) {
                    showSuccessMessage("Flight selected successfully!");
                } else {
                    showErrorMessage("Flight selection failed. Please try again.");
                }
                pressEnterToContinue();
                break;
            }

            case 4: // Choose Seats
                clearScreen();
                // This option needs to be integrated into the main booking flow
                std::cout << "Seat selection is part of the booking process. Please start a new booking." << std::endl;
                pressEnterToContinue();
                break;

            case 5: // Enter Passenger Details
                clearScreen();
                // This option needs to be integrated into the main booking flow
                std::cout << "Passenger details are collected during the booking process." << std::endl;
                pressEnterToContinue();
                break;

            case 6: // Review Booking
                clearScreen();
                bookingSystem.reviewBookingDetails();
                pressEnterToContinue();
                break;

            case 7: // Proceed to Payment
                clearScreen();
                // Finalize the booking after all steps are complete
                if (bookingSystem.validateBookingData()) {
                    if (bookingSystem.processPayment()) {
                        if (bookingSystem.generateBookingConfirmation()) {
                            showSuccessMessage("Booking completed successfully!");
                            bookingInProgress = false;
                        }
                    } else {
                        showErrorMessage("Payment failed. Please try again.");
                    }
                } else {
                    showErrorMessage("Booking data is invalid. Please review and try again.");
                }
                pressEnterToContinue();
                break;

            case 8: // Back to Main Menu
                bookingInProgress = false;
                break;

            default:
                showErrorMessage("Invalid choice. Please select 1-8.");
                pressEnterToContinue();
        }
    }
}

void MenuSystem::handleCancellation() {
    int cancelChoice;
    bool cancelMenuActive = true;

    while (cancelMenuActive) {
        clearScreen();
        showCancellationMenu();

        std::cin >> cancelChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (cancelChoice) {
            case 1: // Cancel by PNR
                clearScreen();
                bookingSystem.cancelByPnr();
                pressEnterToContinue();
                break;

            case 2: // Cancel by Booking ID
                clearScreen();
                bookingSystem.cancelByBookingId();
                pressEnterToContinue();
                break;

            case 3: // View Cancellation Policy
                clearScreen();
                bookingSystem.viewCancellationPolicy();
                pressEnterToContinue();
                break;

            case 4: // Check Refund Status
                clearScreen();
                bookingSystem.checkRefundStatus();
                pressEnterToContinue();
                break;

            case 5: // Back to Main Menu
                cancelMenuActive = false;
                break;

            default:
                showErrorMessage("Invalid choice. Please select 1-5.");
                pressEnterToContinue();
        }
    }
}