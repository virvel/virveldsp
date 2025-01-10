add_library(virveldsp INTERFACE virveldsp/granulator.cpp virveldsp/frequencyshifter.cpp)
target_include_directories(virveldsp INTERFACE ${CMAKE_SOURCE_DIR})
