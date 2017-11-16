// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_uipaintwidget.h"
#include "se_controller.h"
#include <stdlib.h>

SE_UIPaintWidget::SE_UIPaintWidget(QImage image, QWidget* parent):
    QWidget(parent),
    m_labelImage(this)
{
    m_currentShapeCode = 0;
    m_mouseStop = false;
    setMinimumSize (768, 768);
    setMaximumSize (768, 768);
    m_selectedColor = QColor(0, 0, 0, 255);
    m_currentImage = image;
    m_firstImage = m_currentImage;
    m_previousImage.push(m_currentImage);
    m_paintAgain = false;
}

void SE_UIPaintWidget::UndoPainting()
{
    if (m_previousImage.size() == 0)
    {
        m_currentImage = m_firstImage;
    }
    else
    {
        m_currentImage = m_previousImage.pop();
    }
    UpdateImage();
    m_paintAgain = false;
    SE_Controller::GetInstance()->UpdateImageAtCurrentFrame(m_currentImage);
}

void SE_UIPaintWidget::SetOriginalSize(QSize size)
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
    m_scaleNumber = ((double)768)/maxNum;
    UpdateImage();
}

void SE_UIPaintWidget::SetCurrentShape(int shape)
{
    if (shape != m_currentShapeCode)
        m_currentShapeCode = shape;
}

void SE_UIPaintWidget::SetSelectedColor(QColor c)
{
    m_selectedColor = c;
}

void SE_UIPaintWidget::SetCurrentImage(QImage image)
{
    m_previousImage.clear();
    m_firstImage = image;
    m_currentImage = image;
    UpdateImage();
}

QPoint SE_UIPaintWidget::GetScaledCoordinate(QPoint p)
{
    return QPoint(p.x()/m_scaleNumber, p.y()/m_scaleNumber);
}

QColor SE_UIPaintWidget::GetCurrentColor()
{
    return m_selectedColor;
}

void SE_UIPaintWidget::UpdateImage()
{
    QImage scaledImage = m_currentImage.scaled(m_scaleNumber * m_originalSize.width(), m_scaleNumber * m_originalSize.height(), Qt::KeepAspectRatio);
    this->setMaximumSize(QSize(scaledImage.size()));
    this->setMinimumSize(QSize(scaledImage.size()));
    m_labelImage.setStyleSheet("QLabel { background-image: url(:/Icons/bg.png)}");
    m_labelImage.setAlignment(Qt::AlignTop);
    m_labelImage.setPixmap(QPixmap::fromImage(scaledImage));
    m_labelImage.show();
}

