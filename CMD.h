#include <getopt.h>
#include <iostream>
#include <string>
#include <utility>
#include <functional>
#include <exception>

using namespace std;

#define NS_CMD_BEGIN namespace CMD {
#define NS_CMD_END   }


NS_CMD_BEGIN

enum class Arg
{
    No       = 0,
    Optional = 2,
    Required = 1,
};


typedef struct option _getopt_option;


////////////////////////////////////////////////////////////////////////////////
// Flag                                                                       //
////////////////////////////////////////////////////////////////////////////////
class Flag
{
    // Friends //
public:
    friend class Parser;

    // CTOR / DTOR //
public:
    Flag(CMD::Arg arg,
         const std::string &shortStr,
         const std::string &longStr,
         const std::string &description = "") :
        m_arg        (arg     ),
        m_short      (shortStr),
        m_long       (longStr ),
        m_description(description)
    {
        //Validate the strings...
        if(!hasShortStr() && !hasLongStr())
        {
            auto msg = "CMD::Flag must contain either a short or long string";
            throw std::logic_error(msg);
        }

        //Short string must has only one char.
        if(shortStr.size() != 1)
        {
            std::string msg = "CMD::Flag shortStr must be only char long";
            msg += " - shortStr is: " + m_short;

            throw std::domain_error(msg);
        }
    }

    bool hasShortStr() const
    {
        return !m_short.empty();
    }

    char hasLongStr() const
    {
        return !m_long.empty();
    }


    std::string getShortStr() const
    {
        //Check if this operation is valid.
        if(!hasShortStr())
        {
            std::string msg = "CMD::Flag hasn't a shortStr - longStr is: " + m_long;
            throw std::logic_error(msg);
        }

        return m_short;
    }

    std::string getLongStr() const
    {
        //Check if this operation is valid.
        if(!hasShortStr())
        {
            std::string msg = "CMD::Flag hasn't a longStr - shortStr is: " + m_short;
            throw std::logic_error(msg);
        }

        return m_long;
    }


    // Private Methods //
private:
    // This methods are intended to be used only by CMD::Parser //
    void setFlagId(int id)
    {
        m_flagId = id;
    }

    int getFlagId() const
    {
        return m_flagId;
    }


    std::string getShortStrAttributed() const
    {
        return getShortStr() + ((m_arg == CMD::Arg::No) ? "" : ":");
    }


    CMD::_getopt_option get_getopt_option() const
    {
        return CMD::_getopt_option {
            m_long.c_str(),          //Long Flag
            static_cast<int>(m_arg), //Turn to getopt argument type .
            nullptr,                 //We don't want store anything...
            m_flagId                 //Unique id for this long flag...
        };
    }

    // iVars //
private:
    CMD::Arg    m_arg;
    std::string m_short;
    std::string m_long;
    std::string m_description;
    int         m_flagId;
};


////////////////////////////////////////////////////////////////////////////////
// Parser                                                                     //
////////////////////////////////////////////////////////////////////////////////
struct Parser
{
    // CTOR //
public:
    Parser(int argc, char **argv) :
        m_argc(argc),
        m_argv(argv),
        m_currentFlagId(0)
    {
        //Empty...
    }


    // Public Methods //
public:
    Parser& addFlag(const Flag &flag)
    {
        const_cast<CMD::Flag &>(flag).setFlagId(m_currentFlagId);
        m_currentFlagId++;

        m_flagsVec.push_back(flag);

        return *this;
    }

    void parse()
    {
        int  opt_index;
        char curr_opt;

        std::vector<CMD::_getopt_option> long_options;
        std::string options_str;

        for(auto &flag : m_flagsVec)
        {
            if(flag.hasShortStr())
                options_str += flag.getShortStrAttributed();
            if(flag.hasLongStr())
                long_options.push_back(flag.get_getopt_option());
        }

        cout << "OptionsString]" << options_str << endl;
        long_options.push_back(CMD::_getopt_option{0, 0, 0, 0});

        curr_opt = getopt_long(m_argc,
                               m_argv,
                               options_str.c_str(),
                               long_options.data(),
                               &opt_index);


        for(auto &flag : m_flagsVec)
        {
            if(flag.hasLongStr() && flag.getFlagId() == curr_opt ||
               flag.hasShortStr() && flag.getShortStr()[0] == curr_opt)
            {
                cout << "Parsed longStr for flag: " << flag.getLongStr();
                continue;
            }

            if()
            {
                cout << "Parsed shortStr for flag: " << flag.getShortStr();
                continue;
            }
        }

        // if(curr_opt == 100)
        //     cout << "adsfasfdsa" << endl;
        // if(c == 1230 || (int)'a') {
        //     printf("option %s", long_options[option_index].name);
        //     if (optarg)
        //         printf(" with arg %s", optarg);
        //     printf("\n");
        // }
    }//void parse


    // iVars //
private:
    std::vector<Flag> m_flagsVec;

    int    m_argc;
    char **m_argv;

    int m_currentFlagId;
};

NS_CMD_END
