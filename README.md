# c-cnc24

This is the repository of the code developed during the _Digital Manufacturing_ course, academic year 2023-24, Department of Industrial Engineering, University of Trento.

## Contents

* `examples`: contains introductory code examples
* `src`: contains the main project source code (library files)
* `src/main`: contains executables code
* `var`: useful stuff


## Prerequisites

We are going to develop a C project for the UNIX command-line environment. On Linux and MacOS, you are set already, on Windows we are going to use a WSL2 environment with Ubuntu 22.04 OS (more later).

If you have a Windows laptop, you are suggested to use at least Windows 10, preferably Windows 11. Be sure to install Windows Terminal app <https://learn.microsoft.com/en-us/windows/terminal/install>

Regardless your platform, begin with installing Visual Studio Code from here: <https://code.visualstudio.com/download>. Then open a terminal and type the following to install commonly used VSCode extensions:

```sh
code --install-extension xaver.clang-format
code --install-extension tintinweb.graphviz-interactive-preview
code --install-extension canna.figlet
code --install-extension Juancete.gcode-formatter
code --install-extension vscode-gcode.gcode
code --install-extension vadimcn.vscode-lldb
code --install-extension fizzybreezy.gnuplot
```

Also, you want to install the Fira Code font family, from <https://github.com/tonsky/FiraCode/releases/tag/6.2>. Download the file `FiraCode_6.2.zip`, unzip it and install the new font.

## VS Code setup

The development is carried out in Visual Studio Code (VS Code for brevity).

I suggest to configure VS Code with the following settings. Open the settings file: `Ctrl`+`Shift`+`p` then type `json` and select the item "Preferences: Open Settings (JSON)". Then be sure that the list contains the following items (the first two settings **only if you have installed Fira Code font**):

```json
{
  "editor.fontFamily": "Fira Code",
  "editor.fontLigatures": true,
  "editor.tabSize": 2,
  "editor.insertSpaces": true,
  "editor.wrappingIndent": "indent",
  "editor.renderControlCharacters": true,
  "editor.wordWrap": "bounded",
  "editor.wordWrapColumn": 80,
  "editor.rulers": [80],
  "editor.renderLineHighlight": "all",
  "cmake.configureOnEdit": false,
  "cmake.configureOnOpen": false,
  "C_Cpp.default.cppStandard": "c++17",
  "C_Cpp.default.cStandard": "c17",
}
```

It there are already other items in the JSON file, just add (don't replace) the above ones to the list (pay attention to separate each line with a comma and to put everithyng in between the outer curly braces).


### 🪟 Windows (or 🐧 Ubuntu/Debian Linux)
The project must be built with a linux toolchain. On Windows, we are using a WSL2 environment with Ubuntu OS. To install it, follow the instructions at <https://docs.microsoft.com/en-us/windows/wsl/install-win10>. 

To enable the compilation we need to install a few packages: on the linux console, type:

```bash
sudo apt update
sudo apt install build-essential make cmake cmake-curses-gui clang clang-format lldb libgsl-dev ruby figlet sshfs graphviz gnuplot
sudo gem install gv_fsm
sudo update-alternatives --set c++ /usr/bin/clang++
sudo update-alternatives --set cc /usr/bin/clang
```

### 🍎 MacOS
You need to have Xcode installed: do that through the App Store and—once finished—launch Xcode and accept the licence terms. Then you can close it.

On MacOS, the command equivalent to `apt` is `brew`: you have to install it by following the instructions on <https://brew.sh>, which means to type the following in the Terminal.app:

```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then **close the terminal**, open a new one and proceed as follows:

```sh
brew install figlet gsl clang-format graphviz gpg
brew install --cask cmake
curl -sSL https://rvm.io/mpapis.asc | gpg --import -
curl -sSL https://rvm.io/pkuczynski.asc | gpg --import -
curl -sSL https://get.rvm.io | bash -s stable --auto-dotfiles
```

Close and open a new terminal, again, then:

```sh
rvm install ruby-2.7
gem install gv_fsm
```