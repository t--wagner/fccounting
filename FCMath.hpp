#include <random>
#include <deque>

#ifndef FCMATH_HPP
#define FCMATH_HPP

/* Benutze den std:: namespace */
using namespace std;


/* --- Dekleration --- */

template <typename T = unsigned long>
T binomial_coefficient(unsigned long n, unsigned long k);


/* --- Definition --- */

/* Binomial Koeffizient berechnen */
unsigned long binomial_coefficient(unsigned long n, unsigned long k)
{
    /* Spezialfälle durchgehen */

    if (0 == k || n == k)
    {
        return 1;
    }

    if (k > n)
    {
        return 0;
    }

    /* Symmetrie Eigenschaft nutzen */
    if (k > (n - k))
    {
        k = n - k;
    }

    if (1 == k)
    {
        return n;
    }

    /* Binomial Koeffizient ausrechnen */
    unsigned long result = 1;

    for (unsigned long i = 1; i <= k; ++i)
    {
        result *= (n - k + i);
        result /= i;
    }

    /* Ergebniss zurückgeben */
    return result;
}

template<typename T>
void gauss_distribution(deque<T> &data, double mean, double stddev, unsigned int points)
{

    default_random_engine generator;
    normal_distribution<double> distribution(mean, stddev);

    for (int i = 0; i < points; ++i)
    {
        data.push_back(distribution(generator));
    }
}

#endif // FCMATH_HPP
