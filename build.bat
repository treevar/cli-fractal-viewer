::Copyright (C) 2022  eggplantcx
::build.bat
::Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
@echo off
g++ -std=c++11 src/Util.cpp src/MandelbrotSection.cpp main.cpp -o main
pause