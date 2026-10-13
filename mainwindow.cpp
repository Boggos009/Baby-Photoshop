#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPixmap>
#include <QResizeEvent>
#include <QScrollArea>
#include <QStatusBar>
#include <QToolTip>
#include <QComboBox>
#include <QPushButton>
#include <QtMath>

#include <algorithm>
#include <cmath>

namespace {

const QString kImageFilters =
    "Images (*.jpg *.jpeg *.png *.bmp *.tga);;JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp);;TGA (*.tga);;All files (*)";

int optionsPageForFilter(int filterNumber) {
    switch (filterNumber) {
    case 4: return 2;   // Frame
    case 5: return 3;   // Flip
    case 6: return 4;   // Rotate
    case 7: return 5;   // Brightness
    case 8: return 6;   // Resize
    case 9: return 7;   // Merge
    case 10: return 12; // Edge detection
    case 11: return 8;  // Crop
    case 12: return 9;  // Blur
    case 17: return 10; // Skew
    default: return 0;
    }
}

}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadImage);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveImage);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveImageAs);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::resetToOriginal);
    connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(ui->actionFitToWindow, &QAction::triggered, this, &MainWindow::fitToWindow);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);

    // The .ui already maps the visible buttons to their QAction objects.
    connect(ui->btnApply, &QPushButton::clicked, this, &MainWindow::applyFilter);
    connect(ui->mergeLoadSecondButton, &QPushButton::clicked, this, &MainWindow::chooseMergeImage);
    connect(ui->filterComboBox,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::filterChanged);
    connect(ui->frameTypeCombo,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            [this](int) { updateOptionsPage(); });

    // The current CLI implementations have fixed values for these options.

    ui->brightnessSlider->setValue(50);
    ui->brightnessSpin->setValue(50);

    ui->blurLevelSlider->setRange(1, 5);
    ui->blurLevelSpin->setRange(1, 5);
    // The existing classic-frame filter accepts only black or white.
    ui->lblFrameColor->setText(QStringLiteral("Color"));

    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->btnApply->setEnabled(false);

    updateOptionsPage();
    refreshImages();
    refreshLabels();
    setStatus(QStringLiteral("Ready. Load an image to begin."));
}

MainWindow::~MainWindow() {
    delete ui;
}

QImage MainWindow::snapshotToQImage(const GuiImageSnapshot& snapshot) const {
    if (!snapshot.valid()) {
        return {};
    }
    const int bytesPerLine = snapshot.width * 3;
    QImage image(snapshot.pixels.data(),
                 snapshot.width,
                 snapshot.height,
                 bytesPerLine,
                 QImage::Format_RGB888);
    return image.copy();
}

