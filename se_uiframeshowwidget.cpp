// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_uiframeshowwidget.h"

SE_UIFrameShowWidget::SE_UIFrameShowWidget(QImage image, QWidget* parent) : QWidget(parent), m_labelImage(this)
{
    setMinimumSize (200, 200);
    setMaximumSize (200, 200);
    m_currentImage = image;
}

void SE_UIFrameShowWidget::SetOriginalSize(QSize size)
{
    m_originalSize = size;
    double maxNum;
    if (size.width() > size.height())
    {
        maxNum = size.width();
    }
    else
    {
        maxNum = size.height();
    }
    m_scaleNumber = 200/maxNum;
    UpdateImage();
}

void SE_UIFrameShowWidget::SetCurrentImage(QImage image)
{
    m_currentImage = image;
    UpdateImage();
}

void SE_UIFrameShowWidget::UpdateImage()
{
    QImage scaledImage = m_currentImage.scaled(m_scaleNumber * m_originalSize.width(), m_scaleNumber * m_originalSize.height(), Qt::KeepAspectRatio);
    this->setMaximumSize(QSize(scaledImage.size()));
    this->setMinimumSize(QSize(scaledImage.size()));
    if (!m_default)
        m_labelImage.setStyleSheet("QLabel { background-image: url(:/Icons/bg.png)}");
    else
        m_labelImage.setStyleSheet("QLabel { background-image: url()}");
    m_labelImage.setAlignment(Qt::AlignTop);
    m_labelImage.setPixmap(QPixmap::fromImage(scaledImage));
    m_labelImage.show();
}

void SE_UIFrameShowWidget::SetDefault(bool defaultFlag)
{
    m_default = defaultFlag;
}


void SE_UIFrameShowWidget::mousePressEvent(QMouseEvent* event)
{
    emit SignalClicked();
}

