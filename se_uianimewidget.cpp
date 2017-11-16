// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_uianimewidget.h"
#include "se_controller.h"
#include <QDebug>

#define CONTROLLER SE_Controller::GetInstance()

using namespace std;

SE_UIAnimeWidget::SE_UIAnimeWidget(int width, int height, int numOfFramesPerSec, QWidget* parent):
    QWidget(parent),
    m_labelImage(this),
    m_pixmap(),
    m_numOfFramesPerSec(numOfFramesPerSec),
    m_flagResize(0),
    m_sizeOriginal(width , height),
    m_sizePre(width,height)
{
    connect(&m_timer, &QTimer::timeout, this, &SE_UIAnimeWidget::SlotUpdate);

    m_labelImage.setBackgroundRole(QPalette::Base);
    m_labelImage.setScaledContents(true);

    this->setBackgroundRole(QPalette::Dark);
    this->setVisible(false);
    this->resize(width, height);
}

SE_UIAnimeWidget::~SE_UIAnimeWidget()
{
    m_timer.stop();
    disconnect(&m_timer, &QTimer::timeout, this, &SE_UIAnimeWidget::SlotUpdate);
}

void SE_UIAnimeWidget::resizeEvent(QResizeEvent* event)
{
    if(m_flagResize ==  2)
    {
        // Resize from users
        QSize size = this->size();
        m_flagResize = 3;

        if((size.width() < m_sizePre.width() && size.height() < m_sizePre.height()) ||
            (size.width() < m_sizePre.width() && size.height() == m_sizePre.height()) ||
            (size.width() == m_sizePre.width() && size.height() < m_sizePre.height()))
        {
            if(size.width() >= size.height())
            {
                int width = size.width();
                int height = size.height();

                if(height <= 232)
                {
                    width = 232/m_ratioImage;
                    height = 232;
                }
                else
                {
                    width = height/m_ratioImage;
                }

                this->resize(width, height);
            }
            else
            {
                int width = size.width();
                int height = size.height();

                if(width <= 232)
                {
                    width = 232;
                    height = 232*m_ratioImage;
                }
                else
                {
                    height = width*m_ratioImage;
                }

                this->resize(width, height);
            }
        }
        else if ((size.width() > m_sizePre.width() && size.height() > m_sizePre.height()) ||
                 (size.width() > m_sizePre.width() && size.height() == m_sizePre.height()) ||
                 (size.width() == m_sizePre.width() && size.height() > m_sizePre.height()) ||
                 (size.width() > m_sizePre.width() && size.height() < m_sizePre.height()) ||
                 (size.width() < m_sizePre.width() && size.height() > m_sizePre.height()))
        {
             if(size.width() >= size.height())
             {
                 this->resize(size.width() , size.width()*m_ratioImage);
             }
             else
             {
                 this->resize(size.height()/m_ratioImage, size.height());
             }
        }
    }
    else if(m_flagResize == 3)
    {
        //Resize from keeping aspect ratio
        m_sizePre = this->size();
        m_labelImage.resize(this->size().width(),this->size().height());
        m_labelImage.setPixmap(m_pixmap.scaled(this->size(),Qt::IgnoreAspectRatio, Qt::FastTransformation));
        m_flagResize = 2;
    }
    else if (m_flagResize == 10)
    {
        // Actuall Size Button
        QSize size = this->size();

        if(size.width()<232 || size.height() < 232)
        {
            m_sizePre = m_sizeOriginal;
            m_labelImage.resize(232,232*m_ratioImage);
            m_labelImage.setPixmap(m_pixmap.scaled(m_pixmap.size(),Qt::IgnoreAspectRatio, Qt::FastTransformation));

            m_flagResize = 11;
            this->resize(232,232);
        }
        else
        {
            m_sizePre = this->size();
            m_labelImage.resize(m_sizeOriginal);
            m_labelImage.setPixmap(m_pixmap.scaled(m_sizeOriginal,Qt::IgnoreAspectRatio, Qt::FastTransformation));

            m_flagResize = 11;
        }
    }
    else if (m_flagResize == 11)
    {
        m_flagResize = 2;
    }
}

void SE_UIAnimeWidget::SetToActualSize()
{
    m_flagResize = 10;
    QSize size(m_sizeOriginal.width(),m_sizeOriginal.height());
    resize(size);
}

void SE_UIAnimeWidget::SetSpeed(int numOfFramesPerSec)
{
    m_numOfFramesPerSec = numOfFramesPerSec;
    m_timer.stop();

    m_timer.start(1000/m_numOfFramesPerSec);
}

void SE_UIAnimeWidget::SetAllFrames(std::vector<QImage> frames)
{
    m_idxCurrentFrame = 0;
    m_flagResize = 0;
    m_frames = frames;

    if(m_frames.size() > 0)
    {
        m_sizeOriginal = m_frames[0].size();
    }

}

void SE_UIAnimeWidget::UpdateFrame(int idxFrame, QImage newImage)
{
    if(idxFrame >= 0 && idxFrame < m_frames.size())
    {
        m_frames[idxFrame] = newImage;
    }
}

void SE_UIAnimeWidget::ShowUp()
{
    this->setVisible(true);
    ReloadAllFrames();
    m_timer.start(1000/m_numOfFramesPerSec);
}

void SE_UIAnimeWidget::ReloadAllFrames()
{
    vector<QImage> frames = CONTROLLER->GetAllFramesCopy();
    SetAllFrames(frames);

    if(m_frames.size()>0)
        SetImage(0);
}

void SE_UIAnimeWidget::UpdateCurrentFrame()
{
    int idx = CONTROLLER->GetCurrentFrameID();
    QImage image = CONTROLLER->GetFrameCopy(idx);
    UpdateFrame(idx , image);
}

void SE_UIAnimeWidget::SlotShowUp()
{
    ShowUp();
}

void SE_UIAnimeWidget::SlotSetToActualSize()
{
    SetToActualSize();
}

void SE_UIAnimeWidget::SlotSpeedUp()
{
    SetSpeed(m_numOfFramesPerSec+1);
}

void SE_UIAnimeWidget::SlotSpeedDown()
{
    SetSpeed(m_numOfFramesPerSec-1);
}


void SE_UIAnimeWidget::SlotSetSpeed(int numOfFramesPerSec)
{
    if(numOfFramesPerSec <= 0)
        numOfFramesPerSec = 1;
    else if(numOfFramesPerSec > 144)
        numOfFramesPerSec = 144;


    SetSpeed(numOfFramesPerSec);
}

void SE_UIAnimeWidget::SlotUpdate()
{

    if(m_idxCurrentFrame < 0 || m_idxCurrentFrame >= m_frames.size())
    {
        m_idxCurrentFrame = 0;
    }

    SetImage(m_idxCurrentFrame);

    m_idxCurrentFrame++;
}

void SE_UIAnimeWidget::SetImage(int idx)
{
    m_image = m_frames[idx];
    m_ratioImage = ((float)(m_image.size().height()))/(((float)m_image.size().width()));

    if(m_flagResize == 0)
    {
        this->resize(m_image.size());
        m_sizePre = m_image.size();
    }

    m_labelImage.resize(m_sizePre);

    m_pixmap = QPixmap::fromImage(m_image);
    m_labelImage.setPixmap(m_pixmap.scaled(m_sizePre,Qt::IgnoreAspectRatio, Qt::FastTransformation));

    m_flagResize=2;

}
