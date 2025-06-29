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
    \file    : CLIF.cpp
    \brief   : Implementation of CLIF.
    \version : 1.0.0
 */

#include "CLIF.hpp"

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FStr
std::string CLIF::FStr::
trim(const std::string &str, int mode)
{
    if (str.empty()) return str;

    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    size_t last = str.find_last_not_of(" \t\n\r\f\v");

    if (first == std::string::npos ||
        last == std::string::npos) return std::string("");

    switch (mode) {
        case 1: return str.substr(0, last + 1);
        case 2: return str.substr(first);
        case 0: default: return str.substr(first, last - first + 1);
    }
}

std::string CLIF::FStr::
toLower(const std::string& str)
{
    std::string res;

    for (char c : str) res += std::tolower(c);

    return res;
}

std::string CLIF::FStr::
toUpper(const std::string &str)
{
    std::string res;

    for (char c : str) res += std::toupper(c);

    return res;
}

std::string CLIF::FStr::
onlyAlpha(const std::string &str)
{
    std::string res;

    for (char c : str) if (std::isalpha(c)) res += c;

    return res;
}

std::string CLIF::FStr::
onlyAlNum(const std::string &str)
{
    std::string res;

    for (char c : str) if (std::isalnum(c)) res += c;

    return res;
}

std::string CLIF::FStr::
join(const std::vector<std::string> &strs, const char *sign)
{
    size_t i;
    std::string res;

    for (i = 0; i < strs.size(); i++) {
        res += strs[i];
        if (i + 1 < strs.size()) res += sign;
    }
    return res;
}

std::vector<std::string> CLIF::FStr::
splitBy(const std::string &str, const char &delimiter)
{
    size_t start, end;
    std::string substr;
    std::vector<std::string> split_str;

    start = 0;
    end = str.find(delimiter);

    while (end != std::string::npos) {
        substr = str.substr(start, end - start);
        if (!substr.empty()) split_str.push_back(substr);
        start = end + 1;
        end = str.find(delimiter, start);
    }
    substr = str.substr(start);
    if (!substr.empty()) split_str.push_back(substr);

    return split_str;
}

std::vector<std::string> CLIF::FStr::
splitBy(const std::string &str, const std::string &delimiter)
{
    if ((int)delimiter.size() != 1) return std::vector<std::string> ();

    return CLIF::FStr::splitBy(str, delimiter[0]);
}

std::vector<std::string> CLIF::FStr::
splitByBrackets(const std::string &str,
                const char &delimiter,
                bool strip)
{
    /**
        \note :
        Logically speaking, this can support any set of 'bracket' defined as
        pairs to split the original string according to the given delimiter.

        However, in reality, the extent to which various bracket types are
        fully supported largely depends on the behavior of the terminal or
        shell towards brackets(in the conventional command-line usage scenarios).

        In our tests, square brackets '[' and ']' can better meet the
        requirements for parsing across different platforms.
        These situations are not fully explained in 'CLIF::FStrcov'.
     */

    size_t i, segment_start;
    char c;
    bool escape_mode;
    std::stack<char> bracket_stack;
    std::vector<std::vector<char>> bracket_pairs;
    std::vector<std::string> split_str;

    segment_start = 0;
    escape_mode = false;

    /// 0 : open, 1 : close.
    bracket_pairs = {
        {'<', '>'},
        {'(', ')'},
        {'[', ']'},
        {'{', '}'}
    };

    for (i = 0; i < str.size(); ++i) {
        c = str[i];

        if (escape_mode) {
            escape_mode = false;
            continue;
        }
        if (c == '\\') {
            escape_mode = true;
            continue;
        }

        if (!escape_mode) {
            for (auto &bracket_pair : bracket_pairs) {
                if (c == bracket_pair[0]) {
                    bracket_stack.push(bracket_pair[1]);
                    break;
                } else if (c == bracket_pair[1]) {
                    if (!bracket_stack.empty() && c == bracket_stack.top()) {
                        bracket_stack.pop();
                    }
                    break;
                }
            }

            if (c == delimiter && bracket_stack.empty()) {
                auto segment = str.substr(segment_start, i - segment_start);

                split_str.push_back(segment);
                segment_start = i + 1;
            }
        }
    }
    split_str.push_back(str.substr(segment_start));

    /// Enable strip brackets.
    if (strip) {
        for (auto &segment : split_str) {
            if (segment.empty()) continue;
            for (auto &bracket_pair : bracket_pairs) {
                if (segment.front() == bracket_pair[0] &&
                    segment.back() == bracket_pair[1])
                {
                    segment = segment.substr(1, segment.size() - 2);
                    break;
                }
            }
        }
    }
    return split_str;
}

std::vector<std::string> CLIF::FStr::
splitByBrackets(const std::string &str,
                const std::string &delimiter,
                bool strip)
{
    if ((int)delimiter.size() != 1) {
        return std::vector<std::string> ();
    }
    return CLIF::FStr::splitByBrackets(str, delimiter[0], strip);
}

std::pair<std::string, std::string> CLIF::FStr::
splitKeyValue(const std::string &str, const char &delimiter)
{
    size_t pos;
    std::pair<std::string, std::string> split_str;

    pos = str.find(delimiter);

    if (pos != std::string::npos) {
        split_str.first = CLIF::FStr::trim(str.substr(0, pos));
        split_str.second = CLIF::FStr::trim(str.substr(pos + 1));
    } else {
        split_str.first = CLIF::FStr::trim(str);
        split_str.second = "";
    }
    return split_str;
}

