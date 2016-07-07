#include <iostream>
//CMD
#include "CMD.h"

//Usings
using namespace std;


int main(int argc, char **argv)
{
    auto helpFlag = CMD::Flag().RequiresNoArgs().StopOnView()
                               .NotAllowDuplicates()
                               .Short("h").Long("help");

    auto parser = CMD::Parser(argc, argv);

    parser.addFlag(&helpFlag);

    parser.parse();


    //Help
    if(helpFlag.wasFound())
        cout << "Help flag was found." << endl;

}
