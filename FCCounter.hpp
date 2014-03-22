#include <FCHistogram.hpp>
#include <FCSignal.hpp>
#include <fstream>

#ifndef FCCOUNTER_HPP
#define FCCOUNTER_HPP


/* --- Dekleration --- */

class FCCounter
{

    /* --- Public Members --- */
    public:

        /* Histogram zum Speichern der Counting Statistik */
        typedef FCHistogram<unsigned long> histogram_type;
        histogram_type histogram;

    /* --- Private Members --- */
    private:

        /* Buffer für das Signal */
        FCSignal      _signal;

        /* Event */
        int           _state1;
        int           _state2;

        /* Counting Variablen */
        unsigned long _window;
        unsigned long _position;
        unsigned long _events;

    /* --- Konstruktor und Destruktor --- */
    public:
        FCCounter(unsigned int state1, unsigned int state2, unsigned long window);
        ~FCCounter(){}

    /* --- Getter und Setter --- */
    public:
        unsigned long get_position(){return _position;}
        unsigned long get_window(){return _window;}

    /* --- Public Methoden --- */
    public:

        /* Verarbeite das Signal */
        void count(FCSignal &signal);
};


/* --- Definition --- */

FCCounter::FCCounter(unsigned int  state1,
                     unsigned int  state2,
                     unsigned long window)
{
    _state1   = state1;
    _state2   = state2;
    _window   = window;
    _position = 0;
    _events = 0;
}

/*                       FCHistogramm für die States erstellen                           */
/* !!! Die Erzeugung der FCHistogramme ist der zeitkritischte Schritt der Auswertung !!! */
/*              Anpassungen koennen daher an dieser Stelle notwendig werden              */
void FCCounter::count(FCSignal &signal)
{
    /* Signal zum internen signal Buffer hinzufügen */
    _signal.add_signal(signal);

    /* Das Signal durchlaufen */
    while(_signal.size() > 1)
    {
        /* Referenzen auf die vordersten beiden Levels */
        FCLevel &level1 = _signal[0];
        FCLevel &level2 = _signal[1];

        /* Position im Fenster Erhöhen */
        _position += level1.length;

        /* Testen ob das aktulle Event außerhalb des Fensters liegt */
        while(_position > _window)
        {

            /* Füge die gezählten Events zum Histogram hinzu */
            histogram.add_element(_events);

            /* Setze den Eventzähler zurück auf Null */
            _events = 0;

            /* Position im neuen Fenster berechnen */
            _position -= _window;
        }

        /* Event prüfen */
        if((level1.state == _state1) &&
           (level2.state == _state2)   )
        {
           /* Eventcounter erhöhen */
           ++_events;
        }

        _signal.pop_front();
    }
}

#endif // FCCOUNTER_HPP
