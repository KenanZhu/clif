/**
    CLIF - Functional CLI(Command Line Interface)-Framework based on C/C++.

    Copyright (C) 2024 - 2025 KenanZhu. Some rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    SPDX-License-Identifier: GPL-3.0-or-later
 */

/**
    \file    : CLIF.hpp
    \brief   : Header file of CLIF, including all declaration of CLIF classes,
               data types and functions.
    \version : 1.0.0
 */

#pragma once

/////////////////// _INCLUDES_
#include <set>
#include <map>
#include <any>
#include <list>
#include <deque>
#include <stack>
#include <ctime>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <charconv>
#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <unordered_set>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#elif __linux__
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#endif

/////////////////// _DEFINES_
#define CLIF_VERSION                   "1.0.0"

#define SPACE                          std::string(" ")

#define COLOR_(X)                      std::string("\033[3" +\
                                       std::to_string(static_cast<int>(X)) +\
                                       "m")
#define COLOR_BLACK                    std::string("\033[30m")
#define COLOR_RED                      std::string("\033[31m")
#define COLOR_GREEN                    std::string("\033[32m")
#define COLOR_YELLOW                   std::string("\033[33m")
#define COLOR_BLUE                     std::string("\033[34m")
#define COLOR_MAGENTA                  std::string("\033[35m")
#define COLOR_CYAN                     std::string("\033[36m")
#define COLOR_WHITE                    std::string("\033[37m")
#define COLOR_DEFAULT                  std::string("\033[39m")

/////////////////// _CLIF_
namespace CLIF {

    ///////////////////////////////////////////////////////////////////////////
    ///// CLIF::OptType
    enum OptType {
        PT_OPTIONAL = 0x00, ///< Optional option type.
        PT_REQUIRED = 0x01, ///< Required option type.
        PT_UNIQUEED = 0x02, ///< Unique option type.
    };

    ///////////////////////////////////////////////////////////////////////////
    ///// CLIF::Color
    enum Color {
        BLACK   = 0, ///< Black color.
        RED     = 1, ///< Red color.
        GREEN   = 2, ///< Green color.
        YELLOW  = 3, ///< Yellow color.
        BLUE    = 4, ///< Blue color.
        MAGENTA = 5, ///< Magenta color.
        CYAN    = 6, ///< Cyan color.
        WHITE   = 7, ///< White color.
        DEFAULT = 9  ///< Default color.
    };
    struct Wrapper;

    class FStr;
    class FLog;
    class FStdo;
    class FStrcov;
    class FOption;
    class FParser;
    class FCaller;
    class FHooker;
    class FApplication;
}


///////////////////////////////////////////////////////////////////////////////
///// CLIF::Wrapper
struct CLIF::Wrapper {
    std::string subcommand;       ///< Subcommand name.
    std::string description;      ///< Subcommand description.
    std::vector<FOption> options; ///< Subcommand options.

    ///v Subcommand trigger function.
    std::function<void(const std::vector<CLIF::FOption> &)> trigger_func;
};


///////////////////////////////////////////////////////////////////////////////
///// CLIF::FStr
class CLIF::FStr {
public:
    FStr(void) = default;
    ~FStr(void) = default;

    static std::string trim(const std::string &str, int mode = 0);
    static std::string toLower(const std::string &str);
    static std::string toUpper(const std::string &str);
    static std::string onlyAlpha(const std::string &str);
    static std::string onlyAlNum(const std::string &str);
    static std::string join(const std::vector<std::string> &strs, const char *sign);

    static std::vector<std::string>
    splitBy(const std::string &str, const char &delimiter);
    static std::vector<std::string>
    splitBy(const std::string &str, const std::string &delimiter);

    static std::vector<std::string>
    splitByBrackets(const std::string &str, const char &delimiter,bool strip = true);
    static std::vector<std::string>
    splitByBrackets(const std::string &str, const std::string &delimiter, bool strip = true);

    static std::pair<std::string, std::string>
    splitKeyValue(const std::string &str, const char &delimiter);
    static std::pair<std::string, std::string>
    splitKeyValue(const std::string &str, const std::string &delimiter);

