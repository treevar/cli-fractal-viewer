#include "../inc/MandelbrotSection.hpp"
#include "../inc/Util.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>


MandelbrotSection::MandelbrotSection() = default;
MandelbrotSection::MandelbrotSection(Point p) : _start(p) {}
MandelbrotSection::MandelbrotSection(Point p, double step, unsigned int iterations, unsigned int stepsReal, unsigned int stepsImag) :
_start(p), _step(step), _iterations(iterations), _stepsR(stepsReal), _stepsI(stepsImag)
{}
MandelbrotSection::MandelbrotSection(const MandelbrotSection &other){
    *this = other;
}

MandelbrotSection &MandelbrotSection::operator=(const MandelbrotSection &other){
    _start = other._start;
    _step = other._step;
    _iterations = other._iterations;
    _stepsR = other._stepsR;
    _stepsI = other._stepsI;
    _raw = other._raw;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const MandelbrotSection &m){
    if(m._raw.size() == 0){ return out; }
    
    int realSize = m._raw[0].size();
    double realLength = realSize/2 * (m._step * m._realStepMult);
    double imagLength = m._raw.size()/2 * m._step;
    std::cout << "^\n" << Util::trimDouble(m._start.imag() + imagLength, m._precision) << "i\n";
    
    std::stringstream ss;
    ss << Util::trimDouble(m._start.imag() - imagLength, m._precision) << 'i';
    std::string imagStr = ss.str();
    
    ss.clear();
    ss.str(std::string());
    ss << Util::trimDouble(m._start.real() - realLength, m._precision);
    std::string realStr = ss.str();
    
    int k = 0;
    for(int i = 0; i < m._raw.size(); i++){
        if(i >= m._raw.size() - imagStr.size()){
            std::cout << imagStr[k++];
        }
        else{ std::cout << '|'; }
        for(int j = 0; j < realSize; j++){
            if(i == m._raw.size()/2 && j == realSize/2){ std::cout << char(178); }
            else{ std::cout << (m._raw[i][j] ? ' ' : char(177)); }
        }
        std::cout << '\n';
    }

    std::cout << '=' << realStr;
    
    for(int i = realSize - realStr.size() - 1; i > 0; i--){
        std::cout << '=';
    }
    
    std::cout << Util::trimDouble(m._start.real() + realLength, m._precision) << ">\n";
    return out;
}

void MandelbrotSection::printInfo(std::ostream &out){
    out << "Real(x): " << Util::trimDouble(_start.real() - (_stepsR * _step), _precision) << " < " << 
    Util::trimDouble(_start.real(), _precision) << " > " << Util::trimDouble(_start.real() + (_stepsR * _step), _precision) << '\n';
    
    out << "Imag(y): " << Util::trimDouble(_start.imag() - (_stepsI * _step), _precision) << " < " << 
    Util::trimDouble(_start.imag(), _precision) << " > " << Util::trimDouble(_start.imag() + (_stepsI * _step), _precision) << '\n';
    
    out << "Real Step: " << Util::trimDouble(_step * _realStepMult, _precision) << " (" << 
    Util::trimDouble(_realStepMult, _precision) << "x)\n";

    out << "Imag Step: " << Util::trimDouble(_step, _precision) << '\n';
    
    out << "Iter: " << _iterations << '\n';
}

bool MandelbrotSection::save(const std::string &fileName, bool pathRelativeToSavePath, std::ios::openmode om){
    std::string filePath = pathRelativeToSavePath ? _savePath + fileName : fileName;
    std::ofstream out(filePath, om);
    if(_logIf(!out.is_open(), std::cerr, "MandelbrotSection::save: Unable to open \"" + filePath + "\" for saving")){ 
        return false;
    }
    _save(out);
    out.close();
    return true;
}
bool MandelbrotSection::load(const std::string &fileName, bool pathRelativeToSavePath){
    std::string filePath = pathRelativeToSavePath ? _savePath + fileName : fileName;
    std::ifstream in(filePath);
    if(_logIf(!in.is_open(), std::cerr, "MandelbrotSection::load: Unable to open \"" + filePath + "\" for loading")){  
        return false;
    }

    while(!in.eof()){
        bool result = _load(in);
        if(!result){ break; }
    }
    in.close();
    return true;
}

bool MandelbrotSection::createAnimation(const std::vector<double> &steps, const std::string &fileName, bool pathRelativeToSavePath){
    std::string filePath = pathRelativeToSavePath ? _savePath + fileName : fileName;
    std::ofstream out(filePath, std::ios::app);
    if(_logIf(!out.is_open(), std::cerr, "MandelbrotSection::createAnimation: Error opening \"" + filePath + "\"")){
        return false;
    }
    std::cout << "Calculating: ";
    Util::printLoadBar(std::cout, 0, steps.size());
    for(size_t i = 0; i < steps.size(); i++){
        Util::setLoadBar(std::cout, i, steps.size());
        _step = steps[i];
        calculate();
        _save(out);
    }
    Util::setLoadBar(std::cout, steps.size(), steps.size());
    out.close();
    return true;
}
bool MandelbrotSection::loadAnimation(const std::string &fileName, bool pathRelativeToSavePath){
    std::string filePath = pathRelativeToSavePath ? _savePath + fileName : fileName;
    std::ifstream in(filePath);
    if(_logIf(!in.is_open(), std::cerr, "MandelbrotSection::loadAnimation: Unable to open \"" + filePath + "\" for loading")){  
        return false;
    }
    bool reCalcBefore = _reCalcOnChange;
    _reCalcOnChange = false;
    while(!in.eof()){
        bool result = _load(in);
        if(!result){ break; }
        std::cout << "Step: " << Util::trimDouble(_step, _precision) << '\n';
        std::cout << *this;
        Util::pause();
    }
    in.close();
    _reCalcOnChange = reCalcBefore;
    return true;    
}

