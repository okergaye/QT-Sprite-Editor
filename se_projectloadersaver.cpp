// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_projectloadersaver.h"
#include "se_project.h"
#include <fstream>
#include <sstream>

using namespace std;

SE_ProjectLoaderSaver::SE_ProjectLoaderSaver(QObject* parent) : QObject(parent)
{

}

SE_Project* SE_ProjectLoaderSaver::Load(const string path)
{
    string name = path.substr(path.rfind('/'));

    int index = 0;
    int counter = 0;

    int height = 0;
    int width = 0;
    int numOfFrames = 0;

    vector<int> dimensions;
    vector<int> numbers;
    vector<QColor> colors;
    vector<QImage> images;

    SE_Project* project = nullptr;

    string read;
    string readSplit;
    ifstream file;

    try {

        file.open(path);

        if(file.is_open())
        {
            getline(file, readSplit);
            stringstream heightWidth(readSplit);
            while(getline(heightWidth, read, ' '))
            {
                dimensions.push_back(stoi(read));
            }

            if(dimensions.size() != 2)
            {
                throw "Corrupt File";
            }

            height = dimensions[0];
            width = dimensions[1];

            getline(file, read);

            for(int i = 0; i < read.size(); i++){
                if(isspace(read[i]))
                {
                    throw "Corrupt File";
                }
            }

            numOfFrames = stoi(read);

            while(getline(file, readSplit) && file.good())
            {
                stringstream nums(readSplit);
                while(getline(nums, read, ' ') && file.good())
                {
                    numbers.push_back(stoi(read));
                }
            }

            if((numbers.size() % 4) != 0)
            {
                throw "Corrupt File";
            }

            while(index < numbers.size())
            {
                colors.push_back(QColor(numbers[index], numbers[index+1], numbers[index+2], numbers[index+3]));
                index = index + 4;
            }

            while(counter < colors.size())
            {
                for(int eoframe = 0; eoframe < numOfFrames; eoframe++)
                {
                    images.push_back(QImage(width, height, QImage::Format_RGBA8888_Premultiplied));
                    for(int y = 0; y < height; y++)
                    {
                        for(int x = 0; x < width; x++)
                        {
                            images[eoframe].setPixelColor(x,y,colors[counter]);
                            counter++;
                        }
                    }
                }
            }

            project = new SE_Project(name, width, height, images);
            file.close();
        }
        else
        {
            return project;
        }
    }
    catch(...)
    {

    }

    return project;
}

bool SE_ProjectLoaderSaver::Save(SE_Project* project, string path)
{
    if(project == nullptr)
        return false;

    ofstream file;
    file.open(path);

    if(file.is_open())
    {
        file << project->GetHeight() << " " << project->GetWidth() << "\n";
        file << project->GetFrameCount() << "\n";

        vector<QImage> frames = project->GetAllFramesCopy();
        vector<QImage>::iterator it = frames.begin();

        while(it != frames.end())
        {
            for(int i = 0; i < project->GetHeight(); i++)
            {
                for(int j = 0; j < project->GetWidth(); j++)
                {
                    QColor color = (*it).pixelColor(j, i);
                    int r = color.red();
                    int g = color.green();
                    int b = color.blue();
                    int a = color.alpha();

                    file << r << " " << g << " " << b << " " << a << " ";
                }
                file << "\n";
            }
            it++;
        }

        file.close();
    }
    else
    {
        return false;
    }
    return true;
}