    static std::vector<std::vector<std::string>>
    splitIfExist(const std::vector<std::string> &vec, const std::set<std::string> &check_set);
    static std::vector<std::vector<std::string>>
    splitIfExist(const std::vector<std::string> &vec, const std::unordered_set<std::string> &check_set);
    static std::vector<std::vector<std::string>>
    splitIfExist(const std::vector<std::string> &vec, const std::vector<std::string> &check_vec);

    static std::string
    formatByCols(const std::string &str1, const std::string &str2, const int columnL, const int columnR);
};


///////////////////////////////////////////////////////////////////////////////
///// CLIF::FLog
class CLIF::FLog {
public:
    FLog(int *argc, char *argv[]);
    ~FLog(void);

    inline static void infoAt(const std::string &where, const std::string &msg)
    { CLIF::FLog::log(where + " : " + msg, 1); }
    inline static void warnAt(const std::string &where, const std::string &msg)
    { CLIF::FLog::log(where + " : " + msg, 2); }
    inline static void errorAt(const std::string &where, const std::string &msg)
    { CLIF::FLog::log(where + " : " + msg, 3); }
    inline static void fatalAt(const std::string &where, const std::string &msg)
    { CLIF::FLog::log(where + " : " + msg, 4); }

    static void log(const std::string &msg, const int level);
private:
    static int  _log_level;            ///< Log level.
    static bool _is_log_file_ready;    ///< Log file ready flag.
    static std::string _log_file_path; ///< Log file path.
    static std::fstream _log_file;     ///< Log file stream.
protected:
    static void parseDefaultPath(const char *argv);
    static void parsePath(int *argc, char *argv[], int &i);
    static void parseLevel(int *argc, char *argv[], int &i);
    static void filterArgs(int *argc, char *argv[],
                           const std::vector<int> &filter_index);

    static void write(const std::string &log_msg);
    static std::string addTimeStamp(void);
};


///////////////////////////////////////////////////////////////////////////////
///// CLIF::FStdo
class CLIF::FStdo {
public:
    FStdo(void) = default;
    ~FStdo(void) = default;

    static void cout(const std::string &str,
                     const bool endline = true,
                     CLIF::Color color = CLIF::Color::DEFAULT);

    static void budMsg(const std::string &str, const int level = 1);
    static void runMsg(const std::string &str, const int level = 1);
};


///////////////////////////////////////////////////////////////////////////////
///// CLIF::FStrcov
class CLIF::FStrcov {

    /// Pair type checkers.
    template<typename T>
    struct is_pair : std::false_type {};

    template<typename T1, typename T2>
    struct is_pair<std::pair<T1, T2>> : std::true_type {};

    template<typename T>
    static constexpr bool is_pair_v = is_pair<T>::value;

    template <typename T>
    struct is_container : std::false_type {};

    /// Sequence container checkers.
    template <typename... Ts>
    struct is_container<std::vector<Ts...>> : std::true_type {};

    template <typename... Ts>
    struct is_container<std::list<Ts...>> : std::true_type {};

    template <typename... Ts>
    struct is_container<std::deque<Ts...>> : std::true_type {};

    template <typename... Ts>
    struct is_container<std::set<Ts...>> : std::true_type {};

    template <typename T>
    static constexpr bool is_container_v = is_container<T>::value;
public:
    FStrcov(void);
    ~FStrcov(void) = default;

    std::any convert(const std::string &str, const std::type_index &type);

    template<typename T>
    void registerNewBasic(std::function<std::any(const std::string &)> func)
    {
        if (_converter_map.count(typeid(T))) {
            CLIF::FLog::
            warnAt("CLIF::FStrcov::registerNewBasic",
                "Basic type converter '"
                + std::string(typeid(T).name())
                + "' is already registered.");

            return;
        }
        _converter_map[std::type_index(typeid(T))] = func;
        CLIF::FLog::
        infoAt("CLIF::FStrcov::registerNewBasic",
            "New basic type converter '"
            + std::string(typeid(T).name())
            + "' successfully registered.");
    }

