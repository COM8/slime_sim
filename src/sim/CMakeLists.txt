cmake_minimum_required(VERSION 3.16)

add_library(sim TickDurationHistory.cpp
                  TickDurationHistory.hpp
                  TickRate.cpp
                  TickRate.hpp
                  Simulation.cpp
                  Simulation.hpp
                  Slime.cpp
                  Slime.hpp
                  Species.cpp
                  Species.hpp
                  Types.cpp
                  Types.hpp)

target_link_libraries(sim PRIVATE fmt::fmt)
