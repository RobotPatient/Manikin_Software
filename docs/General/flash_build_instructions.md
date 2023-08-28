# Building and Flashing

This guide will guide you through the steps needed to get the tests and firmware compiled. The **prerequisites** for the development environment are:

- PlatformIO compatible IDE (VSCode is recommended) with the PlatformIO plugin installed

- Modern version of Python 3.x

- Modern version of CMake and an C/C++ compiler (favoriably MSVC (under Windows) or GCC (Linux and Mac OS))

- Preferably a VT100 compatible terminal emulator (Putty is recommended)

- Local copy of this git repository (git clone)

## Install an c/c++ compiler

This is needed for compiling the unit-tests and configuring some parts of the build-system.
!!! Installation

	=== "Windows"
		Step 1: Download the [VS Buildtools installer](https://aka.ms/vs/17/release/vs_BuildTools.exe) and launch the installer.
		
		Step 2: Accept the EULA and continue:
		![VS build tools installer UELA screen](images/VS_buildtools_step_1.png)
		
		If done correctly this screen should show up.. Wait for the download to finish :)
		![VS build tools download screen](images/VS_buildtools_downloading_installer.png)
		
		Step 3: Select the following options and click on the install button:
		![VS build tools installation options](images/VS_buildtools_step_2.png)
		
		/// note | C++ Clang tools for Windows
		Clang has tools for static code analysis and code formatting
		(besides being an compiler frontend (llvm)).
		
		Which might be very useful when trying to write consistent code following code
		rules.
		
		///
		Step 4: Wait for the installation to finish :w
		/// success | Done, your setup is now ready :)
		///

	=== "Linux(Ubuntu or Debian)"

		The fastest way to install the compiler and required tools in linux is by using the terminal. 
		
		Step 1: Open a terminal and enter:
		```
		sudo apt-get install gcc cmake
		```
		/// success | Done, your setup is now ready :)
		///
	=== "Linux(Arch or Manjaro)"
		The fastest way to install the compiler and required tools in linux is by using the terminal. 
		
		Step 1: Open a terminal and enter:
		```
		sudo pacman -S gcc cmake
		```

	=== "Mac os"
		In Mac os the same principle applies as would to linux. The fastest way is using  [HomeBrew](https://brew.sh/index_nl) in the terminal.
		
		Step 1: 
		
		```
		brew install gcc cmake
		```
		/// success | Done, your setup is now ready :)
		///




## Setup VSCode
This section is dedicated to describing the steps needed to setup the VSCode editor to support our development environment. 
The following plugins will have to be installed:

- C/C++

- CMake

- CMake Tools

- PlatformIO IDE

Step 1: Open VS Code and head to the extensions tab (4th icon in the left sidebar). 

![Extensions tab in VS Code](images/extensions_tab_vscode.png)

Step 2: Search and install the following plugins:

??? note "Plugin C/C++"
	Search for the plugin in the search bar: **C/C++** from publisher: ms-vscode or Microsoft. Click on install...
	![C/C++ Plugin](images/C_CPP_Plugin_vs_code.png)
	
??? note "Plugins CMake and CMake Tools"
	Search for the plugin in the search bar: **CMake**. Install the plugins CMake and CMake Tools from publisher: twxs and Microsoft. And Click on install...
	
	![CMake plugins](images/CMake_plugins_vscode.png)
	
??? note "Plugin PlatformIO IDE"
	Search for the plugin in the search bar: **PlatformIO IDE** from publisher: PlatformIO. Click on install...
	![PlatformIO plugin](images/Platformio_plugin_vscode.png)
	
/// success | Congratulations🎉 VS code is now set-up!
///

## Building Projects (SensorHub_FW and ActuatorHub_FW)
??? summary "Step 1: Open the SensorHub_FW or ActuatorHub_FW project folder in vs code"
	Open VS code and select **File->Open Folder**. 
	![Open folder](images/Open_folder.png)
	
	Open the SensorHub_FW or ActuatorHub_FW folder.
	![SensorHub_FW folder](images/Path2.png)
	![Inside SensorHub_FW folder](images/Path3.png)

??? summary "Step 2: Configure CMake"
	VS code will ask automatically to configure the project using CMake. 
	![Configure project with CMake](images/Configure_project.png)
	
	Select the right compiler from the list:
	![Select kit](images/Select_kit.png)
	When asked to always configure projects upon opening. Select **No**!

??? summary "Step 3: Build the project using PlatformIO"
	Select the build icon in the bottom left toolbar in VS code. 
	![Build](images/Build.png)
	After some time passes you will get this output...
	
	![Build Success](images/Build_success.png)
	
	/// success | Congratulations🎉 You succesfully built the mcu binary
	///
	
## Formatter and unit-tests

Advanced features like the auto-formatter and unit tests are very useful and help keeping code quality consistent.  

??? summary "How to use the auto-formatter"
	Step 1: Press **CTRL+SHIFT+P** and type "format document"
	![auto-formatter](images/formatter.png)

	/// warning | This uses the rules defined in the .clang-format file
	///

??? summary "How to use the unit testing features in vs code"
	![toolbar](images/Toolbar_explanation.png)
	??? note
		The red icon is the flash icon, when a board is connected via micro-usb the mcu binary will be flashed to the MCU.
		
		The purple icon is the clean icon, when pressed all build files will be deleted.
		
		The green icon is the build icon for the unit tests, when pressed the unit-tests will be build using CMake.
		
		The orange icon is the CTest icon, when pressed the unit tests will be run.

	Step 1: Click on the Build icon for the unit tests (Gear icon with the text "Build").
	
	Step 2: Click on the Run CTest button.