    template<typename T>
    void registerNewContainer(void)
    {
        if (_converter_map.count(std::type_index(typeid(T)))) {
            CLIF::FLog::
            warnAt("CLIF::FStrcov::registerNewContainer",
                "Container type converter'"
                + std::string(typeid(T).name())
                + "' is already registered.");

            return;
        }
        this->registerContainer<T>();

        if (_converter_map.count(std::type_index(typeid(T)))) {
            CLIF::FLog::
            infoAt("CLIF::FStrcov::registerNewContainer",
                "New container type converter '"
                + std::string(typeid(T).name())
                + "' successfully registered.");
        }
    }
private:
    using convertMap = std::map<
        std::type_index,
        std::function<std::any(const std::string &)>
    >;
    convertMap _converter_map;
private:
    template<typename T>
    void registerContainer(void)
    {
        using VT = typename T::value_type;

        if constexpr (is_pair_v<VT>) {
            this->registerPairContainer<T>();
        } else {
            this->registerSequenceContainer<T>();
        }
    }

    template<typename T>
    void registerPairContainer(void)
    {
        using VT = typename T::value_type;
        using KeyT = typename VT::first_type;
        using ValT = typename VT::second_type;

        if (!_converter_map.count(std::type_index(typeid(KeyT))) ||
            !_converter_map.count(std::type_index(typeid(ValT)))) {
            CLIF::FLog::
            errorAt("CLIF::FStrcov::registerContainer",
                "Type converter registration failed.");

            CLIF::FLog::
            errorAt("CLIF::FStrcov::registerPairContainer",
                "Element type '"
                + std::string(typeid(VT).name())
                + "' requires both key and value converters.");

            return;
        }
        _converter_map
        [std::type_index(typeid(T))] = [this](const std::string &str) -> std::any
        {
            /**
                We define the pair container delimiter as ','. the pair key and
                value are delimited by ':'. actually this is same as the
                general expression of 'Key-Value'
                Examples are:

                    "key1:val1,key2:val2"

                You can also add brackets to make arguments more clear :

                    "[key1:val1],[key2:val2]"

                Different forms of brackets are also accepted:

                    "{key1:val1},{key2:val2}"
                    "(key1:val1),(key2:val2)"
                    ...

                For specifically supported brackets types, please refer the
                implement in 'CLIF::FStr::splitByBrackets'.

                etc.
             */

            T res;
            bool have_bad_convert;
            std::any parsed_key, parsed_val;

            have_bad_convert = false;

            for (const auto &pair_str : CLIF::FStr::splitByBrackets(str, ',')) {
                if (pair_str.empty()) continue;

                auto [key_str, val_str] = CLIF::FStr::splitKeyValue(pair_str, ':');
                if (key_str.empty() || val_str.empty()) {
                    CLIF::FLog::
                    warnAt("CLIF::FStrcov::...",
                        " Invalid map pair format: '"
                        + pair_str
                        + "'.");

                    return std::any();
                }

                parsed_key = _converter_map[std::type_index(typeid(KeyT))](key_str);
                parsed_val = _converter_map[std::type_index(typeid(ValT))](val_str);
                if (!parsed_key.has_value() || !parsed_val.has_value()) {
                    CLIF::FLog::
                    warnAt("CLIF::FStrcov::...",
                        " Fail to convert pair element '"
                        + pair_str
                        + "'.");

                    have_bad_convert = true;
                }
                if (!have_bad_convert) res.emplace(std::any_cast<KeyT>(parsed_key),
                                                   std::any_cast<ValT>(parsed_val));
            }
            return have_bad_convert ? std::any() : std::any(res);
        };
    }

