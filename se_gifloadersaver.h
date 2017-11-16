// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_GIF_LOADER_SAVER_H__
#define __SE_GIF_LOADER_SAVER_H__

#include <QObject>

class SE_Project;

// Static Function Class for loading and saving the project as gif file
class SE_GifLoaderSaver : public QObject
{
    Q_OBJECT
public:
    // Load gif files and create a new project instance
    static SE_Project* Load(QString filePath);

    // Save the given project to gif files.
    static bool Save(SE_Project* project, std::string path, int delay);

private:
    // Default Constructor. Prohibited.
    explicit SE_GifLoaderSaver(QObject* parent = nullptr);
};

#endif // __SE_GIF_LOADER_SAVER_H__

