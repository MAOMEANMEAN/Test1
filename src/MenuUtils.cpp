#include "MenuUtils.hpp"
#include "GradeUtil.hpp"
#include <tabulate/table.hpp>
#include <iostream>
#include <iomanip>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
using namespace std;
using namespace tabulate;

const std::string MenuUtils::RESET = "\033[0m";
const std::string MenuUtils::RED = "\033[31m";
const std::string MenuUtils::GREEN = "\033[32m";
const std::string MenuUtils::YELLOW = "\033[33m";
const std::string MenuUtils::BLUE = "\033[34m";
const std::string MenuUtils::MAGENTA = "\033[35m";
const std::string MenuUtils::CYAN = "\033[36m";
const std::string MenuUtils::WHITE = "\033[37m";
const std::string MenuUtils::BOLD = "\033[1m";

Color MenuUtils::getGradeColor(const std::string &grade)
{
    if (grade == "A" || grade == "B" || grade == "C")
    {
        return Color::green;
    }
    else if (grade == "D" || grade == "E")
    {
        return Color::yellow;
    }
    else if (grade == "F")
    {
        return Color::red;
    }
    return Color::white;
}

void MenuUtils::displayTable(const std::vector<Student> &students)
{
    if (students.empty())
    {
        printWarning("No students to display!");
        return;
    }
    Table table;
    table.add_row({"ID", "Name", "Age", "Gender", "Average", "Grade", "GPA", "Remark"});
    for (const auto &student : students)
    {
        table.add_row({student.getStudentId(),
                       student.getName(),
                       to_string(student.getAge()),
                       student.getGender(),
                       to_string(static_cast<int>(student.getAverageScore() * 100) / 100.0),
                       student.getLetterGrade(),
                       to_string(student.getGpa()),
                       student.getRemark()});
    }
    table[0].format().font_style({FontStyle::bold}).font_color(Color::cyan);
    for (size_t i = 1; i < table.size(); ++i)
    {
        std::string grade = students[i - 1].getLetterGrade();
        std::string remark = students[i - 1].getRemark();
        Color gradeColor = getGradeColor(grade);
        table[i].format().font_color(gradeColor);
        if (remark == "Fail")
        {
            table[i].format().font_style({FontStyle::bold});
        }
    }
    cout << table << endl;
    printColorLegend();
}

void MenuUtils::displayStudentDetails(const Student &student)
{
    Table table;
    table.add_row({"Field", "Value"});
    table.add_row({"Student ID", student.getStudentId()});
    table.add_row({"Name", student.getName()});
    table.add_row({"Age", to_string(student.getAge())});
    table.add_row({"Gender", student.getGender()});
    table.add_row({"Date of Birth", student.getDateOfBirth()});
    table.add_row({"Email", student.getEmail()});
    auto subjects = GradeUtil::getSubjectNames();
    auto scores = student.getSubjectScores();
    for (size_t i = 0; i < subjects.size() && i < scores.size(); ++i)
    {
        table.add_row({subjects[i], to_string(scores[i])});
    }
    table.add_row({"Average Score", to_string(student.getAverageScore())});
    table.add_row({"Letter Grade", student.getLetterGrade()});
    table.add_row({"GPA", to_string(student.getGpa())});
    table.add_row({"Remark", student.getRemark()});
    table.add_row({"Last Updated", student.getFormattedTimestamp()});
    table[0].format().font_style({FontStyle::bold}).font_color(Color::cyan);
    for (size_t i = 1; i < table.size(); ++i)
    {
        std::string fieldName = table[i][0].get_text();
        if (fieldName == "Letter Grade")
        {
            Color gradeColor = getGradeColor(student.getLetterGrade());
            table[i].format().font_color(gradeColor).font_style({FontStyle::bold});
        }
        else if (fieldName == "Remark")
        {
            if (student.getRemark() == "Pass")
            {
                table[i].format().font_color(Color::green).font_style({FontStyle::bold});
            }
            else
            {
                table[i].format().font_color(Color::red).font_style({FontStyle::bold});
            }
        }
        else if (fieldName == "Average Score")
        {
            Color gradeColor = getGradeColor(student.getLetterGrade());
            table[i].format().font_color(gradeColor);
        }
    }
    cout << table << endl;
}

