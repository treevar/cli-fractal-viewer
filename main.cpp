/****Copyright (C) 2022  eggplantcx
 **main.cpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */

#include <iostream>
#include <complex>
#include <vector>
#include <sstream>
#include <fstream>
#include "inc/FractalSection.hpp"
#include "inc/Util.hpp"
const unsigned int STEPS_X = 125, STEPS_Y = 35;
const std::string DEFAULT_SECTION {"def.txt"}; 

int main(){
    unsigned int iters = 255;
    //i.32, r.37
    MandelbrotSection m;
    m.load(DEFAULT_SECTION);
    /*m.start.real(0);
    m.start.imag(0);
    m.step = 0.027;
    m.iterations = 255;*/
    std::string op;
    bool quit = false;
    std::string fileName;
    std::cout.precision(15);
    do{
        op = Util::menuWithCheck({{"p", "Set Point"}, {"z", "Set Zoom"}, {"i", "Set iterations"}, {"rm", "Set Real Step Mult"}, 
                                {"pr", "Set Precision of Floating Point Numbers"}, {"s", "Save"}, {"so", "Save and Overwrite"}, 
                                {"sw", "Set viewer width in steps(chars)"}, {"sh", "Set viewer height in steps(chars)"},
                                {"l", "Load"}, {"la", "Load Animation"}, {"a", "Make Animation"}, {"d", "Display"}, 
                                {"di", "Display Info"}, {"dr", "Display Without Computing"}, {"q", "Quit"}});
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
                std::cout << "Saved to " << m.getSavePath() << fileName << "\n\n";
            }
        }
        else if(op == "pr"){
            m.setPrecision(Util::promptWithCheck("Enter Precision: ", 1, 1000));
        }
        else if(op == "so"){
            std::cout << "Enter Filename: ";
            fileName = Util::getNextLine();
            if(m.save(fileName, true, std::ios::trunc)){
                std::cout << "Saved to " << m.getSavePath() << fileName << "\n\n";
            }
        }
        else if(op == "sw"){
            m.setRealSteps(Util::promptWithCheck("Enter steps: ", 1, 10000));
        }
        else if(op == "sh"){
            m.setImagSteps(Util::promptWithCheck("Enter steps: ", 1, 10000));
        }
        else if(op == "l"){
            std::cout << "Enter Filename: ";
            fileName = Util::getNextLine();
            if(m.load(fileName)){
                std::cout << "Loaded from \"" << m.getSavePath() << fileName << "\"\n\n";
            }
            
        }
        else if(op == "rm"){
            m.setRealStepMult(Util::promptWithCheck("Enter Real Step Multiplier: "));
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