void MandelbrotSection::calculate(){
    _raw.clear();
    double iSize = _stepsI * _step;
    double rStep = _step * _realStepMult;
    double rSize = _stepsR * rStep;
    for(double i = _start.imag() + iSize; i >= _start.imag() - iSize; i -= _step){
        std::vector<bool> v;
        for(double r = _start.real() - rSize; r <= _start.real() + rSize; r += rStep){
            //std::cout << r << ", " << i << '\n';
            v.push_back(isInMandelbrotSet(r, i, _iterations));
        }
        _raw.push_back(v);
    }  
}

bool MandelbrotSection::isInMandelbrotSet(const std::complex<double> &c, const unsigned int iterations){
    std::complex<double> z(0, 0);
    for(unsigned int i = 0; i < iterations; i++){
        z = (z * z) + c;
        if(std::abs(z) > 2.0){ return false; }
    }
    return true;
}
bool MandelbrotSection::isInMandelbrotSet(double real, double imag, unsigned int iterations){ 
    return isInMandelbrotSet(std::complex<double>(real, imag), iterations); 
}

MandelbrotSection::RawData MandelbrotSection::getRaw(){ return _raw;}
double MandelbrotSection::getReal(){ return _start.real(); }
double MandelbrotSection::getImag(){ return _start.imag(); }
MandelbrotSection::Point MandelbrotSection::getStart(){ return _start; }
double MandelbrotSection::getStep(){ return _step; }
unsigned int MandelbrotSection::getIterations(){ return _iterations; }
unsigned int MandelbrotSection::getRealSteps(){ return _stepsR; }
unsigned int MandelbrotSection::getImagSteps(){ return _stepsI; }
bool MandelbrotSection::getReCalcOnChange(){ return _reCalcOnChange; }
bool MandelbrotSection::getLogging(){ return _logging; }
std::string MandelbrotSection::getSavePath(){ return _savePath; }
double MandelbrotSection::getRealStepMult(){ return _realStepMult; }
std::streamsize MandelbrotSection::getPrecision(){ return _precision; }


void MandelbrotSection::setReal(double r){
    _start.real(r);
    if(_reCalcOnChange){ calculate(); }
}

void MandelbrotSection::setImag(double i){
    _start.imag(i);
    if(_reCalcOnChange){ calculate(); }
}

void MandelbrotSection::setStart(const Point &p){
    _start = p;
    if(_reCalcOnChange){ calculate(); }
}
void MandelbrotSection::setStep(double s){
    _step = s;
    if(_reCalcOnChange){ calculate(); }
}
void MandelbrotSection::setIterations(unsigned int i){
    _iterations = i;
    if(_reCalcOnChange){ calculate(); }
}
void MandelbrotSection::setRealSteps(unsigned int rs){
    _stepsR = rs;
    if(_reCalcOnChange){ calculate(); }
}
void MandelbrotSection::setImagSteps(unsigned int is){
    _stepsI = is;
    if(_reCalcOnChange){ calculate(); }
}
void MandelbrotSection::setReCalcOnChange(bool re){ _reCalcOnChange = re; }
void MandelbrotSection::setLogging(bool l){ _logging = l; }
void MandelbrotSection::setSavePath(const std::string &s){ _savePath = s; }
void MandelbrotSection::setRealStepMult(double r){ _realStepMult = r; }
void MandelbrotSection::setPrecision(std::streamsize p){ _precision = p; }

MandelbrotSection::~MandelbrotSection() = default;

void MandelbrotSection::_save(std::ostream &out){;  
    out << "!\n";
    out << 'r' << Util::trimDouble(_start.real(), _precision) << '\n';
    out << 'i' << Util::trimDouble(_start.imag(), _precision) << '\n';
    out << "sr" << Util::trimDouble(_realStepMult, _precision) << '\n';
    out << "si" << Util::trimDouble(_step, _precision) << '\n';
    out << 't' << _iterations << '\n';
    out << "[\n";
    for(auto &v : _raw){
        for(auto b : v){
            out << b ? '1' : '0';
        }
        out << '\n';
    }
    out << "]\n_\n"; 
}

bool MandelbrotSection::_load(std::istream &in){
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
            std::vector<bool> data;
            for(auto c : s){
                data.push_back(c == '1' ? 1 : 0);
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

void MandelbrotSection::_log(std::ostream &out, const std::string &str){
    if(!_logging){ return; }
    out << str << '\n';
}

bool MandelbrotSection::_logIf(bool log, std::ostream &out, const std::string &str){
    if(!log){ return false; }
    _log(out, str);
    return true;
}