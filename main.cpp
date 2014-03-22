#include <deque>
#include <iostream>
#include <fstream>
#include <time.h>
#include "FCStatistics.hpp"

using namespace std;


void create_histogram();
void digitize();
void times();
void counting();
void split_signal();


int main()
{
}

void split_signal()
{
    ifstream signal_istream("F:/Counting/14_g3_-0.620V/signal/signal_full_3points_avg.fcsignal");
    ofstream signal_ostream("F:/Counting/14_g3_-0.620V/signal/signal_200m_225m_3points_avg.fcsignal");
    FCSignal signal;

    unsigned long long position = 0;

    while(signal_istream)
    {
        read_level(signal_istream, signal);

        position += signal.back().length;

        if(position < (500e3 * 60 * 200))
        {
            signal.clear();
        }
        else if(position > (500e3 * 60 * 225))
        {
            break;
        }
    }

    cout << "Signal size: " << signal.size() << endl;
    cout << "Position: " << position << endl;

    write_signal(signal, signal_ostream);

    signal_istream.close();
    signal_ostream.close();
}

void create_histogram()
{
    typedef unsigned short datatype;

    FCHistogram<datatype> histo;

    /* Alle einzulesende Files */
    deque<string> filenames;
    filenames.push_back("F:/Counting/14_g3_-0.620V/count0.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count1.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count2.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count3.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count4.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count5.000000.bin");

    deque<datatype> binary;

    /* Über alle Files iterieren */
    for(auto filename : filenames)
    {
        /* File zum Lesen im binary Modus öffnen */
        cout << filename << endl;
        ifstream datafile(filename, ios::binary);

        /* Paket Größe kurz schecken */
        cout << binary.size() << endl;

        /* Die Datafile durchlaufen */
        while(datafile)
        {
            read_trace(5000e3, datafile, binary);

            histo.add_elements(binary);
            binary.clear();
        }
    }
    histo.save("F:/Counting/14_g3_-0.620V/trace_histo_full.fchistogram");
}

void digitize()
{
    typedef unsigned short datatype;

    /* Alle einzulesende Files */
    deque<string> filenames;
    filenames.push_back("F:/Counting/14_g3_-0.620V/count0.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count1.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count2.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count3.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count4.000000.bin");
    filenames.push_back("F:/Counting/14_g3_-0.620V/count5.000000.bin");

    /* Create FCsignal */
    FCSignal signal;
    ofstream signal_stream("F:/Counting/14_g3_-0.620V/signal_full__3_points_avg2.fcsignal");

    /* Open the fitting parameter files */
    //int mu1, mu2;
    //ifstream fmu1("F:/Counting/14_g3_-0.620V/fit_peak1.txt");
    //ifstream fmu2("F:/Counting/14_g3_-0.620V/fit_peak2.txt");

    FCSystem<datatype> system;
    system.add_state(State<datatype>(31775 - 2*34, 31775 + 2*34));
    system.add_state(State<datatype>(31942 - 2*34, 31942 + 2*34));

    system.print();

    deque<datatype> binary;
    FCLevel last_level(-1, 0, 0);

    /* Über alle Files iterieren */
    for(auto filename : filenames)
    {
        /* File zum Lesen im binary Modus öffnen */
        cout << filename << endl;
        ifstream datafile(filename, ios::binary);

        /* Paket Größe kurz schecken */
        cout << binary.size() << endl;

        /* Die Datafile durchlaufen */
        while(datafile)
        {
            /* Datenpaket einlesen */
            read_trace(500e3, datafile, binary);

            //if(binary.size() < 500e3)
            //{
            //    break;
            //}

            //FCSystem<datatype> system;
            //fmu1 >> mu1;
            //system.add_state(State<datatype>(mu1-34, mu1+34));
            //fmu2 >> mu2;
            //system.add_state(State<datatype>(mu2-34, mu2+34));

            digitize(3, system, last_level, binary, signal);

            //cout << binary.size() << endl;
            //binary.clear();
        }

        /* Signal speichern und leeren */
        write_signal(signal, signal_stream);

        /* File schließen */
        datafile.close();
    }

    //fmu1.close();
    //fmu2.close();

    signal_stream.close();
}

void times()
{
    ifstream signal_stream("F:/Counting/14_g3_-0.620V/signal/signal_200m_225m_3points_avg.fcsignal");
    FCSignal signal_buf, signal0, signal1;
    FCHistogram<unsigned int> histo0, histo1;

    //while(signal_stream)
    //{
        read_signal(1000000, signal_stream, signal_buf);

        signal0.add_signal(signal_buf);
        state_times(0, signal0, histo0);

        signal1.add_signal(signal_buf);
        state_times(1, signal1, histo1);

        signal_buf.clear();
    //}

    histo0.save("F:/Counting/14_g3_-0.620V/state0_times.fchistogram");
    histo1.save("F:/Counting/14_g3_-0.620V/state1_times.fchistogram");

    signal_stream.close();
}

void counting()
{
    ifstream signal_stream("F:/Counting/14_g3_-0.620V/signal/signal_200m_225m_3points_avg.fcsignal");
    FCSignal signal;

    ofstream fmoment_stream("F:/Counting/14_g3_-0.620V/factorial_moments.txt");
    ofstream fcummulant_stream("F:/Counting/14_g3_-0.620V/factorial_cummulants.txt");

    /* Alle Counter anlegen */
    deque<FCCounter> counters;
    for(unsigned int t = 1; t <= 1000; t += 1)

    {
        counters.push_back(FCCounter(0, 1, t));
    }

    /* Zeit Variablen Anlegen */
    auto start_sum = clock();
    auto start = clock();

    unsigned int millions = 0;

    //while(signal_stream)
    //{
        cout << "Reading: " << endl;
        read_signal(1000000, signal_stream, signal);

        cout << "Counting: " << endl;
        for(auto &counter : counters)
        {
            counter.count(signal);
            //cout << "Position: " << counter.get_position() << endl;
            //counter.histogram.print();
            //cout << endl << endl;
        }

        signal.clear();

        /* Verarbeite minutes*/
        ++millions;
        cout << "Events in Millionen: " << millions << endl;

        double time = float(clock() - start) / CLOCKS_PER_SEC;
        cout << "Durchschnittszeit: " << time << endl;
        start = clock();
    //}

    double time_sum = float(clock() - start_sum) / CLOCKS_PER_SEC;
    cout << "Gesamtzeit: " << time_sum << endl;

    /* Speichern der Momente in Dateien */
    for(auto &counter : counters)
    {


        auto fmoments = counter.histogram.factorial_moment(50);
        fmoment_stream << counter.get_window() << ";";
        for(const auto &fmoment : fmoments)
        {
            fmoment_stream << fmoment << ";";
        }
        fmoment_stream << endl;



        auto fcummulants = cummulant(fmoments);
        fcummulant_stream << counter.get_window() << ";";
        for(const auto &fcummulant : fcummulants)
        {
            fcummulant_stream << fcummulant << ";";
        }
        fcummulant_stream << endl;
    }

    signal_stream.close();
    fmoment_stream.close();
    fcummulant_stream.close();
}
