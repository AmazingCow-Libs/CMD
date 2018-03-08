//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : Parser.cpp                                                    //
//  Project   : CMD                                                           //
//  Date      : Mar 06, 2018                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2018                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

// Header
#include "../include/Parser.h"
// std
#include <cstring>
#include <algorithm>
#include <sstream>
// AmazingCow Libs
#include "acow/cpp_goodies.h"
#include "CoreAssert/CoreAssert.h"
#include "CoreString/CoreString.h"
#include "CoreLog/CoreLog.h"
// CMD
#include "../include/private/Logger.h"

// Usings
USING_NS_CMD;


//----------------------------------------------------------------------------//
// Helper Functions                                                           //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
// Find Flag Type.
acow_internal_function bool
check_is_short_flag(const std::string &str) noexcept
{
    return  CoreString::StartsWith(str, "-" )
        && !CoreString::StartsWith(str, "--");
}

acow_internal_function bool
check_is_long_flag(const std::string &str) noexcept
{
    return  CoreString::StartsWith(str, "--");
}

//------------------------------------------------------------------------------
// Extract Flag Info.
acow_internal_function std::string
extract_flag_name(
    const std::string &str,
    bool               isShortFlag) noexcept
{
    if(isShortFlag)
    {
        COREASSERT_ASSERT(
            str.size() >= 2,
            "Insufficient flag size - Flag: (%s)", str.c_str()
        );

        return str.substr(1, 1);
    }

    COREASSERT_ASSERT(
        str.size() >= 3,
        "Insufficient flag size - Flag: (%s)", str.c_str()
    );

    auto last_index = CoreString::LastIndexOfAny(str, " =");
    return str.substr(2, last_index - 2);
}

acow_internal_function std::string
extract_short_flag_glued_value(const std::string &str) noexcept
{
    COREASSERT_ASSERT(
        str.size() >= 2,
        "Insufficient flag size - Flag: (%s)", str.c_str()
    );

    auto value = str.substr(2);
    return value;
}

acow_internal_function std::string
extract_long_flag_glued_value(const std::string &str) noexcept
{
    COREASSERT_ASSERT(
        str.size() >= 3,
        "Insufficient flag size - Flag: (%s)", str.c_str()
    );

    auto index = CoreString::IndexOf(str, '=');
    if(index == std::string::npos)
        return "";

    auto value = str.substr(index + 1);
    return value;
}

acow_internal_function std::string
consume_next_flag(
    int  *pCurrIndex,
    int   argc,
    char *argv[],
    bool  isShortFlag) noexcept
{
    COREASSERT_ASSERT(pCurrIndex, "pCurrIndex can't be null");

    while(*pCurrIndex < argc)
    {
        std::string arg = CoreString::Trim(argv[(*pCurrIndex)++]);

        // Empty...
        if(CoreString::IsNullOrWhiteSpace(arg))
            continue;

        // Long option - Ignore the = (separator char)
        if(!isShortFlag && arg == "=")
            continue;

        return arg;
    }

    return "";
}


void CMD::CheckParserOptionsValidity(ParserOptions options) noexcept
{

}

//----------------------------------------------------------------------------//
// CTOR / DTOR                                                                //
//----------------------------------------------------------------------------//
Parser::Parser(
    ParserOptions options,
    const std::initializer_list<Flag::SPtr> &flags) noexcept
    // Members.
    : m_options(options)
{
    COREASSERT_ONLY_IN_DEBUG({
        CheckParserOptionsValidity(options);
    })

    for(const auto &p_flag : flags)
        AddFlag(p_flag);
}

//----------------------------------------------------------------------------//
// Public Methods                                                             //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
// Flags
Parser&
Parser::AddFlag(Flag::SPtr pFlag) noexcept
{
    //--------------------------------------------------------------------------
    // Flag name already used.
    COREASSERT_ASSERT(
        FindFlagByName(pFlag->GetShortName(), pFlag->GetLongName()) == nullptr,
        "Flag name: (short: %s - long: %s) already used.",
        pFlag->GetShortName().c_str(),
        pFlag->GetLongName ().c_str()
    );

    //--------------------------------------------------------------------------
    // Add the flag.
    m_flags.push_back(pFlag);

    return *this;
}

Parser&
Parser::AddFlag(
    const std::string &shortName,
    const std::string &longName,
    const std::string &description,
    CMD::FlagOptions   flagOptions) noexcept
{
    auto p_flag = Flag::Create(shortName, longName, description, flagOptions);
    AddFlag(p_flag);

    return *this;
}

