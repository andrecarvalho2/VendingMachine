cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(VendingMachine_NCS)

# Correctly setting the source files
target_sources(app PRIVATE
  src/main.c  # Ensure main.c is in the correct location
  ../VendingMachine_Itemis/src-gen/Statechart.c  # Correct relative path
)

# Include the directories where headers are found
target_include_directories(app PRIVATE
  src  # Assuming this is correct
  ../VendingMachine_Itemis/src-gen  # Correct relative path for headers
)
