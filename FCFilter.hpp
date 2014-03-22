#include <deque>
#include <numeric>

#ifndef FCFILTER_HPP
#define FCFILTER_HPP

/* Std:: Namespace verwenden */
using namespace std;


/* --- Dekleration --- */

/* Laufender Mittelwert */
template <typename TInput, typename TOutput>
void average(deque<TInput> &input, deque<TOutput> &output, unsigned int length);

/* Differenzierung */
template <typename TInput, typename TOutput>
void differentiate(deque<TInput> &input, deque<TOutput> &output);


/* --- Definition --- */

/* Laufender Mittelwert */
template <typename TInput, typename TOutput>
void average(deque<TInput> &input, deque<TOutput> &output, unsigned int length)
{
    /* Die Datenpunkte im Input durchlaufen */
    while(length <= input.size())
    {
        /* Daten mitteln und Ergebnis in den Output schieben */
        output.push_back(accumulate(begin(input), begin(input) + length , 0.0) / length);

        /* Vordersten Punkt entfernen */
        input.pop_front();
    }
}

/* Differenzierung */
template <typename TInput, typename TOutput>
void differentiate(deque<TInput> &input, deque<TOutput> &output)
{
    /* Die Datenpunkte im Input durchlaufen */
    while(2 <= input.size())
    {
        /* Differenz zwischen zwei benachbarten Punkten in den Output schieben */
        output.push_back(input[1] - input[0]);

        /* Vordersten Punkt entfernen */
        input.pop_front();
    }
}

#endif // FCFILTER_HPP
