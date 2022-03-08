/****Copyright (C) 2022  eggplantcx
 **Config.cpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#include "../inc/Config.hpp"
#include <fstream>
#include <iostream>
#include <utility>

Config::Config() = default;

Config::Config(const RawData &d){ _cfg = d; }

bool Config::isOption(const std::string &key) const{ return _cfg.find(key) != _cfg.end(); }

std::string Config::getValue(const std::string &key) const{
    auto it = _cfg.find(key);
    return it == _cfg.end() ? "" : it->second;
}

Config::RawData Config::getCfg() const{ return _cfg; }

std::ostream &operator<<(std::ostream &out, const Config &c){
    out << "{\n";
    for(auto &pair : c._cfg){
        out << pair.first << ':' << pair.second << '\n';
    }
    out << "}";
    return out;
}

void Config::readFromFile(const std::string &fileName){
    std::ifstream in(fileName);
    if(!in.is_open()){
        std::cerr << "Config::readFromFile: Error opening \"" << fileName << "\"\n";
        fireEvent(ConfigFileEvent(fileName, 'r', false));
        return;
    }
    bool invalidCfgFormat = false;
    while(!in.eof() && !invalidCfgFormat){
        auto str = Util::getNextLine(in);
        if(str.size() == 0){ break; }
        if(str[0] == '}'){ break; }
        if(str[0] == '{'){ 
            _cfg.clear(); 
        }
        else{
            auto pos = str.find(':');
            if(pos == std::string::npos){
                invalidCfgFormat = true;
            }
            else{
                _cfg[str.substr(0, pos)] = str.substr(pos);
            }
        }
    }
    if(invalidCfgFormat){
        std::cerr << "Config::readFromFile: File \"" << fileName << "\" is in incorrect format\n"; 
    }
    else{
        std::cout << "Config::readFile: Loaded config from \"" << fileName << "\"\n"; 
    }
    in.close();
    fireEvent(ConfigFileEvent(fileName, 'r', invalidCfgFormat));
}

void Config::saveToFile(const std::string &fileName, std::ios::openmode om){
    std::ofstream out(fileName, om);
    if(!out.is_open()){
        std::cerr << "Config::saveToFile: Error opening \"" << fileName << "\"\n";
        fireEvent(ConfigFileEvent(fileName, 'w', false));
        return;
    }
    out << *this;
    out.close();
    std::cout << "Config::saveToFile: Saved config to \"" << fileName << "\"\n";
    fireEvent(ConfigFileEvent(fileName, 'w', true));
}

Config::ConfigSetEvent::ConfigSetEvent(const std::pair<std::string, std::string> &pair) : _pair(pair){
    _type = "ConfigSet";
}

void *Config::ConfigSetEvent::_dataToVoidPointer() const{
    auto *ptr = new std::pair<std::string, std::string>;
    *ptr = _pair;
    return ptr;
}

Config::ConfigFileEvent::ConfigFileEvent(std::string fileName, char operation, bool success) : _fileName(fileName),
_operation(operation), _success(success){
    _type = "ConfigFile";
}

void *Config::ConfigFileEvent::_dataToVoidPointer() const{
    struct Data{
        std::string fileName;
        char operation;
        bool success;
    };
    auto *data = new Data{_fileName, _operation, _success};
    return data;
}