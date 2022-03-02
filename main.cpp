#include <iostream>
#include <complex>
#include <vector>
#include <sstream>
#include <fstream>
#include "inc/MandelbrotSection.hpp"
#include "inc/Util.hpp"
const unsigned int STEPS_X = 125, STEPS_Y = 35;
const std::string DEFAULT_PATH {"saves/def.txt"}; 

int main(){
    unsigned int iters = 255;
    //i.32, r.37
    MandelbrotSection m;
    m.load(DEFAULT_PATH);
    /*m.start.real(0);
    m.start.imag(0);
    m.step = 0.027;
    m.iterations = 255;*/
    std::string op;
    bool quit = false;
    std::string fileName;
    std::cout.precision(15);
    do{
        op = Util::menuWithCheck({{"p", "Set Point"}, {"z", "Set Zoom"}, {"i", "Set iterations"}, {"s", "Save"}, {"l", "Load"}, {"la", "Load Animation"}, {"a", "Make Animation"}, {"d", "Display"}, {"di", "Display Info"}, {"dr", "Display Without Computing"}, {"q", "Quit"}});
        if(op == "p"){
            m.setReal(Util::promptWithCheck("Enter real: "));
            m.setImag(Util::promptWithCheck("Enter imaginary: "));
        }
        else if(op == "z"){
            m.setStep(Util::promptWithCheck("Enter step size: "));
        }
        else if(op == "i"){
            m.setIterations(Util::promptWithCheck("Enter iterations: ", 0, 10000000));
        }
        else if(op == "s"){
            std::cout << "Enter Filename: ";
            fileName = Util::getNextLine();
            if(m.save(fileName)){
                std::cout << "Saved to " << fileName << "\n\n";
            }
        }
        else if(op == "l"){
            std::cout << "Enter Filename: ";
            fileName = Util::getNextLine();
            if(m.load(fileName)){
                std::cout << "Loaded from \"" << fileName << "\"\n\n";
            }
            
        }
        else if(op == "la"){
            std::cout << "Enter Filename: ";
            fileName = Util::getNextLine();
            m.loadAnimation(fileName);
        }
        else if(op == "a"){
            std::vector<double> steps;
            
            double exitVal = 104000.123;
            
            auto d = Util::promptWithCheckAndExit("Enter step for animation ", "e", exitVal);
            while(d != exitVal){
                steps.push_back(d);
                d = Util::promptWithCheckAndExit("Enter step for animation ", "e", exitVal);
            }

            std::cout << "Enter Filename: ";
            fileName = Util::getNextLine();
            m.createAnimation(steps, fileName);

        }
        else if(op == "d"){
            m.printInfo(std::cout);
            m.calculate();
            std::cout << m << '\n';
        }
        else if(op == "di"){
            m.printInfo(std::cout);
        }
        else if(op == "dr"){
            std::cout << m << '\n';
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