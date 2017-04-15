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

//Header
#include "../include/Parser.h"
//CMD
#include "../include/CMD_Macros.h"
#include "../include/CMD_Helpers.h"

//Usings
USING_NS_CMD;


////////////////////////////////////////////////////////////////////////////////
// Constants                                                                  //
////////////////////////////////////////////////////////////////////////////////
constexpr int kInvalidFlagIndex = -1;


////////////////////////////////////////////////////////////////////////////////
// CTOR                                                                       //
////////////////////////////////////////////////////////////////////////////////
Parser::Parser(int argc, char **argv)
{
    //Ignore the first index that is the name of program.
    for(int i = 1; i < argc; ++i)
        m_commandLineElements.push_back(argv[i]);
}


Parser::Parser(const std::vector<std::string> &args) :
    m_commandLineElements(std::begin(args), std::end(args))
{
    //Empty...
}


////////////////////////////////////////////////////////////////////////////////
// Public Methods                                                             //
////////////////////////////////////////////////////////////////////////////////
Parser& Parser::addFlag(Flag *flag)
{
    flag->validateStrings();

    m_flagsVec.push_back(flag);
    return *this;
}


void Parser::parse()
{
    cannonizeCommandLineElements();

    //Parse the command line.
    //  After each found flag we gonna remove if from the vector
    //  So eventually it will be empty...
    while(!m_commandLineElements.empty())
    {
        auto currElement = m_commandLineElements.front();
        m_commandLineElements.pop_front();

        //Not a flag
        //  Just add to non flag vec and consume it.
        if(!isFlagStr(currElement))
        {
            m_nonFlagArgs.push_back(currElement);
            continue;
        }


        auto flagIndex = findIndexForFlagWithStr(currElement);

        //Not a valid flag...
        THROW_IF_(
            flagIndex == kInvalidFlagIndex,
            std::invalid_argument,
            "'%s' - Is not a valid flag",
            currElement.c_str()
        );


        //Needs to be a ref because we gonna modify it
        auto &flagObj = m_flagsVec[flagIndex];
        flagObj->addFoundCount();

        //Flag does not allow duplicates,
        //but we already found this earlier
        THROW_IF_(
            (flagObj->getFoundCount() > 1 &&
             flagObj->getAllowDuplicates() == false),
            std::runtime_error,
            "'%s' - Does not allow duplicates but was already found",
            currElement.c_str()
        );


        //This Flag doesn't accept any arguments
        //  We are done here...
        if(flagObj->getRequiredArgs() == false &&
           flagObj->getOptionalArgs() == false)
        {
            //Should stop parsing??
            if(flagObj->getStopOnView()) return;
            else                         continue;
        }


        //Start getting the args for this flag.

        //Override mode
        //  Clear the previous values.
        if(flagObj->getAllowDuplicates() &&
           flagObj->getAllowDuplicatesMode() == Flag::DuplicateMode::Override)
        {
            flagObj->clearValues();
        }


        //Add the REQUIRED ARGS.
        for(std::size_t index = 0;
            flagObj->getRequiredArgsFoundCount() !=
            flagObj->getRequiredArgsCount();
            ++index)
        {
            //We run short of arguments but this flag still requires them.
            THROW_IF_(
                m_commandLineElements.empty(),
                std::runtime_error,
                "'%s' - Requires %d arguments but there is not enough args",
                currElement.c_str(),
                flagObj->getRequiredArgsCount()
            );

            //Get the current first element on the cmd line vec.
            //It can be two things:
            //  1 - A value str (non flag), so we set it to Flag object.
            //  2 - A flag str, meaning that all values for this flag
            //      was "consumed".
            //If all values were consumed means that user didn't passed the
            //number of required values for this flag object.
            auto value = m_commandLineElements.front();

            THROW_IF_(
                isFlagStr(value),
                std::runtime_error,
                 "'%s' - Requires %d arguments but just %d was found",
                 currElement.c_str(),
                 flagObj->getRequiredArgsCount(),
                 index
            );

            //Not a flag str
            //  Add the value to flag and consume it
            //  from the command line elements.
            flagObj->addRequiredValue(value);
            m_commandLineElements.pop_front();
        }


        //Add the OPTIONAL ARGS.
        while(flagObj->getOptionalArgsFoundCount() !=
              flagObj->getOptionalArgsCount())
        {
            //Optional, is err.. optional, so is ok if there is no args.
            if(m_commandLineElements.empty())
                break;

            //Get the current first element on the cmd line vec.
            //It can be two things:
            //  1 - A value str (non flag), so we set it to Flag object.
            //  2 - A flag str, meaning that all values for this flag
            //      was "consumed".
            //If all values were consumed means that there are no
            //optional args for this flag...
            auto value = m_commandLineElements.front();
            if(isFlagStr(value))
                break;

            //Not a flag str - Add the value to flag and consume it
            //from the command line elements.
            flagObj->addOptionalValue(value);
            m_commandLineElements.pop_front();
        }

        //Should stop parsing??
        if(flagObj->getStopOnView()) return;

    }//while(!m_commandLineElements.empty())
}//void parse


