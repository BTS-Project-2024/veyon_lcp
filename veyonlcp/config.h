#ifndef CONFIG_H
#define CONFIG_H

#include <string>

using namespace std;

class Config
{
private:
    string netbios;
    string ip;
    string mac;
    string salle;
    string getCurrentDateTime();
    void deleteConfigVeyon();

public:
    Config();
    void configVeyon(string, string);
};

#endif // CONFIG_H
