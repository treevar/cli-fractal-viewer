#ifndef MANDELBROT_SECTION_HPP
#define MANDELBROT_SECTION_HPP
#include <complex>
#include <vector>
#include <ostream>
#include <string>
class MandelbrotSection{
    public:
        typedef std::vector<std::vector<bool> > RawData;
        typedef std::complex<double> Point;
        
        MandelbrotSection();
        MandelbrotSection(Point p);
        MandelbrotSection(Point p, double step, unsigned int iterations, unsigned int stepsReal, unsigned int stepsImag);
        MandelbrotSection(const MandelbrotSection &other);
        
        MandelbrotSection &operator=(const MandelbrotSection &other);
        
        friend std::ostream &operator<<(std::ostream &out, const MandelbrotSection &m);
        
        void printInfo(std::ostream &out);

        bool save(const std::string &fileName);
        bool load(const std::string &fileName);

        bool createAnimation(const std::vector<double> &steps, const std::string &fileName);
        bool loadAnimation(const std::string &fileName);
       
        void calculate();

        static bool isInMandelbrotSet(const std::complex<double> &c, const unsigned int iterations);
        static bool isInMandelbrotSet(double real, double imag, unsigned int iterations);

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

        void setReal(double r);
        void setImag(double i);
        void setStart(const Point &p);
        void setStep(double s);
        void setIterations(unsigned int i);
        void setRealSteps(unsigned int rs);
        void setImagSteps(unsigned int is);
        void setReCalcOnChange(bool re);
        void setLogging(bool l);

       ~MandelbrotSection();
    private:
        RawData _raw;
        Point _start = {0, 0};
        double _step = 0.03;
        unsigned int _iterations = 255, _stepsR = 125, _stepsI = 35;
        bool _reCalcOnChange = false, _logging = true;

        void _save(std::ostream &out);
        bool _load(std::istream &in);

        void _log(std::ostream &out, const std::string &str);
        bool _logIf(bool log, std::ostream &out, const std::string &str);
};

#endif//MANDELBROT_SECTION_HPP