//------------------------------------------------------------------------------
// Parser
Parser&
Parser::Parse(int argc, char **argv) noexcept
{
    COREASSERT_ASSERT(!m_flags.empty(), "Parser has no flags to parse");

    int curr_index = 1;
    while(curr_index < argc)
    {
        auto arg = CoreString::Trim(argv[curr_index++]);

        //----------------------------------------------------------------------
        // Long Flags.
        auto is_short_flag = check_is_short_flag(arg);
        auto is_long_flag  = check_is_long_flag (arg);

        if(is_short_flag || is_long_flag)
        {
            auto flag_name = extract_flag_name(arg, is_short_flag);
            auto p_flag    = (is_short_flag)
                ? FindFlagByShortName(flag_name)
                : FindFlagByLongName (flag_name);

            //------------------------------------------------------------------
            // Check if we should continue (return true) or let the function
            // make the parser die if the flag isn't found.
            // After this call we are sure that we can continue parse this
            // flag correctly.
            if(!p_flag && ShouldContinueOrDieOnNonValidFlags(arg))
                continue;

            auto flag_options = p_flag->GetOptions();
            auto glued_value  = (is_short_flag)
                ? extract_short_flag_glued_value(arg)
                : extract_long_flag_glued_value (arg);

            //------------------------------------------------------------------
            // No Arguments.
            //   Flag must have NO arguments but has.
            if(ACOW_FLAG_HAS(NoArgument, flag_options))
            {
                if(!glued_value.empty())
                {
                    _private::get_logger_instance()->Fatal(
                        "Flag (%s) must have no arguments - Found: (%s)",
                        arg.c_str(),
                        glued_value.c_str()
                    );
                }
            }
            //------------------------------------------------------------------
            // Required Arguments.
            else if(ACOW_FLAG_HAS(RequiredArgument, flag_options))
            {
                auto value = (!glued_value.empty())
                    ? glued_value
                    : consume_next_flag(&curr_index, argc, argv, is_short_flag);

                //--------------------------------------------------------------
                // Flag must HAVE arguments but doesn't.
                if(value.empty())
                {
                    _private::get_logger_instance()->Fatal(
                        "Flag (%s) requires arguments.",
                        arg.c_str()
                    );
                }

                p_flag->AddValue(value);
            }
            //------------------------------------------------------------------
            // Optional Arguments.
            else if(ACOW_FLAG_HAS(OptionalArgument, flag_options))
            {
                auto value = (!glued_value.empty())
                    ? glued_value
                    : consume_next_flag(&curr_index, argc, argv, is_short_flag);

                if(!value.empty())
                    p_flag->AddValue(value);
            }

            p_flag->IncrementFoundCount();

            //------------------------------------------------------------------
            // Stop on View flag - We must stop parsing now!
            if(ACOW_FLAG_HAS(StopOnView, flag_options))
                return *this;
        }
        //----------------------------------------------------------------------
        // Non flags.
        else
        {
            m_nonFlagValues.push_back(arg);
        }
    }

    return *this;
}

//------------------------------------------------------------------------------
// Utils
std::string
Parser::GenerateHelpString(int maxColumns /* = -1 */) const noexcept
{
    // //--------------------------------------------------------------------------
    // // First pass is to gather info.
    // int short_name_max_len  = -1;
    // int long_name_max_len   = -1;
    // int desc_max_len        = -1;

    // for(const auto &p_flag : m_flags)
    // {
    //     auto flag_options = p_flag->GetOptions    ();
    //     int  short_len    = p_flag->GetShortName  ().size();
    //     int  long_len     = p_flag->GetLongName   ().size();
    //     int  desc_len     = p_flag->GetDescription().size();

    //     // We gonna put an * on front of it.
    //     if(ACOW_FLAG_HAS(StopOnView, flag_options))
    //         ++short_len;

    //     short_name_max_len = std::max(short_name_max_len, short_len);
    //     long_name_max_len  = std::max(long_name_max_len,  long_len );
    //     desc_max_len       = std::max(desc_max_len,       desc_len );
    // }

    // short_name_max_len += 1; // For -
    // long_name_max_len  += 2; // For --

    // //--------------------------------------------------------------------------
    // // Second pass is to actually build the string.
    // std::stringstream ss;
    // // *-h
    // //  -h
    // auto short_format = CoreString::Format("%%c-%%s", short_name_max_len);
    // for(const auto &p_flag : m_flags)
    // {
    //     auto flag_options = p_flag->GetOptions  ();
    //     auto short_name   = p_flag->GetShortName();
    //     auto long_name    = p_flag->GetLongName ();

    //     if(!short_name.empty())
    //     {
    //         auto leading_char = (ACOW_FLAG_HAS(StopOnView, flag_options))
    //             ? '*'
    //             : ' ';
    //         auto formated = CoreString::Format(short_format, leading_char, short_name);
    //         ss << formated;
    //     }

    //     CoreLog::D(ss.str());
    // }

    // return ss.str();
    return "";
}


