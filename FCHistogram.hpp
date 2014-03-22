#include <iostream>     //Ein- und Ausgabe von Streams
#include <fstream>      //Filestreams zum Lesen und Schreiben von Dateien
#include <string>       //Zur Nutzung von Strings
#include <map>
#include <math.h>
#include <FCMath.hpp>

#ifndef FCHISTOGRAM_HPP
#define FCHISTOGRAM_HPP

/* Std:: Namespace verwenden */
using namespace std;


/* --- Dekleration --- */

/* Klasse fuer die FCHistogramme */
template<typename TBins>
class FCHistogram
{
    /* --- Container --- */
    private:

        typedef unsigned long TElements;
        TElements _nr_of_elements;

        typedef map<TBins, TElements> Container;
        Container _histogram;

    /* --- Iterator --- */
    public:

        typedef typename Container::iterator iterator;
        iterator        begin()        {return _histogram.begin();}
        iterator        end()          {return _histogram.end();}

        typedef typename Container::const_iterator const_iterator;
        const_iterator  begin() const  {return _histogram.begin();}
        const_iterator  end()   const  {return _histogram.end();}

    /* --- Konstruktor und Destruktor --- */
    public:
        FCHistogram(string comment = "",
                    string binName = "",
                    string elementsName = "");

    /* --- Private Memebers --- */
    private:

        string _comment;
        string _bin_name;
        string _elements_name;

    /* --- Getter und Setter --- */
    public:

        string get_bin_name(){return _bin_name;}
        void   set_bin_name(string bin_name){_bin_name = bin_name;}

        string get_elements_name(){return _elements_name;}
        void   set_elements_name(string elements_name){_elements_name = elements_name;}

        string get_comment(){return _comment;}
        void   set_comment(string comment){_comment = comment;}

        TElements get_nr_of_elements(){return _nr_of_elements;}

   /* --- Public Methoden --- */
   public:

        /* Elemente zum FCHistogram hinzufuegen */
        template <typename TIterator>
        void add_elements(TIterator start_iter, TIterator stop_iter);

        template <typename TContainer>
        void add_elements(TContainer container);

        void add_element(TBins     bin,
                         TElements nr_of_new_elements = 1);


        /* Adiiert die Werte eines anderen FCHistograms hinzu */
        int add_histogram(FCHistogram &histogram);

        /* Gibt das Maximum des Histograms im Intervall aus */
        TBins max(TBins lower,
                  TBins upper );

        /* Bildschirmausgabe des FCHistograms */
        int print();

        /* Speichern des FCHistograms */
        int save(string filename);

        /* Mittelwert berechen */
        double mean();

        /* Berechnung der ersten n Momente des Histograms */
        deque<double> moment(unsigned int n);
        deque<double> central_moment(unsigned int n);
        deque<double> factorial_moment(unsigned int n);
};

/* Berechnung der Kummulanten aus den gegeben Momenten */
deque<double> cummulant(const deque<double> &moments);


/* --- Definition --- */

/* Konstruktor und Destruktor  */
template<typename TBins>
FCHistogram<TBins>::FCHistogram(string comment,
                                string binName,
                                string elementsName)
{
    _nr_of_elements = 0;

    _comment       = comment;
    _bin_name      = binName;
    _elements_name = elementsName;

}

/* Elemente zum FCHistogram hinzufuegen */
template <typename TBins>
void FCHistogram<TBins>::add_element(TBins     value,
                                     TElements nr_of_new_elements)
{
    if(_histogram.count(value) > 0)
    {
        _histogram[value] += nr_of_new_elements;
    }
    else
    {
        _histogram[value]  = nr_of_new_elements;
    }

    _nr_of_elements += nr_of_new_elements;
}

/* Element aus einem container hinzufügen */
template <typename TBins>
template <typename TContainer>
void FCHistogram<TBins>::add_elements(TContainer container)
{
    for(auto item : container)
    {
        add_element(item);
    }
}

/* Addiert die Werte eines anderen FCHistograms hinzu */
/* Entweder das gesamte FCHistogram per Referenz oder eine Auschnitt mittels Iteratoren */
template<typename TBins>
int FCHistogram<TBins>::add_histogram(FCHistogram &histogram)
{
    /* Das Histogram durchlaufen */
    for(const auto &bin : histogram)
    {
        add_elements(bin.first,
                     bin.second);
    }
    return 0;
}

/* Gibt das Maximum des Histograms im Intervall aus */
template<typename TBins>
TBins FCHistogram<TBins>::max(TBins lower, TBins upper)
{
    TBins     position = 0;
    TElements maximum  = 0;

    /* Das Histogram durchlaufen */
    for(const auto &bin : _histogram)
    {
        if((lower <= bin.first) &&
           (upper >= bin.first    ))
        {
            if(maximum <= bin.second)
            {
                maximum  = bin.second;
                position = bin.first;
            }
        }
    }
    return position;
}

/* Bildschirmausgabe der Zustaende */
template<typename TBins>
int FCHistogram<TBins>::print()
{
    /* Das Histogram durchlaufen */
    for(const auto &bin : _histogram)
    {
        cout << bin.first  << ": "
             << bin.second << endl;
    }
    return 0;
}

