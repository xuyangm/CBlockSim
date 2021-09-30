# CMakeList.txt : CMake project for CBlockSim-Version2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CBlockSim-Version3")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Add source to this project's executable.
add_executable (CBlockSim-Version3 "Main.cpp"  "Models/Transaction.h" "Configs.h" "Models/Node.h" "Models/Block.h" "Models/Event.h" "Scheduler.h" "Scheduler.cpp" "Models/Node.cpp" "Models/Block.cpp" "Factories/TransactionFactory.h" "Factories/TransactionFactory.cpp" "Factories/TopologyFactory.h" "Factories/TopologyFactory.cpp" "Configs.cpp" "Policy/TransactionPolicy.h" "Policy/TransactionPolicy.cpp" "Policy/BlockPolicy.h" "Policy/BlockPolicy.cpp" "EventEngine.h" "EventEngine.cpp")
# TODO: Add tests and install targets if needed.
