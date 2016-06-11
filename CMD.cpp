//Header
#include "CMD.h"
//std
#include <cstdarg>
#include <deque>
#include <exception>
#include <iostream>

//Usings
using namespace std;
using namespace CMD;


std::string va(const char *fmt, ...)
{
    constexpr int kBufferSize = 1024;
    char buffer[kBufferSize]  = { '\0' };

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, kBufferSize, fmt, args);

    va_end(args);

    return buffer;
}

std::string ltrim(const std::string &str, char c)
{
    return str.substr(str.find_first_not_of(c), str.size());
}

#define THROW_IF_(_expr_, _extype_, _fmt_, ...) \
    if((_expr_)) {  \
        throw _extype_(va(_fmt_, ##__VA_ARGS__)); \
    }



#define ASSERT_(_expr_, _fmt_, ...) \
    if(!(_expr_)) { \
        cout << va(_fmt_, ##__VA_ARGS__) << endl; \
    }


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Flag                                                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
const int Flag::kAnyNumber = 200; //COWTODO: fix...

Flag::Flag():
    m_requiredArgs          (false),
    m_requiredArgsCount     (0),

    m_optionalArgs          (false),
    m_optionalArgsCount     (0),

    m_stopOnView       (false),

    m_allowDuplicates  (false),
    m_duplicateMode    (Flag::DuplicateMode::Count),

    m_foundCount       (0)
{
    //Empty...
}


////////////////////////////////////////////////////////////////////////////////
// Options "Setters"                                                          //
////////////////////////////////////////////////////////////////////////////////
//Args
Flag& Flag::NoArgs()
{
    m_requiredArgs = false;
    m_optionalArgs = false;

    m_requiredArgsCount = 0;
    m_optionalArgsCount = 0;

    return *this;
}

Flag& Flag::RequiredArgs(int count /* = 1 */)
{
    m_requiredArgs      = true;
    m_requiredArgsCount = count;

    THROW_IF_(
        m_requiredArgsCount <= 0,
        std::out_of_range,
        "CMD::Flag::RequiredArgs must be called with > 0 value - Current is: %d",
        m_requiredArgsCount
    );

    return *this;
}

Flag& Flag::OptionalArgs(int count /* = 1 */ )
{
    m_optionalArgs      = true;
    m_optionalArgsCount = count;

    THROW_IF_(
        m_optionalArgsCount <= 0,
        std::out_of_range,
        "CMD::Flag::OptionalArgs must be called with > 0 value - Current is: %d",
        m_optionalArgsCount
    );

    return *this;
}


//
Flag& Flag::StopOnView()
{
    m_stopOnView = true;
    return *this;
}


//Duplicates
Flag& Flag::DoNotAllowDuplicates()
{
    m_allowDuplicates = false;
    return *this;
}

Flag& Flag::AllowDuplicates(DuplicateMode duplicateMode)
{
    m_allowDuplicates = true;
    m_duplicateMode  = duplicateMode;

    return *this;
}


//Strings
Flag& Flag::Short(const std::string &shortStr)
{
    m_shortStr = shortStr;
    return *this;
}

Flag& Flag::Long(const std::string &longStr)
{
    m_longStr = longStr;
    return *this;
}



////////////////////////////////////////////////////////////////////////////////
// Options "Gettters"                                                         //
////////////////////////////////////////////////////////////////////////////////
//Args
bool Flag::getNoArgs() const
{
    return m_requiredArgs == false && m_optionalArgsCount == false;
}


bool Flag::getRequiredArgs() const
{
    return m_requiredArgs;
}

int Flag::getRequiredArgsCount() const
{
    return m_requiredArgsCount;
}

int Flag::getRequiredArgsFoundCount() const
{
    return m_requiredValuesVec.size();
}

const std::vector<std::string>& Flag::getRequiredValues() const
{
    return m_requiredValuesVec;
}


bool Flag::getOptionalArgs() const
{
    return m_optionalArgs;
}

int Flag::getOptionalArgsCount() const
{
    return m_optionalArgsCount;
}

int Flag::getOptionalArgsFoundCount() const
{
    return m_optionalValuesVec.size();
}

const std::vector<std::string>& Flag::getOptionalValues() const
{
    return m_optionalValuesVec;
}


//
bool Flag::getStopOnView() const
{
    return m_stopOnView;
}


//Duplicates
bool Flag::getAllowDuplicates() const
{
    return m_allowDuplicates;
}

Flag::DuplicateMode Flag::getAllowDuplicatesMode() const
{
   ASSERT_(getAllowDuplicates() == true,
           "BAD LOGIC - We should not call this when getAllowDuplicates() is false. %s",
           getLongStr().c_str());

   return m_duplicateMode;
}


//Strings
bool Flag::hasShortStr() const
{
    return !m_shortStr.empty();
}

const std::string& Flag::getShortStr() const
{
   ASSERT_(hasShortStr() == true,
           "BAD LOGIC - We should not call this when hasShortStr() is false.");

   return m_shortStr;
}


bool Flag::hasLongStr() const
{
    return !m_longStr.empty();
}

const std::string& Flag::getLongStr() const
{
    ASSERT_(hasLongStr() == true,
           "BAD LOGIC - We should not call this when hasLongStr() is false.");

    return m_longStr;
}

//Found
bool Flag::wasFound() const
{
    return m_foundCount != 0;
}

int Flag::getFoundCount() const
{
    return m_foundCount;
}


////////////////////////////////////////////////////////////////////////////////
// Private Methods                                                            //
////////////////////////////////////////////////////////////////////////////////
void Flag::addFoundCount()
{
    //COWTODO: Trhow if do not allow duplicates and this value is > 1
    m_foundCount++;
}

void Flag::addRequiredValue(const std::string &value)
{
    //COWTODO: Throw if already reached the ammount needed.
    m_requiredValuesVec.push_back(value);
}

void Flag::addOptionalValue(const std::string &value)
{
    //COWTODO: Throw if already reached the ammount needed.
    m_optionalValuesVec.push_back(value);
}


void Flag::validateStrings() const
{
    //Validate the strings...
    THROW_IF_(
        (!hasShortStr() && !hasLongStr()),
        std::logic_error,
        "CMD::Flag must contain either a short or long string"
    );

    //Short string must has only one char.
    THROW_IF_(
        (hasShortStr() && m_shortStr.size() != 1),
        std::domain_error,
        "CMD::Flag shortStr must be only char long - shortStr is: %s",
        m_shortStr.c_str()
    );
}

void Flag::clearValues()
{
    m_requiredValuesVec.clear();
    m_optionalValuesVec.clear();
}



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Parser                                                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// CTOR                                                                       //
////////////////////////////////////////////////////////////////////////////////
Parser::Parser(int argc, char **argv)
{
    for(int i = 1; i < argc; ++i)
        m_commandLineElements.push_back(argv[i]);
}


Parser::Parser(const std::vector<std::string> &args) :
    m_commandLineElements(std::begin(args), std::end(args))
{

}


////////////////////////////////////////////////////////////////////////////////
// Public Methods                                                             //
////////////////////////////////////////////////////////////////////////////////
// Public Methods //
Parser& Parser::addFlag(Flag *flag)
{
    flag->validateStrings();

    m_flagsVec.push_back(flag);
    return *this;
}


void Parser::parse()
{
    //COWTODO: Comment.
    for(int i = 0; i < m_commandLineElements.size(); ++i)
    {
        auto argStr = m_commandLineElements[i];

        if(!isFlagStr(argStr))
            continue;

        auto nameStr  = extractFlagName (argStr);
        auto valueStr = extractFlagValue(argStr);

        m_commandLineElements[i] = nameStr;
        if(!valueStr.empty())
        {
            m_commandLineElements.insert(
                std::begin(m_commandLineElements) + i + 1,
                valueStr
            );
        }
    }


    //Parse the command line.
    while(!m_commandLineElements.empty())
    {
        auto currElement = m_commandLineElements.front();
        m_commandLineElements.pop_front();

        //Not a flag - Just add to non flag vec and consume it.
        if(!isFlagStr(currElement))
        {
            m_nonFlagArgs.push_back(currElement);
            continue;
        }

        int flagIndex = findIndexForFlagWithStr(currElement);

        //Not a valid flag...
        THROW_IF_(
            flagIndex == -1,
            std::invalid_argument,
            "'%s' - Is not a valid flag",
            currElement.c_str()
        );


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

        //Start getting the args for this flag...
        if(flagObj->getRequiredArgs())
        {
            //Override mode - Clear the previous values.
            if(flagObj->getAllowDuplicates() &&
               flagObj->getAllowDuplicatesMode() == Flag::DuplicateMode::Override)
            {
                flagObj->clearValues();
            }

            //Add the REQUIRED ARGS.
            for(int index = 0;
                flagObj->getRequiredArgsFoundCount() !=
                flagObj->getRequiredArgsCount();
                ++index)
            {
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

                //Not a flag str - Add the value to flag and consume it
                //from the command line elements.
                flagObj->addRequiredValue(value);
                m_commandLineElements.pop_front();
            }


            //Add the OPTIONAL ARGS.
            while(flagObj->getOptionalArgsFoundCount() !=
                  flagObj->getOptionalArgsCount())
            {
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
        }

        //Should stop parsing??
        if(flagObj->getStopOnView())
            return;

    }//while(!m_commandLineElements.empty())
}//void parse


std::vector<std::string> Parser::getNonFlagArgs()
{
    return m_nonFlagArgs;
}


////////////////////////////////////////////////////////////////////////////////
// Private Methods                                                            //
////////////////////////////////////////////////////////////////////////////////
int Parser::findIndexForFlagWithStr(const std::string &str)
{
    for(int i = 0; i < m_flagsVec.size(); ++i)
    {
        auto &flag = m_flagsVec[i];

        if(isShortFlagStr(str) &&
           flag->hasShortStr()  &&
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

    return -1;
}

bool Parser::isFlagStr(const std::string &str)
{
    return isLongFlagStr(str) || isShortFlagStr(str);
}
bool Parser::isLongFlagStr(const std::string &str)
{
    return (str.size() > 2 && str[0] == '-' && str[1] == '-');
}
bool Parser::isShortFlagStr(const std::string &str)
{
    return (str.size() > 1 && str[0] == '-' && str[1] != '-');
}

std::string Parser::extractFlagName(const std::string &str)
{
    if(isShortFlagStr(str))
        return str.substr(0, 2);

    auto delimiterPos = str.find("=");
    if(delimiterPos == std::string::npos)
        delimiterPos = str.size();

    return str.substr(0, delimiterPos);
}

std::string Parser::extractFlagValue(const std::string &str)
{
    if(isShortFlagStr(str))
        return str.substr(2, str.size() -2);

    auto delimiterPos = str.find("=");
    if(delimiterPos == std::string::npos)
        return "";

    return str.substr(delimiterPos+1, str.size());
}
