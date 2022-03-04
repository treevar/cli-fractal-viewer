/****Copyright (C) 2022  eggplantcx
 **FractalSection.hpp
 **Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
 */
#ifndef FRACTAL_SECTION_HPP
#define FRACTAL_SECTION_HPP
#include <complex>
#include <vector>
#include <ostream>
#include <string>
class FractalSection{
    public:
        typedef std::vector<std::vector<char> > RawData;
        typedef std::complex<double> Point;
        
        FractalSection();
        FractalSection(Point p);
        FractalSection(Point p, double step, unsigned int iterations, unsigned int stepsReal, unsigned int stepsImag);
        FractalSection(const FractalSection &other);
        
        FractalSection &operator=(const FractalSection &other);
        
        friend std::ostream &operator<<(std::ostream &out, const FractalSection &m);
        
        void printInfo(std::ostream &out);

        bool save(const std::string &fileName, bool pathRelativeToSavePath = true, std::ios::openmode om = std::ios::app);
        bool load(const std::string &fileName, bool pathRelativeToSavePath = true);

        bool createAnimation(const std::vector<double> &steps, const std::string &fileName, bool pathRelativeToSavePath = true);
        bool loadAnimation(const std::string &fileName, bool pathRelativeToSavePath = true);
       
        void calculate();

        char calculatePoint(const std::complex<double> &c, const unsigned int iterations);
        char calculatePoint(double real, double imag, unsigned int iterations);

        RawData getRaw();
        double getReal();
        double getImag();
        Point getStart();
        double getStep();
        unsigned int getIterations();
        unsigned int getRealSteps();
        unsigned int getImagSteps();
        bool getReCalcOnChange();
        bool getLogging();
        std::string getSavePath();
        double getRealStepMult();
        std::streamsize getPrecision();

        void setReal(double r);
        void setImag(double i);
        void setStart(const Point &p);
        void setStep(double s);
        void setIterations(unsigned int i);
        void setRealSteps(unsigned int rs);
        void setImagSteps(unsigned int is);
        void setReCalcOnChange(bool re);
        void setLogging(bool l);
        //void setSavePath(const std::string &s);
        void setRealStepMult(double r);
        void setPrecision(std::streamsize p);

       ~FractalSection();
    protected:
        virtual char _func(Point, unsigned int iters) = 0;
        std::string _savePath = "./saves/";

    private:
        RawData _raw;
        Point _start = {0, 0};
        double _step = 0.03, _realStepMult = 0.5;
        unsigned int _iterations = 255, _stepsR = 125, _stepsI = 35;
        bool _reCalcOnChange = false, _logging = true;
        std::streamsize _precision = 15;

        void _save(std::ostream &out);
        bool _load(std::istream &in);

        void _log(std::ostream &out, const std::string &str);
        bool _logIf(bool log, std::ostream &out, const std::string &str);
};

class MandelbrotSection : public FractalSection{
    private:
        char _func(Point p, unsigned int iters) override{
            Point z{0, 0};
            for(unsigned int i = 0; i < iters; i++){
                z = (z * z) + p;
                if(std::abs(z) > 2.0){ return static_cast<char>(177); }
            }
            return ' ';
        }
};
#endif//FRACTAL_SECTION_HPP