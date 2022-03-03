/****Copyright (C) 2022  eggplantcx
 **Util.cpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#include "../inc/Util.hpp"
#include <iostream>
#include <sstream>
bool Util::isDouble(const std::string &str){
    if(str.size() == 0){ return false; }
    if(str == "-."){ return false; }
    bool decimalFound = false;
    for(int i = 0; i < str.size(); i++){
        char c = str[i];
        if(!std::isdigit(c)){
            if(c == '-'){ 
                if(i > 0 || str.size() == 1){ return false; }
            }
            else if(c == '.'){
                if(decimalFound || str.size() == 1){ return false; }
                decimalFound = true;
            }
            else{ return false; }
        }
    }
    return true;
}

bool Util::inRange(std::string d, double min, double max){ 
    if(!isDouble(d)){ return false; }
    double du = std::stod(d);
    return du >= min && du <= max;
}

double Util::promptWithCheck(const std::string &prompt, double min, double max){
    std::cout << prompt;
    std::string in = {""};
    in = getNextLine();
    while(!inRange(in, min, max)){
        std::cout << "Input was not a double between " << min << " and " << max << '\n';
        std::cout << prompt;
        in = getNextLine();
    }
    return std::stod(in);
}

double Util::promptWithCheckAndExit(const std::string &prompt, const std::string &exitCode, double exitVal, double min, double max){
    std::string exitReminder = "(type \"" + exitCode + "\" to exit): ";
    std::cout << prompt << exitReminder;
    std::string in = {""};
    in = getNextLine();
    if(in == exitCode){ return exitVal; }
    while(!inRange(in, min, max)){
        std::cout << "Input was not a double between " << min << " and " << max << '\n';
        std::cout << prompt << exitReminder;
        in = getNextLine();
    }
    return std::stod(in);    
}

bool Util::isValidMenuOption(const Menu &v, const std::string &in){
    for(auto &op : v){
        if(op.first == in){ return true; }
    }
    return false;
}

std::string Util::menuWithCheck(const Menu &v){
    std::cout << "Select Option:\n";
    for(auto &op : v){
        std::cout << '\t' << op.first << " = " << op.second << '\n';
    }
    std::cout << "> ";
    std::string in = {""};
    in = getNextLine();
    while(!isValidMenuOption(v, in)){
        std::cout << "\nInvalid Choice\nSelect Option:\n";
        for(auto &op : v){
            std::cout << '\t' << op.first << " = " << op.second << '\n';
        }
        std::cout << "> ";
        in = getNextLine();
    }
    return in;
}

std::string Util::getNextLine(){
    std::string str;
    std::getline(std::cin, str);
    return str;
}

void Util::pause(){
    std::cout << "Press Enter to Continue...";
    getNextLine();
}

void Util::printProgressBar(std::ostream &out, unsigned int done, unsigned int total){
    out << '[';
    for(unsigned int i = 0; i < done; i++){
        out << '*';
    }
    for(unsigned int i = 0; i < total - done; i++){
        out << '_';
    }
    out << "] " << done << '/' << total;    
}

void Util::setProgressBar(std::ostream &out, unsigned int done, unsigned int total){
    clearProgressBar(out, done, total);
    printProgressBar(out, done, total);
}

void Util::clearProgressBar(std::ostream &out, unsigned int done, unsigned int total){
    std::stringstream ss;
    ss << done;
    int totalLength = 4 + total + ss.str().length();//4 for [ ]/
    ss.clear();
    ss.str(std::string());
    ss << total;
    totalLength += ss.str().length();
    for(unsigned int i = 0; i < totalLength; i++){
        out << '\b';
    }    
}

void Util::printProgressCounter(std::ostream &out, unsigned int done, unsigned int total){
    out << done << '/' << total;    
}

void Util::clearProgressCounter(std::ostream &out, unsigned int done, unsigned int total){
    std::stringstream ss;
    ss << done;
    int totalLength = 1 + ss.str().length();//1 for /
    ss.clear();
    ss.str(std::string());
    ss << total;
    totalLength += ss.str().length();
    for(int i = 0; i < totalLength; i++){
        out << '\b';
    }
}

void Util::setProgressCounter(std::ostream &out, unsigned int done, unsigned int total){
    clearProgressCounter(out, done, total);
    printProgressCounter(out, done, total);
}

std::string Util::trimDouble(double d, std::streamsize precision){
    std::stringstream ss;
    ss.precision(precision);
    ss.setf(std::ios_base::fixed);
    ss << d;
    std::string f = ss.str();
    for(size_t i = f.size()-1; i > 0; i--){
        if(f[i] == '.'){ 
            return f.substr(0, i+2); 
        }
        if(f[i] != '0'){ 
            return f.substr(0, i+1); 
        }
    }
    return f;
}