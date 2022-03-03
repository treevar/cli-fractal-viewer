/****Copyright (C) 2022  eggplantcx
 **Util.hpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
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

    void printProgressBar(std::ostream &out, unsigned int done, unsigned int total);
    void setProgressBar(std::ostream &out, unsigned int done, unsigned int total);
    void clearProgressBar(std::ostream &out, unsigned int done, unsigned int total);

    void printProgressCounter(std::ostream &out, unsigned int done, unsigned int total);
    void setProgressCounter(std::ostream &out, unsigned int done, unsigned int total);
    void clearProgressCounter(std::ostream &out, unsigned int done, unsigned int total);

    std::string trimDouble(double d, std::streamsize precision = 15);

}
#endif//UTIL_HPP