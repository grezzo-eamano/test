# test

## Hello World Window

A simple Windows desktop application. Click the button to switch between `Hello, World!` and `こんにちは！`.

### Build and run

After editing and saving `main.cpp`, rebuild before running the application.
Saving the source does not update `main.exe` automatically. The application
window stays open until you close it.

From a Visual Studio Developer Command Prompt in this repository's folder:

```powershell
cl /utf-8 /EHsc main.cpp /link /SUBSYSTEM:WINDOWS user32.lib gdi32.lib
.\main.exe
```

Alternatively, with MinGW-w64:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -mwindows main.cpp -o hello.exe -luser32 -lgdi32
.\hello.exe
```

Generated executables and build files are excluded from Git by `.gitignore`.