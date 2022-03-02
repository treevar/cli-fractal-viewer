#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <vector>
#include <utility>//std::pair
namespace Util{
    bool isDouble(const std::string &str);

    bool inRange(std::string d, double min, double max);
    
    double promptWithCheck(const std::string &prompt, double min = -100, double max = 100);
    
    double promptWithCheckAndExit(const std::string &prompt, const std::string &exitCode, double exitVal, double min = -100, double max = 100);

    typedef std::vector<std::pair<std::string, std::string>> Menu;
    bool isValidMenuOption(const Menu &v, const std::string &in);

    std::string menuWithCheck(const Menu &v);

    std::string getNextLine();

    void pause();

    void printLoadBar(std::ostream &out, unsigned int done, unsigned int total);
    void setLoadBar(std::ostream &out, unsigned int done, unsigned int total);

    std::string trimDouble(double d, std::streamsize precision = 15);

}
#endif//UTIL_HPP