std::pair<std::string, std::string> CLIF::FStr::
splitKeyValue(const std::string &str, const std::string &delimiter)
{
    if ((int)delimiter.size() != 1) {
        return std::pair<std::string, std::string>();
    }
    return CLIF::FStr::splitKeyValue(str, delimiter[0]);
}

std::vector<std::vector<std::string>> CLIF::FStr::
splitIfExist(const std::vector<std::string> &vec,
             const std::set<std::string> &check_set)
{
    bool active_group;
    std::vector<std::string> current_group;
    std::vector<std::vector<std::string>> split_groups;

    active_group = false;

    for (const auto &s : vec) {
        if (check_set.count(s)) {
            if (active_group) {
                split_groups.push_back(current_group);
                current_group.clear();
            }
            current_group.push_back(s);
            active_group = true;
        } else if (active_group) {
            current_group.push_back(s);
        }
    }
    if (active_group) split_groups.push_back(current_group);

    return split_groups;
}

std::vector<std::vector<std::string>> CLIF::FStr::
splitIfExist(const std::vector<std::string> &vec,
             const std::unordered_set<std::string> &check_set)
{
    std::set<std::string> check_set_;

    for (const auto &set : check_set) check_set_.insert(set);

    return CLIF::FStr::splitIfExist(vec, check_set_);
}

std::vector<std::vector<std::string>> CLIF::FStr::
splitIfExist(const std::vector<std::string> &vec,
             const std::vector<std::string> &check_vec)
{
    std::unordered_set<std::string> check_set(check_vec.begin(), check_vec.end());

    return CLIF::FStr::splitIfExist(vec, check_set);
}

std::string CLIF::FStr::
formatByCols(const std::string &strL,
             const std::string &strR,
             const int widthL, const int widthR)
{
    size_t target, last, next;
    std::string temp_strL, temp_strR, res;
    std::deque<std::string> strL_lines, strR_lines;

    temp_strL = strL;
    temp_strR = strR;

    while ((int)temp_strL.size() > widthL + (int)widthR/10) {
        next = temp_strL.find(' ',widthL);
        last = temp_strL.rfind(' ',widthL);
        target = std::min<size_t>(next, last);
        target = target == std::string::npos ? widthL : target;
        strL_lines.push_back(temp_strL.substr(0, target) + '\n');
        temp_strL.erase(0, target + 1);
    }
    (int)temp_strL.size() <= widthL ? temp_strL.resize(widthL, ' ') :
                                      temp_strL.resize(widthL + widthR, ' ');
    strL_lines.push_back(temp_strL);

    while ((int)temp_strR.size() > widthR) {
        next = temp_strR.find(' ',widthR);
        last = temp_strR.rfind(' ',widthR);
        target = std::min<size_t>(next, last);
        target = target == std::string::npos ? widthL : target;
        strR_lines.push_back(temp_strR.substr(0, target) + '\n');
        temp_strR.erase(0, target + 1);
    }
    strR_lines.push_back(temp_strR);

    while ((int)strL_lines.size() != 1) {
        res += SPACE + strL_lines[0];
        strL_lines.pop_front();
    }
    if ((int)strL_lines[0].size() > widthL) {
        res += SPACE + strL_lines[0] + "\n";
    } else {
        res += SPACE + strL_lines[0] + SPACE + strR_lines[0];
        strR_lines.pop_front();
    }
    strL_lines.pop_front();

    while ((int)strR_lines.size()) {
        res += SPACE + strR_lines[0].insert(0, widthL + 1, ' ');
        strR_lines.pop_front();
    }
    return res;
}

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FLog
int CLIF::FLog::_log_level = 0;
bool CLIF::FLog::_is_log_file_ready = false;
std::string CLIF::FLog::_log_file_path;
std::fstream CLIF::FLog::_log_file;

CLIF::FLog::
FLog(int *argc, char *argv[])
{
    int i;
    std::vector<int> filter_index;

    for (i = 1; i < *argc; i++) {
        if (std::string(argv[i]) == "--log-file") {
            filter_index.push_back(i);
            CLIF::FLog::parsePath(argc, argv, i);
            if (filter_index.back() < i) filter_index.push_back(i);
        } else if (std::string(argv[i]) == "--log-level") {
            filter_index.push_back(i);
            CLIF::FLog::parseLevel(argc, argv, i);
            if (filter_index.back() < i) filter_index.push_back(i);
        }
    }
    CLIF::FLog::filterArgs(argc, argv, filter_index);
}
CLIF::FLog::
~FLog(void)
{
    if (_log_file.is_open()) _log_file.close();
}

void CLIF::FLog::
log(const std::string &msg, const int level)
{
    std::string level_sign, log_msg;

    if (level < _log_level) return;

    switch (level) {
        case 1: level_sign = "[INFO ]"; break;
        case 2: level_sign = "[WARN ]"; break;
        case 3: level_sign = "[ERROR]"; break;
        case 4: level_sign = "[FATAL]"; break;
        default:level_sign = "[OTHER]"; break;
    }
    log_msg = CLIF::FLog::addTimeStamp() + level_sign;
    log_msg = log_msg + " : " + msg;

    if (_is_log_file_ready) CLIF::FLog::write(log_msg);
}

void CLIF::FLog::
parseDefaultPath(const char *argv)
{
    std::filesystem::path exe_path(argv);

    _log_file_path = (exe_path.parent_path() / exe_path.stem()).string() + ".log";
    _log_file.open(_log_file_path, std::ios::app);
    _is_log_file_ready = _log_file.is_open();
}

