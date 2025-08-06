#include "MenuSystem.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>

MenuSystem::MenuSystem() : isRunning(true) {}

MenuSystem::~MenuSystem() {}

void MenuSystem::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void MenuSystem::pressEnterToContinue() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clearScreen();
}

void MenuSystem::showWelcomeBanner() {
    clearScreen();
    std::cout << "+==============================================================+" << std::endl;
    std::cout << "|                                                              |" << std::endl;
    std::cout << "|                    IBM AIRLINES                              |" << std::endl;
    std::cout << "|                                                              |" << std::endl;
    std::cout << "|              Your Journey Begins With Us                     |" << std::endl;
    std::cout << "|                                                              |" << std::endl;
    std::cout << "+==============================================================+" << std::endl;
    std::cout << std::endl;
}

void MenuSystem::showMainMenu() {
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                      MAIN MENU                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|  1. Book New Flight                                        |" << std::endl;
    std::cout << "|  2. View My Bookings                                       |" << std::endl;
    std::cout << "|  3. Modify Existing Booking                                |" << std::endl;
    std::cout << "|  4. Cancel Booking                                         |" << std::endl;
    std::cout << "|  5. Customer Support                                       |" << std::endl;
    std::cout << "|  6. Exit                                                   |" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << std::endl;
    std::cout << "Please enter your choice (1-6): ";
}

void MenuSystem::showBookingMenu() {
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                   FLIGHT BOOKING                          |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|  1. Search Flights                                         |" << std::endl;
    std::cout << "|  2. View Flight Options                                    |" << std::endl;
    std::cout << "|  3. Select Flight                                          |" << std::endl;
    std::cout << "|  4. Choose Seats                                           |" << std::endl;
    std::cout << "|  5. Enter Passenger Details                                |" << std::endl;
    std::cout << "|  6. Review Booking                                         |" << std::endl;
    std::cout << "|  7. Proceed to Payment                                     |" << std::endl;
    std::cout << "|  8. Back to Main Menu                                      |" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << std::endl;
    std::cout << "Please enter your choice (1-8): ";
}

void MenuSystem::showCancellationMenu() {
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                   BOOKING CANCELLATION                    |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|  1. Cancel by PNR                                          |" << std::endl;
    std::cout << "|  2. Cancel by Booking ID                                   |" << std::endl;
    std::cout << "|  3. View Cancellation Policy                              |" << std::endl;
    std::cout << "|  4. Check Refund Status                                   |" << std::endl;
    std::cout << "|  5. Back to Main Menu                                      |" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
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
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                        SUCCESS                             |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|  " << message;
    int messageLen = strlen(message);
    for (int i = messageLen; i < 58; i++) {
        std::cout << " ";
    }
    std::cout << "|" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
}

void MenuSystem::showErrorMessage(const char* message) {
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                         ERROR                              |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|  " << message;
    int messageLen = strlen(message);
    for (int i = messageLen; i < 58; i++) {
        std::cout << " ";
    }
    std::cout << "|" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
}

void MenuSystem::showCustomerSupport() {
    clearScreen();
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                    CUSTOMER SUPPORT                       |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|  Email: support@ibm-airlines.com                           |" << std::endl;
    std::cout << "|  Phone: +1-800-IBM (1-800-759-9297)                        |" << std::endl;
    std::cout << "|  Website: www.ibm-airlines.com                             |" << std::endl;
    std::cout << "|  Live Chat: Available 24/7 on our website                  |" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|  Support Hours:                                            |" << std::endl;
    std::cout << "|     Monday - Friday: 6:00 AM - 10:00 PM EST                |" << std::endl;
    std::cout << "|     Saturday - Sunday: 8:00 AM - 8:00 PM EST               |" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
}

void MenuSystem::showExitMessage() {
    clearScreen();
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|                   Thank you for choosing                   |" << std::endl;
    std::cout << "|                        IBM AIRLINES!                       |" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "|                 Have a wonderful journey!                  |" << std::endl;
    std::cout << "|                                                            |" << std::endl;
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << std::endl;
}

void MenuSystem::run() {
    showWelcomeBanner();
    pressEnterToContinue();

    int choice;
    bool running = true;

    while (running) {
        showWelcomeBanner();
        showMainMenu();

        while (!(std::cin >> choice) || choice < 1 || choice > 6) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            showWelcomeBanner();
            showMainMenu();
            showErrorMessage("Invalid input. Please enter a number between 1-6.");
            std::cout << "\nPlease enter your choice (1-6): ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                handleNewBooking();
                break;
            case 2:
                clearScreen();
                showLoadingAnimation("Loading your bookings");
                bookingSystem.viewBookingDetails();
                pressEnterToContinue();
                break;
            case 3:
                clearScreen();
                bookingSystem.modifyExistingBooking();
                pressEnterToContinue();
                break;
            case 4:
                handleCancellation();
                break;
            case 5:
                showCustomerSupport();
                pressEnterToContinue();
                break;
            case 6:
                showExitMessage();
                running = false;
                break;
        }
    }
}

void MenuSystem::handleNewBooking() {
    clearScreen();
    showLoadingAnimation("Starting booking process");

    // The entire booking process is encapsulated in this single call.
    bookingSystem.startBookingProcess();

    pressEnterToContinue();
}

void MenuSystem::handleCancellation() {
    int cancelChoice;
    bool cancelMenuActive = true;

    while (cancelMenuActive) {
        showCancellationMenu();

        while (!(std::cin >> cancelChoice) || cancelChoice < 1 || cancelChoice > 5) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            showCancellationMenu();
            showErrorMessage("Invalid input. Please enter a number between 1-5.");
            std::cout << "\nPlease enter your choice (1-5): ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (cancelChoice) {
            case 1:
                clearScreen();
                bookingSystem.cancelByPnr();
                pressEnterToContinue();
                break;
            case 2:
                clearScreen();
                bookingSystem.cancelByBookingId();
                pressEnterToContinue();
                break;
            case 3:
                clearScreen();
                bookingSystem.viewCancellationPolicy();
                pressEnterToContinue();
                break;
            case 4:
                clearScreen();
                bookingSystem.checkRefundStatus();
                pressEnterToContinue();
                break;
            case 5:
                cancelMenuActive = false;
                break;
        }
    }
}