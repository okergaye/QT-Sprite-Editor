// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_UI_MAIN_WINDOW_H__
#define __SE_UI_MAIN_WINDOW_H__

#include <QMainWindow>
#include <QColorDialog>
#include "se_uipaintwidget.h"
#include "se_uinewdialog.h"
#include "se_uianimewidget.h"
#include "se_uiframeshowwidget.h"

namespace Ui
{
    class SE_UIMainWindow;
}

// The main window, holding all widgets
class SE_UIMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor of the main window
    explicit SE_UIMainWindow(QWidget* parent = 0);
    // The distructor of the main window
    ~SE_UIMainWindow();

private slots:
    // Open the color picker for the user
    void SlotColorChosen();

    // Set the drawing tool to line drawer
    void SlotDrawLineChosen();

    // Set the drawing tool to point drawer
    void SlotDrawPointChosen();

    // Set the drawing tool to rectangle drawer
    void SlotDrawRectangleChosen();

    // Set the drawing tool to rectangle filler
    void SlotFillRectangleChosen();

    // Set the drawing tool to circle drawer
    void SlotDrawCircleChosen();

    // Set the drawing tool to circle filler
    void SlotFillCircleChosen();

    // Set the drawing tool to eraser
    void SlotDrawEraserChosen();

    // Show the project loader dialog window
    void SlotLoadFileMenu();

    // Show the project saver dialog window
    void SlotSaveToFileMenu();

    // Show the new project dialog window
    void SlotNewFileMenu();

    // Show the helper dialog window
    void SlotHelpFileMenu();

    // Show the gif saver dialog window
    void SlotLoadGifMenu();

    // Show the gif loader dialog window
    void SlotSaveGifMenu();

    // To set a new project with given project name and actual size
    void SlotSetNewProject(QString name, QSize size);

    // Let the user undo the latest job
    void SlotUndoChosen();

    // Set the current frame to given number
    void SlotSetFrame(int frameID);

    // Get the previous frame
    void SlotGetPreviousFrame();

    // Get the next frame
    void SlotGetNextFrame();

    // Refresh the display for animation
    void SlotRefreshWidgetDisplay();

    // To add a new frame to the project
    void SlotAddNewFrame();

protected:
    // Accept an event to close
    virtual void	closeEvent(QCloseEvent* event);

private:
    QSize m_size;
    Ui::SE_UIMainWindow* m_ui;
    SE_UIPaintWidget* m_paintWidget;
    SE_UIFrameShowWidget* m_previousWidget;
    SE_UIFrameShowWidget* m_nextWidget;
    QColorDialog* m_colorPicker;
    SE_UINewDialog m_newDialog;
    SE_UIAnimeWidget m_animePreview;
    QImage m_currentImage;
    QString m_projectName;
    int m_frameNumber;
    QImage m_defaultImage;
};

#endif // __SE_UI_MAIN_WINDOW_H__

