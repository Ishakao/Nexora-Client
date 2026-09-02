@echo off
set VCPKG_ROOT=C:\vcpkg
set VCPKG_BIN=%VCPKG_ROOT%\installed\x64-windows\bin
set VCPKG_LIB=%VCPKG_ROOT%\installed\x64-windows\lib
set VCPKG_INC=%VCPKG_ROOT%\installed\x64-windows\include
set BUILD_DIR=VSC_BUILD

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

g++ ^
 src/Nexora.cpp ^
 src/nexoraNETWORK.cpp ^
 src/nexoraUI.cpp ^
 include/apiUTILS.cpp ^
 -I"%VCPKG_INC%" ^
 -L"%VCPKG_LIB%" ^
 -o "%BUILD_DIR%\nexora.exe" ^
 -lraylib -lopengl32 -lgdi32 -lwinmm -lssl -lcrypto -lws2_32 -lcrypt32 -lcomdlg32 -std=c++20 -w

echo Copying dependencies...
xcopy /y /d "%VCPKG_BIN%\*.dll" "%BUILD_DIR%\"