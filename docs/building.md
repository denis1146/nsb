## Build instructions

### Install third party software

* Install **Git** from https://git-scm.com/downloads
* Install **CMake 4.0.1** or later from https://cmake.org/download
* If you are using **Linux**, you need to run these commands:
  ```bash
  sudo apt -y install build-essential
  sudo apt -y install libtbb-dev
  ```

### Clone source code

  * Clone and update submodules
    ```bash
    git clone https://github.com/denis1146/nsb.git
    git submodule update --init --recursive
    ```

### Build external libraries

* Run the script to download and build external libraries.
    ```bash
    bash -c ./ext_libs/prepare/build_libs.sh
    ```

### Build the project

* You can open the main CMake file in **Qt Creator** and build the project there.
* You can also use **VS Code** and build the project there. The [**C/C++ Extension Pack**](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) will help you with this.
* If you are using Windows, you can also run the solution generator **gen.sh**, which will create a **Microsoft Visual Studio (MVS)** solution in the **_build_gen** directory.
