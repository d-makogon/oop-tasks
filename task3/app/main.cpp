#include <memory>

#include <optionparser.h>
#include <gameView.h>
#include <consoleView.h>
#include <playerController.h>
#include <consolePlayerController.h>
#include <optimalPlayerController.h>
#include <randomPlayerController.h>
#include <factory.h>
#include <utils.h>

struct Arg : public option::Arg
{
    static void printError(const char* msg1, const option::Option& opt, const char* msg2)
    {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Unknown(const option::Option& option, bool msg)
    {
        if (msg) printError("Unknown option '", option, "'\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Required(const option::Option& option, bool msg)
    {
        if (option.arg != 0)
            return option::ARG_OK;
        if (msg) printError("Option '", option, "' requires an argument\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus NonEmpty(const option::Option& option, bool msg)
    {
        if (option.arg != 0 && option.arg[0] != 0)
            return option::ARG_OK;
        if (msg) printError("Option '", option, "' requires a non-empty argument\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Numeric(const option::Option& option, bool msg)
    {
        char* endptr = 0;
        if (option.arg != 0 && strtol(option.arg, &endptr, 10)) {};
        if (endptr != option.arg && *endptr == 0)
            return option::ARG_OK;
        if (msg) printError("Option '", option, "' requires a numeric argument\n");
        return option::ARG_ILLEGAL;
    }
};

enum optionIndex
{
    UNKNOWN, HELP, COUNT, PLAYER1, PLAYER2
};

const option::Descriptor usage[] = {
        {UNKNOWN, 0, "",  "",       Arg::Unknown,  "USAGE: path/to/executable [options]\n\n"
                                                   "Options:"},
        {HELP,    0, "h", "help",   Arg::None,     "  \t--help  \tPrint usage and exit."},
        {COUNT,   0, "c", "count",  Arg::Numeric,  "  -c <num>, \t--count <num>  \tNumber of rounds to play. Default: 1"},
        {PLAYER1, 0, "f", "first",  Arg::NonEmpty, "  -f <arg>, \t--first <arg>"
                                                   "  \tType of 1st player (console, random, optimal). Default: random."},
        {PLAYER2, 0, "s", "second", Arg::NonEmpty, "  -s <arg>, \t--second <arg>"
                                                   "  \tType of 2nd player (console, random, optimal). Default: random."},
        {0,       0, 0,   0,        0,             0}};

int main(int argc, char** argv)
{
    argc -= (argc > 0);
    argv += (argc > 0);

    option::Stats stats(usage, argc, argv);
    option::Option options[stats.options_max], buffer[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);

    if (parse.error())
        return 1;

    int roundsCount;
    if (options[HELP])
    {
        option::printUsage(std::cout, usage);
        return 0;
    }
    if (!options[COUNT].arg)
    {
        roundsCount = 1;
    }
    else if (!str2num(options[COUNT].arg, roundsCount))
    {
        option::printUsage(std::cout, usage);
        return 1;
    }

    Factory<bs::PlayerController> controllersFactory;
    controllersFactory.Register<bs::ConsolePlayerController>("console");
    controllersFactory.Register<bs::RandomPlayerController>("random");
    controllersFactory.Register<bs::OptimalPlayerController>("optimal");

    auto logic = std::make_unique<bs::GameLogic>(roundsCount);

    auto p1 = controllersFactory.Create((options[PLAYER1].arg) ? (options[PLAYER1].arg) : "random");
    if (p1 == nullptr)
    {
        std::cout << "Unknown player type: " << options[PLAYER1].arg << std::endl;
        option::printUsage(std::cout, usage);
        return 1;
    }
    auto p2 = controllersFactory.Create((options[PLAYER2].arg) ? (options[PLAYER2].arg) : "random");
    if (p2 == nullptr)
    {
        std::cout << "Unknown player type: " << options[PLAYER2].arg << std::endl;
        option::printUsage(std::cout, usage);
        return 1;
    }

    std::unique_ptr<bs::GameView> view = std::make_unique<bs::ConsoleView>(std::move(logic), std::move(p1),
                                                                           std::move(p2));

    view->Do();

    return 0;
}