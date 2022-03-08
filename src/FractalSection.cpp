/****Copyright (C) 2022  eggplantcx
 **FractalSection.cpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#include "../inc/FractalSection.hpp"
#include "../inc/Util.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>


FractalSection::FractalSection(){
    _registerCallbacks();
}
FractalSection::FractalSection(Point p){ 
    auto prec = std::stod(_settings.getValue("precision"));
    _settings.setValue("start.real", Util::trimDouble(p.real(), prec));
    _settings.setValue("start.imag", Util::trimDouble(p.imag(), prec));
    _registerCallbacks(); 
}
FractalSection::FractalSection(Point p, double step, unsigned int iterations, unsigned int stepsReal, 
    unsigned int stepsImag){
        auto prec = std::stod(_settings.getValue("precision"));
        _settings.setValue("start.real", Util::trimDouble(p.real(), prec));
        _settings.setValue("start.imag", Util::trimDouble(p.imag(), prec));
        _settings.setValue("iters", iterations);
        _settings.setValue("steps.real", stepsReal);
        _settings.setValue("steps.imag", stepsImag);
        _registerCallbacks();
}

FractalSection::FractalSection(const FractalSection &other){
    *this = other;
}

FractalSection &FractalSection::operator=(const FractalSection &other){
    _cfg = other._cfg;
    _settings = other._settings;
    _raw = other._raw;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const FractalSection &m){
    if(m._raw.size() == 0){ return out; }
    
    int realSize = m._raw[0].size();
    auto &cfg = m._cfg;
    auto &settings = m._settings;

    double step = std::stod(settings.getValue("step"));
    double prec = std::stoi(cfg.getValue("precision"));
    double imag = std::stod(settings.getValue("start.imag"));
    double real = std::stod(settings.getValue("start.real"));

    double realLength = realSize/2 * (step * std::stod(cfg.getValue("realStepMult")));
    double imagLength = m._raw.size()/2 * step;
    std::cout << "^\n" << Util::trimDouble(imag + imagLength, prec) << "i\n";
    
    std::stringstream ss;
    ss << Util::trimDouble( - imagLength, prec) << 'i';
    std::string imagStr = ss.str();
    
    ss.clear();
    ss.str(std::string());
    ss << Util::trimDouble(real - realLength, prec);
    std::string realStr = ss.str();
    
    int k = 0;
    for(int i = 0; i < m._raw.size(); i++){
        if(i >= m._raw.size() - imagStr.size()){
            std::cout << imagStr[k++];
        }
        else{ std::cout << '|'; }
        for(int j = 0; j < realSize; j++){
            if(i == m._raw.size()/2 && j == realSize/2){ std::cout << char(178); }
            else{ std::cout << m._raw[i][j]; }
        }
        std::cout << '\n';
    }

    std::cout << '=' << realStr;
    
    for(int i = realSize - realStr.size() - 1; i > 0; i--){
        std::cout << '=';
    }
    
    std::cout << Util::trimDouble(real + realLength, prec) << ">\n";
    return out;
}

void FractalSection::printInfo(std::ostream &out){
    double prec = std::stod(_cfg.getValue("precision"));
    double real = std::stod(_settings.getValue("start.real"));
    double imag = std::stod(_settings.getValue("start.imag"));
    double step = std::stod(_settings.getValue("step"));
    double realStepMult = std::stod(_cfg.getValue("realStepMult"));
    unsigned int stepsR = std::stoi(_cfg.getValue("steps.real")), stepsI = std::stoi(_cfg.getValue("steps.imag"));
    out << "Real(x): " << Util::trimDouble(real - (stepsR * step), prec) << " < " << 
    Util::trimDouble(real, prec) << " > " << Util::trimDouble(real + (stepsR * step), prec) << '\n';
    
    out << "Imag(y): " << Util::trimDouble(imag - (stepsI * step), prec) << " < " << 
    Util::trimDouble(imag, prec) << " > " << Util::trimDouble(imag + (stepsI * step), prec) << '\n';
    
    out << "Real Step: " << Util::trimDouble(step * realStepMult, prec) << " (" << 
    Util::trimDouble(realStepMult, prec) << "x)\n";

    out << "Imag Step: " << Util::trimDouble(step, prec) << '\n';
    
    out << "Iter: " << std::stoi(_settings.getValue("iters")) << '\n';
}

bool FractalSection::save(const std::string &fileName, bool pathRelativeToSavePath, std::ios::openmode om){
    std::string filePath = pathRelativeToSavePath ? _cfg.getValue("savePath") + fileName : fileName;
    std::ofstream out(filePath, om);
    if(_logIf(!out.is_open(), std::cerr, "FractalSection::save: Unable to open \"" + filePath + "\" for saving")){ 
        return false;
    }
    _save(out);
    out.close();
    return true;
}
bool FractalSection::load(const std::string &fileName, bool pathRelativeToSavePath){
    std::string filePath = pathRelativeToSavePath ? _cfg.getValue("savePath") + fileName : fileName;
    std::ifstream in(filePath);
    if(_logIf(!in.is_open(), std::cerr, "FractalSection::load: Unable to open \"" + filePath + "\" for loading")){  
        return false;
    }

    while(!in.eof()){
        bool result = _load(in);
        if(!result){ break; }
    }
    in.close();
    return true;
}

bool FractalSection::createAnimation(const std::vector<double> &steps, const std::string &fileName, bool pathRelativeToSavePath){
    std::string filePath = pathRelativeToSavePath ? _cfg.getValue("savePath") + fileName : fileName;
    std::ofstream out(filePath, std::ios::app);
    if(_logIf(!out.is_open(), std::cerr, "FractalSection::createAnimation: Error opening \"" + filePath + "\"")){
        return false;
    }
    std::cout << "Calculating: ";
    Util::printProgressBar(std::cout, 0, steps.size());
    for(size_t i = 0; i < steps.size(); i++){
        Util::setProgressBar(std::cout, i, steps.size());
        _settings.setValue("step", Util::trimDouble(steps[i], std::stoi(_cfg.getValue("precision"))));
        calculate();
        _save(out);
    }
    Util::setProgressBar(std::cout, steps.size(), steps.size());
    out.close();
    return true;
}
bool FractalSection::loadAnimation(const std::string &fileName, bool pathRelativeToSavePath){
    std::string filePath = pathRelativeToSavePath ? _cfg.getValue("savePath") + fileName : fileName;
    std::ifstream in(filePath);
    if(_logIf(!in.is_open(), std::cerr, "FractalSection::loadAnimation: Unable to open \"" + filePath + "\" for loading")){  
        return false;
    }
    bool reCalcBefore = _cfg.getValue("reCalcOnChange") == "0" ? 0 : 1;
    _cfg.setValue("reCalcOnChange", 0);
    while(!in.eof()){
        bool result = _load(in);
        if(!result){ break; }
        std::cout << "Step: " << _settings.getValue("step") << '\n';
        std::cout << *this;
        Util::pause();
    }
    in.close();
    _cfg.setValue("reCalcOnChange", reCalcBefore);
    return true;    
}

void FractalSection::calculate(){
    _raw.clear();
    double step = std::stod(_settings.getValue("step"));
    double iSize = std::stoi(_cfg.getValue("steps.imag")) * step;
    double rStep = step * std::stod(_cfg.getValue("realStepMult"));
    double rSize = std::stoi(_cfg.getValue("steps.real")) * rStep;

    double imag = std::stod(_settings.getValue("start.imag"));
    double real = std::stod(_settings.getValue("start.real"));

    for(double i = imag + iSize; i >= imag - iSize; i -= step){
        std::vector<char> v;
        for(double r = real - rSize; r <= real + rSize; r += rStep){
            //std::cout << r << ", " << i << '\n';
            v.push_back(calculatePoint(r, i, std::stoi(_settings.getValue("iterations"))));
        }
        _raw.push_back(v);
    }  
}

char FractalSection::calculatePoint(const std::complex<double> &c, const unsigned int iterations){
    return _func(c, iterations);
}
char FractalSection::calculatePoint(double real, double imag, unsigned int iterations){ 
    return calculatePoint(std::complex<double>(real, imag), iterations); 
}

FractalSection::RawData FractalSection::getRaw(){ return _raw;}
double FractalSection::getReal(){ return _start.real(); }
double FractalSection::getImag(){ return _start.imag(); }
FractalSection::Point FractalSection::getStart(){ return _start; }
double FractalSection::getStep(){ return _step; }
unsigned int FractalSection::getIterations(){ return _iterations; }
unsigned int FractalSection::getRealSteps(){ return _stepsR; }
unsigned int FractalSection::getImagSteps(){ return _stepsI; }
bool FractalSection::getReCalcOnChange(){ return _reCalcOnChange; }
bool FractalSection::getLogging(){ return _logging; }
std::string FractalSection::getSavePath(){ return _cfg.getValue("savePath"); }
double FractalSection::getRealStepMult(){ return _realStepMult; }
std::streamsize FractalSection::getPrecision(){ return _precision; }


void FractalSection::setReal(double r){
    _start.real(r);
    if(_reCalcOnChange){ calculate(); }
}

void FractalSection::setImag(double i){
    _start.imag(i);
    if(_reCalcOnChange){ calculate(); }
}

void FractalSection::setStart(const Point &p){
    _start = p;
    if(_reCalcOnChange){ calculate(); }
}
void FractalSection::setStep(double s){
    _step = s;
    if(_reCalcOnChange){ calculate(); }
}
void FractalSection::setIterations(unsigned int i){
    _iterations = i;
    if(_reCalcOnChange){ calculate(); }
}
void FractalSection::setRealSteps(unsigned int rs){
    _stepsR = rs;
    _cfg.setValue("realSteps", rs);
    if(_reCalcOnChange){ calculate(); }
}
void FractalSection::setImagSteps(unsigned int is){
    _stepsI = is;
    _cfg.setValue("imagSteps", is);
    if(_reCalcOnChange){ calculate(); }
}
void FractalSection::setReCalcOnChange(bool re){ 
    _reCalcOnChange = re; 
    _cfg.setValue("reCalcOnChange", re);
}
void FractalSection::setLogging(bool l){ 
    _logging = l; 
    _cfg.setValue("logging", l);
}
//void FractalSection::setSavePath(const std::string &s){ _cfg.getValue("savePath") = s; }
void FractalSection::setRealStepMult(double r){ 
    _realStepMult = r; 
    _cfg.setValue("realStepMult", Util::trimDouble(r));
}
void FractalSection::setPrecision(std::streamsize p){ 
    _precision = p; 
    _cfg.setValue("precision", p);
}

void FractalSection::_loadConfig(const Config &c){
    auto rd = c.getCfg();
}

FractalSection::~FractalSection() = default;

void FractalSection::_save(std::ostream &out){;  
    out << "!\n";
    out << 'r' << Util::trimDouble(_start.real(), _precision) << '\n';
    out << 'i' << Util::trimDouble(_start.imag(), _precision) << '\n';
    out << "sr" << Util::trimDouble(_realStepMult, _precision) << '\n';
    out << "si" << Util::trimDouble(_step, _precision) << '\n';
    out << 't' << _iterations << '\n';
    out << "[\n";
    for(auto &v : _raw){
        for(auto b : v){
            out << b;
        }
        out << '\n';
    }
    out << "]\n_\n"; 
}

bool FractalSection::_load(std::istream &in){
    std::string s;
    
    bool readingData = false, prevState = false;
    
    while(true){
        in >> s;
        if(s == "_"){ return true; }

        if(std::isspace(s[0]) || !s.size()){ return false; }

        if(prevState != readingData){
            prevState = readingData;
            _raw.clear();
        }

        if(readingData && s[0] != ']'){
            std::vector<char> data;
            for(auto c : s){
                data.push_back(c);
            }
            _raw.push_back(data);
        }

        switch(s[0]){
            case 'r':
                _start.real(std::stod(s.substr(1)));
                break;
            case 'i':
                _start.imag(std::stod(s.substr(1)));
                break;
            case 's':
                switch(s[1]){
                    case 'r':
                        _realStepMult = std::stod(s.substr(2));
                        break;
                    case 'i':
                        _step = std::stod(s.substr(2));
                        break;
                    default:
                        _realStepMult = 1;
                        _step = std::stod(s.substr(1));
                }
                break;
            case 't':
                _iterations = std::stoi(s.substr(1));
                break;
            case '[':
                prevState = readingData;
                readingData = true;
                break;
            case ']':
                prevState = readingData;
                readingData = false;
                break;

        }
    }    
}

void FractalSection::_log(std::ostream &out, const std::string &str){
    if(!_logging){ return; }
    out << str << '\n';
}

bool FractalSection::_logIf(bool log, std::ostream &out, const std::string &str){
    if(!log){ return false; }
    _log(out, str);
    return true;
}

void FractalSection::_registerCallbacks(){
    Callback c;
    c.types.push_back(ALL_EVENTS);
    c.call = [this](void *data){//pass events from config to FractalSection's event handler
        if(data == nullptr){ return; }
        fireEvent(static_cast<AllEventsData*>(data));
    };
    _cfgCallbackID = _cfg.registerCallback(c);
    
    c.call = [this](void *data){
        if(data == nullptr){ return; }
        auto d = static_cast<AllEventsData*>(data);
        d->type = "Settings" + d->type;
        fireEvent(d);
    };
    _settingsCallbackID = _settings.registerCallback(c);
}