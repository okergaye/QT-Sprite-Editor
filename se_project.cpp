// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_project.h"
#include "se_frame.h"
#include <QDebug>

using namespace std;

SE_Project::SE_Project(string name, int width, int height, QObject* parent):
    QObject(parent),
    m_name(name),
    m_width(width),
    m_height(height),
    m_idxCurrentFrame(-1),
    m_countFrames(0),
    m_isSaved(false)
{
    AddNewFrame();
    m_idxCurrentFrame=0;
}

SE_Project::SE_Project(std::string name, int width, int height, vector<QImage> images, QObject* parent):
    QObject(parent),
    m_name(name),
    m_width(width),
    m_height(height),
    m_idxCurrentFrame(-1),
    m_countFrames(0),
    m_isSaved(false)
{

    for(int i = 0; i < images.size();++i)
    {
        m_frames.push_back(SE_Frame(images[i]));
    }

    if(m_frames.size()>0)
    {
        m_countFrames = m_frames.size();
        m_idxCurrentFrame = 0;
    }
}

std::string SE_Project::GetName()
{
    return m_name;
}

int SE_Project::GetWidth()
{
    return m_width;
}

int SE_Project::GetHeight()
{
    return m_height;
}

bool SE_Project::AddNewFrame()
{
    SE_Frame frame(m_width , m_height);
    m_frames.push_back(frame);
    m_countFrames++;
    return true;
}

bool SE_Project::AddNewFrameAfter(int idx)
{
    if(IsIndexValid(idx))
    {
        SE_Frame frame(m_width , m_height);
        m_frames.insert(m_frames.begin()+idx,frame);
        m_countFrames++;
        return true;
    }
    return false;
}

void SE_Project::SetIsSaved(bool isSaved)
{
    m_isSaved = isSaved;
}

bool SE_Project::IsSaved()
{
    return m_isSaved;
}

int SE_Project::GetIndexOfCurrentFrame()
{
    return m_idxCurrentFrame;
}

bool SE_Project::SetCurrentFrame(int idx)
{
    if(IsIndexValid(idx))
    {
        m_idxCurrentFrame = idx;
        return true;
    }

    return false;
}

bool SE_Project::UpdateImageAtCurrent(QImage newImage)
{
    if(IsIndexValid(m_idxCurrentFrame))
    {
        m_frames[m_idxCurrentFrame].RefreshImage(newImage);
        return true;
    }

    return false;
}

int SE_Project::GetCurrentFrameID()
{
    return m_idxCurrentFrame;
}

int SE_Project::GetFrameCount()
{
    return m_countFrames;
}

QImage SE_Project::GetFrameCopy(int idxFrame)
{
    if(IsIndexValid(idxFrame))
        return m_frames[idxFrame].GetCopy();
    else
        return m_frames[m_idxCurrentFrame].GetCopy();
}

vector<QImage> SE_Project::GetAllFramesCopy()
{
    vector<QImage> rtn;
    for(int i = 0; i < m_frames.size(); ++i)
    {
        rtn.push_back(m_frames[i].GetCopy());
    }
    return rtn;
}

bool SE_Project::IsIndexValid(int index)
{
    if(index < 0 || index >= m_countFrames)
        return false;

    return true;

}


