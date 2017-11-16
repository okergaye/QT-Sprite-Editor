// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_FRAME_H__
#define __SE_FRAME_H__
#include <QImage>
#include <QObject>
#include <QColor>

class SE_Frame : public QObject
{
    Q_OBJECT
public:
    // Create an empty frame with given size.
    SE_Frame(int width, int height, QObject* parent = nullptr);

    // Create an frame with given image
    SE_Frame(QImage image, QObject* parent = nullptr);

    // Deep copy constructor for the std library
    SE_Frame(const SE_Frame& other);

    // Rule of Three
    SE_Frame& operator =(const SE_Frame& other);

     // Returns a deep copy of QImage using the copy constructor.
    QImage GetCopy();

    // Set the Color at the given coordinates
    void SetColor(int x, int y, QColor color);

    // Retreive the color at the given coordinates
    QColor GetColor(int x, int y);

    // Replace the image with the given one.
    void RefreshImage(QImage image);

private:
    QImage m_image;
};

#endif // __SE_FRAME_H__

