// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_frame.h"

SE_Frame::SE_Frame(int width, int height, QObject* parent):QObject(parent)
{
    m_image = QImage(width, height, QImage::Format_RGBA8888_Premultiplied);
    m_image.fill(QColor(255, 255, 255, 0));
}

SE_Frame::SE_Frame(QImage image, QObject* parent):QObject(parent)
{
    m_image = image.copy();
}

SE_Frame::SE_Frame(const SE_Frame& other)
{
    setParent(other.parent());
    m_image = other.m_image.copy();
}

SE_Frame& SE_Frame::operator =(const SE_Frame& other)
{
    setParent(other.parent());
    m_image = other.m_image.copy();
    return *this;
}

void SE_Frame::SetColor(int x, int y, QColor color)
{
  m_image.setPixelColor(x, y, color);
}

QImage SE_Frame::GetCopy()
{
    QRect rect;
    return m_image.copy(rect);
}

QColor SE_Frame::GetColor(int x, int y)
{
    return m_image.pixelColor(x, y);
}

void SE_Frame::RefreshImage(QImage image)
{
	m_image = image.copy();
}