void CLIF::FLog::
parsePath(int *argc, char *argv[], int &i)
{
    if(i + 1 >= *argc) {
        CLIF::FLog::parseDefaultPath(argv[0]);
        return;
    }

    _log_file_path = argv[++i];
    _log_file.open(_log_file_path, std::ios::app);

    if(!_log_file.is_open()) {
        CLIF::FLog::parseDefaultPath(argv[0]);
        i--;
    } else {
        _is_log_file_ready = true;
    }
}

void CLIF::FLog::
parseLevel(int *argc, char *argv[], int &i)
{
    if(i + 1 >= *argc) return;

    try {
        _log_level = std::stoi(argv[++i]);
        _log_level = std::max<int>(0, std::min<int>(_log_level, 4));
    } catch(...) { i--; }
}

void CLIF::FLog::
filterArgs(int *argc, char *argv[],
           const std::vector<int> &filter_index)
{
    int i, new_argc;
    std::unordered_set<int> filter_set(filter_index.begin(), filter_index.end());

    new_argc = 0;

    for (i = 0; i < *argc; i++) {
        if (filter_set.count(i)) continue;

        argv[new_argc] = argv[i];
        new_argc++;
    }
    *argc = new_argc;
    if (new_argc < *argc) argv[new_argc] = nullptr;
}

void CLIF::FLog::
write(const std::string &log_msg)
{
    if (_log_file && _log_file.is_open()) {
        _log_file << log_msg << std::endl;
        _log_file.flush();
    }
}

std::string CLIF::FLog::
addTimeStamp(void)
{
    char buffer[20];
    std::string time_stamp;
    std::time_t now = std::time(nullptr);
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now);
#elif __linux__
    localtime_r(&now, &localTime);
#endif
    std::strftime(buffer, sizeof(buffer), "[%y/%m/%d %H:%M:%S]", &localTime);
    time_stamp = buffer;

    return time_stamp;
}

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FStdo
void CLIF::FStdo::
cout(const std::string &str,
     const bool endline,
     CLIF::Color color)
{
    if (endline) std::cout << COLOR_(color)
                           << str
                           << COLOR_(CLIF::Color::DEFAULT)
                           << std::endl << std::flush;
    if (!endline) std::cout << COLOR_(color)
                            << str
                            << COLOR_(CLIF::Color::DEFAULT) << std::flush;
}

void CLIF::FStdo::
budMsg(const std::string &str, const int level)
{
    CLIF::FStdo::cout(" ** " + str);
    CLIF::FLog::log("build -- " + str, level);
}

void CLIF::FStdo::
runMsg(const std::string &str, const int level)
{
    CLIF::FStdo::cout(" -- " + str);
    CLIF::FLog::log("runtime -- " + str, level);
}

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FStrcov
CLIF::FStrcov::
FStrcov(void)
{
    /// Register basic and basic container type converter;
    this->registerBasicConverter();
    this->registerBasicContainer();
}

std::any CLIF::FStrcov::
convert(const std::string &str, const std::type_index &type)
{
    auto it = _converter_map.find(type);
    if (it != _converter_map.end()) {
        return _converter_map[type](str);
    } else {
        CLIF::FLog::
        warnAt("CLIF::FStrcov::convert",
            "Converter received unsupport data type '"
            + std::string(type.name())
            + "'.");
    }
    return std::any();
}

void CLIF::FStrcov::
registerBasicConverter(void)
{
    /// 'int' converter.
    _converter_map
    [std::type_index(typeid(int))] = [](const std::string &s)
    {
        int value;

        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
        if (ec != std::errc() || ptr != s.data() + s.size()) {
            CLIF::FLog::
            warnAt("CLIF::FStrcov::...",
                "invalid int, '" + s + "'.");

            return std::any();
        }
        return std::any(value);
    };

    /// 'float' converter.
    _converter_map
    [std::type_index(typeid(float))] = [](const std::string &s)
    {
        float value;

        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
        if (ec != std::errc() || ptr != s.data() + s.size()) {
            CLIF::FLog::
            warnAt("CLIF::FStrcov::...",
                "invalid float, '" + s + "'.");

            return std::any();
        }
        return std::any(value);
    };

    /// 'double' converter.
    _converter_map
    [std::type_index(typeid(double))] = [](const std::string &s)
    {
        double value;

        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
        if (ec != std::errc() || ptr != s.data() + s.size()) {
            CLIF::FLog::
            warnAt("CLIF::FStrcov::...",
                "invalid double, '" + s + "'.");

            return std::any();
        }
        return std::any(value);
    };

    /// 'bool' converter.
    _converter_map
    [std::type_index(typeid(bool))] = [](const std::string &s)
    {
        bool value;

        if (s == "true" || s == "1") value = true;
        else if (s == "false" || s == "0") value = false;
        else {
            CLIF::FLog::
            warnAt("CLIF::FStrcov::...",
                "invalid bool, '" + s + "'.");

            return std::any();
        }
        return std::any(value);
    };

    /// 'char' converter.
    _converter_map
    [std::type_index(typeid(char))] = [](const std::string &s)
    {
        if (s.size()!= 1) {
            CLIF::FLog::
            warnAt("CLIF::FStrcov::...",
                "invalid char,'" + s + "'.");

            return std::any();
        }
        return std::any(s[0]);
    };

    /// 'const char*' converter.
    _converter_map
    [std::type_index(typeid(const char*))] = [](const std::string &s)
    {
        return std::any(s.data());
    };

    /// 'string' converter.
    _converter_map
    [std::type_index(typeid(std::string))] = [](const std::string &s)
    {
        return std::any(s);
    };

    /// Add what you need...
}

