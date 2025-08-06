#pragma once
#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include "BookingSystem.h"
#include <iostream>
#include <limits>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <cstring>  // Add this for strlen

class MenuSystem {
private:
    BookingSystem bookingSystem;
    bool isRunning;

    // UI and helper methods
    void clearScreen();
    void pressEnterToContinue();
    void showWelcomeBanner();
    void showMainMenu();
    void showBookingMenu();
    void showCancellationMenu();
    void showLoadingAnimation(const char* message);
    void showSuccessMessage(const char* message);
    void showErrorMessage(const char* message);
    void showCustomerSupport();
    void showExitMessage();

public:
    MenuSystem();
    ~MenuSystem();

    // Main menu methods
    void run();

private:
    // Helper methods for menu logic
    void handleNewBooking();
    void handleCancellation();
};

#endif // MENUSYSTEM_H