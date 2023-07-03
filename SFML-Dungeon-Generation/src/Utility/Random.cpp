#include "Random.hpp"
Random::Random() = default;
Random::~Random() = default;

Random& Random::getInstance()
{
    static Random instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
}

int Random::generateInt(const int t_low, const int t_high)
{
    std::uniform_int_distribution dist(t_low, t_high);
    return dist(m_generator);
}

float Random::generateFloat(const float t_low, const float t_high)
{
    std::uniform_real_distribution dist(t_low, t_high);
    return dist(m_generator);
}

void Random::setSeed(const unsigned t_seed)
{
    m_generator.seed(t_seed);
    m_seed = t_seed;
    std::cout << "Seed set to: " << m_seed << std::endl;
}

unsigned& Random::getSeedRef()
{
    return m_seed;
}

void Random::reset()
{
    setSeed(std::random_device()());
}
