# Build instructions

**Install dependencies (Ubuntu):**

`sudo apt-get install gcc make build-essential`

Install Qt using the installer and make sure all required modules are installed (see the .pro file).

As of writing, Open-Typer (officially) supports Qt 5 (from 5.9.5 to 5.15.8) and Qt 6 (6.2.4).

```
lrelease *.pro
qmake
make -j$(nproc --all)
```

You may need to use the platform-specifc deploy tool to add shared libraries, for example `windeployqt` for Windows and `macdeployqt` for MacOS.
