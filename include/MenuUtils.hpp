#pragma once
#include <vector>
#include <string>
#include "Student.hpp"
namespace tabulate
{
    enum class Color;
}
class MenuUtils
{
public:
    static void displayTable(const std::vector<Student> &students);
    static void displayStudentDetails(const Student &student);
    static void displayGradeReport(const std::vector<Student> &students);
    static void displayFailingStudents(const std::vector<Student> &students);

    static void printMenu(const std::vector<std::string> &items);
    static void printWelcome();
    static void printMainMenu();
    static void printAdminMenu();
    static void printStudentMenu();

    static void printColored(const std::string &text, const std::string &color);
    static void printSuccess(const std::string &message);
    static void printError(const std::string &message);
    static void printWarning(const std::string &message);
    static void printInfo(const std::string &message);

    static void printColorLegend();
    static tabulate::Color getGradeColor(const std::string &grade);

    static int getMenuChoice(int maxOptions);
    static std::string getHiddenInput(const std::string &prompt);
    static bool askContinue();
    static std::string getStringInput(const std::string &prompt);
    static int getIntInput(const std::string &prompt);
    static double getDoubleInput(const std::string &prompt);

    static void clearScreen();
    static void pauseScreen();
    static void printSeparator();
    static void printHeader(const std::string &title);

    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    static const std::string BOLD;
};