void CLIF::FStrcov::
registerBasicContainer(void)
{
    /// 'vector<...>' converter.
    this->registerContainer<std::vector<int>>();
    this->registerContainer<std::vector<float>>();
    this->registerContainer<std::vector<double>>();
    this->registerContainer<std::vector<bool>>();
    this->registerContainer<std::vector<char>>();
    this->registerContainer<std::vector<const char*>>();
    this->registerContainer<std::vector<std::string>>();

    /// 'deque<...>' converter.
    this->registerContainer<std::deque<int>>();
    this->registerContainer<std::deque<float>>();
    this->registerContainer<std::deque<double>>();
    this->registerContainer<std::deque<bool>>();
    this->registerContainer<std::deque<char>>();
    this->registerContainer<std::deque<const char*>>();
    this->registerContainer<std::deque<std::string>>();

    /// 'list<...>' converter.
    this->registerContainer<std::list<int>>();
    this->registerContainer<std::list<float>>();
    this->registerContainer<std::list<double>>();
    this->registerContainer<std::list<bool>>();
    this->registerContainer<std::list<char>>();
    this->registerContainer<std::list<const char*>>();
    this->registerContainer<std::list<std::string>>();

    /// 'set<...>' converter.
    this->registerContainer<std::set<int>>();
    this->registerContainer<std::set<float>>();
    this->registerContainer<std::set<double>>();
    this->registerContainer<std::set<bool>>();
    this->registerContainer<std::set<char>>();
    this->registerContainer<std::set<const char*>>();
    this->registerContainer<std::set<std::string>>();

    /// Add what you need....
}

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FOption
CLIF::FOption::
FOption(void)
    :_data_type(std::type_index(typeid(void)))
{
    _option_type = 0;
}
CLIF::FOption::
FOption(const std::string &long_name,
        const std::string &short_name,
        const std::string &description,
        const std::any &default_value)
    :_data_type(default_value.type())
{
    _option_type = 0;
    this->longName(long_name).shortName(short_name).help(description);

    if (default_value.has_value() && _data_type != std::type_index(typeid(void))) {
        _default_value = default_value;
        _current_value = _default_value;
    }
}

/**
    \note :
    This function will filter the given long name, and only keep the
    alphanumeric characters, '-', and '_'.
 */
CLIF::FOption &CLIF::FOption::
longName(const std::string &long_name)
{
    std::string t_name;

    for (char c : long_name) {
        if (std::isalnum(c) || c == '-' || c == '_') {
            t_name += c;
        }
    }

    if (CLIF::FStr::onlyAlNum(t_name).size() >= 1) {
        if (t_name.find("--") == 0) {
            _long_name = t_name;
        } else {
            _long_name = "--" + t_name;
        }
    }
    return *this;
}

/**
   \note :
   This function will find the first valid character, and then add "-"
   to the front.

   If can not find the valid character, this function will do nothing.
 */
CLIF::FOption &CLIF::FOption::
shortName(const std::string &short_name)
{
    auto it = CLIF::FStr::onlyAlNum(short_name);
    if (it.size() >= 1) {
        _short_name = "-" + std::string(1, it[0]);
    }
    return *this;
}

CLIF::FOption &CLIF::FOption::
help(const std::string &help_description)
{
    _help = help_description;

    return *this;
}

CLIF::FOption &CLIF::FOption::
defaultValue(const std::any &default_value)
{
    if (!default_value.has_value()) return *this;

    if (!this->validate(default_value)) return *this;

    /// Void type refer the default value is empty.
    if (_data_type == std::type_index(typeid(void))) {
        _data_type = default_value.type();
        _default_value = default_value;
        _current_value = _default_value;
    } else if (std::type_index(default_value.type()) == _data_type) {
        _default_value = default_value;
    }
    return *this;
}

CLIF::FOption &CLIF::FOption::
currentValue(const std::any &current_value)
{
    if (!current_value.has_value()) return *this;

    if (!this->validate(current_value)) return *this;

    /// Only default value is empty, this brach can be accessible.
    if (_data_type == std::type_index(typeid(void))) {
        _data_type = current_value.type();
        _current_value = current_value;
        _default_value = _current_value;
    } else if (std::type_index(current_value.type()) == _data_type) {
        _current_value = current_value;
    }
    return *this;
}

CLIF::FOption &CLIF::FOption::
required(void)
{
    if (!this->isRequired()) {
        _option_type |= CLIF::OptType::PT_REQUIRED;
    }
    return *this;
}

CLIF::FOption &CLIF::FOption::
optional(void)
{
    if (!this->isOptional()) {
        _option_type &= ~CLIF::OptType::PT_REQUIRED;
    }
    return *this;
}

CLIF::FOption &CLIF::FOption::
uniqueed(void)
{
    if (!this->isUniqueed()) {
        _option_type |= CLIF::OptType::PT_UNIQUEED;
    }
    return *this;
}

int CLIF::FOption::
getNumOfArgs(void) const
{
    if (_data_type == typeid(bool)) return 0;
    if (_data_type == typeid(void)) return 0;
    return 1;
}

bool CLIF::FOption::
validate(const std::any &value) const
{
    if (!_validator) return true;

    return _validator(value);
}

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FParser
CLIF::FParser::
FParser(const CLIF::FStrcov &str_converter)
{
    _str_converter = str_converter;
    _parse_ok = true;
}

