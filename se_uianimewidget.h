// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_UI_ANIME_WIDGET_H__
#define __SE_UI_ANIME_WIDGET_H__

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <vector>

// The animation preview window
class SE_UIAnimeWidget : public QWidget
{
    Q_OBJECT

public:
    // Default Constructor for creating a window with given size and animation fps
    explicit SE_UIAnimeWidget(int width, int height, int numOfFramesPerSec = 2, QWidget* parent = nullptr);

    // Default Destructor
    virtual ~SE_UIAnimeWidget();

    // Override the resize event.
    virtual void resizeEvent(QResizeEvent *event);

    // Change the animation preview according to the actual size of the sprites
    void SetToActualSize();

    // Set animation fps
    void SetSpeed(int numOfFramesPerSec);

    // Replace current frames with new frames
    void SetAllFrames(std::vector<QImage> frames);

    // Update the one of the frame according to the index
    void UpdateFrame(int idxFrame, QImage newImage);

    // Pop up the window
    void ShowUp();

    // Reload all frames from the controller
    void ReloadAllFrames();

    // Update the changed frame from the controller
    void UpdateCurrentFrame();

public slots:

    // Slot for main window to pop up this window
    void SlotShowUp();

    // Slot for main window to resize the frame according to the actual size
    void SlotSetToActualSize();

    // Slot for main window to speed up the fps
    void SlotSpeedUp();

    // Slot for main window to speed down the fps
    void SlotSpeedDown();

    // Slot for main window to update the fps
    void SlotSetSpeed(int numOfFramesPerSec);

    // Slot for timer to update the frame
    void SlotUpdate();

private:
    // Helper function to set the current frame
    void SetImage(int idx);

    QImage m_image;
    QLabel m_labelImage;
    QPixmap m_pixmap;
    int m_flagResize;
    float m_ratioImage;
    QSize m_sizeOriginal;
    QSize m_sizePre;
    int m_idxCurrentFrame;
    int m_numOfFramesPerSec;
    QTimer m_timer;
    std::vector<QImage> m_frames;
};

#endif // __SE_UI_ANIME_WIDGET_H__
