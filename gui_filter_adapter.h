#pragma once

#include <string>
#include <vector>

struct GuiImageSnapshot {
    int width = 0;
    int height = 0;
    int channels = 3;
    std::vector<unsigned char> pixels;

    bool valid() const noexcept {
        return width > 0 && height > 0 && !pixels.empty();
    }
};

struct GuiFilterParams {
    int frameType = 0;          // 0 classic, 1 ornate
    int frameColor = 0;         // 0 black, 1 white (the existing CLI only supports these two)
    int flipDirection = 1;      // 1 horizontal, 2 vertical
    int rotateDegrees = 90;
    int brightnessMode = 1;     // 0 dark, 1 light
    int brightnessLevel = 50;   // existing CLI uses fixed 50%; kept for UI compatibility
    bool resizeByRatio = false;
    int resizeWidth = 500;
    int resizeHeight = 500;
    double resizeRatioPercent = 100.0;
    bool resizeKeepAspect = false;
    std::string mergeSecondImage;
    int mergeMethod = 1;        // 1 biggest dimensions, 2 common area
    double mergeOpacity = 0.5;
    int cropX = 0;
    int cropY = 0;
    int cropWidth = 100;
    int cropHeight = 100;
    int blurLevel = 1;          // existing CLI accepts 1..5
    bool detectEdgeSmooth = true;
    int skewAngle = 40;
    int oilRadius = 5;          // existing CLI is fixed at 5
    int oilLevels = 20;         // existing CLI is fixed at 20
};

bool guiLoadImage(const std::string& path, std::string& error);
bool guiSaveImage(const std::string& path, std::string& error);
bool guiHasImage();

GuiImageSnapshot guiCurrentSnapshot();
GuiImageSnapshot guiOriginalSnapshot();
void guiSetCurrentSnapshot(const GuiImageSnapshot& snapshot);
void guiResetToOriginal();

bool guiApplyFilter(int filterNumber,
                    const GuiFilterParams& params,
                    std::string& error,
                    std::string& notice);