void CLIF::FParser::
parse(int argc, char* argv[],
      CLIF::FCaller &caller,
      std::map<std::string, CLIF::Wrapper> &func_linkers)
{
    int i;
    std::vector<std::string> cmdlines;

    if (argc < 2) return;

    for (i = 1; i < argc; i++) {
        cmdlines.push_back(std::string(argv[i]));
    }
    CLIF::FLog::
    infoAt("CLIF::FParser::parse()",
        "Receive command to parse : '"
        + CLIF::FStr::join(cmdlines, " ")
        + "'.");

    this->parse(cmdlines, caller, func_linkers);

    CLIF::FLog::
    infoAt("CLIF::FParser::parse()",
        "Command '"
        + CLIF::FStr::join(cmdlines, " ")
        + "' parse over.");
}

void CLIF::FParser::
parse(const std::vector<std::string> &cmdlines,
      CLIF::FCaller &caller,
      std::map<std::string, CLIF::Wrapper> &func_linkers)
{
    this->initialize();
    if (cmdlines.empty()) return;

    if (!this->parseSubCommand(cmdlines, func_linkers)) return;
    if (!this->parseAllOptions()) return;

    caller.call(_active_subcommand, {{_active_subcommand, _active_wrapper}});
}

void CLIF::FParser::
initialize(void)
{
    _parse_ok = true;

    _active_subcommand.clear();
    _active_options.clear();
    _active_wrapper = CLIF::Wrapper();
    _unparsed_cmdline.clear();
    _parsed_options_groups.clear();
}

void CLIF::FParser::
generateSubCommandHelp(void)
{
    std::string t_str;

    CLIF::FStdo::
    cout("Usage : ... " + _active_subcommand + " [...options] [...arguments]\n");
    CLIF::FStdo::
    cout("Options :");

    for (const auto &option : _active_wrapper.options) {
        t_str.clear();

        if (option.isRequired()) t_str += "M";
        else t_str += "O";
        if (option.isUniqueed()) t_str += "U";
        else t_str += SPACE;

        t_str += SPACE + option.getLongName() + ", " +option.getShortName();
        CLIF::FStdo::
        cout(CLIF::FStr::formatByCols(t_str, option.getHelp(), 20, 60));
    }
}

void CLIF::FParser::
generateGlobalHelp(const std::map<std::string, CLIF::Wrapper> &func_linkers)
{
    std::map<std::string,
             std::pair<std::vector<std::string>,
                       std::string>> subcommand_groups;

    CLIF::FStdo::
    cout("Usage : <program> [subcommand] [...options] [...arguments]\n");
    for (const auto &[subcommand, wrapper] : func_linkers) {
        auto &group = subcommand_groups[wrapper.subcommand];

        group.first.push_back(subcommand);
        group.second += wrapper.description;
    }

    CLIF::FStdo::
    cout("Subcommands :");
    for (const auto &[subcommand, group] : subcommand_groups) {
        auto it = CLIF::FStr::join(group.first, ", ");

        CLIF::FStdo::
        cout(CLIF::FStr::formatByCols(it, group.second, 20, 70));
    }
}

void CLIF::FParser::
tryToNormOptionSyntax(void)
{
    size_t i;
    std::vector<std::string> t_rest_cmdline;

    for (auto &section : _unparsed_cmdline) {
        if (section.find('=') != std::string::npos) {
            auto it = CLIF::FStr::splitBy(section, '=');
            t_rest_cmdline.insert(t_rest_cmdline.end(), it.begin(), it.end());
        } else {
            t_rest_cmdline.push_back(section);
        }
    }
    _unparsed_cmdline = std::move(t_rest_cmdline);
}

void CLIF::FParser::
tryToAutoMatchOptions(void)
{
    /**
        Only all options only own one argument, this will
        regard each input is sort by given options vector.

        But, we have plan to support positional arguments in the future.
     */
    size_t i, idx, opts_amount;
    std::vector<std::string> auto_matched, long_opts_name;

    i = 0;
    opts_amount = 0;

    for (const auto &option : _active_wrapper.options) {
        if (option.getNumOfArgs() != 1) return;

        opts_amount += option.getNumOfArgs();
        long_opts_name.push_back(option.getLongName());
    }

    for (const auto &section : _unparsed_cmdline) {
        if (!_active_options.count(section)) i++;
    }

    if (i == opts_amount && _unparsed_cmdline.size() == opts_amount) {
        auto_matched.reserve(long_opts_name.size() + _unparsed_cmdline.size());

        for (idx = 0; idx < long_opts_name.size(); ++idx) {
            auto_matched.push_back(long_opts_name[idx]);
            auto_matched.push_back(_unparsed_cmdline[idx]);
        }
        _unparsed_cmdline = std::move(auto_matched);
    }
}

bool CLIF::FParser::
validateMissingRequired(void)
{
    int exp_option_amount, act_option_amount;
    bool valid;
    std::string long_option_name, short_option_name;
    std::unordered_set<std::string> parsed_options;

    valid = true;
    exp_option_amount = act_option_amount = 0;

    for (const auto &group : _parsed_options_groups) {
        parsed_options.insert(group[0]);
    }

    for (const auto &option : _active_wrapper.options) {
        if (!option.isRequired()) continue;

        long_option_name = option.getLongName();
        short_option_name = option.getShortName();
        exp_option_amount++;

        if (!parsed_options.count(long_option_name) && !parsed_options.count(short_option_name)) {
            CLIF::FStdo::
            runMsg("required [options] '"
                + long_option_name + "/" + short_option_name
                + "' is not found.", 2);

            continue;
        }
        act_option_amount++;
    }
    if (act_option_amount < exp_option_amount) {
        CLIF::FStdo::
        runMsg("lost "
            + std::to_string(exp_option_amount-act_option_amount)
            + " required [options].", 2);

        valid = false;
    }
    return valid;
}