void SE_UIPaintWidget::swap(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

void SE_UIPaintWidget::DrawPoint(QPoint p, QColor c)
{
    if (!(p.x() < 0 || p.x() > m_originalSize.width()-1 || p.y() < 0 || p.y() > m_originalSize.height()-1))
        m_currentImage.setPixelColor(p, c);
}

void SE_UIPaintWidget::DrawLine(QPoint start, QPoint end, QColor c)
{
    int x1 = start.x();
    int x2 = end.x();
    int y1 = start.y();
    int y2 = end.y();

    bool cond = abs(y2 - y1) > abs(x2 - x1);

    if (cond)
    {
         swap(x1, y1);
         swap(x2, y2);
    }
    if (x1 > x2)
    {
         swap(x1, x2);
         swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = abs(y2 - y1);

    int D = 2 * dy - dx;

    int inc0 = 2 * dy;
    int inc1 = 2 * (dy - dx);

    bool y_Cond = y1 < y2;

    while (x1 <= x2)
    {
         if (cond)
             DrawPoint(QPoint(y1, x1), c);
         else
             DrawPoint(QPoint(x1, y1), c);

         if (D <= 0)
         {
             D = D + inc0;
         }
         else
         {
             D = D + inc1;
             if (y_Cond)
                 y1++;
             else {
                 y1--;
             }
         }
         x1++;
     }
}

void SE_UIPaintWidget::DrawRectangle(QPoint start, QPoint end, QColor c)
{
    DrawLine(start, QPoint(end.x(), start.y()), c);
    DrawLine(start, QPoint(start.x(), end.y()), c);
    DrawLine(QPoint(end.x(), start.y()), end, c);
    DrawLine(QPoint(start.x(), end.y()), end, c);
}

void SE_UIPaintWidget::DrawCirclePoints(int centerX, int centerY , int x , int y, QColor c)
{
    DrawPoint(QPoint(centerX + x, centerY + y), c);
    DrawPoint(QPoint(centerX - x, centerY + y), c);

    DrawPoint(QPoint(centerX - y, centerY + x), c);
    DrawPoint(QPoint(centerX - y, centerY - x), c);

    DrawPoint(QPoint(centerX + x, centerY - y), c);
    DrawPoint(QPoint(centerX - x, centerY - y), c);

    DrawPoint(QPoint(centerX + y, centerY - x), c);
    DrawPoint(QPoint(centerX + y, centerY + x), c);
}

void SE_UIPaintWidget::DrawCircle(int x0, int y0, int R, QColor c)
{
    int x = 0;
    int y = R;
    int D = 1 - R;

    DrawCirclePoints(x0, y0, 0, R, c);

    while (y > x)
    {
        if (D < 0)
            D = D + 3 + 2 * x;
        else
        {
            D = D + 2 * (x - y) + 5;
            y = y - 1;
        }
        x++;
        DrawCirclePoints(x0, y0, x, y, c);
    }
}

void SE_UIPaintWidget::FillRectangle(QPoint start, QPoint end, QColor c)
{
    int start_X = start.x();
    int start_Y = start.y();
    int end_X = end.x();
    int end_Y = end.y();

    for (int i = 0; i < abs(start_Y - end_Y); i++)
    {
        if (end_Y < start_Y)
        {
            DrawLine(QPoint(start_X, start_Y - i), QPoint(end_X, start_Y - i), c);
        }
        if (end_Y > start_Y)
        {
            DrawLine(QPoint(start_X, start_Y + i), QPoint(end_X, start_Y + i), c);
        }
    }
}

void SE_UIPaintWidget::FillCirclePoints(int x, int y , int centerX , int centerY, QColor c)
{
    DrawPoint(QPoint(centerX + x, centerY + y), c);
    DrawPoint(QPoint(centerX - x, centerY + y), c);
    DrawLine(QPoint(centerX + x, centerY + y), QPoint(centerX - x, centerY + y), c);

    DrawPoint(QPoint(centerX - y, centerY + x), c);
    DrawPoint(QPoint(centerX - y, centerY - x), c);
    DrawLine(QPoint(centerX - y, centerY + x), QPoint(centerX - y, centerY - x), c);

    DrawPoint(QPoint(centerX + x, centerY - y), c);
    DrawPoint(QPoint(centerX - x, centerY - y), c);
    DrawLine(QPoint(centerX + x, centerY - y), QPoint(centerX - x, centerY - y), c);

    DrawPoint(QPoint(centerX + y, centerY - x), c);
    DrawPoint(QPoint(centerX + y, centerY + x), c);
    DrawLine(QPoint(centerX + y, centerY - x), QPoint(centerX + y, centerY + x), c);

    for (int i = -x; i <= x; ++i)
    {
        DrawLine(QPoint(centerX + i, centerY + y), QPoint(centerX + i, centerY - y), c);
    }
}

void SE_UIPaintWidget::FillCircle(int x0, int y0, int R, QColor c)
{

    int x = 0;
    int y = R;
    int D = 1 - R;

    FillCirclePoints(0, R, x0, y0, c);
    while (y > x)
    {
        if (D < 0)
        {
            D = D + 2 * x + 3;
        }
        else
        {
            D = D + 2 * (x - y) + 5;
            y = y - 1;
        }
        x = x + 1;
        FillCirclePoints(x, y, x0, y0, c);
    }

}

void SE_UIPaintWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_paintAgain)
    {
        m_previousImage.push(m_currentImage);
    }
    m_mouseStop = false;
    m_startPoint = event->pos();
    m_endPoint = event->pos();
    if (m_currentShapeCode == 0)
    {
        DrawPoint(GetScaledCoordinate(m_startPoint), m_selectedColor);
    }
    else if (m_currentShapeCode == 1)
    {
        DrawLine(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
    }
    else if (m_currentShapeCode == 2)
    {
        DrawRectangle(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
    }
    else if (m_currentShapeCode == 3)
    {
        DrawPoint(GetScaledCoordinate(m_startPoint), QColor(0, 0, 0, 0));
    }
    else if (m_currentShapeCode == 4)
    {
        FillRectangle(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
    }

    m_tmpImage = m_currentImage;
    m_tmpStartPoint = m_startPoint;
}

void SE_UIPaintWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_currentShapeCode == 0 && !m_mouseStop)
    {
        if (m_startPoint != m_tmpStartPoint)
        {
            m_startPoint = m_tmpStartPoint;
        }
        m_mouseStop = false;
        m_endPoint = event->pos();
        DrawLine(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
        UpdateImage();
        m_tmpStartPoint = event->pos();
        m_tmpImage = m_currentImage;
    }
    if (m_currentShapeCode == 3 && !m_mouseStop)
    {
        if (m_startPoint != m_tmpStartPoint)
        {
            m_startPoint = m_tmpStartPoint;
        }
        m_mouseStop = false;
        m_endPoint = event->pos();
        DrawLine(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), QColor(0, 0, 0, 0));
        UpdateImage();
        m_tmpStartPoint = event->pos();
        m_tmpImage = m_currentImage;
    }

    if (m_currentShapeCode != 0 && m_currentShapeCode != 3 && !m_mouseStop)
    {
        m_currentImage = m_tmpImage;

        m_endPoint = event->pos();
        if (m_currentShapeCode == 1)
        {
            DrawLine(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
        }
        else if (m_currentShapeCode == 2)
        {
            DrawRectangle(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
        }
        else if (m_currentShapeCode == 4)
        {
            FillRectangle(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
        }
        else if (m_currentShapeCode == 5)
        {
            if (((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() < m_endPoint.y()))) ||
                 ((m_startPoint.x() > m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y()))))
            {
                int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
                int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
                if (abs(widthDiff) >= abs(heightDiff))
                {
                    int radius = heightDiff/2;
                    DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
                if (abs(widthDiff) < abs(heightDiff))
                {
                    int radius = widthDiff/2;
                    DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
            }
            else if ((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y())))
            {
                int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
                int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
                if (widthDiff >= abs(heightDiff))
                {
                    int radius = heightDiff/2;
                    DrawCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
                if (widthDiff < abs(heightDiff))
                {
                    int radius = widthDiff/2;
                    DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
                }
            }
            else
            {
                int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
                int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
                if (abs(widthDiff) >= heightDiff)
                {
                    int radius = heightDiff/2;
                    DrawCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
                if (abs(widthDiff) < heightDiff)
                {
                    int radius = widthDiff/2;
                    DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
                }
            }
        }
        else if (m_currentShapeCode == 6)
        {
            if (((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() < m_endPoint.y()))) ||
                 ((m_startPoint.x() > m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y()))))
            {
                int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
                int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
                if (abs(widthDiff) >= abs(heightDiff))
                {
                    int radius = heightDiff/2;
                    FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
                if (abs(widthDiff) < abs(heightDiff))
                {
                    int radius = widthDiff/2;
                    FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
            }
            else if ((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y())))
            {
                int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
                int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
                if (widthDiff >= abs(heightDiff))
                {
                    int radius = heightDiff/2;
                    FillCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
                if (widthDiff < abs(heightDiff))
                {
                    int radius = widthDiff/2;
                    FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
                }
            }
            else
            {
                int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
                int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
                if (abs(widthDiff) >= heightDiff)
                {
                    int radius = heightDiff/2;
                    FillCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
                }
                if (abs(widthDiff) < heightDiff)
                {
                    int radius = widthDiff/2;
                    FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
                }
            }
        }
        UpdateImage();
    }

}

void SE_UIPaintWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mouseStop = true;
    m_currentImage = m_tmpImage;
    if (m_currentShapeCode == 0)
    {
        DrawPoint(GetScaledCoordinate(m_startPoint), m_selectedColor);
    }
    else if (m_currentShapeCode == 1)
    {
        DrawLine(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
    }
    else if (m_currentShapeCode == 2)
    {
        DrawRectangle(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
    }
    else if (m_currentShapeCode == 3)
    {
        DrawPoint(GetScaledCoordinate(m_startPoint), QColor(0, 0, 0, 0));
    }
    else if (m_currentShapeCode == 4)
    {
        FillRectangle(GetScaledCoordinate(m_startPoint), GetScaledCoordinate(m_endPoint), m_selectedColor);
    }
    else if (m_currentShapeCode == 5)
    {
        if (((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() < m_endPoint.y()))) ||
             ((m_startPoint.x() > m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y()))))
        {
            int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
            int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
            if (abs(widthDiff) >= abs(heightDiff))
            {
                int radius = heightDiff/2;
                DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
            if (abs(widthDiff) < abs(heightDiff))
            {
                int radius = widthDiff/2;
                DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
        }
        else if ((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y())))
        {
            int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
            int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
            if (widthDiff >= abs(heightDiff))
            {
                int radius = heightDiff/2;
                DrawCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
            if (widthDiff < abs(heightDiff))
            {
                int radius = widthDiff/2;
                DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
            }
        }
        else
        {
            int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
            int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
            if (abs(widthDiff) >= heightDiff)
            {
                int radius = heightDiff/2;
                DrawCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
            if (abs(widthDiff) < heightDiff)
            {
                int radius = widthDiff/2;
                DrawCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
            }
        }
    }
    else if (m_currentShapeCode == 6)
    {
        if (((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() < m_endPoint.y()))) ||
             ((m_startPoint.x() > m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y()))))
        {
            int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
            int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
            if (abs(widthDiff) >= abs(heightDiff))
            {
                int radius = heightDiff/2;
                FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
            if (abs(widthDiff) < abs(heightDiff))
            {
                int radius = widthDiff/2;
                FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
        }
        else if ((m_startPoint.x() < m_endPoint.x()) && ((m_startPoint.y() > m_endPoint.y())))
        {
            int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
            int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
            if (widthDiff >= abs(heightDiff))
            {
                int radius = heightDiff/2;
                FillCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
            if (widthDiff < abs(heightDiff))
            {
                int radius = widthDiff/2;
                FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
            }
        }
        else
        {
            int widthDiff = GetScaledCoordinate(m_endPoint).x() - GetScaledCoordinate(m_startPoint).x();
            int heightDiff = GetScaledCoordinate(m_endPoint).y() - GetScaledCoordinate(m_startPoint).y();
            if (abs(widthDiff) >= heightDiff)
            {
                int radius = heightDiff/2;
                FillCircle(GetScaledCoordinate(m_startPoint).x() - radius, GetScaledCoordinate(m_startPoint).y() + radius, abs(radius), m_selectedColor);
            }
            if (abs(widthDiff) < heightDiff)
            {
                int radius = widthDiff/2;
                FillCircle(GetScaledCoordinate(m_startPoint).x() + radius, GetScaledCoordinate(m_startPoint).y() - radius, abs(radius), m_selectedColor);
            }
        }
    }
    UpdateImage();
    SE_Controller::GetInstance()->UpdateImageAtCurrentFrame(m_currentImage);
    emit SignalUpdatedPainting();
    m_paintAgain = true;
}
