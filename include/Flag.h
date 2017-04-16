//----------------------------------------------------------------------------//
//               █      █                                                     //
//               ████████                                                     //
//             ██        ██                                                   //
//            ███  █  █  ███        Flag.h                                    //
//            █ █        █ █        CMD                                       //
//             ████████████                                                   //
//           █              █       Copyright (c) 2016, 2017                  //
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

#ifndef __CMD_include_Flag_h__
#define __CMD_include_Flag_h__

//std
#include <vector>
#include <string>
//CMD
#include "CMD_Utils.h"


NS_CMD_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Flag                                                                       //
////////////////////////////////////////////////////////////////////////////////
class Flag
{
    // Enum //
public:
    static const int kAnyNumber;
    enum class DuplicateMode { Override, Count };


    // Friends //
public:
    friend class Parser;


    // CTOR / DTOR //
public:
    Flag();


    // Options "Setters" //
public:
    //Args
    ///@brief
    ///     Set the the CMD::Flag doesn't requires any args.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& RequiresNoArgs();

    ///@brief
    ///     Set the the CMD::Flag requires args.
    ///     The required args are processed before the optional args.
    ///@param count
    ///     The number of args that this flag requires.
    ///@throws
    ///     std::out_of_range if count <= 0.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& RequiresArgs(int count = 1);

    ///@brief
    ///     Set the the CMD::Flag has optional args.
    ///     The optional args are processed after the required args.
    ///@param count
    ///     The maximum number of args that this flag can gets.
    ///@throws
    ///     std::out_of_range if count <= 0.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& OptionalArgs(int count = 1);


    ///@brief
    ///     Set the CMD::Parser should stop the parsing after
    ///     find this flag. Helpful for help, version flags.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& StopOnView();


    //Defaults
    ///
    Flag& Default(const std::string &value);


    //Duplicates
    ///@brief
    ///     Set the this flag cannot have any duplicates, i.e
    ///     cannot happen more then once in the command line.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& NotAllowDuplicates();

    ///@brief
    ///     Set that this flag can happen more the one time in
    ///     command line.
    ///@param duplicateMode
    ///     The way that the duplicates will be treated.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& AllowDuplicates(DuplicateMode duplicateMode);


    //Strings
    ///@brief
    ///     Set the short string (one dash, one char) for this flag.
    ///@param shortStr
    ///     A one char string.
    ///@note
    ///     The string should not have the prefix -.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& Short(const std::string &shortStr);

    ///@brief
    ///     Set the long string (two dashes, multi chars) for this flag.
    ///@param longStr
    ///     A string of any size.
    ///@note
    ///     The string should not have the prefix -.
    ///@returns
    ///     Reference to itself enabling cascading method calls.
    Flag& Long(const std::string &longStr);


    // Options "Getters" //
public:
    //Args
    bool getNoArgsRequired() const;

    bool getRequiredArgs() const;
    int getRequiredArgsCount() const;
    int getRequiredArgsFoundCount() const;
    const std::vector<std::string>& getRequiredValues() const;


    bool getOptionalArgs() const;
    int getOptionalArgsCount() const;
    int getOptionalArgsFoundCount() const;
    const std::vector<std::string>& getOptionalValues() const;


    bool getStopOnView() const;

    //Defaults
    const std::string& getDefaultValue() const;


    //Duplicates
    bool getAllowDuplicates() const;
    DuplicateMode getAllowDuplicatesMode() const;


    //Strings
    bool hasShortStr() const;
    const std::string& getShortStr() const;
    const std::string  getShortDisplayStr() const;

    bool hasLongStr() const;
    const std::string& getLongStr() const;
    const std::string  getLongDisplayStr() const;

    const std::string getShortLongDisplayStr() const;


    //Found
    bool wasFound() const;
    int  getFoundCount() const;


    // Private Methods //
private:
    // This methods are intended to be used only by CMD::Parser //
    void addFoundCount();

    void addRequiredValue(const std::string &value);
    void addOptionalValue(const std::string &value);

    void validateStrings() const;
    void clearValues();


    // iVars //
private:
    //Required Args
    bool m_requiredArgs;
    int  m_requiredArgsCount;
    std::vector<std::string> m_requiredValuesVec;

    //Optional Args
    bool m_optionalArgs;
    int  m_optionalArgsCount;
    std::vector<std::string> m_optionalValuesVec;

    //Stop
    bool m_stopOnView;

    //Default
    std::string m_defaultValue;

    //Duplicate
    bool          m_allowDuplicates;
    DuplicateMode m_duplicateMode;

    //Strings
    std::string m_shortStr;
    std::string m_longStr;

    //Found
    int m_foundCount;
};

NS_CMD_END
#endif // defined(__CMD_include_Flag_h__) //