bool CLIF::FParser::
validateDuplicateUnique(void)
{
    int repeat_times;
    bool valid;
    std::string long_option_name, short_option_name;
    std::unordered_multiset<std::string> input_options;

    valid = true;
    repeat_times = 0;

    for (const auto &group : _parsed_options_groups) {
        input_options.insert(group[0]);
    }

    for (const auto &option : _active_wrapper.options) {
        if (!option.isUniqueed()) continue;

        long_option_name = option.getLongName();
        short_option_name = option.getShortName();
        repeat_times = (int)(input_options.count(long_option_name)
                     + (int)input_options.count(short_option_name));

        if (repeat_times > 1) {
            CLIF::FStdo::
            runMsg("got total "
                + std::to_string(repeat_times)
                + " [options] '" + long_option_name + "/" + short_option_name
                + "' in command, but it is unique.", 2);

            valid = false;
        }
    }
    return valid;
}

bool CLIF::FParser::
validateArgumentsAmount(void)
{
    size_t exp_argument_amount, act_argument_amount;
    std::string option_name;
    bool valid;

    valid = true;

    for (const auto &group : _parsed_options_groups) {
        option_name = group[0];
        auto it = std::find_if(_active_wrapper.options.begin(),
                               _active_wrapper.options.end(),
        [&](const CLIF::FOption &option)
        {
            return (option.getLongName() == option_name ||
                    option.getShortName() == option_name);
        });
        if (it == _active_wrapper.options.end()) continue;

        exp_argument_amount = it->getNumOfArgs();
        act_argument_amount = (int)group.size() - 1;

        if (it->getType() == std::type_index(typeid(bool))) {
            if (act_argument_amount > 1) {
                CLIF::FStdo::
                runMsg("[options] '"
                    + option_name
                    + "' expect at most 1 argument, but got "
                    + std::to_string(act_argument_amount)
                    + ".", 2);

                valid = false;
            }
        } else {
            if (act_argument_amount != exp_argument_amount) {
                CLIF::FStdo::
                runMsg("[options] '"
                    + option_name
                    + "' expect " + std::to_string(exp_argument_amount)
                    + " argument, but got " + std::to_string(act_argument_amount)
                    + ".", 2);

                valid = false;
            }
        }
    }
    return valid;
}

bool CLIF::FParser::
validateOptionsSyntax(void)
{
    bool valid;

    valid = false;

    for (const auto &option : _active_wrapper.options) {
        _active_options.insert(option.getLongName());
        _active_options.insert(option.getShortName());
    }
    this->tryToNormOptionSyntax();
    this->tryToAutoMatchOptions();

    _parsed_options_groups = CLIF::FStr::
                      splitIfExist(_unparsed_cmdline, _active_options);

    if (!this->validateMissingRequired()) {
    } else if (!this->validateDuplicateUnique()) {
    } else if (!this->validateArgumentsAmount()) {
    } else {
        valid = true;
    }
    return valid;
}

bool CLIF::FParser::
parseSubCommand(const std::vector<std::string> &cmdlines,
                std::map<std::string, CLIF::Wrapper> &func_linkers)
{
    bool valid;
    std::string first_section;
    std::unordered_set<std::string> HELP_TRIGGERS;

    valid = false;
    HELP_TRIGGERS = {"?", "h", "-h", "help", "--help"};
    _unparsed_cmdline = cmdlines;

    if (_unparsed_cmdline.empty()) {
        CLIF::FStdo::
        runMsg("invalid command : no input.", 2);

        _parse_ok = false;
    }
    first_section = _unparsed_cmdline[0];

    if (HELP_TRIGGERS.count(first_section)) {
        this->generateGlobalHelp(func_linkers);

        CLIF::FLog::
        infoAt("CLIF::FParser::parseSubCommand()",
            "Global help trigger '"
            + first_section
            + "' parsed.");
    } else if (!func_linkers.count(first_section)) {
        CLIF::FStdo::
        runMsg("invalid command : unknown [subcommand] '"
            + first_section
            + "'.", 2);

        _parse_ok = false;
    } else {
        _active_subcommand = first_section;
        _unparsed_cmdline.erase(_unparsed_cmdline.begin());
        _active_wrapper =
        func_linkers.at(func_linkers.at(_active_subcommand).subcommand);

        CLIF::FLog::
        infoAt("CLIF::FParser::parseSubCommand()",
            "Subcommand '"
            + first_section
            + "' parsed, target at '"
            + func_linkers.at(first_section).subcommand
            + "'.");

        valid = true;
    }
    return valid;
}

bool CLIF::FParser::
parseOption(const std::vector<std::string> &option_group)
{
    bool valid;
    std::any value;
    std::string option_name, param_value;

    valid = true;
    option_name = option_group[0];

    /// We use the any one of option's name to get the FOption instance.
    auto it = std::find_if(_active_wrapper.options.begin(),
                           _active_wrapper.options.end(),
    [&](const CLIF::FOption &option)
    {
        return (option.getLongName() == option_name ||
                option.getShortName() == option_name);
    });
    if (it == _active_wrapper.options.end()) return valid = false;

    /**
        Here, we consider the bool option type. It accept a bool argument or
        no argument, only as a bool flag, so we can accept both conditions.
     */
    if (it->getType() == std::type_index(typeid(bool)) && option_group.size() == 1) {
        value = std::any(true);
        param_value = "true";
    } else if (option_group.size() > 1) {
        value = _str_converter.convert((param_value = option_group[1]), it->getType());
    }

    if (!value.has_value()) {
        CLIF::FStdo::
        runMsg("Invalid argument, can't parse '"
            + param_value
            + "' as '"
            + it->getTypeName()
            + "'.", 3);

        valid = false;
    } else {
        /// Here, we consider the validator of option.
        if (!it->validate(value)) {
            CLIF::FStdo::
            runMsg("Invalid argument, '"
                + param_value
                + "' is reject by validator in option '"
                + option_name
                + "'.", 3);

            valid = false;
        } else {
            it->currentValue(value);
        }
    }
    CLIF::FLog::
    infoAt("CLIF::FParser::parseOption()",
        "Option '"
        + option_name
        + "' parsed with given value '"
        + param_value
        + "'.");

    return valid;
}

