// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_UI_PAINT_WIDGET_H__
#define __SE_UI_PAINT_WIDGET_H__

#include <QtWidgets>

// this class provides the drawing board
class SE_UIPaintWidget : public QWidget
{
    Q_OBJECT
public:
    // constructor for paint widget with default image
    explicit SE_UIPaintWidget(QImage image, QWidget* parent = 0);

    // Set the size of original image
    void SetOriginalSize(QSize size);

    // Set the color that the user selected
    void SetSelectedColor(QColor c);

    // Set the current image to display
    void SetCurrentImage(QImage image);

    // Let the user undo the last job
    void UndoPainting();

    // Get the color giving by the color picker
    QColor GetCurrentColor();

    // Set the current shape the user chooses with an int
    // 0 : default pen
    // 1 : draw line
    // 2 : draw rectangle
    // 3 : eraser
    // 4 : fill rectangle
    // 5 : draw circle
    // 6 : fill circle
    void SetCurrentShape(int shape);

signals:
    // A signal telling the paint board is editted
    void SignalUpdatedPainting();

protected:
    // Accept a mouse press event
    virtual void mousePressEvent(QMouseEvent* event);

    // Accept a mouse move event
    virtual void mouseMoveEvent(QMouseEvent* event);

    // Accept a mouse release event
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    // Update the image displayed on the widget
    void UpdateImage();

    // Get the scaled coordinate of a point
    QPoint GetScaledCoordinate(QPoint p);

    // Swap two ints
    void swap(int& a, int& b);

    // Put a selected color onto a pixel
    void DrawPoint(QPoint p, QColor c);

    // Draw a line from the start point to the end point with selected color
    // by using Bresenham Algorithm
    void DrawLine(QPoint start, QPoint end, QColor c);

    // Draw a rectangle from the start point to the end point with selected color
    void DrawRectangle(QPoint start, QPoint end, QColor c);

    // Draw eight points together for every one eighth of circle
    void DrawCirclePoints(int x, int y, int centerX, int centerY, QColor c);

    // Draw a circle from the center through the radius with selected color
    // by using Bresenham Algorithm
    void DrawCircle(int x, int y, int r, QColor c);

    // Fill a rectangle
    void FillRectangle(QPoint start, QPoint end, QColor c);

    // Draw the points and lines for one eighth of a circle
    void FillCirclePoints(int x, int y, int centerX, int centerY, QColor c);

    // Fill a circle
    void FillCircle(int x, int y, int r, QColor c);

    int m_currentShapeCode;
    bool m_mouseStop;
    bool m_paintAgain;
    QPoint m_startPoint;
    QPoint m_endPoint;
    QPoint m_tmpStartPoint;
    QSize m_originalSize;
    QColor m_selectedColor;
    QImage m_currentImage;
    QImage m_tmpImage;
    QImage m_firstImage;
    QStack<QImage> m_previousImage;
    QLabel m_labelImage;
    double m_scaleNumber;
};
#endif // __SE_UI_PAINT_WIDGET_H__
