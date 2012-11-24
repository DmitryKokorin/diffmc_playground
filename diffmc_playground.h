#pragma once

#include <string>
#include <vector>

#include "vector3.h"


class DiffMCPlaygroundApp
{
public:

    DiffMCPlaygroundApp();

    int  run();
    bool parseOptions(int argc, char ** argv);
    void printHelp();

private:

    enum FileOptions { Create, Load, Save };

    struct Options
    {
        Options()
          : execName()
          , workDir()
          , eeProfileOptions(Create)
          , oeProfileOptions(Create)
          , eoProfileOptions(Create)
          , eeProfileName()
          , oeProfileName()
          , eoProfileName()
        {}

        std::string execName;
        std::string workDir;

        FileOptions eeProfileOptions;
        FileOptions oeProfileOptions;
        FileOptions eoProfileOptions;
        std::string eeProfileName;
        std::string oeProfileName;
        std::string eoProfileName;
    };


    Options options_;

private:

    DiffMCPlaygroundApp& operator=(const DiffMCPlaygroundApp& other);
    DiffMCPlaygroundApp(const DiffMCPlaygroundApp& other);
};