void MenuUtils::displayGradeReport(const std::vector<Student> &students)
{
    printHeader("GRADE REPORT");

    if (students.empty())
    {
        printWarning("No students to display!");
        return;
    }
    int totalStudents = students.size();
    int passingStudents = 0;
    int gradeA = 0, gradeB = 0, gradeC = 0, gradeD = 0, gradeE = 0, gradeF = 0;
    double totalAverage = 0.0;
    for (const auto &student : students)
    {
        if (student.hasPassingGrade())
        {
            passingStudents++;
        }
        totalAverage += student.getAverageScore();
        std::string grade = student.getLetterGrade();
        if (grade == "A")
            gradeA++;
        else if (grade == "B")
            gradeB++;
        else if (grade == "C")
            gradeC++;
        else if (grade == "D")
            gradeD++;
        else if (grade == "E")
            gradeE++;
        else if (grade == "F")
            gradeF++;
    }

    double classAverage = totalAverage / totalStudents;
    double passRate = (static_cast<double>(passingStudents) / totalStudents) * 100.0;
    Table summaryTable;
    summaryTable.add_row({"Statistic", "Value"});
    summaryTable.add_row({"Total Students", to_string(totalStudents)});
    summaryTable.add_row({"Passing Students (50+)", to_string(passingStudents)});
    summaryTable.add_row({"Failing Students (<50)", to_string(totalStudents - passingStudents)});
    summaryTable.add_row({"Pass Rate", to_string(static_cast<int>(passRate * 100) / 100.0) + "%"});
    summaryTable.add_row({"Class Average", to_string(static_cast<int>(classAverage * 100) / 100.0)});
    summaryTable.add_row({"Grade A (90-100)", to_string(gradeA)});
    summaryTable.add_row({"Grade B (80-89)", to_string(gradeB)});
    summaryTable.add_row({"Grade C (70-79)", to_string(gradeC)});
    summaryTable.add_row({"Grade D (60-69)", to_string(gradeD)});
    summaryTable.add_row({"Grade E (50-59)", to_string(gradeE)});
    summaryTable.add_row({"Grade F (<50)", to_string(gradeF)});
    summaryTable[0].format().font_style({FontStyle::bold}).font_color(Color::magenta);
    for (size_t i = 1; i < summaryTable.size(); ++i)
    {
        std::string field = summaryTable[i][0].get_text();
        if (field.find("Grade A") == 0 || field.find("Grade B") == 0 || field.find("Grade C") == 0)
        {
            summaryTable[i].format().font_color(Color::green);
        }
        else if (field.find("Grade D") == 0 || field.find("Grade E") == 0)
        {
            summaryTable[i].format().font_color(Color::yellow);
        }
        else if (field.find("Grade F") == 0)
        {
            summaryTable[i].format().font_color(Color::red);
        }
        else if (field.find("Failing") == 0)
        {
            summaryTable[i].format().font_color(Color::red);
        }
        else if (field.find("Passing") == 0)
        {
            summaryTable[i].format().font_color(Color::green);
        }
    }
    cout << summaryTable << endl;
    printSeparator();
    displayTable(students);
}

void MenuUtils::displayFailingStudents(const std::vector<Student> &students)
{
    if (students.empty())
    {
        printSuccess("No failing students found!");
        return;
    }
    printError("FAILING STUDENTS (<50 Average) - " + to_string(students.size()) + " found:");
    displayTable(students);
}

void MenuUtils::printColorLegend()
{
    cout << "\n"
         << BOLD << "Grade Color Legend:" << RESET << endl;
    cout << GREEN << "● A-C: Excellent Performance" << RESET << endl;
    cout << YELLOW << "● D-E: Acceptable Performance" << RESET << endl;
    cout << RED << "● F: Failing Performance (<50)" << RESET << endl;
    cout << endl;
}

void MenuUtils::printMenu(const std::vector<std::string> &items)
{
    Table table;
    table.add_row({"No", "Menu"});
    for (size_t i = 0; i < items.size(); i++)
    {
        table.add_row({to_string(i + 1), items[i]});
    }
    table[0].format().font_style({FontStyle::bold}).font_color(Color::cyan);
    cout << table << endl;
}

void MenuUtils::printWelcome()
{
    system("cls");
    cout << string(10, '\n');
    string text = R"(
  ██╗███████╗████████╗ █████╗ ██████╗     ██████╗ ██████╗ ███████╗      ██████╗ ███████╗███╗   ██╗██╗  ██╗
  ██║██╔════╝╚══██╔══╝██╔══██╗██╔══██╗    ██╔══██╗██╔══██╗██╔════╝     ██╔════╝ ██╔════╝████╗  ██║██║  ██║
  ██║███████╗   ██║   ███████║██║  ██║    ██████╔╝██████╔╝█████╗       ██║  ███╗█████╗  ██╔██╗ ██║███████║
  ██║╚════██║   ██║   ██╔══██║██║  ██║    ██╔═══╝ ██╔══██╗██╔══╝       ██║   ██║██╔══╝  ██║╚██╗██║╚════██║
  ██║███████║   ██║   ██║  ██║██████╔╝    ██║     ██║  ██║███████╗     ╚██████╔╝███████╗██║ ╚████║     ██║
  ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═════╝     ╚═╝     ╚═╝  ╚═╝╚══════╝      ╚═════╝ ╚══════╝╚═╝  ╚═══╝     ╚═╝
  )";
    cout << "        ";
    cout << "\033[94m";

    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            cout << text[i];
            cout << "        ";
#ifdef _WIN32
            Sleep(100);
#else
            usleep(200);
