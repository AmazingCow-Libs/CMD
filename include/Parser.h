//----------------------------------------------------------------------------//
//               █      █                                                     //
//               ████████                                                     //
//             ██        ██                                                   //
//            ███  █  █  ███        Parser.h                                  //
//            █ █        █ █        CMD                                       //
//             ████████████                                                   //
//           █              █       Copyright (c) 2016                        //
//          █     █    █     █      AmazingCow - www.AmazingCow.com           //
//          █     █    █     █                                                //
//           █              █       N2OMatt - n2omatt@amazingcow.com          //
//             ████████████         www.amazingcow.com/n2omatt                //
//                                                                            //
//                  This software is licensed as GPLv3                        //
//                 CHECK THE COPYING FILE TO MORE DETAILS                     //
//                                                                            //
//    Permission is granted to anyone to use this software for any purpose,   //
//   including commercial applications, and to alter it and redistribute it   //
//               freely, subject to the following restrictions:               //
//                                                                            //
//     0. You **CANNOT** change the type of the license.                      //
//     1. The origin of this software must not be misrepresented;             //
//        you must not claim that you wrote the original software.            //
//     2. If you use this software in a product, an acknowledgment in the     //
//        product IS HIGHLY APPRECIATED, both in source and binary forms.     //
//        (See opensource.AmazingCow.com/acknowledgment.html for details).    //
//        If you will not acknowledge, just send us a email. We'll be         //
//        *VERY* happy to see our work being used by other people. :)         //
//        The email is: acknowledgment_opensource@AmazingCow.com              //
//     3. Altered source versions must be plainly marked as such,             //
//        and must not be misrepresented as being the original software.      //
//     4. This notice may not be removed or altered from any source           //
//        distribution.                                                       //
//     5. Most important, you must have fun. ;)                               //
//                                                                            //
//      Visit opensource.amazingcow.com for more open-source projects.        //
//                                                                            //
//                                  Enjoy :)                                  //
//----------------------------------------------------------------------------//

#ifndef __CMD_include_Parser_h__
#define __CMD_include_Parser_h__

//std
#include <deque>
#include <string>
#include <vector>
//CMD
#include "CMD_Utils.h"
#include "Flag.h"

NS_CMD_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Parser                                                                     //
////////////////////////////////////////////////////////////////////////////////
struct Parser
{
    // CTOR //
public:
    ///@brief
    ///     Constructs a CMD::Parse with argc and argv.
    ///@param argc
    ///     The number of strings in argv.
    ///@param argv
    ///     The array of strings that will be parsed.
    ///@note
    ///     This constructor assumes that the name of program
    ///     is given in the first index of the argv.
    ///     The name of program will be discarted, so you
    ///     can pass it directly from main().
    ///@warning
    ///     The constructor will not attempt to validade any
    ///     of the given arguments, it's user's responsabillity
    ///     to ensure that values are valid and consistent.
    Parser(int argc, char **argv);

    ///@brief
    ///     Constructs a CMD::Parse with a std::vector of std::string.
    ///@param args
    ///     A vector of strings taht will be parsed.
    ///@note
    ///     This constructor assumes that the name of program
    ///     IS NOT GIVEN in the first index of vector.
    ///     If you want pass it directly from main() use the
    ///     other constructor or remove the first index from
    ///     argv first.
    ///@warning
    ///     The constructor will not attempt to validade any
    ///     of the given arguments, it's user's responsabillity
    ///     to ensure that values are valid and consistent.
    Parser(const std::vector<std::string> &args);


    // Public Methods //
public:
    ///@brief
    ///     Add a CMD::Flag into the flags that will be processed.
    ///param flag
    ///     A pointer to a CMD::Flag.
    ///@note
    ///     CMD::Parse WILL NOT GET the ownership of the
    ///     CMD::Flag object. The CMD::Flag object that is passed
    ///     to the CMD::Parse MUST stay valid.
    ///@warning
    ///     The CMD::Parse will not check for the validity of the
    ///     CMD::Flags pointers that was passed to it.
    ///@throws
    ///     Any exceptions throw by the CMD::Flag::validateStrings().
    ///@returns
    ///     A reference to this CMD::Parser, enabling the cascading
    ///     methods calls. Ex: parser.addFlag(a).addFlag(b).addFlag(c);
    Parser& addFlag(Flag *flag);



    ///@brief
    ///     Will parse the command line arguments given in the CTOR.
    ///     It will modify the CMD::Flags added in CMD::Parse::addFlag()
    ///     if them are found in the parsing process.
    ///     All the stuff that don't matches with any CMD::Flag are
    ///     placed into a CMD::Parse::getNonFlagArgs().
    ///@throws
    ///     std::invalid_argument if CMD::Parser cannot find a CMD::Flag
    ///     with the same string (short or long) for the command line argument.
    ///@throws
    ///     std::runtime_error if the CMD::Flag doesn't allow duplicates but
    ///     more the one are found.
    ///@throws
    ///     std::runtime_error if the CMD::Flag requires a number of arguments
    ///     but not enough are found.
    void parse();


    ///@brief
    ///     Gets all non flag arguments.
    std::vector<std::string> getNonFlagArgs();


    // Private //
private:
    //Helpers
    void cannonizeCommandLineElements();
    int findIndexForFlagWithStr(const std::string &str);

    //Flag Checkers
    bool isFlagStr     (const std::string &str);
    bool isLongFlagStr (const std::string &str);
    bool isShortFlagStr(const std::string &str);

    //Flag extractors
    std::string extractFlagName (const std::string &str);
    std::string extractFlagValue(const std::string &str);


    // iVars //
private:
    std::vector<Flag *>      m_flagsVec;
    std::vector<std::string> m_nonFlagArgs;

    std::deque<std::string> m_commandLineElements;
};

NS_CMD_END
#endif // defined(__CMD_include_Parser_h__) //
