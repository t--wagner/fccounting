#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef FCSYSTEM_HPP
#define FCSYSTEM_HPP

/* Benutze den std:: namespace */
using namespace std;


/* State Klasse */
template<typename TState>
class State
{
    /* --- Private Memeber --- */
    private:

        TState _lower_boundary;
        TState _upper_boundary;

    /* --- Konstruktor und Destruktor --- */
    public:

        State(TState lower_boundary,
              TState upper_boundary);
        ~State(){}

    /* ---- Getter und Setter ---- */
    public:

        TState get_lower_boundary(){return _lower_boundary;}
        void   set_lower_boundary(TState lower_boundary){_lower_boundary = lower_boundary;}

        TState get_upper_boundary(){return _upper_boundary;}
        void   set_upper_boundary(TState upper_boundary){_upper_boundary = upper_boundary;}

    /* ---- Public Methoden ---- */
    public:

        /* Prüfen ob ein Datenpunkt zum State gehört */
        bool check(TState datapoint);

        /* Bildschirmausgabe des States */
        int print();
};

/* Konstruktor */
template<typename TState>
 State<TState>::State(TState lower_boundary, TState upper_boundary)
{
    _lower_boundary = lower_boundary;
    _upper_boundary = upper_boundary;
}

/* Prüfen ob ein Datenpunkt zum State gehört */
template<typename TState>
bool State<TState>::check(TState datapoint)
{
    if(_lower_boundary <= datapoint &&
       _upper_boundary >= datapoint )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Bildschirmausgabe des FCHistograms */
template<typename TState>
int State<TState>::print()
{
    cout << "[" << get_lower_boundary() << ", "
                << get_upper_boundary() << "]"  << endl;

    return 0;
}


/* FCSystem Container für States */
template<typename TState>
class FCSystem
{

    /* --- Container --- */
    private:

        typedef vector<State<TState>> Container;
        Container _states;

    /* --- Iteratoren --- */
    public:

        typedef typename Container::iterator iterator;
        iterator begin() {return _states.begin();}
        iterator end()   {return _states.end();}

        typedef typename Container::const_iterator const_iterator;
        const_iterator begin() const {return _states.begin();}
        const_iterator end()   const {return _states.end();}

    /* ---- Getter und Setter ---- */
    public:

        unsigned int get_nr_of_states(){return _states.size();}

    /* ---- Public Methoden ---- */
    public:

        /* einen neuen State hinzufügen */
        int add_state(State<TState> state);

        /* Prüfe in welchem State ein Datenpunkt liegt */
        template<typename TDatapoint>
        int what_state(TDatapoint datapoint);

        /* Bildschirmausgabe der States */
        int print();

        /* Speichern des Systems in Textdateien */
        int save(string filename);

};


/*Definiere einen neuen State */
template<typename TState>
int FCSystem<TState>::add_state(State<TState> state)
{
    _states.push_back(state);

    return 0;
}

/* Prüfe in welchem State ein Datenpunkt liegt. */
template<typename TState>
template<typename TDatapoint>
int FCSystem<TState>::what_state(TDatapoint datapoint)
{
    /* Über alle States iterieren */
    for(auto states_iter  = begin();
             states_iter != end();
             ++states_iter)
    {
        /* Prüfe ob der Datenpunkt im aktuelle State liegt. */
        if((*states_iter).check(datapoint))
        {
            /* Gebe den State des Datenpunktes zurück. */
            return states_iter - begin();
        }
    }

    /* Gebe -1 zurück falls der Datenpunkt zu keinem State gehört */
    return -1;
}

/* Bildschirmausgabe der States */
template<typename TState>
int FCSystem<TState>::print()
{
    for(auto states_iter  = begin();
             states_iter != end();
             ++states_iter)
    {
        cout <<   states_iter - begin()      << ": [" <<
                (*states_iter).get_lower_boundary() << ", "  <<
                (*states_iter).get_upper_boundary() << "]"   << endl;
    }

    return 0;
}

/* Speichern der Systems in Textdateien */
template<typename TState>
int FCSystem<TState>::save(string filename)
{
    /* Öffnen der Datei */
    ofstream system_file(filename);

    if(!system_file)
    {
        cout << "Die Datei konnte nicht geöffnet werden!" << endl;
        return 0;
    }

    /* Datei Header schreiben */
    system_file << "State" << ";" << "LowerBoundary" << ";"  << "UpperBoundary" << ";" << endl;

    for(auto states_iter  = begin();
             states_iter != end();
             ++states_iter)
    {
        system_file <<  states_iter - begin()      << ";" <<
                      (*states_iter).get_lower_boundary() << ";" <<
                      (*states_iter).get_upper_boundary() << endl;
    }

    /* Schließen der Datei */
    system_file.close();

    return 0;
}

#endif // FCSYSTEM_HPP