#endif
        }
        else
        {
            cout << text[i];
            cout.flush();
            if (text[i] == '█')
            {
#ifdef _WIN32
                Sleep(30);
#else
                usleep(30000);
#endif
            }
            else if (text[i] != ' ')
            {
#ifdef _WIN32
                Sleep(15);
#else
                usleep(15000);
#endif
            }
        }
    }
    cout << "\033[0m" << endl;
    cout << endl;
    cout << "                                               ";
    cout << RED << "Loading";

    for (int i = 0; i < 3; i++)
    {
#ifdef _WIN32
        Sleep(400);
#else
        system("sleep 1");
#endif
        cout << ".";
        cout.flush();
    }
    cout << RESET << endl;

#ifdef _WIN32
    Sleep(2000);
#else
    system("sleep 2");
#endif
#ifdef _WIN32
    Sleep(2000);
#else
    usleep(2000000);
#endif

    system("cls");
    cout << GREEN << endl;
    cout << " #############################################################################" << endl;
    cout << " #                                                                           #" << endl;
    cout << " #                      WELCOME TO SCOREME GENERATOR                         #" << endl;
    cout << " #                                                                           #" << endl;
    cout << " #                   Student Grade Management System                         #" << endl;
    cout << " #                                                                           #" << endl;
    cout << " #############################################################################" << endl;
    cout << RESET << endl;
#ifdef _WIN32
    Sleep(1000);
#else
    usleep(1000000);
#endif
    cout << YELLOW << "                                           ";

    string developer = "Developed by ISTAD Pre-Gen4 GroupI";
    for (int i = 0; i < developer.length(); i++)
    {
        cout << developer[i];
        cout.flush();
#ifdef _WIN32
        Sleep(40);
#else
        usleep(40000);
#endif
    }
    cout << RESET << endl;
    cout << endl;
#ifdef _WIN32
    Sleep(2000);
#else
    usleep(2000000);
#endif
}

void MenuUtils::printMainMenu()
{
    printHeader("MAIN MENU");
    vector<string> mainMenu = {
        "Admin Login",
        "Student Login",
        "Exit"};
    printMenu(mainMenu);
}

void MenuUtils::printAdminMenu()
{
    printHeader("ADMIN DASHBOARD");
    vector<string> adminMenu = {
        "Manage Students",
        "Import Excel Data",
        "Export Grade Report",
        "Backup Data",
        "Sign Out",
        "Back to Main Menu"};
    printMenu(adminMenu);
}

void MenuUtils::printStudentMenu()
{
    printHeader("STUDENT MENU");
    vector<string> studentMenu = {
        "Search Your Data",
        "View Your Grades",
        "Sign Out"};
    printMenu(studentMenu);
}

void MenuUtils::printColored(const std::string &text, const std::string &color)
{
    cout << color << text << RESET << endl;
}

void MenuUtils::printSuccess(const std::string &message)
{
    cout << GREEN << "✅ " << message << RESET << endl;
}

void MenuUtils::printError(const std::string &message)
{
    cout << RED << "❌ " << message << RESET << endl;
}

void MenuUtils::printWarning(const std::string &message)
{
    cout << YELLOW << "⚠️ " << message << RESET << endl;
}

void MenuUtils::printInfo(const std::string &message)
{
    cout << BLUE << "ℹ️ " << message << RESET << endl;
}

int MenuUtils::getMenuChoice(int maxOptions)
{
    int choice;
    while (true)
    {
        cout << CYAN << ">> Choose your option (1-" << maxOptions << "): " << RESET;
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > maxOptions)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printError("Invalid choice! Please enter a number between 1 and " + to_string(maxOptions));
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
    }
}

std::string MenuUtils::getHiddenInput(const std::string &prompt)
{
    cout << prompt;
    string input;
#ifdef _WIN32
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!input.empty())
            {
                input.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            input += ch;
            cout << '*';
        }
    }
    cout << endl;
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getline(cin, input);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
#endif
    return input;
}

bool MenuUtils::askContinue()
{
    string response;
    cout << YELLOW << "Do you want to continue? (Y/N): " << RESET;
    getline(cin, response);
    return (response == "Y" || response == "y" || response == "yes" || response == "Yes");
}

std::string MenuUtils::getStringInput(const std::string &prompt)
{
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int MenuUtils::getIntInput(const std::string &prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printError("Invalid input! Please enter a valid number.");
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

double MenuUtils::getDoubleInput(const std::string &prompt)
{
    double value;
    while (true)
    {
        cout << prompt;
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printError("Invalid input! Please enter a valid number.");
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

void MenuUtils::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MenuUtils::pauseScreen()
{
    cout << CYAN << "\nPress Enter to continue..." << RESET;
    cin.get();
}

void MenuUtils::printSeparator()
{
    cout << CYAN << string(60, '=') << RESET << endl;
}

void MenuUtils::printHeader(const std::string &title)
{
    printSeparator();
    cout << BOLD << CYAN << "               " << title << RESET << endl;
    printSeparator();
}