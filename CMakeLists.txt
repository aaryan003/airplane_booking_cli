cmake_minimum_required(VERSION 3.30)
project(airplane_booking_cli)

set(CMAKE_CXX_STANDARD 20)

# Include header directory
include_directories(include)

# Add the executable with all source files
add_executable(airplane_booking_cli
        main.cpp
        src/MenuSystem.cpp
        src/BookingSystem.cpp
        src/BookingManager.cpp
        src/Booking.cpp
        src/UserInput.cpp
        src/Flight.cpp
        src/FlightList.cpp
        src/StringUtils.cpp
)

# Find and link the nlohmann_json library
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(airplane_booking_cli PRIVATE nlohmann_json::nlohmann_json)

# Compiler flags
if(MSVC)
    target_compile_options(airplane_booking_cli PRIVATE /W4)
else()
    target_compile_options(airplane_booking_cli PRIVATE -Wall -Wextra -Wpedantic)
endif()