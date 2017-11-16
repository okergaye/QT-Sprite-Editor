// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_UI_FRAME_SHOW_WIDGET_H__
#define __SE_UI_FRAME_SHOW_WIDGET_H__

#include <QWidget>
#include <QPaintEvent>
#include <QList>
#include <QPoint>
#include <QColor>
#include <QLabel>
#include <QStack>

// this class provides the clickable preview board for previous and next frames
class SE_UIFrameShowWidget : public QWidget
{
    Q_OBJECT
public:
    // constructor for frame view widget with default image
    explicit SE_UIFrameShowWidget(QImage image, QWidget* parent = 0);

    // Set the size of original image
    void SetOriginalSize(QSize size);

    // Set the current image to display
    void SetCurrentImage(QImage image);

    // Set a bool flag to true if the image passed in is a default one
    void SetDefault(bool defaultFlag);

protected:
    // Accept mouse event
    virtual void mousePressEvent(QMouseEvent *event);

signals:
    // Send a signal saying this widget is clicked
    void SignalClicked();

private:
    // Update the image displayed on the widget
    void UpdateImage();

    QSize m_originalSize;
    QSize m_scaledSize;
    QImage m_currentImage;
    QLabel m_labelImage;
    double m_scaleNumber;
    int m_frameNumber;
    bool m_default;
};

#endif // __SE_UI_FRAME_SHOW_WIDGET_H__

