#include <iostream>
#include <fstream>
#include <string>

#include "csv_test.cpp"
#include "simtest.cpp"

int main()
{
    CSVTest test_reader;
    // SimTest sim_test;

    //    SimTest::TableauA* tableauA = sim_test.simRequeteA();   // Crée une simulation de requête

    //    string** ordered = test_reader.sortTab(*tableauA);      // Ordonne le retour de la reqûete

    //    for (int i = 0; i < 15; ++i)                            // Affiche le tableau ordonné
    //        cout << ordered[i][0] << "; " << ordered[i][1] << "; " << ordered[i][2] << endl;

    //    test_reader.createCSVConfig(ordered);                   // Sauvegarde le tableau dans un .csv

    string** fichier_csv = test_reader.readCSV("../fichier.csv");
    cout << fichier_csv[1][1] << endl;





    return 0;
}