//----------------------------------------------------------------------------//
// Private Methods                                                            //
//----------------------------------------------------------------------------//
bool Parser::ShouldContinueOrDieOnNonValidFlags(
        const std::string &flag) const noexcept
{
    //--------------------------------------------------------------------------
    // Die.
    if(ACOW_FLAG_HAS(DieOnNonValidFlags, m_options))
    {
        _private::get_logger_instance()->Fatal(
            "Flag not recognized - (%s)", flag
        );
    }

    //--------------------------------------------------------------------------
    // Ignore.
    if(ACOW_FLAG_HAS(IgnoreNonValidFlags, m_options))
    {
        //----------------------------------------------------------------------
        // Warn.
        if(ACOW_FLAG_HAS(WarnNonValidFlags, m_options))
        {
            _private::get_logger_instance()->Warn(
                "Flag not recognized - (%s)", flag
            );
        }
    }

    return true;
}

//------------------------------------------------------------------------------
// Find Flag.
Flag* Parser::FindFlagByShortName(
    const std::string &shortName) const noexcept
{
    auto it = std::find_if(
        std::begin(m_flags),
        std::end  (m_flags),
        [&shortName](const Flag::SPtr &pFlag) {
            return shortName == pFlag->GetShortName();
        }
    );

    if(it != std::end(m_flags))
        return const_cast<Flag *>(it->get());

    return nullptr;
}

Flag* Parser::FindFlagByLongName (
    const std::string &longName) const noexcept
{
    auto it = std::find_if(
        std::begin(m_flags),
        std::end  (m_flags),
        [&longName](const Flag::SPtr &pFlag) {
            return longName == pFlag->GetLongName();
        }
    );

    if(it != std::end(m_flags))
        return const_cast<Flag *>(it->get());

    return nullptr;
}

Flag* Parser::FindFlagByName(
    const std::string &shortName,
    const std::string &longName) const noexcept
{
    auto it = std::find_if(
        std::begin(m_flags),
        std::end(m_flags),
        [&shortName, &longName](const Flag::SPtr &pFlag){
            if(!shortName.empty() && shortName == pFlag->GetShortName())
                return true;
            if(!longName .empty() && longName  == pFlag->GetLongName ())
                return true;

            return false;
        }
    );

    if(it != std::end(m_flags))
        return const_cast<Flag*>(it->get());

    return nullptr;
}






// //----------------------------------------------------------------------------//
// //               █      █                                                     //
// //               ████████                                                     //
// //             ██        ██                                                   //
// //            ███  █  █  ███        Parser.cpp                                //
// //            █ █        █ █        CMD                                       //
// //             ████████████                                                   //
// //           █              █       Copyright (c) 2016, 2017                  //
// //          █     █    █     █      AmazingCow - www.AmazingCow.com           //
// //          █     █    █     █                                                //
// //           █              █       N2OMatt - n2omatt@amazingcow.com          //
// //             ████████████         www.amazingcow.com/n2omatt                //
// //                                                                            //
// //                  This software is licensed as GPLv3                        //
// //                 CHECK THE COPYING FILE TO MORE DETAILS                     //
// //                                                                            //
// //    Permission is granted to anyone to use this software for any purpose,   //
// //   including commercial applications, and to alter it and redistribute it   //
// //               freely, subject to the following restrictions:               //
// //                                                                            //
// //     0. You **CANNOT** change the type of the license.
               //
// //     1. The origin of this software must not be misrepresented;             //
// //        you must not claim that you wrote the original software.            //
// //     2. If you use this software in a product, an acknowledgment in the     //
// //        product IS HIGHLY APPRECIATED, both in source and binary forms.     //
// //        (See opensource.AmazingCow.com/acknowledgment.html for details).    //
// //        If you will not acknowledge, just send us a email. We'll be         //
// //        *VERY* happy to see our work being used by other people. :)         //
// //        The email is: acknowledgment_opensource@AmazingCow.com              //
// //     3. Altered source versions must be plainly marked as such,             //
// //        and must not be misrepresented as being the original software.      //
// //     4. This notice may not be removed or altered from any source           //
// //        distribution.                                                       //
// //     5. Most important, you must have fun. ;)                               //
// //                                                                            //
// //      Visit opensource.amazingcow.com for more open-source projects.        //
// //                                                                            //
// //                                  Enjoy :)                                  //
// //----------------------------------------------------------------------------//

