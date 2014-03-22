#include <deque>

#include <FCSystem.hpp>
#include <FCSignal.hpp>

#ifndef FCDETECTOR_HPP
#define FCDETECTOR_HPP

template <typename TState, typename TInput>
void digitize(int averaging_length, FCSystem<TState> &system, FCLevel &level, deque<TInput> &input, FCSignal &signal)
{
    int datapoint_state;

    while(input.size() >= averaging_length)
    {
        /* Referenz auf das forderste Element aus dem Buffer */
        TInput  &datapoint = input.front();

        /* Ermittle den Zustand des aktuellen Datenpunktes */
        datapoint_state = system.what_state(datapoint);

        /* Der Zustand ist unveraendert (der wahrscheinlichste aller Fälle) */
        if((datapoint_state == level.state))
        {
            /* Den letzten Level verlaengern und den Mittelwert updaten */
            level.length += 1;
            level.value  += (datapoint - level.value) / level.length;
        }
        /* Der Zustand ist undefiniert -1 */
        else if(datapoint_state == -1)
        {
            level.length += 1;
        }
        /* Der Zustand hat sich geaendert */
        else if(datapoint_state != level.state)
        {
            /* Fehlermittelung durchfuehren */
            if(datapoint_state == system.what_state(accumulate(begin(input), begin(input) + averaging_length, 0.0) / averaging_length))
            {
                /* Den Level an das Signal anhängen */
                signal.add_level(level);

                /* Reset Level */
                level.state  = -1;
                level.length =  0;
                level.value  =  0;
            }
            else
            {
                level.length += 1;
            }
       }
        /* Entferne den vordersten Datenpunkt aus dem Input und fahre mit dem nächsten fort */
        input.pop_front();
    }
    return;
}

//deque<Level> _buffer;
//float _fehler_mittelung;

//template<typename TLevel>
//template <typename TState>
//int digitize(FCSystem<TState> fcsystem, FCDatatype &datapoint)
//{
//    int fehlermittelung = 10;

//    /* Den Buffer fuellen bis genuegen Punkte zum mitteln da sind */
//    _buffer.push_back(datapoint);

//    if(_buffer.size() <= fehlermittelung)
//    {
//        return 0;
//    }

//    /* Nehme das forderste Element aus dem Buffer */
//    datapoint = _buffer.front();

//    /* Ermittle den Zustand des aktuellen Datenpunktes */
//    unsigned int datapoint_state = fcsystem.what_state(datapoint);

//    /* Referenz auf den letzten Level */
//    if(_levels.empty())
//    {
//        add_level(datapoint_state, 0, 0, 0);

//    }

//    /* Der Zustand ist unveraendert */
//    if(datapoint_state == _levels.back().state)
//    {
//        /* Den letzten Level verlaengern und den Mittelwert updaten */
//        _levels.back().length += 1;
//        _levels.back().value  += (datapoint - _levels.back().value) / _levels.back().length;

//        /* Fehlermittelung verringern */
//        if(_fehler_mittelung > 3) _fehler_mittelung -= 1;
//    }
//    /* Der Zustand hat sich geaendert */
//    else if(datapoint_state != 0)
//    {
//        /* Fehlermittelung durchfuehren */
//        if(fcsystem.what_state(accumulate(_buffer.begin(),
//                                          _buffer.begin() + _fehler_mittelung,
//                                          static_cast<FCDatatype>(0)          ) / _fehler_mittelung))
//        {
//            add_level(datapoint_state,
//                      _levels.back().start + _levels.back().length,
//                      1,
//                      datapoint);
//        }
//        else
//        {
//            datapoint_state = 0;
//        }
//    }

//    /* Datenpunkt war fehlerhaft */
//    if(datapoint_state == 0)
//    {
//        /* Den letzten Level verlaengern aber den Datenpunkt nicht zur Mittelung zaehlen */
//        _levels.back().length++;

//        /* Die Fehlermittelung erhoehen */
//        if(_fehler_mittelung < fehlermittelung)
//        {
//            _fehler_mittelung += 1;
//        }
//    }

//    /* Entferne das vorderste Element aus dem Buffer */
//    _buffer.pop_front();

//    return 0;
//}

#endif // FCDETECTOR_HPP
