::Copyright (C) 2022  eggplantcx
::build.bat
::Full License can be found in LICENSE or <https://www.gnu.org/licenses/gpl-3.0.html>
@echo off
g++ -std=c++11 src/Config.cpp src/Util.cpp src/FractalSection.cpp main.cpp -o main
pause