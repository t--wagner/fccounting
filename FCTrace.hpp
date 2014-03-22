#include <fstream>
#include <deque>

#ifndef FCTRACE_HPP
#define FCTRACE_HPP

/* Benutze den std:: namespace */
using namespace std;


/* --- Dekleration --- */

/* Einen einzelnen Datenpunkt aus einer Binaerdatei in einen Deque lesen */
template <typename TDatatype>
void read_datapoint(istream &input_stream, deque<TDatatype> &output);

/* Mehrer Datenpunkte aus einer Binaerdatei in einen Deque lesen */
template <typename TDatatype>
void read_trace(unsigned long nr_of_points, istream &input_stream, deque<TDatatype> &output);

/* Daten aus einen Container */
template <typename TDatatype>
bool write_trace(deque<TDatatype> &input, ofstream &output_stream);


/* --- Definition --- */

/* Einen einzelnen Datenpunkt aus einer Binaerdatei in einen Deque lesen */
template <typename TDatatype>
void read_datapoint(istream          &input_stream,
                    deque<TDatatype> &output       )
{
    /* Datepunkt aus inputstream lesen und in datapoint buffern */
    TDatatype datapoint;

    if(input_stream.read(reinterpret_cast<char*>(&datapoint), sizeof(TDatatype)))
    {
        /* Datenpunkt dem Deque hinzufügen und true returnen */
        output.push_back(datapoint);
    }
}

/* Mehrer Datenpunkte aus einer Binaerdatei in einen Deque lesen */
template <typename TDatatype>
void read_trace(unsigned long    nr_of_points,
                istream          &input_stream,
                deque<TDatatype> &output       )
{
    for(; nr_of_points > 0 && input_stream; --nr_of_points)
    {
        read_datapoint(input_stream, output);
    }
}

/* Datenpunkte eines Deque in eine Binaerdatei schreiben */
template <typename TDatatype>
bool write_trace(deque<TDatatype> &input,
                 ofstream         &output_stream)
{
    for(const auto &datapoint : input)
    {
        output_stream.write(reinterpret_cast<const char*>(&datapoint), sizeof(TDatatype));
    }
    input.clear();

    return true;
}

#endif // FCDATA_HPP