void MainWindow::setImageOnLabel(const GuiImageSnapshot& image,
                                 QLabel* label,
                                 QScrollArea* scrollArea) {
    if (!image.valid()) {
        label->clear();
        label->setText(QStringLiteral("No image loaded."));
        label->setAlignment(Qt::AlignCenter);
        label->adjustSize();
        return;
    }

    const QImage qimage = snapshotToQImage(image);
    if (qimage.isNull()) {
        label->clear();
        label->setText(QStringLiteral("Unable to display image."));
        return;
    }

    const int maxWidth = std::max(1, scrollArea->viewport()->width() - 8);
    const int maxHeight = std::max(1, scrollArea->viewport()->height() - 8);

    QSize targetSize;
    if (fitToWindow_) {
        const double fitX = static_cast<double>(maxWidth) / qimage.width();
        const double fitY = static_cast<double>(maxHeight) / qimage.height();
        const double scale = std::min(1.0, std::min(fitX, fitY));
        targetSize = qimage.size() * std::max(0.01, scale);
    } else {
        targetSize = qimage.size() * zoomFactor_;
    }

    targetSize.setWidth(std::max(1, targetSize.width()));
    targetSize.setHeight(std::max(1, targetSize.height()));

    label->setPixmap(QPixmap::fromImage(qimage.scaled(
        targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    label->setFixedSize(targetSize);
    label->setAlignment(Qt::AlignCenter);
}

void MainWindow::refreshImages() {
    const GuiImageSnapshot original = guiOriginalSnapshot();
    const GuiImageSnapshot current = guiCurrentSnapshot();
    setImageOnLabel(original, ui->originalImageLabel, ui->originalScrollArea);
    setImageOnLabel(current, ui->currentImageLabel, ui->currentScrollArea);
    refreshLabels();
}

void MainWindow::refreshLabels() {
    const GuiImageSnapshot current = guiCurrentSnapshot();
    if (!current.valid()) {
        ui->imageInfoLabel->setText(QStringLiteral("File: -   |   Size: -   |   Zoom: Fit"));
        ui->btnApply->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSaveAs->setEnabled(false);
        return;
    }

    const QString fileName = currentFilePath_.isEmpty()
                                 ? QStringLiteral("-")
                                 : QFileInfo(currentFilePath_).fileName();
    const QString zoomText = fitToWindow_
                                 ? QStringLiteral("Fit")
                                 : QStringLiteral("%1%").arg(qRound(zoomFactor_ * 100.0));
    ui->imageInfoLabel->setText(
        QStringLiteral("File: %1   |   Size: %2 x %3   |   Zoom: %4")
            .arg(fileName)
            .arg(current.width)
            .arg(current.height)
            .arg(zoomText));

    ui->btnApply->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);
}

void MainWindow::setStatus(const QString& message) {
    statusBar()->showMessage(message, 6000);
}

void MainWindow::setDirty(bool dirty) {
    dirty_ = dirty;
    const QString baseTitle = QStringLiteral("Baby Photoshop - CS213 FCAI Cairo University");
    setWindowTitle(dirty_ ? baseTitle + QStringLiteral(" *") : baseTitle);
}

int MainWindow::currentFilterNumber() const {
    return ui->filterComboBox->currentIndex() + 1;
}

void MainWindow::updateOptionsPage() {
    const int filterNumber = currentFilterNumber();
    const int page = optionsPageForFilter(filterNumber);
    ui->filterOptionsStack->setCurrentIndex(page);

    ui->cropSelectMouseCheck->setEnabled(false);

    // Page 0 is shared by filters that have no GUI parameters. Always reset
    // its text so one filter cannot leave a stale message for another filter.
    ui->noOptionsLabel->setText(QStringLiteral("This filter has no options."));

    if (filterNumber == 4) {
        const bool classic = ui->frameTypeCombo->currentIndex() == 0;
        ui->frameColorCombo->setEnabled(classic);
    } else if (filterNumber == 7) {
        ui->brightnessSlider->setToolTip(
            QStringLiteral("Choose brightness level from 0% to 100%."));
        ui->brightnessSpin->setToolTip(
            QStringLiteral("Choose brightness level from 0% to 100%."));
    }
}

void MainWindow::filterChanged(int) {
    updateOptionsPage();
}

bool MainWindow::confirmDiscardOrSave(const QString& actionText) {
    if (!dirty_) {
        return true;
    }

    const QMessageBox::StandardButton result = QMessageBox::question(
        this,
        QStringLiteral("Unsaved changes"),
        QStringLiteral("You have unsaved changes. Save them before %1?").arg(actionText),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    if (result == QMessageBox::Save) {
        saveImage();
        return !dirty_;
    }
    if (result == QMessageBox::Discard) {
        return true;
    }
    return false;
}

void MainWindow::loadImage() {
    if (!confirmDiscardOrSave(QStringLiteral("loading another image"))) {
        return;
    }

    const QString path = QFileDialog::getOpenFileName(this,
                                                       QStringLiteral("Load Image"),
                                                       QString(),
                                                       kImageFilters);
    if (path.isEmpty()) {
        return;
    }

    std::string error;
    if (!guiLoadImage(path.toStdString(), error)) {
        QMessageBox::critical(this,
                              QStringLiteral("Load failed"),
                              QString::fromStdString(error));
        return;
    }

    currentFilePath_ = path;
    mergeSecondPath_.clear();
    hasUndo_ = false;
    hasRedo_ = false;
    undoSnapshot_ = {};
    redoSnapshot_ = {};
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->mergeSecondPathLabel->setText(QStringLiteral("No second image selected."));
    setDirty(false);
    zoomFactor_ = 1.0;
    fitToWindow_ = true;
    refreshImages();
    setStatus(QStringLiteral("Loaded: %1").arg(QFileInfo(path).fileName()));
}

void MainWindow::saveImage() {
    if (!guiHasImage()) {
        QMessageBox::information(this, QStringLiteral("Save"), QStringLiteral("No image is loaded."));
        return;
    }

    if (currentFilePath_.isEmpty()) {
        saveImageAs();
        return;
    }

    std::string error;
    if (!guiSaveImage(currentFilePath_.toStdString(), error)) {
        QMessageBox::critical(this,
                              QStringLiteral("Save failed"),
                              QString::fromStdString(error));
        return;
    }

    setDirty(false);
    setStatus(QStringLiteral("Saved: %1").arg(QFileInfo(currentFilePath_).fileName()));
}

void MainWindow::saveImageAs() {
    if (!guiHasImage()) {
        QMessageBox::information(this, QStringLiteral("Save As"), QStringLiteral("No image is loaded."));
        return;
    }

    QString suggested = currentFilePath_;
    if (suggested.isEmpty()) {
        suggested = QDir::homePath() + QStringLiteral("/edited.jpg");
    }

    const QString path = QFileDialog::getSaveFileName(this,
                                                       QStringLiteral("Save Image As"),
                                                       suggested,
                                                       kImageFilters);
    if (path.isEmpty()) {
        return;
    }

    std::string error;
    if (!guiSaveImage(path.toStdString(), error)) {
        QMessageBox::critical(this,
                              QStringLiteral("Save As failed"),
                              QString::fromStdString(error));
        return;
    }

    currentFilePath_ = path;
    setDirty(false);
    refreshLabels();
    setStatus(QStringLiteral("Saved as: %1").arg(QFileInfo(path).fileName()));
}

GuiFilterParams MainWindow::collectFilterParams() const {
    GuiFilterParams params;

    params.frameType = ui->frameTypeCombo->currentIndex();
    params.frameColor = ui->frameColorCombo->currentIndex();
    params.flipDirection = ui->flipHorizontalRadio->isChecked() ? 1 : 2;
    params.rotateDegrees = ui->rotate90Radio->isChecked() ? 90 :
                           ui->rotate180Radio->isChecked() ? 180 : 270;
    params.brightnessMode = ui->darkenRadio->isChecked() ? 0 : 1;
    params.brightnessLevel = ui->brightnessSpin->value();

    params.resizeByRatio = ui->resizeByRatioRadio->isChecked();
    params.resizeWidth = ui->resizeWidthSpin->value();
    params.resizeHeight = ui->resizeHeightSpin->value();
    params.resizeRatioPercent = ui->resizeRatioSpin->value();
    params.resizeKeepAspect = ui->resizeKeepAspectCheck->isChecked();

    params.mergeSecondImage = mergeSecondPath_.toStdString();
    params.mergeMethod = ui->mergeResizeRadio->isChecked() ? 1 : 2;
    params.mergeOpacity = 1.0 - ui->mergeOpacitySpin->value() / 100.0;

    params.cropX = ui->cropXSpin->value();
    params.cropY = ui->cropYSpin->value();
    params.cropWidth = ui->cropWidthSpin->value();
    params.cropHeight = ui->cropHeightSpin->value();

    params.blurLevel = ui->blurLevelSpin->value();
    params.detectEdgeSmooth = ui->detectEdgeSmoothCheck->isChecked();
    params.skewAngle = ui->skewAngleSpin->value();

    return params;
}

void MainWindow::applyFilter() {
    if (!guiHasImage()) {
        QMessageBox::information(this,
                                 QStringLiteral("Apply Filter"),
                                 QStringLiteral("Please load an image first."));
        return;
    }

    const int filterNumber = currentFilterNumber();
    const GuiImageSnapshot before = guiCurrentSnapshot();
    const GuiFilterParams params = collectFilterParams();

    std::string error;
    std::string notice;
    if (!guiApplyFilter(filterNumber, params, error, notice)) {
        QMessageBox::critical(this,
                              QStringLiteral("Filter failed"),
                              QString::fromStdString(error));
        return;
    }

    undoSnapshot_ = before;
    hasUndo_ = true;
    hasRedo_ = false;
    redoSnapshot_ = {};
    ui->actionUndo->setEnabled(true);
    ui->actionRedo->setEnabled(false);
    setDirty(true);
    refreshImages();

    const QString filterName = ui->filterComboBox->currentText();
    setStatus(QStringLiteral("Applied %1.").arg(filterName));
    if (!notice.empty()) {
        statusBar()->showMessage(QStringLiteral("%1 — %2").arg(filterName, QString::fromStdString(notice)), 10000);
    }
}

void MainWindow::undo() {
    if (!hasUndo_) {
        return;
    }

    redoSnapshot_ = guiCurrentSnapshot();
    guiSetCurrentSnapshot(undoSnapshot_);
    hasRedo_ = true;
    hasUndo_ = false;
    ui->actionRedo->setEnabled(true);
    ui->actionUndo->setEnabled(false);
    setDirty(true);
    refreshImages();
    setStatus(QStringLiteral("Last filter undone."));
}

void MainWindow::redo() {
    if (!hasRedo_) {
        return;
    }

    undoSnapshot_ = guiCurrentSnapshot();
    guiSetCurrentSnapshot(redoSnapshot_);
    hasUndo_ = true;
    hasRedo_ = false;
    ui->actionUndo->setEnabled(true);
    ui->actionRedo->setEnabled(false);
    setDirty(true);
    refreshImages();
    setStatus(QStringLiteral("Filter reapplied."));
}

void MainWindow::resetToOriginal() {
    if (!guiHasImage()) {
        return;
    }

    guiResetToOriginal();
    undoSnapshot_ = {};
    redoSnapshot_ = {};
    hasUndo_ = false;
    hasRedo_ = false;
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    setDirty(false);
    refreshImages();
    setStatus(QStringLiteral("Reset to the original image."));
}

void MainWindow::chooseMergeImage() {
    const QString path = QFileDialog::getOpenFileName(this,
                                                       QStringLiteral("Choose second image"),
                                                       QString(),
                                                       kImageFilters);
    if (path.isEmpty()) {
        return;
    }

    mergeSecondPath_ = path;
    ui->mergeSecondPathLabel->setText(path);
    setStatus(QStringLiteral("Second image selected for merge: %1").arg(QFileInfo(path).fileName()));
}

void MainWindow::zoomIn() {
    if (!guiHasImage()) return;

    if (fitToWindow_) {
        fitToWindow_ = false;
        zoomFactor_ = 1.0;
    }

    zoomFactor_ = std::min(8.0, zoomFactor_ * 1.25);
    refreshImages();
}

void MainWindow::zoomOut() {
    if (!guiHasImage()) return;

    if (fitToWindow_) {
        fitToWindow_ = false;
        zoomFactor_ = 1.0;
    }

    zoomFactor_ = std::max(0.1, zoomFactor_ / 1.25);
    refreshImages();
}

void MainWindow::fitToWindow() {
    if (!guiHasImage()) return;

    fitToWindow_ = true;
    zoomFactor_ = 1.0;
    refreshImages();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    if (fitToWindow_ && guiHasImage()) {
        refreshImages();
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(
        this,
        QStringLiteral("About Baby Photoshop"),
        QStringLiteral("Baby Photoshop\n\nCS213 OOP Programming\nFCAI, Cairo University\n2026-2027\n\nGUI built with Qt 6."));
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (!dirty_) {
        event->accept();
        return;
    }

    const QMessageBox::StandardButton result = QMessageBox::question(
        this,
        QStringLiteral("Unsaved changes"),
        QStringLiteral("You have unsaved changes. Save before exiting?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    if (result == QMessageBox::Save) {
        saveImage();
        if (dirty_) {
            event->ignore();
        } else {
            event->accept();
        }
    } else if (result == QMessageBox::Discard) {
        event->accept();
    } else {
        event->ignore();
    }
}
