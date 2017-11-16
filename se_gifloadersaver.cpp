// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_gifloadersaver.h"
#include <vector>
#include <string>
#include <QImage>
#include <QMovie>
#include "se_project.h"
#include "gif.h"

using namespace std;

SE_GifLoaderSaver::SE_GifLoaderSaver(QObject* parent) : QObject(parent)
{

}

SE_Project* SE_GifLoaderSaver::Load(QString filePath)
{
    string path = filePath.toStdString();
    SE_Project* project = nullptr;
    QSize size(0,0);

    try
    {
        QMovie movie(filePath);

        if(!movie.isValid())
            throw "Error in loading Gif file";

        movie.jumpToNextFrame();

        vector<QImage> frames;

        for(int i = 0; i < movie.frameCount(); ++i)
        {
            QImage image = movie.currentImage();
            frames.push_back(image);
            movie.jumpToNextFrame();
        }

        if(frames.size()>0)
        {
            string name = path.substr(path.rfind('/'));
            size = frames[0].size();
            project = new SE_Project(name, size.width(), size.height(), frames);
        }
    }
    catch(const exception& ex)
    {

    }

    return project;
}

bool SE_GifLoaderSaver::Save(SE_Project* project, std::string path, int delay)
{
    if(project == nullptr)
        return false;

    vector<QImage> frames = project->GetAllFramesCopy();
    vector<QImage>::iterator it = frames.begin();

    GifWriter gif;

    GifBegin(&gif, path.c_str(), project->GetWidth(), project->GetHeight(), 60, 8, true);

    while(it != frames.end())
    {
        GifWriteFrame(&gif, (*it).bits(), project->GetWidth(), project->GetHeight(), 60, 8, true);
        it++;
    }

    return GifEnd(&gif);
}


