//std
#include <deque>
#include <functional>
#include <string>
#include <vector>

#define NS_CMD_BEGIN namespace CMD {
#define NS_CMD_END   }


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
    Flag& NoArgs();
    Flag& RequiredArgs(int count = 1);
    Flag& OptionalArgs(int count = 1);

    //
    Flag& StopOnView();

    //Duplicates
    Flag& DoNotAllowDuplicates();
    Flag& AllowDuplicates(DuplicateMode duplicateMode);

    //Strings
    Flag& Short(const std::string &shortStr);
    Flag& Long(const std::string &longStr);


    // Options "Getters" //
public:
    //Args
    bool getNoArgs() const;

    bool getRequiredArgs() const;
    int  getRequiredArgsCount() const;
    int  getRequiredArgsFoundCount() const;
    const std::vector<std::string>& getRequiredValues() const;

    bool getOptionalArgs() const;
    int  getOptionalArgsCount() const;
    int  getOptionalArgsFoundCount() const;
    const std::vector<std::string>& getOptionalValues() const;

    //
    bool getStopOnView() const;

    //Duplicates
    bool          getAllowDuplicates() const;
    DuplicateMode getAllowDuplicatesMode() const;

    //Strings
    bool hasShortStr() const;
    const std::string& getShortStr() const;

    bool hasLongStr() const;
    const std::string& getLongStr() const;

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
    bool m_requiredArgs;
    int  m_requiredArgsCount;
    std::vector<std::string> m_requiredValuesVec;

    bool m_optionalArgs;
    int  m_optionalArgsCount;
    std::vector<std::string> m_optionalValuesVec;

    bool m_stopOnView;

    bool          m_allowDuplicates;
    DuplicateMode m_duplicateMode;

    std::string m_shortStr;
    std::string m_longStr;

    int m_foundCount;
};


////////////////////////////////////////////////////////////////////////////////
// Parser                                                                     //
////////////////////////////////////////////////////////////////////////////////
struct Parser
{
    // CTOR //
public:
    Parser(int argc, char **argv);
    Parser(const std::vector<std::string> &args);


    // Public Methods //
public:
    Parser& addFlag(Flag *flag);
    void parse();

    std::vector<std::string> getNonFlagArgs();


    // Private //
private:
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