    template<typename T>
    void registerSequenceContainer(void)
    {
        using VT = typename T::value_type;

        if (!_converter_map.count(std::type_index(typeid(VT)))) {
            CLIF::FLog::
            errorAt("CLIF::FStrcov::registerContainer",
                "Type converter registration failed.");

            CLIF::FLog::
            errorAt("CLIF::FStrcov::registerSequenceContainer",
                "Element type '"
                + std::string(typeid(VT).name())
                + "' must be registered first.");

            return;
        }
        _converter_map
        [std::type_index(typeid(T))] = [this](const std::string &str) -> std::any
        {
            /**
                we define the delimiter of element in sequence container as ','.
                Examples are:

                    "99,78,66,07"  ->  num : [99, 78, 66, 07]
                    "Aa,Bb,Cc,Dd"  ->  str : [Aa, Bb, Cc, Dd]
                    ...

                This class provide some basic type converter, such as 'int',
                'float', 'char' .... The container converter specifically
                supportted number of element types, mainly due to the basic
                converters.

                You can also add brackets around arguments to let parser know
                 they are the containers :

                    "1,2,3,4"      ->  num : [1, 2, 3, 4]
                    "[1,2,3],4"    ->  num : [[1, 2, 3], [4]]
                    "[1,2],[3,4]"  ->  num : [[1, 2], [3, 4]]

                Different forms of brackets are also accepted:

                    "[1,2],[3,4]"  ->  num : [[1, 2], [3, 4]]
                    "(1,2),(3,4)"  ->  num : [[1, 2], [3, 4]]
                    ...

                For specifically supported brackets types, please refer the
                implement in 'CLIF::FStr::splitByBrackets'.

                etc.
             */

            T res;
            bool elem_is_container, have_bad_convert;
            char delim_sign;
            std::any parsed_elem;

            delim_sign = ',';
            have_bad_convert = false;
            elem_is_container = is_container_v<VT>;

            for (const auto &elem : CLIF::FStr::splitByBrackets(str, delim_sign)) {
                parsed_elem = _converter_map[std::type_index(typeid(VT))](elem);

                if (!parsed_elem.has_value()) {
                    CLIF::FLog::
                    warnAt("CLIF::FStrcov::...",
                        " Fail to convert element '"
                        + elem
                        + "'.");

                    have_bad_convert = true;
                }
                if (!have_bad_convert)  {
                    res.insert(res.end(), std::any_cast<VT>(parsed_elem));
                }
            }
            return have_bad_convert ? std::any() : std::any(res);
        };
    }
protected:
    virtual void registerBasicConverter(void);
    virtual void registerBasicContainer(void);
};

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FOption
class CLIF::FOption {
public:
    FOption(void);
    FOption(const std::string &long_name,
            const std::string &short_name,
            const std::string &help,
            const std::any &default_value);
    ~FOption(void) = default;

    /// Setter for option's infos and its value.
    CLIF::FOption &longName(const std::string &long_name);
    CLIF::FOption &shortName(const std::string &short_name);
    CLIF::FOption &help(const std::string &help_description);
    CLIF::FOption &defaultValue(const std::any &default_value);
    CLIF::FOption &currentValue(const std::any &current_value);

    template <typename T>
    CLIF::FOption &type(void)
    {
        auto new_type = std::type_index(typeid(T));

        if (new_type == _data_type) {
            /// Directly jump out if the new type is same as current.
        } else if (_default_value.has_value() || _current_value.has_value()) {
            /**
                If already exist value in default or current, this means the
                data type is already decided by the given value.

                So CLIF::FOption will not change data type again.
             */
        } else {
            _data_type = new_type;
        }

        return *this;
    }

    template <typename T>
    CLIF::FOption &validator(std::function<bool(T)> validator)
    {
        /// We must set the same type validator as this option.
        if (std::type_index(typeid(T)) != _data_type) return *this;

        _validator = [validator](const std::any &value)
        {
            try {
                return validator(std::any_cast<T>(value));
            } catch(...) {
                return false;
            }
        };
        return *this;
    }

    /// Setters and getter for option type.
    CLIF::FOption &required(void);
    inline bool isRequired(void) const
    { return (_option_type & CLIF::OptType::PT_REQUIRED); }

    CLIF::FOption &optional(void);
    inline bool isOptional(void) const
    { return !(_option_type & CLIF::OptType::PT_REQUIRED); }

    CLIF::FOption &uniqueed(void);
    inline bool isUniqueed(void) const
    { return (_option_type & CLIF::OptType::PT_UNIQUEED); }

    /// Getters for option info and its value.
    int getNumOfArgs(void) const;
    bool validate(const std::any &value) const;

