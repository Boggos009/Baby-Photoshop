Baby Photoshop - GUI integration
=================================

This project keeps the provided filter implementation in:
CS213_A1_Part1_SectionNum_20250113_20250125_20250247_20250513.cpp

The GUI is added through gui_filter_adapter.cpp/.h, mainwindow.cpp/.h and mainwindow.ui.
The adapter includes the existing filter source and calls its functions; the filter algorithms
were not rewritten here.

Build requirements
------------------
- C++20
- Qt 6.x Widgets (this setup was configured for the installed Qt 6.12 MinGW 64-bit)
- CMake
- MinGW 64-bit

In CLion
--------
1. Open this folder as a CMake project.
2. Set the CLion toolchain to the MinGW 13.1 toolchain you installed with Qt.
3. Set Qt CMake prefix path to the installed Qt prefix, for example:
   C:\Qt\6.12.0\mingw_64
4. Reload CMake.
5. Build and run the BabyPhoto target.

Important
---------
The existing CLI filters 1-16 were treated as pre-existing team work. The GUI layer feeds
parameters to the existing console-oriented functions where needed. Some GUI controls are
therefore informational when the existing implementation does not expose that parameter:
- Filter 2 uses threshold 125.
- Filter 4 classic frame supports black/white; thickness/color picker are mapped to that behavior.
- Filter 7 uses a fixed 50% darken/lighten effect.
- Filter 9 connects merge method + opacity; the unused offset controls have been removed.
- Filter 12 accepts levels 1..5.
- Filter 18 uses fixed radius 5 and 20 levels.

The assignment permits AI assistance for Task 4 (Filters 17 & 18 and GUI), while Tasks 1-3
should remain understood and explained by the team.

GUI note: images use Fit-to-Window mode by default. The preview is recomputed automatically when the main window is resized. Zoom In/Out switches from Fit mode to manual zoom; Fit to Window returns to full-image preview.
