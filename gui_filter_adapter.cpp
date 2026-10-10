#include "gui_filter_adapter.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <sstream>
#include <streambuf>
#include <string>

// The team's Skew filter uses M_PI. Define it locally so the original
// filter source can remain unchanged for the GUI build.
#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884
#endif

// Keep the team's existing CLI/filter implementation intact.
// We compile it through this adapter so the GUI can call its functions without
// changing the algorithms or creating a second copy of them.
#define main babyPhotoCliMain
#include "CS213_A1_Part1_SectionNum_20250113_20250125_20250247_20250513.cpp"
#undef main

namespace {

GuiImageSnapshot snapshotOf(const Image& image) {
    GuiImageSnapshot snapshot;
    if (image.imageData == nullptr || image.width <= 0 || image.height <= 0) {
        return snapshot;
    }

    snapshot.width = image.width;
    snapshot.height = image.height;
    snapshot.channels = image.channels;
    const std::size_t bytes = static_cast<std::size_t>(image.width) *
                              static_cast<std::size_t>(image.height) *
                              static_cast<std::size_t>(image.channels);
    snapshot.pixels.assign(image.imageData, image.imageData + bytes);
    return snapshot;
}

void restoreCin(std::streambuf* oldBuffer, std::ios::iostate oldState) {
    std::cin.rdbuf(oldBuffer);
    std::cin.clear(oldState);
}

template <typename Fn>
bool runWithInput(const std::string& input, Fn&& fn, std::string& error) {
    const std::string inputCopy = input;
    std::istringstream in(inputCopy);
    std::streambuf* oldBuffer = std::cin.rdbuf(in.rdbuf());
    const std::ios::iostate oldState = std::cin.rdstate();
    std::cin.clear();

    try {
        fn();
        restoreCin(oldBuffer, oldState);
        return true;
    } catch (const std::exception& ex) {
        restoreCin(oldBuffer, oldState);
        error = ex.what();
        return false;
    } catch (...) {
        restoreCin(oldBuffer, oldState);
        error = "Unknown error while applying the filter.";
        return false;
    }
}

bool replaceCurrentFromSnapshot(const GuiImageSnapshot& snapshot, std::string& error) {
    if (!snapshot.valid()) {
        error = "The image snapshot is empty.";
        return false;
    }

    try {
        Image image(snapshot.width, snapshot.height);
        if (snapshot.pixels.size() != static_cast<std::size_t>(snapshot.width) *
                                      static_cast<std::size_t>(snapshot.height) *
                                      static_cast<std::size_t>(snapshot.channels)) {
            error = "Invalid snapshot size.";
            return false;
        }
        std::memcpy(image.imageData, snapshot.pixels.data(), snapshot.pixels.size());
        currentImg = image;
        return true;
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }
}

} // namespace

bool guiLoadImage(const std::string& path, std::string& error) {
    try {
        currentImg.loadNewImage(path);
        originalImg = currentImg;
        backupImg.imageData = nullptr;
        return true;
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }
}

bool guiSaveImage(const std::string& path, std::string& error) {
    if (!guiHasImage()) {
        error = "No image is loaded.";
        return false;
    }

    try {
        currentImg.saveImage(path);
        return true;
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }
}

bool guiHasImage() {
    return currentImg.imageData != nullptr && currentImg.width > 0 && currentImg.height > 0;
}

GuiImageSnapshot guiCurrentSnapshot() {
    return snapshotOf(currentImg);
}

GuiImageSnapshot guiOriginalSnapshot() {
    return snapshotOf(originalImg);
}

void guiSetCurrentSnapshot(const GuiImageSnapshot& snapshot) {
    std::string error;
    replaceCurrentFromSnapshot(snapshot, error);
}

void guiResetToOriginal() {
    currentImg = originalImg;
    backupImg.imageData = nullptr;
}

