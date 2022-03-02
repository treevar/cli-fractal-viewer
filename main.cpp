#include <iostream>
#include <complex>
#include <vector>
#include <sstream>
#include <fstream>

const unsigned int STEPS_X = 125, STEPS_Y = 35;

std::string getNextLine(){
    std::string str;
    std::getline(std::cin, str);
    return str;
}

void pause(){
    std::cout << "Press Enter to Continue...";
    getNextLine();
}

bool isInMandelbrotSet(const std::complex<double> &c, const unsigned int iterations){
    std::complex<double> z(0, 0);
    for(unsigned int i = 0; i < iterations; i++){
        z = (z * z) + c;
        if(std::abs(z) > 2.0){ return false; }
    }
    return true;    
}

bool isInMandelbrotSet(double real, double imag, unsigned int iterations){ return isInMandelbrotSet(std::complex<double>(real, imag), iterations); }

//TODO
//make into class, put all mandel functions in it
struct MandelbrotSection{
    std::vector<std::vector<bool> > raw;
    std::complex<double> start;
    double step;
    unsigned int iterations;
};

void makeMandelbrot(MandelbrotSection &m, unsigned int rSteps, unsigned int iSteps){
    m.raw.clear();
    double iSize = iSteps * m.step;
    double rSize = rSteps * m.step;
    for(double i = m.start.imag() + iSize; i >= m.start.imag() - iSize; i -= m.step){
        std::vector<bool> v;
        for(double r = m.start.real() - rSize; r <= m.start.real() + rSize; r += m.step){
            //std::cout << r << ", " << i << '\n';
            v.push_back(isInMandelbrotSet(r, i, m.iterations));
        }
        m.raw.push_back(v);
    }  
}

void printMandel(const MandelbrotSection &m){
    for(auto &v : m.raw){
        for(bool b : v){
            std::cout << (b ? '*' : ' ');
        }
        std::cout << '\n';
    }
}


//.38016, .36
void printMandelWithXY(const MandelbrotSection &m){
    if(m.raw.size() == 0){ return; }
    
    int realSize = m.raw[0].size();
    double realLength = realSize/2 * m.step;
    double imagLength = m.raw.size()/2 * m.step;
    std::cout << "^\n" << (m.start.imag() + imagLength) << "i\n";
    
    std::stringstream ss;
    ss << (m.start.imag() - imagLength) << 'i';
    std::string imagStr = ss.str();
    
    ss.clear();
    ss.str(std::string());
    ss << (m.start.real() - realLength);
    std::string realStr = ss.str();
    
    int k = 0;
    for(int i = 0; i < m.raw.size(); i++){
        if(i >= m.raw.size() - imagStr.size()){
            std::cout << imagStr[k++];
        }
        else{ std::cout << '|'; }
        for(int j = 0; j < realSize; j++){
            if(i == m.raw.size()/2 && j == realSize/2){ std::cout << '\\'; }
            else{ std::cout << (m.raw[i][j] ? '*' : ' '); }
        }
        std::cout << '\n';
    }

    std::cout << '=' << realStr;
    
    for(int i = realSize - realStr.size() - 1; i > 0; i--){
        std::cout << '=';
    }
    
    std::cout << (m.start.real() + realLength) << ">\n";
}

void saveMandelbrotSection(const MandelbrotSection &m, const std::string &fileName){
    std::ofstream out;
    out.open(fileName, std::ios::app);
    if(!out.is_open()){ 
        std::cerr << "saveMandelbrotSection: Unable to open " << fileName << " for saving\n"; 
        return;
    }
    out << "!\n";
    out << 'r' << m.start.real() << '\n';
    out << 'i' << m.start.imag() << '\n';
    out << 's' << m.step << '\n';
    out << 't' << m.iterations << '\n';
    out << "[\n";
    for(auto &v : m.raw){
        for(auto b : v){
            out << b ? '1' : '0';
        }
        out << '\n';
    }
    out << "]\n_\n";
    out.close();
}

MandelbrotSection loadMandelbrotSection(std::ifstream &in){
    MandelbrotSection m;
    if(!in.is_open()){ 
        std::cerr << "loadMandelbrotSection: Unable to open file for loading\n"; 
        return m;
    }
    std::string s;
    bool readingData = false;
    while(!in.eof()){
        in >> s;
        if(s == "_"){ break; }
        if(readingData && s[0] != ']'){
            std::vector<bool> data;
            for(auto c : s){
                data.push_back(c == '1' ? 1 : 0);
            }
            m.raw.push_back(data);
        }
        switch(s[0]){
            case 'r':
                m.start.real(std::stod(s.substr(1)));
                break;
            case 'i':
                m.start.imag(std::stod(s.substr(1)));
                break;
            case 's':
                m.step = std::stod(s.substr(1));
                break;
            case 't':
                m.iterations = std::stoi(s.substr(1));
                break;
            case '[':
                readingData = true;
                break;
            case ']':
                readingData = false;
                break;
        }
    }
    return m;
}

MandelbrotSection loadMandelbrotSection(const std::string &fileName){
    std::ifstream in(fileName);
    auto m = loadMandelbrotSection(in);
    in.close();
    return m;
}

std::vector<MandelbrotSection> readAnimationFile(const std::string &fileName){
    std::vector<MandelbrotSection> v;
    std::ifstream in(fileName);
    while(!in.eof()){
        v.push_back(loadMandelbrotSection(in));
    }
    in.close();
    return v;
}

// std::vector<std::vector<bool> > raw;
//    std::complex<double> start;
//    double step;
//    unsigned int iterations;
//TODO
//Add input validation
//

