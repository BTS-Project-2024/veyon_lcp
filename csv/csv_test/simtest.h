#ifndef SimTest_H
#define SimTest_H

#include <fstream>

using namespace std;

class SimTest{
public:
    using TableauA = string[15][3]; // simulation du retour des reqûetes

    TableauA* simRequeteA();
};

#endif // SimTest_H
