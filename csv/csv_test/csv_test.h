#ifndef CSV_TEST_H
#define CSV_TEST_H

#include <fstream>
#include "simtest.h"

using namespace std;

class CSVTest {
public:
    void reader(string);
    void initialize(ifstream&);
    int getNbPoste(string);
    int getNbLignes(ifstream&);
    int getNbColonnes(ifstream&);

    void createCSVConfig(string**);
    string** sortTab(SimTest::TableauA& tableauA);
    string** readCSV(string);

    int* detectRegex(string **);

};


#endif // CSV_TEST_H