    inline std::string getLongName(void) const
    { return _long_name; }
    inline std::string getShortName(void) const
    { return _short_name; }
    inline std::string getHelp(void) const
    { return _help; }
    inline std::type_index getType(void) const
    { return _data_type; }
    inline const char *getTypeName(void) const
    { return _data_type.name(); }
    inline std::any getCurrentValue(void) const
    { return _current_value.has_value() ? _current_value : _default_value; }
    inline std::any getDefaultValue(void) const
    { return _default_value; }
private:
    unsigned _option_type; ///< Option type.

    std::type_index _data_type; ///< Option data type.
    std::string _long_name;     ///< Option long name.
    std::string _short_name;    ///< Option short name.
    std::string _help;          ///< Option help.
    std::any _default_value;    ///< Option default value.
    std::any _current_value;    ///< Option current value.

    std::function<bool(const std::any &)> _validator; ///< Validator function.
};

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FParser
class CLIF::FParser {
public:
    FParser(void) = default;
    FParser(const CLIF::FStrcov &str_converter);
    ~FParser(void) = default;

    void parse(int argc, char *argv[],
               CLIF::FCaller &caller,
               std::map<std::string, CLIF::Wrapper> &func_linkers);

    void parse(const std::vector<std::string> &cmdlines,
               CLIF::FCaller &caller,
               std::map<std::string,CLIF::Wrapper> &func_linkers);

    /// .
    inline void strConverter(const CLIF::FStrcov &str_converter)
    { _str_converter = str_converter; }

    /// .
    inline bool isParseOK(void) const
    { return _parse_ok; }
private:
    bool _parse_ok = true; ///< Parse status.

    std::string _active_subcommand; ///< Active subcommand, parsed subcommand.
    CLIF::Wrapper _active_wrapper;  ///< Active subcommand's wrapper.

    ///v Active subcommand's options' vector.
    std::unordered_set<std::string> _active_options;

    std::vector<std::string> _unparsed_cmdline; ///< Unparsed cmdline parts.
    std::vector<std::vector<std::string>> _parsed_options_groups; ///<

    CLIF::FStrcov _str_converter; ///<
private:
    /// Initialize the parser.
    void initialize(void);

    /// Auto generate the help infomation.
    void generateSubCommandHelp(void);
    void generateGlobalHelp(const std::map<std::string, CLIF::Wrapper> &func_linkers);

    /// Preprocess the input command line.
    void tryToNormOptionSyntax(void);
    void tryToAutoMatchOptions(void);

    /// Validate the options.
    bool validateMissingRequired(void);
    bool validateDuplicateUnique(void);
    bool validateArgumentsAmount(void);
    bool validateOptionsSyntax(void);

    /// Parse the subcommand.
    bool parseSubCommand(const std::vector<std::string> &cmdlines,
                         std::map<std::string, CLIF::Wrapper> &func_linkers);

    /// Parse and validate the input options.
    bool parseOption(const std::vector<std::string> &option_group);
    bool parseOptions(const std::vector<std::vector<std::string>> &option_groups);
    bool parseAllOptions(void);
};

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FCaller
class CLIF::FCaller {
public:
    FCaller(void) = default;
    ~FCaller(void) = default;

    /// Call back function with a subcommand and its options.
    void call(const std::string &subcommand,
              const std::map<std::string, CLIF::Wrapper> &func_linkers);
};

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FHooker
class CLIF::FHooker {
public:
    FHooker(void) = default;
    ~FHooker(void) = default;

