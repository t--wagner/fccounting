#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <numeric>

#ifndef FCSIGNAL_HPP
#define FCSIGNAL_HPP

/* Benutze den std:: namespace */
using namespace std;


/* --- Dekleration --- */

/* Level Klasse */
class FCLevel
{
    /*     !!! Die Erzeugung der FCHistogramme aus den Levels ist der zeitkritischte Schritt der Auswertung !!!        */
    /* Die Member werden alle public gemacht um einen schnelleren Zugriff beim Erstellen der FCHistogramme zu haben    */
    /* Bei einem 100MB Trace und 2*1000 FCHistogrammen konnte so eine Verkuerzung von 120sec auf 80sec erreicht werden */

    public:

        typedef int state_type;
        state_type state;

        typedef unsigned int length_type;
        length_type length;

        typedef float value_type;
        value_type value;

    /* --- Konstruktor und Destruktor --- */
    public:
        FCLevel(state_type  state_  = 0,
                length_type length_ = 0,
                value_type  value_  = 0)
        {
                state  = state_;
                length = length_;
                value  = value_;
        }

        ~FCLevel(){}
};


/* FCSignal Class */
class FCSignal
{
    /* --- Container --- */
    private:
        typedef deque<FCLevel> Container;
        Container _levels;

    /* --- Iteratoren --- */
    public:

        typedef typename Container::iterator iterator;
        iterator begin() {return _levels.begin();}
        iterator end()   {return _levels.end();}

        typedef typename Container::const_iterator const_iterator;
        const_iterator begin() const {return _levels.begin();}
        const_iterator end()   const {return _levels.end();}

    /* --- Konstruktor und Destruktor --- */
    public:

        FCSignal(){}
        ~FCSignal(){}

    /* Operatoren */
    FCLevel& operator[](deque<FCLevel>::size_type index);

    /* ---- Public Methoden ---- */
    public:

        /* Die Anzahl an Levels zurueckgeben */
        typedef typename Container::size_type size_type;
        size_type size();

        /* Fuege ein neues Level hinzu */
        void add_level(FCLevel level);
        void add_signal(FCSignal &signal);

        /* Referenz auf das vorderste und hinterste Level */
        FCLevel& front();
        FCLevel& back();

        /* States löschen */
        void pop_front();
        void pop_back();
        void clear();

        /* Bildschirmausgabe der Levels */
        void print();
};

/* Lesen von FCSignal Dateien */
void read_level(ifstream &input, FCSignal &signal);
void read_signal(unsigned long nr_of_levels, ifstream &input_stream, FCSignal &signal);

/* Speichern von FCSignal Dateien */
void write_signal(FCSignal &signal, ofstream &output_stream);

/* Ausgabe der Statezeiten */
template <typename TBins>
void state_times(int state, FCSignal &signal, FCHistogram<TBins> &histogram);

/* Ausgabe der Eventzeiten */
template <typename TBins>
void event_times(int state1, int state2, FCSignal &signal, FCHistogram<TBins> &histogram);


/* --- Definition --- */

/* Operatoren */
FCLevel& FCSignal::operator[](deque<FCLevel>::size_type index)
{
    return _levels[index];
}

/* Fuege ein Level hinzu */
void FCSignal::add_level(FCLevel level)
{
    _levels.push_back(level);
}

void FCSignal::add_signal(FCSignal &signal)
{
    _levels.insert(end(), signal.begin(), signal.end());
}

/* Die Anzahl an Levels zurueckgeben */
FCSignal::size_type FCSignal::size()
{
    return _levels.size();
}

/* Referenz auf das vorderste LOevel */
FCLevel& FCSignal::front()
{
    return _levels.front();
}

/* Referenz auf das letzte Level */
FCLevel& FCSignal::back()
{
    return _levels.back();
}

/* Das vorderste Level löschen */
void FCSignal::pop_front()
{
    _levels.pop_front();
}

/* Das hinterste Element löschen */
void FCSignal::pop_back()
{
    _levels.pop_back();
}

/* Alle Levels löschen */
void FCSignal::clear()
{
    _levels.clear();
}

/* Bildschirmausgabe der Zustaende */
void FCSignal::print()
{
    cout << "State" << "\t"
         << "Length"  << "\t"
         << "Average" << endl;

    for(const auto &level : _levels)
    {
        cout << level.state  << "\t"
             << level.length << "\t"
             << level.value  << endl;
    }
}

/* Lesen eines einzelnen Levels aus einer FCSignal Textdatei */
void read_level(ifstream &input_stream, FCSignal &signal)
{
    /* Variablen zum zwischenspeichern der eingelesen Stringfolgen */
    string str_state,
           str_length,
           str_value;

    /* Die Daten einlesen */
    if(getline(input_stream, str_state,  ';'))
    {
        getline(input_stream, str_length, ';');
        getline(input_stream, str_value);

        /* Strings in die richtigen Datentypen umwandeln mit Hilfe eines Stringstreams */
        FCLevel level;
        istringstream(str_state)  >> level.state;
        istringstream(str_length) >> level.length;
        istringstream(str_value)  >> level.value;

        /* Das Level hinzufügen */
        signal.add_level(level);
    }
}

/* Lesen mehrer Level aus einer FCSignal Textdatei */
void read_signal(unsigned long nr_of_levels, ifstream &input_stream, FCSignal &signal)
{
    for(; nr_of_levels > 0 && input_stream; --nr_of_levels)
    {
        read_level(input_stream, signal);
    }
}

/* Speichern der einzelnen Daten in Textdateien */
void write_signal(FCSignal &signal, ofstream &output_stream)
{
    /* Levels einzeln in die Dateio schreiben */
    for(const auto &level : signal)
    {
        output_stream << level.state  << ";"
                      << level.length << ";"
                      << level.value  << endl;
    }

    /* Levels löschen */
    signal.clear();
}

/* Ausgabe der Statezeiten */
template <typename TBins>
void state_times(int                state,
                 FCSignal           &signal,
                 FCHistogram<TBins> &histogram)
{
    /* Das Signal durchlaufen */
    while(signal.size() > 0)
    {
        /* Referenz auf das vorderste Level */
        FCLevel &level = signal.front();

        /* State des Levels Prüfen */
        if(level.state == state)
        {
            /* Level Länge ins Histogram eintragen */
            histogram.add_element(level.length);
        }

        /* Entferne das vorderste Level des Signals */
        signal.pop_front();
    }
}

/* Ausgabe der Eventzeiten */
template <typename TBins>
void event_times(int                 state1,
                 int                 state2,
                 FCSignal           &signal,
                 FCHistogram<TBins> &histogram)
{
    /* Das Signal durchlaufen */
    while(signal.size() > 1)
    {
        /* Referenzen auf die vordersten beiden Levels */
        FCLevel &level1 = signal[0];
        FCLevel &level2 = signal[1];

        /* Event prüfen */
        if((level1.state == state1) &&
           (level2.state == state2)   )
        {
            histogram.add_element(level1.length);
        }

        /* Entferne das vorderste Level des Signals */
        signal.pop_front();
    }
}

#endif // FCSIGNAL_HPP
