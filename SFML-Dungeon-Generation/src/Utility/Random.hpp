#pragma once
#include <iostream>
#include <random>
#include <vector>

class Random
{
public:

    // Private Constructor & Destructor
    Random();
    ~Random();

    // Prevent copying
    Random(Random const&) = delete;
    void operator=(Random const&) = delete;

    // The static method that controls the access to the singleton instance.
    static Random& getInstance();

    // Generates a random number.
    int generateInt(const int t_low, const int t_high);

    float generateFloat(const float t_low, const float t_high);

    // Sets the seed for the random number generator.
    void setSeed(const unsigned t_seed);

    unsigned& getSeedRef();

    void reset();

private:
    std::mt19937 m_generator;  // The random number generator with a default seed.
    unsigned m_seed{};           // The seed for the random number generator.
};