// //Header
// #include "../include/Parser.h"
// //CMD
// #include "../include/CMD_Macros.h"
// #include "../include/CMD_Helpers.h"

// //Usings
// USING_NS_CMD;


// ////////////////////////////////////////////////////////////////////////////////
// // Constants                                                                  //
// ////////////////////////////////////////////////////////////////////////////////
// constexpr int kInvalidFlagIndex = -1;


// ////////////////////////////////////////////////////////////////////////////////
// // CTOR                                                                       //
// ////////////////////////////////////////////////////////////////////////////////
// Parser::Parser(int argc, char **argv)
// {
//     //Ignore the first index that is the name of program.
//     for(int i = 1; i < argc; ++i)
//         m_commandLineElements.push_back(argv[i]);
// }


// Parser::Parser(const std::vector<std::string> &args) :
//     m_commandLineElements(std::begin(args), std::end(args))
// {
//     //Empty...
// }


// ////////////////////////////////////////////////////////////////////////////////
// // Public Methods                                                             //
// ////////////////////////////////////////////////////////////////////////////////
// Parser& Parser::addFlag(Flag *flag)
// {
//     flag->validateStrings();

//     m_flagsVec.push_back(flag);
//     return *this;
// }


// void Parser::parse()
// {
//     cannonizeCommandLineElements();

//     //Parse the command line.
//     //  After each found flag we gonna remove if from the vector
//     //  So eventually it will be empty...
//     while(!m_commandLineElements.empty())
//     {
//         auto currElement = m_commandLineElements.front();
//         m_commandLineElements.pop_front();

//         //Not a flag
//         //  Just add to non flag vec and consume it.
//         if(!isFlagStr(currElement))
//         {
//             m_nonFlagArgs.push_back(currElement);
//             continue;
//         }


//         auto flagIndex = findIndexForFlagWithStr(currElement);

//         //Not a valid flag...
//         THROW_IF_(
//             flagIndex == kInvalidFlagIndex,
//             std::invalid_argument,
//             "'%s' - Is not a valid flag",
//             currElement.c_str()
//         );


//         //Needs to be a ref because we gonna modify it
//         auto &flagObj = m_flagsVec[flagIndex];
//         flagObj->addFoundCount();

//         //Flag does not allow duplicates,
//         //but we already found this earlier
//         THROW_IF_(
//             (flagObj->getFoundCount() > 1 &&
//              flagObj->getAllowDuplicates() == false),
//             std::runtime_error,
//             "'%s' - Does not allow duplicates but was already found",
//             currElement.c_str()
//         );


//         //This Flag doesn't accept any arguments
//         //  We are done here...
//         if(flagObj->getRequiredArgs() == false &&
//            flagObj->getOptionalArgs() == false)
//         {
//             //Should stop parsing??
//             if(flagObj->getStopOnView()) return;
//             else                         continue;
//         }


//         //Start getting the args for this flag.

//         //Override mode
//         //  Clear the previous values.
//         if(flagObj->getAllowDuplicates() &&
//            flagObj->getAllowDuplicatesMode() == Flag::DuplicateMode::Override)
//         {
//             flagObj->clearValues();
//         }


//         //Add the REQUIRED ARGS.
//         for(std::size_t index = 0;
//             flagObj->getRequiredArgsFoundCount() !=
//             flagObj->getRequiredArgsCount();
//             ++index)
//         {
//             //We run short of arguments but this flag still requires them.
//             THROW_IF_(
//                 m_commandLineElements.empty(),
//                 std::runtime_error,
//                 "'%s' - Requires %d arguments but there is not enough args",
//                 currElement.c_str(),
//                 flagObj->getRequiredArgsCount()
//             );

//             //Get the current first element on the cmd line vec.
//             //It can be two things:
//             //  1 - A value str (non flag), so we set it to Flag object.
//             //  2 - A flag str, meaning that all values for this flag
//             //      was "consumed".
//             //If all values were consumed means that user didn't passed the
//             //number of required values for this flag object.
//             auto value = m_commandLineElements.front();

//             THROW_IF_(
//                 isFlagStr(value),
//                 std::runtime_error,
//                  "'%s' - Requires %d arguments but just %d was found",
//                  currElement.c_str(),
//                  flagObj->getRequiredArgsCount(),
//                  index
//             );

//             //Not a flag str
//             //  Add the value to flag and consume it
//             //  from the command line elements.
//             flagObj->addRequiredValue(value);
//             m_commandLineElements.pop_front();
//         }


