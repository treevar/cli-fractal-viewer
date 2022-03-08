/****Copyright (C) 2022  eggplantcx
 **Config.hpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <map>
#include <ostream>
#include <sstream>
#include "../inc/Util.hpp"
#include "../inc/EventHandler.hpp"
class Config : public EventHandler{
    public:
        typedef std::map<std::string, std::string> RawData; 
        Config();
        Config(const RawData &d);
        
        bool isOption(const std::string &key) const;
        
        std::string getValue(const std::string &key) const;
        RawData getCfg() const;
        
        template<typename T>
        void setValue(const std::string &key, const T &value){
            std::stringstream ss;
            ss << value;
            _cfg[key] = ss.str();
            fireEvent(ConfigSetEvent(std::make_pair(key, ss.str())));
        }
        template<>
        void setValue<std::string>(const std::string &key, const std::string &value){
            _cfg[key] = value;
            fireEvent(ConfigSetEvent(std::make_pair(key, value)));
        }
        
        void readFromFile(const std::string &fileName);
        void saveToFile(const std::string &fileName, std::ios::openmode om= std::ios::trunc);

        friend std::ostream &operator<<(std::ostream &out, const Config &c);

        class ConfigSetEvent : public Event{
            public:
                ConfigSetEvent(const std::pair<std::string, std::string> &pair);
            private:
                std::pair<std::string, std::string> _pair;
                void *_dataToVoidPointer() const;
        };
        class ConfigFileEvent : public Event{
            public:
                struct Data{
                    std::string fileName;
                    char operation;
                    bool success;
                };
                ConfigFileEvent(std::string fileName, char operation, bool success);
            private:
                std::string _fileName;
                char _operation;
                bool _success;
                void *_dataToVoidPointer() const;
        };
    private:
        RawData _cfg;
};
#endif//CONFIG_HPP
