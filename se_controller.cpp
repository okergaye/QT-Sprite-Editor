// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_controller.h"
#include "se_project.h"
#include "se_projectloadersaver.h"
#include "se_gifloadersaver.h"

using namespace std;

SE_Controller* SE_Controller::m_instance = new SE_Controller(nullptr);

SE_Controller::SE_Controller(QObject* parent):
    QObject(parent),
    m_project(nullptr)
{

}

SE_Controller::~SE_Controller()
{
    CloseProject();
}

SE_Controller* SE_Controller::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SE_Controller(nullptr);
    }

    return m_instance;
}

bool SE_Controller::NewProject(QString name, int width, int height)
{
    if(CloseProject())
    {
        m_project = new SE_Project(name.toStdString(), width, height);
        return true;
    }
    else
    {
            return false;
    }
}

bool SE_Controller::CloseProject()
{

    if(m_project != nullptr)
    {
        delete m_project;
    }

    m_project = nullptr;
    return true;
}

bool SE_Controller::LoadProject(QString projFilePath)
{
    SE_Project* proj = SE_ProjectLoaderSaver::Load(projFilePath.toStdString());
    if(proj != nullptr)
    {
        CloseProject();
        m_project = proj;
        m_project->SetIsSaved(true);
        return true;
    }
    return false;
}


bool SE_Controller::SaveProject(QString projFilePath)
{
    bool result = SE_ProjectLoaderSaver::Save(m_project , projFilePath.toStdString());

    if(result == true)
    {
        m_project->SetIsSaved(true);
        return true;
    }

    m_project->SetIsSaved(false);
    return false;
}

bool SE_Controller::LoadGif(QString gifPilePath)
{
    SE_Project* proj = SE_GifLoaderSaver::Load(gifPilePath);
    if(proj != nullptr)
    {
        CloseProject();
        m_project = proj;
        m_project->SetIsSaved(true);
        return true;
    }
    return false;
}

bool SE_Controller::SaveGif(QString gitFilePath, int speedAnimation)
{
    if(speedAnimation <= 0)
        speedAnimation = 1;

    bool result = SE_GifLoaderSaver::Save(m_project , gitFilePath.toStdString(), speedAnimation);

    if(result == true)
    {
        return true;
    }
    return false;
}

QString SE_Controller::GetProjectName()
{
    if(m_project != nullptr)
    {
        QString name(m_project->GetName().c_str());
        return name;
    }
    return "";
}

bool SE_Controller::IsProjectSaved()
{
    if(m_project != nullptr)
        return m_project->IsSaved();

    return true;
}

QSize SE_Controller::GetFrameSize()
{
    if(m_project != nullptr)
        return QSize(m_project->GetWidth(), m_project->GetHeight());

    return QSize(0, 0);
}

bool SE_Controller::AddNewFrame()
{
    if(m_project != nullptr)
    {
        bool result = m_project->AddNewFrame();
        if(result)
        {
            m_project->SetIsSaved(false);
            return result;
        }
    }
    return false;
}

bool SE_Controller::AddNewFrameAfter(int idx)
{
    if(m_project != nullptr)
    {
        bool result = m_project->AddNewFrameAfter(idx);
        if(result)
        {
            m_project->SetIsSaved(false);
            return result;
        }
    }
    return false;
}

bool SE_Controller::SetCurrentFrame(int idxFrame)
{
    if(m_project != nullptr)
    {
        return m_project->SetCurrentFrame(idxFrame);
    }

    return false;
}

bool SE_Controller::UpdateImageAtCurrentFrame(QImage newImage)
{
    if(m_project != nullptr)
    {
        bool result = m_project->UpdateImageAtCurrent(newImage);

        if(result)
        {
            m_project->SetIsSaved(false);
            return result;
        }
    }

    return false;
}

int SE_Controller::GetCurrentFrameID()
{
    if(m_project != nullptr)
        return m_project->GetCurrentFrameID();

    return 0;

}

int SE_Controller::GetFrameCount()
{
    if(m_project != nullptr)
        return m_project->GetFrameCount();

    return 0;
}

QImage SE_Controller::GetFrameCopy(int idxFrame)
{
    if(m_project != nullptr)
        return m_project->GetFrameCopy(idxFrame);
}

std::vector<QImage> SE_Controller::GetAllFramesCopy()
{
    if(m_project != nullptr)
        return m_project->GetAllFramesCopy();

    return vector<QImage>();
}

