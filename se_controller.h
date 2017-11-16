// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_CONTROLLER_H__
#define __SE_CONTROLLER_H__

#include <string>
#include <QObject>
#include <QImage>

class SE_Project;

// The singleton controller for controlling the projects
class SE_Controller : public QObject
{
    Q_OBJECT
public:

    // Static Function for getting the instance of this singleton class
    static SE_Controller* GetInstance();

    // Delete the project inside
    virtual ~SE_Controller();

    // Create a new project and close the current one.
    bool NewProject(QString name, int width, int height);

    // Close the current project.
    bool CloseProject();

    // Try to load the project by given ssp file, if failed, return false and the current project will be kept
    // If success, the current project will be closed and be replaced by the new project loaded
    bool LoadProject(QString projFilePath);

    // Try to save the project as ssp file to the given path
    bool SaveProject(QString projFilePath);

    // Try to load the project by given gif file, if failed, return false and the current project will be kept
    // If success, the current project will be closed and be replaced by the new project loaded
    bool LoadGif(QString gifPilePath);

    // Try to save the project as ssp file to the given path
    bool SaveGif(QString gitFilePath, int speedAnimation);

    // Return the current project name
    QString GetProjectName();

    // Return true if the current project has been saved recently
    bool IsProjectSaved();

    // Return the size of current project
    QSize GetFrameSize();

    // Add a new frame to the end of the all frames. Return true if success
    bool AddNewFrame();

    // Add a new frame after the given idx. Return true if success , false if the index is invalid
    bool AddNewFrameAfter(int idx);

    // Change the current frame users are working on. Return false if the index is invalid
    bool SetCurrentFrame(int idxFrame);

    // Update the current frame users are working on. Return false if the index is invalid
    bool UpdateImageAtCurrentFrame(QImage newImage);

    // Retreive the current frame's id users are working on.
    int GetCurrentFrameID();

    // Return the total count of the frames in the project
    int GetFrameCount();

    // Retreive the copy of one frame by given id. Return 0 if the index is invalid
    QImage GetFrameCopy(int idxFrame);

    // Retreive all copies of the frames.
    std::vector<QImage> GetAllFramesCopy();

private:
    // Private constructor for the singleton class
    explicit SE_Controller(QObject* parent = nullptr);

private:
    static SE_Controller*  m_instance;
    SE_Project* m_project;

};

#endif // __SE_CONTROLLER_H__