bool isDouble(const std::string &str){
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

bool inRange(std::string d, double min, double max){ 
    if(!isDouble(d)){ return false; }
    double du = std::stod(d);
    return du >= min && du <= max;
}

double promptWithCheck(const std::string &prompt, double min = -100, double max = 100){
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

bool validMenuOption(const std::vector<std::pair<std::string, std::string>> &v, const std::string &in){
    for(auto &op : v){
        if(op.first == in){ return true; }
    }
    return false;
}

std::string menuWithCheck(const std::vector<std::pair<std::string, std::string>> &v){
    std::cout << "Select Option:\n";
    for(auto &op : v){
        std::cout << '\t' << op.first << " = " << op.second << '\n';
    }
    std::cout << "> ";
    std::string in = {""};
    in = getNextLine();
    while(!validMenuOption(v, in)){
        std::cout << "\nInvalid Choice\nSelect Option:\n";
        for(auto &op : v){
            std::cout << '\t' << op.first << " = " << op.second << '\n';
        }
        std::cout << "> ";
        in = getNextLine();
    }
    return in;
}

void makeAnimation(MandelbrotSection m, double zoomStep, unsigned int zooms, const std::string &fileName){
    for(unsigned int i = 0; i < zooms; i++){
        m.step = i * zoomStep;
        makeMandelbrot(m, STEPS_X, STEPS_Y);
        saveMandelbrotSection(m, fileName);
    }
}

void playAnimation(const std::string &fileName){
    std::ifstream in(fileName);
    if(!in.is_open()){
        std::cerr << "playAnimation: Can't open " << fileName << " for playback\n";
        return;
    }
    while(!in.eof()){
        printMandelWithXY(loadMandelbrotSection(in));
    }
}

int main(){
    unsigned int iters = 255;
    //i.32, r.37
    MandelbrotSection m;
    m = loadMandelbrotSection("saves/def.txt");
    /*m.start.real(0);
    m.start.imag(0);
    m.step = 0.027;
    m.iterations = 255;*/
    std::string op;
    bool quit = false;
    std::string fileName;
    do{
        op = menuWithCheck({{"p", "Set Point"}, {"z", "Set Zoom"}, {"i", "Set iterations"}, {"s", "Save"}, {"l", "Load"}, {"la", "Load Animation"}, {"a", "Make Animation"}, {"d", "Display"}, {"di", "Display Info"}, {"dr", "Display Without Computing"}, {"q", "Quit"}});
        if(op == "p"){
            m.start.real(promptWithCheck("Enter real: "));
            m.start.imag(promptWithCheck("Enter imaginary: "));
        }
        else if(op == "z"){
            m.step = promptWithCheck("Enter step size: ");
        }
        else if(op == "i"){
            m.iterations = promptWithCheck("Enter iterations: ", 0, 10000000);
        }
        else if(op == "s"){
            std::cout << "Enter Filename: ";
            fileName = getNextLine();
            saveMandelbrotSection(m, fileName);
            std::cout << "Saved to " << fileName << "\n\n";
        }
        else if(op == "l"){
            std::cout << "Enter Filename: ";
            fileName = getNextLine();
            m = loadMandelbrotSection(fileName);
            std::cout << "Loaded from " << fileName << "\n\n";
        }
        else if(op == "la"){
            std::cout << "Enter Filename: ";
            fileName = getNextLine();
            std::ifstream in(fileName);
            if(!in.is_open()){
                std::cerr << "Error opening " << fileName << '\n';
                break;
            }
            do{
                MandelbrotSection man = loadMandelbrotSection(in);
                if(man.raw.size() == 0){ break; }
                m = man;
                printMandelWithXY(m);
                pause();
            }while(!in.eof());
        }
        else if(op == "a"){
            double zStep = promptWithCheck("Enter Zoom Step: ");
            double nSteps = promptWithCheck("Enter Number of Steps: ");
            std::cout << "Enter Filename: ";
            fileName = getNextLine();
            makeAnimation(m, zStep, nSteps, fileName);
        }
        else if(op == "d"){
            std::cout << "Real(x): " << m.start.real() - (STEPS_X * m.step) << " < " << m.start.real() << " > " << 
                                        m.start.real() + (STEPS_X * m.step) << '\n';
            
            std::cout << "Imag(y): " << m.start.imag() - (STEPS_Y * m.step) << " < " << m.start.imag() << " > " << 
                                        m.start.imag() + (STEPS_Y * m.step) << '\n';
            
            std::cout << "Step: " << m.step << '\n';
            
            std::cout << "Iter: " << m.iterations << '\n';
            
            makeMandelbrot(m, STEPS_X, STEPS_Y);
            printMandelWithXY(m);
        }
        else if(op == "di"){
            std::cout << "Real(x): " << m.start.real() - (STEPS_X * m.step) << " < " << m.start.real() << " > " << 
                                        m.start.real() + (STEPS_X * m.step) << '\n';
            
            std::cout << "Imag(y): " << m.start.imag() - (STEPS_Y * m.step) << " < " << m.start.imag() << " > " << 
                                        m.start.imag() + (STEPS_Y * m.step) << '\n';
            
            std::cout << "Step: " << m.step << '\n';
            
            std::cout << "Iter: " << m.iterations << '\n';
        }
        else if(op == "dr"){
            printMandelWithXY(m);
        }
        else if(op == "q"){
            quit = true;
        }
        else{
            std::cout << "Invalid option\n";
        }
        std::cout << '\n';
    }while(!quit);
}