std::vector<std::string> Parser::getNonFlagArgs()
{
    return m_nonFlagArgs;
}


////////////////////////////////////////////////////////////////////////////////
// Private Methods                                                            //
////////////////////////////////////////////////////////////////////////////////
//Helpers
void Parser::cannonizeCommandLineElements()
{
    //This method will make all the flags that contains
    //options be in a consistent form. i.e. If a flag
    //contains an argument, the argument will be placed
    //just after it into the vector.
    //So if we want to get the flag argument we just
    //need to look for the index after the flag in the vector.
    //
    //The possible ways of represent a flag with value is:
    //  -f 2      | Short with the space between the flag and value.
    //  -f2       | Short with the flag together with value.
    //  --flag 2  | Long with the space between the flag and value.
    //  --flag=2  | Long with the flag together with value.
    //              Notice that here we MUST have the '=' char
    //
    for(std::size_t i = 0; i < m_commandLineElements.size(); ++i)
    {
        auto argStr = m_commandLineElements[i];

        if(!isFlagStr(argStr))
            continue;

        auto nameStr  = extractFlagName (argStr);
        auto valueStr = extractFlagValue(argStr);

        m_commandLineElements[i] = nameStr;
        if(!valueStr.empty())
        {
            //Insert just after the current flag string.
            m_commandLineElements.insert(
                std::begin(m_commandLineElements) + i + 1,
                valueStr
            );
        }
    }
}


int Parser::findIndexForFlagWithStr(const std::string &str)
{
    for(std::size_t i = 0; i < m_flagsVec.size(); ++i)
    {
        auto &flag = m_flagsVec[i];

        if(isShortFlagStr(str) &&
           flag->hasShortStr() &&
           ltrim(str, '-') == flag->getShortStr())
        {
            return i;
        }

        if(isLongFlagStr(str) &&
           flag->hasLongStr()  &&
           ltrim(str, '-') == flag->getLongStr())
        {
            return i;
        }
    }

    return kInvalidFlagIndex;
}

bool Parser::isFlagStr(const std::string &str)
{
    return isLongFlagStr(str) || isShortFlagStr(str);
}

bool Parser::isLongFlagStr(const std::string &str)
{
    //--AnyChars
    return (str.size() > 2 && str[0] == '-' && str[1] == '-');
}

bool Parser::isShortFlagStr(const std::string &str)
{
    //-c | MUST be size 2 with only one non - char after the first -.
    return (str.size() == 2 && str[0] == '-' && str[1] != '-');
}

std::string Parser::extractFlagName(const std::string &str)
{
    //Remove all stuff that is not the name of the flag.
    //  -f2       | -f
    //  -o        | -o
    //  --flag=2  | --flag
    //  --other   | --other

    if(isShortFlagStr(str))
        return str.substr(0, 2);

    auto delimiterPos = str.find("=");
    if(delimiterPos == std::string::npos)
        delimiterPos = str.size();

    return str.substr(0, delimiterPos);
}

std::string Parser::extractFlagValue(const std::string &str)
{
    //Remove all stuff that is not the value of the flag.
    //  -f2       | 2
    //  -o        | "" (empty)
    //  --flag=2  | 2
    //  --other   | "" (empty)

    if(isShortFlagStr(str))
        return str.substr(2, str.size() -2);

    auto delimiterPos = str.find("=");
    if(delimiterPos == std::string::npos)
        return "";

    return str.substr(delimiterPos+1, str.size());
}