//         //Add the OPTIONAL ARGS.
//         while(flagObj->getOptionalArgsFoundCount() !=
//               flagObj->getOptionalArgsCount())
//         {
//             //Optional, is err.. optional, so is ok if there is no args.
//             if(m_commandLineElements.empty())
//                 break;

//             //Get the current first element on the cmd line vec.
//             //It can be two things:
//             //  1 - A value str (non flag), so we set it to Flag object.
//             //  2 - A flag str, meaning that all values for this flag
//             //      was "consumed".
//             //If all values were consumed means that there are no
//             //optional args for this flag...
//             auto value = m_commandLineElements.front();
//             if(isFlagStr(value))
//                 break;

//             //Not a flag str - Add the value to flag and consume it
//             //from the command line elements.
//             flagObj->addOptionalValue(value);
//             m_commandLineElements.pop_front();
//         }

//         //Should stop parsing??
//         if(flagObj->getStopOnView()) return;

//     }//while(!m_commandLineElements.empty())
// }//void parse


// std::vector<std::string> Parser::getNonFlagArgs()
// {
//     return m_nonFlagArgs;
// }


// ////////////////////////////////////////////////////////////////////////////////
// // Private Methods                                                            //
// ////////////////////////////////////////////////////////////////////////////////
// //Helpers
// void Parser::cannonizeCommandLineElements()
// {
//     //This method will make all the flags that contains
//     //options be in a consistent form. i.e. If a flag
//     //contains an argument, the argument will be placed
//     //just after it into the vector.
//     //So if we want to get the flag argument we just
//     //need to look for the index after the flag in the vector.
//     //
//     //The possible ways of represent a flag with value is:
//     //  -f 2      | Short with the space between the flag and value.
//     //  -f2       | Short with the flag together with value.
//     //  --flag 2  | Long with the space between the flag and value.
//     //  --flag=2  | Long with the flag together with value.
//     //              Notice that here we MUST have the '=' char
//     //
//     for(std::size_t i = 0; i < m_commandLineElements.size(); ++i)
//     {
//         auto argStr = m_commandLineElements[i];

//         if(!isFlagStr(argStr))
//             continue;

//         auto nameStr  = extractFlagName (argStr);
//         auto valueStr = extractFlagValue(argStr);

//         m_commandLineElements[i] = nameStr;
//         if(!valueStr.empty())
//         {
//             //Insert just after the current flag string.
//             m_commandLineElements.insert(
//                 std::begin(m_commandLineElements) + i + 1,
//                 valueStr
//             );
//         }
//     }
// }


// int Parser::findIndexForFlagWithStr(const std::string &str)
// {
//     for(std::size_t i = 0; i < m_flagsVec.size(); ++i)
//     {
//         auto &flag = m_flagsVec[i];

//         if(isShortFlagStr(str) &&
//            flag->hasShortStr() &&
//            ltrim(str, '-') == flag->getShortStr())
//         {
//             return i;
//         }

//         if(isLongFlagStr(str) &&
//            flag->hasLongStr()  &&
//            ltrim(str, '-') == flag->getLongStr())
//         {
//             return i;
//         }
//     }

//     return kInvalidFlagIndex;
// }

// bool Parser::isFlagStr(const std::string &str)
// {
//     return isLongFlagStr(str) || isShortFlagStr(str);
// }

// bool Parser::isLongFlagStr(const std::string &str)
// {
//     //--AnyChars
//     return (str.size() > 2 && str[0] == '-' && str[1] == '-');
// }

// bool Parser::isShortFlagStr(const std::string &str)
// {
//     //-c | MUST be size 2 with only one non - char after the first -.
//     return (str.size() == 2 && str[0] == '-' && str[1] != '-');
// }

// std::string Parser::extractFlagName(const std::string &str)
// {
//     //Remove all stuff that is not the name of the flag.
//     //  -f2       | -f
//     //  -o        | -o
//     //  --flag=2  | --flag
//     //  --other   | --other

//     if(isShortFlagStr(str))
//         return str.substr(0, 2);

//     auto delimiterPos = str.find("=");
//     if(delimiterPos == std::string::npos)
//         delimiterPos = str.size();

//     return str.substr(0, delimiterPos);
// }

// std::string Parser::extractFlagValue(const std::string &str)
// {
//     //Remove all stuff that is not the value of the flag.
//     //  -f2       | 2
//     //  -o        | "" (empty)
//     //  --flag=2  | 2
//     //  --other   | "" (empty)

//     if(isShortFlagStr(str))
//         return str.substr(2, str.size() -2);

//     auto delimiterPos = str.find("=");
//     if(delimiterPos == std::string::npos)
//         return "";

//     return str.substr(delimiterPos+1, str.size());
// }
