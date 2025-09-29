/**
    \file main.hpp
    \brief an simple example for "CLIF".
 */

/**
    [INTRODUCTION]

        This is a functional CLI-Framework based on C/C++, which have features below:
            a.  ingle File Dependency
             :  Requires only the C++17 standard library with no external dependencies.
            b.  Functional Programming
             :  Direct function binding eliminates manual argument parsing.
            c.  Strong Type Safety
             :  Catches type conflicts during compilation to prevent runtime errors.
            d.  Generic Container Support
             :  Native support for nested containers with custom parsers and validators.

    [INSTRUCTION]

        1.  Copy folder '../CLIF' into your project.
        2.  Include the directory to your building system.
        3.  Create a new file like './func/funcs.hpp' which include all the functions.
            The return value must be void.
        4.  Create a new file like './main.cpp'. Include './funcs/func.hpp' and
            'CLIF/CLIF.hpp'.
        5.  Initialize FApplication and register commands:

                CLIF::FApplication app("MyApp");

                app.addSubCommand<int, int>
                ("subcommand","description of subcommand",
                {
                    CLIF::FOption()
                        .longName("first")
                        .help("first option."),
                    CLIF::FOption()
                        .longName("second")
                        .help("second option")
                    ...
                }, func);

        6.  Register user data type:

                app.registerBasicTypeConverter<MyDataType>
                ([](const std::string &str){ ... } -> std::any);

        7.  Register container types (if needed):

                app.registerContainerConverter<vector<vector<int>>>();

        8.  Start parsing:

                app.run(argc, argv); -> int : 1(fail) 0(ok)

    [ATTENTION]

        1.  Requires C++17 or higher : Framework heavily uses structured
                                       bindings and std::any.
        2.  Container limits         : Pre-register nested containers
                                       using registerNewContainer<>().
        3.  Type safety              : Ensure function signatures exactly
                                       match option definitions.
        4.  Unicode support          : Windows requires UTF-8 encoding for
                                       command line parameters.
        5.  Error handling           : Validate parameters before use with
                                       validator() methods.
        6.  Custom types             : Use registerBasicTypeConverter<>() for
                                       user-defined types before command registration.
 */

#include <CLIF.hpp>
#include "./funcs/funcs.hpp"

int main(int argc, char *argv[])
{
    CLIF::FLog logger(&argc, argv);
    CLIF::FApplication app("A CLIF example");

    app.registerContainerConverter<std::vector<std::vector<int>>>();
    app.registerContainerConverter<std::map<std::string, int>>();

    app.addSubCommand
    ("request", "make an HTTP request to a specified URL",
    {
        CLIF::FOption()
            .longName("type")
            .shortName("T")
            .help("URL type (http/https)")
            .defaultValue(std::string("https"))
            .validator<std::string>(
                [](std::string host)
                {
                    return host == "http" || host == "https";
                }
            )
            .uniqueed(),
        CLIF::FOption()
            .longName("host")
            .shortName("H")
            .help("URL host")
            .type<std::string>()
            .required()
            .uniqueed(),
        CLIF::FOption()
            .longName("port")
            .shortName("P")
            .help("URL port")
            .defaultValue(8080)
            .validator<int>(
                [](int port)
                {
                    return port > 0 && port < 65535;
                }
            )
            .required()
            .uniqueed()
    }, req);

    app.addSubCommand
    ("map", "print a map <std::string, int>.",
    {
        CLIF::FOption()
            .longName("map")
            .shortName("M")
            .help("a map <std::string, int>.")
            .type<std::map<std::string, int>>()
            .required()
            .uniqueed()
    }, map);

    app.addSubCommand
    ("matrix", "print a 2D matrix <int>.",
    {
        CLIF::FOption()
            .longName("matrix")
            .shortName("M")
            .help("a 2D matrix <int>")
            .type<std::vector<std::vector<int>>>()
            .validator<std::vector<std::vector<int>>>(
                [](std::vector<std::vector<int>> mat)
                {
                    return mat.size() > 0;
                }
            )
            .required()
            .uniqueed()
    }, matrix);

    return app.start(argc, argv);
}