    /// Hook function with a subcommand.
    template<typename... Args>
    void hook(const std::string &subcommand,
              std::function<void(Args...)> func,
              std::map<std::string, CLIF::Wrapper> &func_linkers)
    {
        auto func_wrapper =
        [func](const std::vector<CLIF::FOption> &options)
        {
            trigger<Args...>(func,
                             options,
                             std::make_index_sequence<sizeof...(Args)>{});
        };
        func_linkers[subcommand].trigger_func = func_wrapper;
    }
private:
    /// Trigger for function call back.
    template<typename... Args, size_t... Is>
    static void trigger(std::function<void(Args...)> func,
                        const std::vector<CLIF::FOption> &options,
                        std::index_sequence<Is...>)
    {
        try {
            func(std::any_cast<Args>(options[Is].getCurrentValue())...);
        } catch (const std::bad_any_cast &e) {
            CLIF::FLog::
            fatalAt("CLIF::FHooker::trigger",
                "call back with fatal error '"
                + std::string(e.what())
                + "'.");
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FApplication
class CLIF::FApplication {
public:
    FApplication(const std::string &program_name);
    ~FApplication(void);

    /// Start the application.
    int start(int argc, char *argv[]);

    /// Add a subcommand.
    template<typename... Args>
    void addSubCommand(const std::string &subcommand,
                       const std::string &description,
                       const std::vector<CLIF::FOption> &options,
                       void (*func)(Args...))
    {
        if (!this->validateAddSubCommand(subcommand,
                                         options,
                                         std::function<void(Args...)>(func)))
        { return; }

        _func_linkers[subcommand].subcommand = subcommand;
        _func_linkers[subcommand].description = description;
        _func_linkers[subcommand].options = options;

        _hooker->hook(subcommand,
                      std::function<void(Args...)>(func),
                      _func_linkers);

        CLIF::FLog::
        infoAt("CLIF::FApplication::addSubCommand",
            "Add subcommand '"
            + subcommand
            + "' with "
            + std::to_string(options.size())
            + " options.");
    }

    /// Add new subcommand to an exist subcommand.
    void addSubCommand(const std::string &exist_subcommand,
                       const std::string &new_subcommand);

    /// Add new basic type converter.
    template<typename T>
    void registerBasicTypeConverter(std::function<std::any(const std::string &)> func)
    {
        _str_converter.registerNewBasic<T>(func);
    }

    /// Add new container converter.
    template<typename T>
    void registerContainerConverter(void)
    {
        _str_converter.registerNewContainer<T>();
    }
private:
    bool _build_ok; ///< CLIF build status.

    CLIF::FParser *_parser; ///< CLIF parser.
    CLIF::FCaller *_caller; ///< CLIF call back function caller.
    CLIF::FHooker *_hooker; ///< CLIF call back function hooker.
    CLIF::FStrcov _str_converter; ///< CLIF string converter.

    std::string _program_name; ///< Program name of CLIF application.

    ///v Subcommand and its call back function linkers.
    std::map<std::string, CLIF::Wrapper> _func_linkers;
private:
    /// Validate subcommand.
    bool validateSubCommand(const std::string &subcommand);

    /// Validate options amount.
    bool validateOptionAmount(const std::string &subcommand,
                              const size_t &options_amount,
                              const size_t &args_amount);

    /// Validate options name.
    bool validateEmptyOptionName(const std::string &subcommand,
                                 const std::vector<CLIF::FOption> &options);
    bool validateDuplicateOptionName(const std::string &subcommand,
                                     const std::vector<CLIF::FOption> &options);

    /// Validate option type.
    bool validateMismatchedOptionType(const std::string &subcommand,
                                      const std::vector<CLIF::FOption> &options,
                                      const std::vector<std::type_index> &exp_types);

    /// Validate empty default value in optional.
    bool validateOptionalDefaultValueEmpty(const std::string &subcommand,
                                           const std::vector<CLIF::FOption> &options);

    /// Validate the options.
    template<typename... Args>
    bool validateAddSubCommand(const std::string &subcommand,
                               const std::vector<CLIF::FOption> &options,
                               std::function<void(Args...)> func)
    {
        bool valid;
        std::vector<std::type_index> exp_types = { typeid(Args)... };

        valid = false;

        if (!this->validateSubCommand(subcommand)) {
        } else if (!this->validateOptionAmount(subcommand, options.size(), sizeof...(Args))) {
        } else if (!this->validateEmptyOptionName(subcommand, options)) {
        } else if (!this->validateDuplicateOptionName(subcommand, options)) {
        } else if (!this->validateMismatchedOptionType(subcommand, options, exp_types)) {
        } else if (!this->validateOptionalDefaultValueEmpty(subcommand, options)) {
        } else {
            valid = true;
        }

        if (!valid) {
            _build_ok = false;
        }
        return valid;
    }
};