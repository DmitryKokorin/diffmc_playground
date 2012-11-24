#include "mathcompat.h"

#include <exception>
#include <sstream>
#include <iostream>

#include <omp.h>

#include "common.h"

#include "channel.h"
#include "partition.h"
#include "photon.h"
#include "indicatrix.h"
#include "freepath.h"
#include "optics.h"

#include "diffmc_playground.h"


int main(int argc, char ** argv)
{
    DiffMCPlaygroundApp app;

    int res = 0;

    if (app.parseOptions(argc, argv)) {

        res = app.run();
    }
    else {

        app.printHelp();
        res = -1;
    }

    return res;
}


/////////////////////////////////////////////


DiffMCPlaygroundApp::DiffMCPlaygroundApp()
  : options_()
{
}

bool DiffMCPlaygroundApp::parseOptions(int argc, char ** argv)
{
    options_.execName = argv[0];

    for (int i = 1; i < argc; ++i) {

        std::string arg = argv[i];

        if (arg == "--workdir") {

            if (++i == argc)
                return false;

            options_.workDir = argv[i];

            if (!options_.workDir.empty())
                options_.workDir += '/';
        }
        else if (arg == "--loadoeprofile") {

            if (++i == argc)
                return false;

            options_.oeProfileOptions = Load;
            options_.oeProfileName    = argv[i];
        }
        else if (arg == "--loadeoprofile") {

            if (++i == argc)
                return false;

            options_.eoProfileOptions = Load;
            options_.eoProfileName    = argv[i];
        }
        else if (arg == "--loadeeprofile") {

            if (++i == argc)
                return false;

            options_.eeProfileOptions = Load;
            options_.eeProfileName    = argv[i];
        }
        else if (arg == "--saveoeprofile") {

            if (++i == argc)
                return false;

            options_.oeProfileOptions = Save;
            options_.oeProfileName    = argv[i];
        }
        else if (arg == "--saveeoprofile") {

            if (++i == argc)
                return false;

            options_.eoProfileOptions = Save;
            options_.eoProfileName    = argv[i];
        }
        else if (arg == "--saveeeprofile") {

            if (++i == argc)
                return false;

            options_.eeProfileOptions = Save;
            options_.eeProfileName    = argv[i];
        }
        else
            return false;
    }

    return true;
}


int DiffMCPlaygroundApp::run()
{
//    IndicatrixProfile pOE, pEO, pEE;

    try {

//        prepareEEProfile(iEE);
    }
    catch (std::exception &e) {

        std::cerr << e.what();
        return -1;
    }


    return 0;
}

void DiffMCPlaygroundApp::printHelp()
{
    using namespace std;

    cerr << "Usage: " << options_.execName << "[options]" << endl;
    cerr << "Available options:" << endl;
    cerr << "--workdir [path]\t\t\toutput path" << endl;
    cerr << "--loadoepartition [filename]\t\tload o-e partition from file" << endl;
    cerr << "--loadeopartition [filename]\t\tload e-o partition from file" << endl;
    cerr << "--loadeepartition [filename]\t\tload e-e partition from file" << endl;
    cerr << "--saveoepartition [filename]\t\tsave o-e partition to file" << endl;
    cerr << "--saveeopartition [filename]\t\tsave e-o partition to file" << endl;
    cerr <<  "\n--saveeepartition [filename]\t\tsave e-e partition to file" << endl;
}
