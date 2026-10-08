#pragma once

#include <QCloseEvent>
#include <QImage>
#include <QMainWindow>
#include <QString>

class QLabel;
class QResizeEvent;
class QScrollArea;

#include "gui_filter_adapter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void loadImage();
    void saveImage();
    void saveImageAs();
    void applyFilter();
    void undo();
    void redo();
    void resetToOriginal();
    void chooseMergeImage();
    void filterChanged(int index);
    void zoomIn();
    void zoomOut();
    void fitToWindow();
    void showAbout();

private:
    void updateOptionsPage();
    void refreshImages();
    void refreshLabels();
    void setImageOnLabel(const GuiImageSnapshot& image, QLabel* label, QScrollArea* scrollArea);
    void setStatus(const QString& message);
    void setDirty(bool dirty);
    bool confirmDiscardOrSave(const QString& actionText);
    GuiFilterParams collectFilterParams() const;
    QImage snapshotToQImage(const GuiImageSnapshot& snapshot) const;
    int currentFilterNumber() const;

    Ui::MainWindow* ui = nullptr;

    QString currentFilePath_;
    QString mergeSecondPath_;

    GuiImageSnapshot undoSnapshot_;
    GuiImageSnapshot redoSnapshot_;
    bool hasUndo_ = false;
    bool hasRedo_ = false;
    bool dirty_ = false;

    double zoomFactor_ = 1.0;
    bool fitToWindow_ = true;
};