bool CLIF::FParser::
parseOptions(const std::vector<std::vector<std::string>> &option_groups)
{
    int i, fail, total;
    bool valid;

    i = fail = 0;
    total = (int)option_groups.size();
    valid = false;

    for (const auto &group : option_groups) {
        if (!this->parseOption(group)) {
            CLIF::FStdo::
            runMsg("Fail to parse option '"
                + group[0]
                + "', at position " + std::to_string(i + 1)
                + ".", 3);

            fail++;
        }
        i++;
    }
    if (fail == 0) {
        valid = true;
    }
    CLIF::FLog::
    infoAt("CLIF::FParser::parseOptions()",
        "All "
        + std::to_string(total)
        + " options parsed, "
        + std::to_string(fail)
        + " failed.");

    return valid;
}

bool CLIF::FParser::
parseAllOptions(void)
{
    bool valid;
    std::unordered_set<std::string> HELP_TRIGGERS;

    valid = false;
    HELP_TRIGGERS = {"?", "h", "-h", "help", "--help"};

    /**
        We accept the empty options of input when 2 conditions:

        1. All the options in the subcommand are optional.
        2. The subcommand receive none arguments.
     */

    /// Get the amount of required options in the subcommand.
    auto requiredAmount = [](const std::vector<CLIF::FOption> &options){
        int amount = 0;
        for (const auto &option : options) if (option.isRequired()) amount++;
        return amount;
    };

    if (_unparsed_cmdline.empty()) {
        if (requiredAmount(_active_wrapper.options) == 0) {
            /**
                This consider the none options or optional arguments
                in subcommand.

                We accept the empty command line, and return true,
                this will directly use the default value of options.
             */
            valid = true;
        }
        else {
            CLIF::FStdo::
            runMsg("invalid command : no options in subcommand '" + _active_subcommand + "'.", 2);

            CLIF::FStdo::
            runMsg("use '? /h /-h /help /--help' to show help message.");

            CLIF::FLog::
            infoAt("CLIF::FParser::parseAllOptions()",
                "No options in subcommand '"
                + _active_subcommand
                + "', parsed.");

            _parse_ok = false;
        }
    } else if (HELP_TRIGGERS.count(_unparsed_cmdline[0])) {
        this->generateSubCommandHelp();

        CLIF::FLog::
        infoAt("CLIF::FParser::parseAllOptions()",
            "Help trigger '"
            + _unparsed_cmdline[0]
            + "' for subcommand '"
            + _active_subcommand
            + "' parsed.");
    } else if (!this->validateOptionsSyntax()) {
        /**
            Here, we verify the validity of the input command and return false
            when the verification is false.
         */
        _parse_ok = false;
    } else {
        _parse_ok = (valid = this->parseOptions(_parsed_options_groups));
    }
    return valid;
}

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FCaller
void CLIF::FCaller::
call(const std::string &subcommand,
     const std::map<std::string, CLIF::Wrapper> &func_linkers)
{
    auto it = func_linkers.find(subcommand);
    if (it != func_linkers.end()) {
        it->second.trigger_func(it->second.options);
    } else {
        CLIF::FLog::
        warnAt("CLIF::FCaller::call",
            "call back with unknown subcommand '"
            + subcommand
            + "'.");
    }
}

///////////////////////////////////////////////////////////////////////////////
///// CLIF::FApplication
CLIF::FApplication::
FApplication(const std::string &program_name)
{
    _build_ok = true;

    _parser = new CLIF::FParser();
    _caller = new CLIF::FCaller();
    _hooker = new CLIF::FHooker();

    _program_name = program_name;
}
CLIF::FApplication::
~FApplication(void)
{
    delete _parser;
    delete _caller;
    delete _hooker;

    _program_name.clear();
    _func_linkers.clear();
}

int CLIF::FApplication::
start(int argc, char *argv[])
{
    if (!_build_ok) {
        CLIF::FStdo::
        budMsg("can't start application '"
            + _program_name
            + "', because CLIF build failed with error.", 2);

        return 1;
    }
    CLIF::FLog::
    infoAt("CLIF::FApplication::start()",
        "Application '"
        + _program_name
        + "' start.");

    _parser->strConverter(_str_converter);
    _parser->parse(argc, argv, *_caller, _func_linkers);

    return _parser->isParseOK() ? 0 : 1;
}

void CLIF::FApplication::
addSubCommand(const std::string &exist_subcommand,
              const std::string &new_subcommand)
{
    if (!_func_linkers.count(exist_subcommand)) {
        CLIF::FStdo::
        budMsg("add new subcommand with unknown subcommand : '"
            + exist_subcommand
            + "'.", 2);

        _build_ok = false;
    } else if (_func_linkers.count(new_subcommand)) {
        CLIF::FStdo::
        budMsg("add new subcommand with existing subcommand : '"
            + new_subcommand
            + "'.", 2);

        _build_ok = false;
    } else {
        _func_linkers[new_subcommand].subcommand =
        _func_linkers[exist_subcommand].subcommand;

        CLIF::FLog::
        infoAt("CLIF::FApplication::addSubCommand()",
            "Add new subcommand '"
            + new_subcommand
            + "' with exist subcommand '"
            + exist_subcommand
            + "'.");
    }
    return;
}