/* Speichern des FCHistograms */
template<typename TBins>
int FCHistogram<TBins>::save(string filename)
{
    /* Oeffnen der Datei */
    ofstream datafile(filename.c_str());
    if(!datafile)
    {
        cout << "Die Datei konnte nicht geoeffnet werden!" << endl;
        return -1;
    }

    /* Schreiben der Datei */
    datafile << "<FCHistogram>"       << endl;
    datafile << "<comment>"           << endl;
    datafile << get_comment()         << endl;
    datafile << "<elements>"          << endl;
    datafile <<  get_nr_of_elements() << endl;
    datafile << "<data>"              << endl;

    /* Das Histogram durchlaufen */
    for(const auto &bin : _histogram)
    {
        datafile << bin.first  << "; "
                 << bin.second << endl;
    }

    /* Schliessen der Datei und beenden der Funktion */
    datafile.close();
    return 0;
}

/* Berechne den Mittelwert */
template<typename TBins>
double FCHistogram<TBins>::mean()
{
    double sum = 0;

    /* Das Histogram durchlaufen */
    for(const auto &bin : _histogram)
    {
        sum += bin.second * static_cast<double>(bin.first);
    }

    /* Mittelwert zurückgeben */
    return sum / get_nr_of_elements();
}

/* Berechnung der ersten n Momente des Histograms */
template<typename TBins>
deque<double> FCHistogram<TBins>::moment(unsigned int n)
{
    /* Deque Container zum speichern der Momente */
    deque<double> moments(n, 0);

    /* Anzahl aller Elemente des Histograms */
    auto nr_of_elements = get_nr_of_elements();

    /* Variablen zum speichern der Zwischenergebnisse */
    double product;

    /* Das Histogram durch gehen */
    for(const auto &bin : _histogram)
    {
        /* Die Summen der Momente berechen */
        product = 1;
        for(auto &moment : moments)
        {
            product *= bin.first;
            moment += bin.second * product;
        }
    }

    /* Normieren mit der Anzahl der Elemente */
    for(auto &moment : moments)
    {
        moment /= nr_of_elements;
    }

    /* Noch schnell das 0-te Moment gleich Eins setzen */
    moments.push_front(1);

    return moments;
}

/* Berechnung der ersten n zentralen Momente des Histograms */
template<typename TBins>
deque<double> FCHistogram<TBins>::central_moment(unsigned int n)
{
    /* Deque Container zum speichern der Momente */
    deque<double> central_moments(n, 0);

    /* Anzahl aller Elemente */
    auto nr_of_elements = get_nr_of_elements();

    /* Mittelwert berechen */
    double mean_val = mean();

    /* Variablen zum speichern der Zwischenergebnisse */
    double delta, product;

    /* Das Histogram durch gehen */
    for(const auto &bin : _histogram)
    {
        /* Die Differenz berechen */
        delta = bin.first - mean_val;

        /* Die Summen der Momente berechen */
        product = 1;
        for(auto &central_moment : central_moments)
        {
            product *= delta;
            central_moment += bin.second * product;
        }
    }

    /* Normieren mit der Anzahl aller Elemente */
    for(auto &central_moment : central_moments)
    {
        central_moment /= nr_of_elements;
    }

    /* Noch schnell das 0-te zentrale Moment gleich Eins setzen */
    central_moments.push_front(1);

    return central_moments;
}

/* Berechnung der ersten n faktoriellen Momente des Histograms */
template<typename TBins>
deque<double> FCHistogram<TBins>::factorial_moment(unsigned int n)
{
    /* Deque Container zum speichern der Momente */
    deque<double> factorial_moments(n, 0);

    /* Anzahl aller Elemente des Histograms */
    auto nr_of_elements = get_nr_of_elements();

    /* Variablen zum speichern der Zwischenergebnisse */
    double factorial;
    unsigned int moment;

    /* Das Histogram durch gehen */
    for(const auto &bin : _histogram)
    {
        /* Die Summen der faktoriellen Momente berechen */
        moment = 1;
        factorial = 1;
        for(auto &factorial_moment : factorial_moments)
        {
            factorial *= bin.first - moment + 1;
            factorial_moment += bin.second * factorial;
            ++moment;
        }
    }

    /* Normieren mit der Anzahl aller Elemente */
    for(auto &factorial_moment : factorial_moments)
    {
        factorial_moment /= nr_of_elements;
    }

    /* Noch schnell das 0-te faktorielle Momente gleich Eins setzen */
    factorial_moments.push_front(1);

    return factorial_moments;
}

/* Berechnung der Kummulanten aus den gegeben Momenten */
deque<double> cummulant(const deque<double> &moments)
{
    deque<double> cummulants(moments.size(), 0);

    unsigned int m = 0;
    for(auto &c : cummulants)
    {
        c = moments[m];
        for (unsigned int k=1; k < m; ++k)
        {
            c -= binomial_coefficient(m - 1, k - 1) * cummulants[k] * moments[m - k];
        }
        ++m;
    }

    return cummulants;
}

#endif // FCHISTOGRAM_HPP
