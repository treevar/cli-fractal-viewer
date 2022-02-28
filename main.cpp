#include <iostream>
#include <complex>
#include <vector>
#include <sstream>

bool isInMandelbrotSet(const std::complex<double> &c, const unsigned int iterations){
    std::complex<double> z(0, 0);
    for(unsigned int i = 0; i < iterations; i++){
        z = (z * z) + c;
        if(std::abs(z) > 2.0){ return false; }
    }
    return true;    
}

bool isInMandelbrotSet(double real, double imag, unsigned int iterations){ return isInMandelbrotSet(std::complex<double>(real, imag), iterations); }

struct MandelbrotSection{
    std::vector<std::vector<bool> > raw;
    std::complex<double> start;
    double step;
    unsigned int iterations;
};

MandelbrotSection makeMandelbrot(double start, double end, double step, unsigned int iters){
    MandelbrotSection m;
    for(double imag = start; imag < end; imag += step){
        std::vector<bool> v;
        for(double real = start; real < end; real += step){
            v.push_back(isInMandelbrotSet(real, imag, iters));
        }
        m.raw.push_back(v);
    }
    return m;
}

MandelbrotSection makeMandelbrot(double real, double imag, double rSize, double iSize, double step, unsigned int iters){
    MandelbrotSection m;
    for(double i = imag + iSize; i > imag - iSize; i -= step){
        std::vector<bool> v;
        for(double r = real - rSize; r < real + rSize; r += step){
            //std::cout << i << ", " << r << '\n';
            bool b = isInMandelbrotSet(r, i, iters);
            //std::cout << r << ", " << i << "i " << (b ? "IN\n" : "OUT\n");
            v.push_back(b);
        }
        m.raw.push_back(v);
    }
    return m;
}

MandelbrotSection makeMandelbrot(double real, double imag, double step, unsigned int rSteps, unsigned int iSteps, unsigned int iterations){
    MandelbrotSection m;
    double iSize = iSteps * step;
    double rSize = rSteps * step;
    for(double i = imag - iSize; i <= imag + iSize; i += step){
        std::vector<bool> v;
        for(double r = real - rSize; r <= real + rSize; r += step){
            //std::cout << r << ", " << i << '\n';
            v.push_back(isInMandelbrotSet(r, i, iterations));
        }
        m.raw.push_back(v);
    }
    return m;    
}

void printMandel(const MandelbrotSection &m){
    for(auto &v : m.raw){
        for(bool b : v){
            std::cout << (b ? '*' : ' ');
        }
        std::cout << '\n';
    }
}



void printMandelWithXY(const MandelbrotSection &m){
    std::cout << "^\n" << (m.raw.size() * m.step) << '\n';
    std::stringstream ss;
    ss << m.start.imag();
    std::string imagStr = ss.str();
    ss.ignore();
    ss << (m.start.real() + (m.raw[0].size() * m.step));
    std::string realStr = ss.str();
    for(int i = 0; i < m.raw.size(); i++){
        std::cout << '|';
        for(int j = 0; j < m.raw[i].size(); j++){
            std::cout << (m.raw[i][j] ? '*' : ' ');
        }
        std::cout << '\n';
    }
    ss.ignore();
    std::cout << '=' << realStr;
    for(int i = m.raw[0].size() - realStr.size(); i > 0; i--){
        std::cout << '-';
    }
    std::cout << '\n';
}

//TODO
//Add input validation
//Add save/load system

int main(){
    unsigned int iters = 255;
    const unsigned int STEPS_X = 125, STEPS_Y = 35;
    //i.32, r.37
    while(1){
        std::cout << "Select mode[p/s/o]: ";
        char mode = 0;
        std::cin >> mode;
        MandelbrotSection m;
        double start, end, step;
        double i, r, rS, iS;
        double dIn;
        switch(mode){
            case 's':
                std::cout << "Enter start: ";
                std::cin >> start;
                std::cout << "Enter end: ";
                std::cin >> end;
                std::cout << "Enter step: ";
                std::cin >> step;
                std::cout << "Enter iterations: ";
                std::cin >> iters;
                m = makeMandelbrot(start, end, step, iters);
                break;
            case 'p':
                std::cout << "Enter Real: ";
                std::cin >> r;
                std::cout << "Enter imaginary: ";
                std::cin >> i;
                std::cout << "Enter Step: ";
                std::cin >> step;
                std::cout << "Enter Real Size: ";
                std::cin >> rS;
                std::cout << "Enter Imaginary Size: ";
                std::cin >> iS;
                std::cout << "Enter Iterations: ";
                std::cin >> iters;
                m = makeMandelbrot(r, i, rS, iS, step, iters);
                break;
            case 'o':
                std::cout << "Enter real: ";
                std::cin >> r;
                std::cout << "Enter imag: ";
                std::cin >> i;
                std::cout << r << ", " << i << "i " << (isInMandelbrotSet(r, i, iters) ? "IN" : "OUT");
                break;
            case 'l':
                char op;
                bool quit = false;
                std::cout << "Enter real: ";
                std::cin >> dIn;
                m.start.real();
                std::cout << "Enter imaginary: ";
                std::cin >> dIn;
                m.start.imag();
                std::cout << "Enter step size: ";
                std::cin >> m.step;
                std::cout << "Enter iterations: ";
                std::cin >> m.iterations;
                do{
                    std::cout << "Select option:\n";
                    std::cout << "\t 1. Set Point\n";
                    std::cout << "\t 2. Set Zoom\n";
                    std::cout << "\t 3. Set iterations\n";
                    std::cout << " Type 'd' to display with current settings\n";
                    std::cout << " Type 'q' to exit\n";
                    std::cout << "Selection: ";
                    std::cin >> op;
                    switch(op){
                        case('1'):
                            std::cout << "Enter real: ";
                            std::cin >> dIn;
                            m.start.real(dIn);
                            std::cout << "Enter imaginary: ";
                            std::cin >> dIn;
                            m.start.imag(dIn);
                            break;
                        case('2'):
                            std::cout << "Enter step size: ";
                            std::cin >> dIn;
                            m.step = dIn;
                            break;
                        case('3'):
                            std::cout << "Enter iterations: ";
                            std::cin >> dIn;
                            m.iterations = dIn;
                            break;
                        case('d'):
                            std::cout << "Real(x): " << r - (STEPS_X * step) << " < " << r << " > " << r + (STEPS_X * step) << '\n';
                            std::cout << "Imag(y): " << i - (STEPS_Y * step) << " < " << i << " > " << i + (STEPS_Y * step) << '\n';
                            std::cout << "Step: " << step << '\n';
                            std::cout << "Iter: " << iters << '\n';
                            m.raw = makeMandelbrot(m.start.real(), m.start.imag(), m.step, STEPS_X, STEPS_Y, m.iterations).raw;
                            printMandelWithXY(m);
                            break;
                        case('q'):
                            quit = true;
                            break;
                        default:
                            std::cout << "Invalid option\n";
                            break;
                    }
                }while(!quit);
                break;
        }
        printMandel(m);
        std::cout << "\n\n";
    }
    std::getchar();
    std::getchar();
}