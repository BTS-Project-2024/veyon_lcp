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
    string executePowerShellCommand(const char* command);
    string getCurrentDateTime();

public:
    Config();
    void ConfigVeyon();
};

#endif // CONFIG_H