bool guiApplyFilter(int filterNumber,
                    const GuiFilterParams& params,
                    std::string& error,
                    std::string& notice) {
    error.clear();
    notice.clear();

    if (!guiHasImage()) {
        error = "Please load an image first.";
        return false;
    }

    try {
        switch (filterNumber) {
        case 1:
            if (!runWithInput("0\n", [] { GrayscaleConversion(); }, error)) return false;
            break;

        case 2:
            if (!runWithInput("0\n", [] { BlackandWhite(); }, error)) return false;
            notice = "Black & White uses the threshold 125 from the existing CLI implementation.";
            break;

        case 3:
            if (!runWithInput("0\n", [] { InvertImage(); }, error)) return false;
            break;

        case 4: {
            const int color = params.frameColor ? 1 : 0;
            std::ostringstream input;

            // AddingFrame() always reads the frame type. The classic frame
            // then reads a color, while the ornate frame does not read one.
            input << params.frameType << '\n';
            if (params.frameType == 0) {
                input << color << '\n';
            }
            // Prevent successFilterImplementation() from entering the
            // original CLI save flow during a GUI Apply operation.
            input << "0\n";

            if (!runWithInput(input.str(), [] { AddingFrame(); }, error)) return false;

            if (params.frameType == 0) {
                notice = "Classic frame: the existing filter supports black or white.";
            } else {
                notice = "Fancy (ornate) frame applied. The existing ornate implementation uses its own white/black pattern and does not use the color control.";
            }
            break;
        }

        case 5: {
            std::ostringstream input;
            input << params.flipDirection << '\n' << "0\n";
            if (!runWithInput(input.str(), [] { FlipImage(); }, error)) return false;
            break;
        }

        case 6: {
            std::ostringstream input;
            input << params.rotateDegrees << '\n' << "0\n";
            if (!runWithInput(input.str(), [] { RotateImage(); }, error)) return false;
            break;
        }

        case 7: {
            DarkandLight(
     params.brightnessMode,
     params.brightnessLevel);
            break;
        }

        case 8: {
            if (params.resizeByRatio) {
                if (!(params.resizeRatioPercent > 0.0)) {
                    error = "Resize ratio must be greater than 0%.";
                    return false;
                }
                const double scale = params.resizeRatioPercent / 100.0;
                const int width = std::max(1, static_cast<int>(std::lround(currentImg.width * scale)));
                const int height = std::max(1, static_cast<int>(std::lround(currentImg.height * scale)));
                resizing(width, height);
            } else {
                int width = std::max(1, params.resizeWidth);
                int height = std::max(1, params.resizeHeight);
                if (params.resizeKeepAspect && currentImg.width > 0 && currentImg.height > 0) {
                    const double widthScale = static_cast<double>(width) / currentImg.width;
                    const double heightScale = static_cast<double>(height) / currentImg.height;
                    const double scale = std::min(widthScale, heightScale);
                    width = std::max(1, static_cast<int>(std::lround(currentImg.width * scale)));
                    height = std::max(1, static_cast<int>(std::lround(currentImg.height * scale)));
                }
                resizing(width, height);
            }
            break;
        }

        case 9: {
            if (params.mergeSecondImage.empty()) {
                error = "Please choose the second image for merging.";
                return false;
            }

            Image secondImage;
            secondImage.loadNewImage(params.mergeSecondImage);
            const double opacity = std::clamp(params.mergeOpacity, 0.0, 1.0);

            if (params.mergeMethod == 2) {
                currentImg = Merge_smallest(currentImg, secondImage, opacity);
            } else {
                currentImg = Merge_biggest(currentImg, secondImage, opacity);
            }
            notice = "Merge offsets are not used by the existing CLI merge functions; merge mode and opacity are connected.";
            break;
        }

        case 10: {
            std::ostringstream input;
            input << (params.detectEdgeSmooth ? 1 : 0) << '\n' << "0\n";
            if (!runWithInput(input.str(), [] { DetectEdges(); }, error)) return false;
            break;
        }

        case 11: {
            if (params.cropX < 0 || params.cropY < 0 || params.cropWidth <= 0 || params.cropHeight <= 0 ||
                params.cropX + params.cropWidth > currentImg.width ||
                params.cropY + params.cropHeight > currentImg.height) {
                error = "Crop area is outside the current image.";
                return false;
            }
            std::ostringstream input;
            input << params.cropX << ' ' << params.cropY << '\n'
                  << params.cropWidth << ' ' << params.cropHeight << '\n'
                  << "0\n";
            if (!runWithInput(input.str(), [] { Crop(); }, error)) return false;
            break;
        }

        case 12: {
            if (params.blurLevel < 1 || params.blurLevel > 5) {
                error = "The existing blur implementation accepts levels 1 to 5.";
                return false;
            }
            std::ostringstream input;
            input << params.blurLevel << '\n' << "0\n";
            if (!runWithInput(input.str(), [] { Blur(); }, error)) return false;
            break;
        }

        case 13:
            if (!runWithInput("0\n", [] { Sunlight(); }, error)) return false;
            break;

        case 14:
            if (!runWithInput("0\n", [] { OldTv(); }, error)) return false;
            break;

        case 15:
            if (!runWithInput("0\n", [] { PurpleWano(); }, error)) return false;
            break;

        case 16:
            if (!runWithInput("0\n", [] { Infrared(); }, error)) return false;
            break;

        case 17: {
            std::ostringstream input;
            input << params.skewAngle << '\n' << "0\n";
            if (!runWithInput(input.str(), [] { Skew(); }, error)) return false;
            break;
        }

        case 18:
            if (!runWithInput("0\n", [] { OilPainting(); }, error)) return false;
            notice = "The existing oil-painting implementation uses fixed radius 5 and 20 intensity levels; the GUI fields remain informational.";
            break;

        default:
            error = "Unknown filter number.";
            return false;
        }
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }

    return true;
}
