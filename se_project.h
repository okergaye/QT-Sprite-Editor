// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_PROJECT_H__
#define __SE_PROJECT_H__

#include <vector>
#include <string>
#include <QObject>
#include <QImage>
#include "se_frame.h"

// The class holding basic information and all frames.
class SE_Project : public QObject
{
    Q_OBJECT
public:

    // Constructor for create an empty project
    explicit SE_Project(std::string name, int width, int height, QObject* parent = nullptr);

    // Constructor for create a project with given images. Works for project loader.
    explicit SE_Project(std::string name, int width, int height, std::vector<QImage> images, QObject* parent = nullptr);

    // Return the name of the project.
    std::string GetName();

    // Return the width of the frames
    int GetWidth();

    // Return the height of the frames
    int GetHeight();

    // Add a new frame to the end of the all frames. Return true if success
    bool AddNewFrame();

    // Add a new frame after the given idx. Return true if success , false if the idx is invalid
    bool AddNewFrameAfter(int idx);

    // Change the state of the project is saved or not.
    void SetIsSaved(bool isSaved);

    // Return the result if the project is saved or not
    bool IsSaved();

    // Change the current frame users are working on. Return false if the index is invalid
    bool SetCurrentFrame(int idx);

    // Return the id of the current frame
    int GetIndexOfCurrentFrame();

    // Update the current frame users are working on. Return false if the index is invalid
    bool UpdateImageAtCurrent(QImage newImage);

    // Retreive the current frame's id users are working on.
    int GetCurrentFrameID();

    // Return the total count of the frames in the project
    int GetFrameCount();

    // Retreive the copy of one frame by given id. Return 0 if the index is invalid
    QImage GetFrameCopy(int idxFrame);

    // Retreive all copies of the frames.
    std::vector<QImage> GetAllFramesCopy();

private:
    // Check the given index is valid or not.
    bool IsIndexValid(int index);

private:
    std::string m_name;
    int m_width;
    int m_height;
    int m_idxCurrentFrame;
    int m_countFrames;
    bool m_isSaved;
    std::vector<SE_Frame> m_frames;

};

#endif // __SE_PROJECT_H__