bool CLIF::FApplication::
validateSubCommand(const std::string &subcommand)
{
    bool valid;

    valid = false;

    /// Validate if the subcommand is empty.
    if (CLIF::FStr::onlyAlNum(subcommand).empty()) {
        CLIF::FStdo::
        budMsg("there exist an empty or invalid subcommand '"
            + subcommand
            + "'.", 2);
    }
    /// Validate if the subcommand is already exist.
      else if (_func_linkers.count(subcommand)) {
        CLIF::FStdo::
        budMsg("subcommand '"
            + subcommand
            + "' is already exist", 2);
    } else {
        valid = true;
    }
    return valid;
}

bool CLIF::FApplication::
validateOptionAmount(const std::string &subcommand,
                     const size_t &options_amount,
                     const size_t &args_amount)
{
    bool valid;

    valid = false;

    /// Validate if the amount of options is equal with function.
    if (options_amount != args_amount) {
        CLIF::FStdo::
        budMsg("subcommand '"
            + subcommand
            + "' expect "
            + std::to_string(args_amount) + " options, "
            + "but got "
            + std::to_string(options_amount), 2);
    } else {
        valid = true;
    }

    return valid;
}

bool CLIF::FApplication::
validateEmptyOptionName(const std::string &subcommand,
                        const std::vector<CLIF::FOption> &options)
{
    bool valid;
    size_t i;
    std::string t_long_name, t_short_name;

    valid = true;

    /// Validate if exist empty option name.
    for (i = 0; i < options.size(); i++) {
        t_long_name = CLIF::FStr::onlyAlNum(options[i].getLongName());
        t_short_name = CLIF::FStr::onlyAlNum(options[i].getShortName());

        if (!t_long_name.empty() && !t_short_name.empty()) continue;
        if (t_long_name.empty() && t_short_name.empty()) {
            CLIF::FStdo::
            budMsg("subcommand '"
                   + subcommand
                   + "' exist an empty or invalid option at position "
                   + std::to_string((int)i + 1), 2);
        } else if (t_long_name.empty()) {
            CLIF::FStdo::
            budMsg("subcommand '"
                + subcommand
                + "' exist an empty or invalid long name option at position "
                + std::to_string((int)i + 1), 2);
        } else {
            CLIF::FStdo::
            budMsg("subcommand '"
                + subcommand
                + "' exist an empty or invalid short name option at position "
                + std::to_string((int)i + 1), 2);
        }
        valid = false;
    }
    return valid;
}

bool CLIF::FApplication::
validateDuplicateOptionName(const std::string &subcommand,
                            const std::vector<CLIF::FOption> &options)
{
    bool valid;
    size_t i;
    std::string t_long_name, t_short_name;
    std::unordered_set<std::string> exist_options;

    valid = true;

    /// Validate if exist duplicate option name.
    for (i = 0; i < options.size(); i++) {
        t_long_name = options[i].getLongName();
        t_short_name = options[i].getShortName();

        if (!exist_options.count(t_long_name) && !exist_options.count(t_short_name)) {
            exist_options.insert(t_long_name);
            exist_options.insert(t_short_name);
            continue;
        }
        if (exist_options.count(t_long_name)) {
            CLIF::FStdo::
            budMsg("subcommand '"
                + subcommand
                + "' exist duplicate long name option '"
                + t_long_name
                + "' at position "
                + std::to_string((int)i + 1), 2);
        }
        if (exist_options.count(t_short_name)) {
            CLIF::FStdo::
            budMsg("subcommand '"
                + subcommand
                + "' exist duplicate short name option '"
                + t_short_name
                + "' at position "
                + std::to_string((int)i + 1), 2);
        }
        valid = false;
    }
    return valid;
}

bool CLIF::FApplication::
validateMismatchedOptionType(const std::string &subcommand,
                             const std::vector<CLIF::FOption> &options,
                             const std::vector<std::type_index> &exp_types)
{
    bool valid;
    size_t i;

    valid = true;

    /// Validate if the type of each option is matched.
    for (i = 0; i < options.size(); i++) {
        if (options[i].getType() != exp_types[i]) {
            CLIF::FStdo::
            budMsg("subcommand '"
                + subcommand
                + "' expect '"
                + std::string(exp_types[i].name())
                + "' in option '"
                + options[i].getLongName() + "/" + options[i].getShortName()
                + "' at position "
                + std::to_string((int)i + 1)
                + ", but got '"
                + std::string(options[i].getTypeName())
                + "'", 2);

            valid = false;
        }
    }
    return valid;
}

bool CLIF::FApplication::
validateOptionalDefaultValueEmpty(const std::string& subcommand,
                                  const std::vector<CLIF::FOption>& options)
{
    bool valid;
    size_t i;

    valid = true;

    for (i = 0; i < options.size(); i++) {
        if (!options[i].isOptional()) continue;

        if (!options[i].getDefaultValue().has_value()) {
            CLIF::FStdo::
            budMsg("subcommand '"
                + subcommand
                + "' option '"
                + options[i].getLongName() + "/" + options[i].getShortName()
                + "' at position "
                + std::to_string((int)i + 1)
                + " is optional, but the default value is empty", 2);

            CLIF::FStdo::
            budMsg("please set a default value for option '"
                + options[i].getLongName() + "/" + options[i].getShortName()
                + "'");

            valid = false;
        }
    }
    return valid;
}
