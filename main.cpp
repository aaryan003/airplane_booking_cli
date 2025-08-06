#include "MenuSystem.h"
#include <iostream>
#include <cstdlib>

int main() {
    std::cout << "=== Welcome to Airline Reservation System ===" << std::endl;
    std::cout << "Dynamic Pricing & Real-time Booking System" << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;

    MenuSystem menuSystem;
    menuSystem.run();

    return 0;
}