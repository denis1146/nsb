## Build instructions

### Install third party software

* Install **Git** from https://git-scm.com/downloads
* Install **CMake 4.0.1** or later from https://cmake.org/download

### Clone source code

  ```
  git clone https://github.com/denis1146/nsb.git
  git submodule update --init --recursive
  ```

### Build external libraries

* Go to the *nsb/ext_libs/prepare* directory

  ```
  cd ./nsb/ext_libs/prepare
  ```

* Run the script to download and build external libraries.
  * For Linux:

    ```
    ./build_libs.sh linux
    ```

  * For Windows:

    ```
    ./build_libs.sh windows
    ```

### Build the project

* You can open the main CMake file in **Qt Creator** and build the project there.
* You can also use **VS Code** and build the project there.
* If you are using Windows, you can also run the solution generator **gen.sh**, which will create a **Microsoft Visual Studio (MVS)** solution in the **_build_gen** directory.
