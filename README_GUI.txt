BABY PHOTOSHOP - GUI PROJECT CS213 OOP Programming - FCAI, Cairo
University ================================================

1. PROJECT OVERVIEW

Baby Photoshop is a C++ image-processing project with a Qt Widgets
graphical user interface (GUI). The GUI lets the user load an image,
select an available filter, configure supported options, preview the
result, and save the output.

The existing filter implementation is kept in:
main.cpp

The GUI is connected to the filter functions through the adapter files.

2. PROJECT FILES

-   main.cpp
    Main filter implementation and console-oriented filter functions.
-   Image_Class.h Image class header supplied for the project.
-   stb_image.h / stb_image_write.h Image loading/writing headers used
    by the project.
-   start.cpp Application entry point for the GUI.
-   mainwindow.cpp / mainwindow.h / mainwindow.ui GUI behavior,
    declarations, and Qt Designer interface.
-   gui_filter_adapter.cpp / gui_filter_adapter.h Connect GUI parameters
    and actions to the existing filter functions.
-   CMakeLists.txt CMake project and build configuration.
-   README_GUI.txt Setup and build instructions.

3. REQUIREMENTS
-   C++20-compatible compiler
-   CMake
-   Qt 6 Widgets
-   A compatible C++ toolchain

The project was configured for this setup: - Qt 6.12.0 - MinGW 64-bit -
MinGW 13.1 - CLion on Windows

Other compatible setups may require adjusting the compiler/toolchain and
Qt CMake prefix path.

4. SETTING UP AND RUNNING IN CLION (WINDOWS)

1.  Extract the submitted ZIP file into a folder.
2.  Open that folder in CLion as a CMake project. Select the folder
    containing CMakeLists.txt.
3.  Configure a compatible MinGW 64-bit toolchain in CLion.
4.  Make sure the Qt 6 installation is available.
5.  Set the Qt CMake prefix path to your Qt installation, for example:
    C:\6.12.0_64
6.  Reload the CMake project when prompted, or use the Reload CMake
    Project action.
7.  Select the BabyPhoto run target.
8.  Build the project, then run the target.
9.  If CMake reports a missing Qt package or compiler, verify the Qt
    path, compiler, and CMake toolchain settings.

The exact installation paths may differ from one computer to another.

5. FILTER NOTES AND LIMITATIONS
The GUI passes parameters to the existing filter functions where
supported.
-   Filter 4 (classic frame): the GUI offers black/white color
    selection.
-   Filter 7 (Darken/lighten) : the GUI offers level slider [0%:100%].
-   Filter 9 (merge): connects the merge method and opacity.
-   Filter 12: accepts levels from 1 to 5.


6. IMAGE PREVIEW
Images use Fit-to-Window mode by default. The preview is recomputed when
the main window is resized. Zoom In/Out switches to manual zoom, while
Fit to Window returns to the full-image preview.
