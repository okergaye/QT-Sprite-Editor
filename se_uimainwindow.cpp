// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_uimainwindow.h"
#include "ui_se_uimainwindow.h"
#include "se_controller.h"
#include <QtWidgets>

#define CONTROLLER SE_Controller::GetInstance()

SE_UIMainWindow::SE_UIMainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::SE_UIMainWindow),
    m_newDialog(),
    m_animePreview(256,256)
{
    m_ui->setupUi(this);

    m_size = QSize(128, 128);

    connect(&m_newDialog, &SE_UINewDialog::SignalNewFile, this,  &SE_UIMainWindow::SlotSetNewProject);
    m_projectName = "Untitled Project";

    CONTROLLER->NewProject(m_projectName, m_size.width(), m_size.height());
    m_currentImage = CONTROLLER->GetFrameCopy(0);
    m_ui->inputBox_frame->setValue(1);
    m_ui->inputBox_frame->setMinimum(1);
    m_ui->inputBox_frame->setMaximum(CONTROLLER->GetFrameCount());
    m_defaultImage = QImage(m_size.width(), m_size.height(), QImage::Format_RGBA8888_Premultiplied);
    m_defaultImage.fill(QColor(0, 0, 0, 0));

    QActionGroup* group = new QActionGroup(m_ui->mainToolBar);

    QAction* undoAction = new QAction("U", m_ui->mainToolBar);
    undoAction->setToolTip(tr("Undo last drawing."));
    group->addAction(undoAction);
    QIcon undoIcon(":/Icons/undoIcon.ico");
    undoAction->setIcon(undoIcon);
    m_ui->mainToolBar->addAction(undoAction);

    QAction* drawPointAction = new QAction("P", m_ui->mainToolBar);
    drawPointAction->setToolTip(tr("The default pen."));
    group->addAction(drawPointAction);
    QIcon penIcon(":/Icons/pen.ico");
    drawPointAction->setIcon(penIcon);
    drawPointAction->setCheckable(true);
    drawPointAction->setChecked(true);
    m_ui->mainToolBar->addAction(drawPointAction);

    QAction* drawLineAction = new QAction("L", m_ui->mainToolBar);
    drawLineAction->setToolTip(tr("Draw a line."));
    group->addAction(drawLineAction);
    QIcon lineIcon(":/Icons/lineIcon.png");
    drawLineAction->setIcon(lineIcon);
    drawLineAction->setCheckable(true);
    m_ui->mainToolBar->addAction(drawLineAction);

    QAction* drawRectangleAction = new QAction("R", m_ui->mainToolBar);
    drawRectangleAction->setToolTip(tr("Draw a rectangle."));
    group->addAction(drawRectangleAction);
    QIcon recIcon(":/Icons/rectangleIcon.ico");
    drawRectangleAction->setIcon(recIcon);
    drawRectangleAction->setCheckable(true);
    m_ui->mainToolBar->addAction(drawRectangleAction);

    QAction* drawCircleAction = new QAction("C", m_ui->mainToolBar);
    drawCircleAction->setToolTip(tr("Draw a circle."));
    group->addAction(drawCircleAction);
    QIcon emptyCircleIcon(":/Icons/emptyCircleIcon.png");
    drawCircleAction->setIcon(emptyCircleIcon);
    drawCircleAction->setCheckable(true);
    m_ui->mainToolBar->addAction(drawCircleAction);

    QAction* fillRectangleAction = new QAction("F", m_ui->mainToolBar);
    fillRectangleAction->setToolTip(tr("Fill a rectangle."));
    group->addAction(fillRectangleAction);
    QIcon fillRecIcon(":/Icons/fillRecIcon.png");
    fillRectangleAction->setIcon(fillRecIcon);
    fillRectangleAction->setCheckable(true);
    m_ui->mainToolBar->addAction(fillRectangleAction);

    QAction* fillCircleAction = new QAction("FC", m_ui->mainToolBar);
    fillCircleAction->setToolTip(tr("Fill a circle."));
    group->addAction(fillCircleAction);
    QIcon fillCircleIcon(":/Icons/filledCircleIcon.png");
    fillCircleAction->setIcon(fillCircleIcon);
    fillCircleAction->setCheckable(true);
    m_ui->mainToolBar->addAction(fillCircleAction);

    QAction* drawEraserAction = new QAction("E", m_ui->mainToolBar);
    drawEraserAction->setToolTip(tr("Use the eraser."));
    group->addAction(drawEraserAction);
    QIcon eraserIcon(":/Icons/eraserIcon.png");
    drawEraserAction->setIcon(eraserIcon);
    drawEraserAction->setCheckable(true);
    m_ui->mainToolBar->addAction(drawEraserAction);

    QAction* colorAction = new QAction("C", m_ui->mainToolBar);
    colorAction->setToolTip(tr("Use the color pannel."));
    group->addAction(colorAction);
    QIcon paletteIcon(":/Icons/paletteIcon.png");
    colorAction->setIcon(paletteIcon);
    m_ui->mainToolBar->addAction(colorAction);

    QAction* actionHelp = new QAction("Help");
    m_ui->menuBar->addAction(actionHelp);

    QLabel* statusMsg = new QLabel;
    statusBar()->addWidget(statusMsg);

    m_ui->inputBox_fps->setMaximum(144);
    m_ui->inputBox_fps->setMinimum(1);

    m_paintWidget = new SE_UIPaintWidget(m_currentImage, this);
    m_paintWidget->SetOriginalSize(m_size);

    m_ui->widget = m_paintWidget;
    m_ui->widget->move(35, 25);

    m_previousWidget = new SE_UIFrameShowWidget(m_defaultImage, this);
    m_previousWidget->SetOriginalSize(m_size);

    m_ui->lastWidget = m_previousWidget;
    m_ui->lastWidget->move(825, 70);

    m_nextWidget = new SE_UIFrameShowWidget(m_defaultImage, this);
    m_nextWidget->SetOriginalSize(m_size);

    m_ui->nextWidget = m_nextWidget;
    m_ui->nextWidget->move(825, 350);

    m_colorPicker = new QColorDialog;
    m_colorPicker->setOption(QColorDialog::ShowAlphaChannel);

    m_paintWidget->SetSelectedColor(QColor(0, 0, 0, 255));

    connect(drawPointAction, SIGNAL(triggered()),
                            this, SLOT(SlotDrawPointChosen()));
    connect(drawLineAction, SIGNAL(triggered()),
                            this, SLOT(SlotDrawLineChosen()));
    connect(drawRectangleAction, SIGNAL(triggered()),
                            this, SLOT(SlotDrawRectangleChosen()));
    connect(fillRectangleAction, SIGNAL(triggered()),
                            this, SLOT(SlotFillRectangleChosen()));
    connect(drawCircleAction, SIGNAL(triggered()),
                            this, SLOT(SlotDrawCircleChosen()));
    connect(fillCircleAction, SIGNAL(triggered()),
                            this, SLOT(SlotFillCircleChosen()));
    connect(drawEraserAction, SIGNAL(triggered()),
                            this, SLOT(SlotDrawEraserChosen()));
    connect(colorAction, SIGNAL(triggered()),
                            this, SLOT(SlotColorChosen()));
    connect(undoAction, SIGNAL(triggered()),
                            this, SLOT(SlotUndoChosen()));

    connect(m_ui->actionLoad, SIGNAL(triggered()),
                            this, SLOT(SlotLoadFileMenu()));
    connect(m_ui->actionSave_As, SIGNAL(triggered()),
                            this, SLOT(SlotSaveToFileMenu()));
    connect(m_ui->actionNew, SIGNAL(triggered()),
                            this, SLOT(SlotNewFileMenu()));
    connect(actionHelp, SIGNAL(triggered()),
                            this, SLOT(SlotHelpFileMenu()));
    connect(m_ui->actionImport, SIGNAL(triggered()),
                            this, SLOT(SlotLoadGifMenu()));
    connect(m_ui->actionExport, SIGNAL(triggered()),
                            this, SLOT(SlotSaveGifMenu()));

    connect(m_previousWidget, &SE_UIFrameShowWidget::SignalClicked, this, &SE_UIMainWindow::SlotGetPreviousFrame);
    connect(m_nextWidget, &SE_UIFrameShowWidget::SignalClicked, this, &SE_UIMainWindow::SlotGetNextFrame);
    connect(m_ui->button_addFrame, &QPushButton::clicked, this, &SE_UIMainWindow::SlotAddNewFrame);
    connect(m_ui->inputBox_frame, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SE_UIMainWindow::SlotSetFrame);

    connect(m_ui->button_actual, &QPushButton::clicked, &m_animePreview, &SE_UIAnimeWidget::SlotSetToActualSize);
    connect(m_ui->button_anime, &QPushButton::clicked, &m_animePreview, &SE_UIAnimeWidget::SlotShowUp);
    connect(m_ui->inputBox_fps, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &m_animePreview, &SE_UIAnimeWidget::SlotSetSpeed);
    connect(m_paintWidget, &SE_UIPaintWidget::SignalUpdatedPainting, this, &SE_UIMainWindow::SlotRefreshWidgetDisplay);

    m_animePreview.ReloadAllFrames();
}

SE_UIMainWindow::~SE_UIMainWindow()
{
    delete m_ui;
}

void SE_UIMainWindow::SlotDrawPointChosen()
{
    m_paintWidget->SetCurrentShape(0);
}

void SE_UIMainWindow::SlotDrawLineChosen()
{
    m_paintWidget->SetCurrentShape(1);
}

void SE_UIMainWindow::SlotDrawRectangleChosen()
{
    m_paintWidget->SetCurrentShape(2);
}

void SE_UIMainWindow::SlotFillRectangleChosen()
{
    m_paintWidget->SetCurrentShape(4);
}

void SE_UIMainWindow::SlotDrawEraserChosen()
{
    m_paintWidget->SetCurrentShape(3);
}

void SE_UIMainWindow::SlotDrawCircleChosen()
{
    m_paintWidget->SetCurrentShape(5);
}

void SE_UIMainWindow::SlotFillCircleChosen()
{
    m_paintWidget->SetCurrentShape(6);
}

void SE_UIMainWindow::SlotColorChosen()
{

    QColor myColor = QColorDialog::getColor(m_paintWidget->GetCurrentColor(), this, "Choose a color", QColorDialog::ShowAlphaChannel);
    if (myColor.isValid())
    {
        m_paintWidget->SetSelectedColor(myColor);
    }
}

void SE_UIMainWindow::SlotUndoChosen()
{
    m_paintWidget->UndoPainting();
    m_animePreview.UpdateCurrentFrame();
}

void SE_UIMainWindow::SlotRefreshWidgetDisplay()
{
    m_animePreview.UpdateCurrentFrame();
}

void SE_UIMainWindow::SlotSetNewProject(QString name, QSize size)
{
    m_projectName = name;

    CONTROLLER->NewProject(m_projectName, size.width(), size.height());
    m_currentImage = CONTROLLER->GetFrameCopy(0);

    m_size = CONTROLLER->GetFrameSize();
    m_paintWidget->SetOriginalSize(m_size);
    m_paintWidget->SetCurrentImage(m_currentImage);

    m_animePreview.ReloadAllFrames();

    m_defaultImage = QImage(m_size.width(), m_size.height(), QImage::Format_RGBA8888_Premultiplied);
    m_defaultImage.fill(QColor(0, 0, 0, 0));
    m_previousWidget->SetDefault(true);
    m_previousWidget->SetOriginalSize(m_size);
    m_previousWidget->SetCurrentImage(m_defaultImage);
    m_nextWidget->SetDefault(true);
    m_nextWidget->SetOriginalSize(m_size);
    m_nextWidget->SetCurrentImage(m_defaultImage);

    m_ui->inputBox_frame->setValue(1);
    m_ui->inputBox_frame->setMaximum(CONTROLLER->GetFrameCount());
}

void SE_UIMainWindow::SlotAddNewFrame()
{
    if(CONTROLLER->AddNewFrame())
    {
        int currentID = CONTROLLER->GetCurrentFrameID();
        m_nextWidget->SetDefault(false);
        m_nextWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(currentID + 1));
        m_ui->inputBox_frame->setMaximum(CONTROLLER->GetFrameCount());

        if(m_animePreview.isVisible())
            m_animePreview.ShowUp();
        else
            m_animePreview.ReloadAllFrames();
    }
}

void SE_UIMainWindow::SlotGetPreviousFrame()
{
    int currentID = CONTROLLER->GetCurrentFrameID();
    CONTROLLER->SetCurrentFrame(currentID - 1);
    QImage previousImage = CONTROLLER->GetFrameCopy(currentID - 1);
    m_paintWidget->SetCurrentImage(previousImage);

    m_defaultImage = QImage(m_size.width(), m_size.height(), QImage::Format_RGBA8888_Premultiplied);
    m_defaultImage.fill(QColor(0, 0, 0, 0));
    if (currentID >= 2)
    {
        m_previousWidget->SetDefault(false);
        m_previousWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(currentID - 2));
    }
    else
    {
        m_previousWidget->SetDefault(true);
        m_previousWidget->SetCurrentImage(m_defaultImage);
    }
    if (currentID >= 1)
    {
        m_nextWidget->SetDefault(false);
        m_nextWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(currentID));
        m_ui->inputBox_frame->setValue(currentID);
    }

}

void SE_UIMainWindow::SlotGetNextFrame()
{
    int currentID = CONTROLLER->GetCurrentFrameID();
    CONTROLLER->SetCurrentFrame(currentID + 1);
    QImage nextImage = CONTROLLER->GetFrameCopy(currentID + 1);
    m_paintWidget->SetCurrentImage(nextImage);

    m_defaultImage = QImage(m_size.width(), m_size.height(), QImage::Format_RGBA8888_Premultiplied);
    m_defaultImage.fill(QColor(0, 0, 0, 0));
    if (currentID + 1< CONTROLLER->GetFrameCount())
    {
        m_previousWidget->SetDefault(false);
        m_previousWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(currentID));
        m_previousWidget->SetOriginalSize(m_size);
        m_ui->inputBox_frame->setValue(currentID + 2);
    }

    if (currentID + 2 < CONTROLLER->GetFrameCount())
    {
        m_nextWidget->SetDefault(false);
        m_nextWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(currentID + 2));

    }
    else
    {
        m_nextWidget->SetDefault(true);
        m_nextWidget->SetCurrentImage(m_defaultImage);
    }
    m_nextWidget->SetOriginalSize(m_size);
}

void SE_UIMainWindow::SlotSetFrame(int frameID)
{
    frameID--;
    CONTROLLER->SetCurrentFrame(frameID);
    QImage chosenImage = CONTROLLER->GetFrameCopy(frameID);
    m_paintWidget->SetCurrentImage(chosenImage);

    m_defaultImage = QImage(m_size.width(), m_size.height(), QImage::Format_RGBA8888_Premultiplied);
    m_defaultImage.fill(QColor(0, 0, 0, 0));
    if (frameID >= 1)
    {
        QImage lastImage = CONTROLLER->GetFrameCopy(frameID - 1);
        m_previousWidget->SetDefault(false);
        m_previousWidget->SetCurrentImage(lastImage);
    }
    else
    {
        m_previousWidget->SetDefault(true);
        m_previousWidget->SetCurrentImage(m_defaultImage);
    }
    if (frameID + 1 < CONTROLLER->GetFrameCount())
    {
        m_nextWidget->SetDefault(false);
        m_nextWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(frameID + 1));

    }
    else
    {
        m_nextWidget->SetDefault(true);
        m_nextWidget->SetCurrentImage(m_defaultImage);
    }
    m_nextWidget->SetOriginalSize(m_size);
}

void SE_UIMainWindow::SlotLoadFileMenu()
{
    bool canContinue = false;
    if(!CONTROLLER->IsProjectSaved())
    {
        QMessageBox msgBox;
        msgBox.setText("The file has not been saved yet.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
            {
                QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), "", tr("Spirte Sheet Project (*.ssp);;All Files (*)"));
                canContinue = CONTROLLER->SaveProject(fileName);
                break;
            }
            case QMessageBox::Discard:
            {
                canContinue = true;
                break;
            }
            case QMessageBox::Cancel:
              break;
          default:
              break;
        }
    }
    else
    {
        canContinue = true;
    }

    if(canContinue)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Project File"), "",
            tr("Spirte Sheet Project (*.ssp)"));

        if(fileName.length()==0)
            return;

        bool loadSuccess = CONTROLLER->LoadProject(fileName);
        if (!loadSuccess)
        {
            QMessageBox msgBox;
            msgBox.setText("Failed loading project.");
            msgBox.setInformativeText("Please double check the project file you want to load.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        else
        {
            m_currentImage = CONTROLLER->GetFrameCopy(0);
            m_projectName = CONTROLLER->GetProjectName();
            m_size = CONTROLLER->GetFrameSize();
            m_paintWidget->SetOriginalSize(m_size);
            m_paintWidget->SetCurrentImage(m_currentImage);

            m_defaultImage = QImage(m_size.width(), m_size.height(), QImage::Format_RGBA8888_Premultiplied);
            m_defaultImage.fill(QColor(0, 0, 0, 0));
            m_previousWidget->SetDefault(true);
            m_previousWidget->SetCurrentImage(m_defaultImage);
            m_previousWidget->SetOriginalSize(m_size);
            if (CONTROLLER->GetFrameCount() > 1)
            {
                m_nextWidget->SetDefault(false);
                m_nextWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(1));

            }
            else
            {
                m_nextWidget->SetDefault(true);
                m_nextWidget->SetCurrentImage(m_defaultImage);
            }
            m_nextWidget->SetOriginalSize(m_size);

            if(m_animePreview.isVisible())
            {
                m_animePreview.ShowUp();
            }
            else
            {
                m_animePreview.ReloadAllFrames();
            }
            m_ui->inputBox_frame->setValue(1);
            m_ui->inputBox_frame->setMaximum(CONTROLLER->GetFrameCount());
        }
    }
}

void SE_UIMainWindow::SlotLoadGifMenu()
{
    bool canContinue = false;
    if(!CONTROLLER->IsProjectSaved())
    {
        QMessageBox msgBox;
        msgBox.setText("The file has not been saved yet.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
            {
                QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), "", tr("Spirte Sheet Project (*.ssp);;All Files (*)"));
                canContinue = CONTROLLER->SaveProject(fileName);
                break;
            }
            case QMessageBox::Discard:
            {
                canContinue = true;
                break;
            }
            case QMessageBox::Cancel:
              break;
          default:
              break;
        }
    }
    else
    {
        canContinue = true;
    }

    if(canContinue)
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project File"), "", tr("Graphics Interchange Format (*.gif)"));
        if(fileName.length()==0)
            return;

        bool loadSuccess = CONTROLLER->LoadGif(fileName);
        if (!loadSuccess)
        {
            QMessageBox msgBox;
            msgBox.setText("Failed loading a gif file.");
            msgBox.setInformativeText("Please double check your gif to be imported.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        else
        {
            m_currentImage = CONTROLLER->GetFrameCopy(0);
            m_projectName = CONTROLLER->GetProjectName();
            m_size = CONTROLLER->GetFrameSize();
            m_paintWidget->SetOriginalSize(m_size);
            m_paintWidget->SetCurrentImage(m_currentImage);

            m_defaultImage = QImage(m_size.width(), m_size.height(), QImage::Format_RGBA8888_Premultiplied);
            m_defaultImage.fill(QColor(0, 0, 0, 0));
            m_previousWidget->SetDefault(true);
            m_previousWidget->SetCurrentImage(m_defaultImage);
            m_previousWidget->SetOriginalSize(m_size);
            if (CONTROLLER->GetFrameCount() > 1)
            {
                m_nextWidget->SetDefault(false);
                m_nextWidget->SetCurrentImage(CONTROLLER->GetFrameCopy(1));
            }
            else
            {
                m_nextWidget->SetDefault(true);
                m_nextWidget->SetCurrentImage(m_defaultImage);
            }
            m_nextWidget->SetOriginalSize(m_size);

            if(m_animePreview.isVisible())
            {
                m_animePreview.ShowUp();
            }
            else
            {
                m_animePreview.ReloadAllFrames();
            }

            m_ui->inputBox_frame->setValue(1);
            m_ui->inputBox_frame->setMaximum(CONTROLLER->GetFrameCount());
        }
    }
}

void SE_UIMainWindow::SlotSaveToFileMenu()
{
    QString projectName = CONTROLLER->GetProjectName();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), projectName, tr("Spirte Sheet Project (*.ssp);;All Files (*)"));
    CONTROLLER->SaveProject(fileName);

}

void SE_UIMainWindow::SlotSaveGifMenu()
{
    QString gifProjectName = CONTROLLER->GetProjectName();
    QString gifName = QFileDialog::getSaveFileName(this, tr("Open Project File"), gifProjectName, tr("Graphics Interchange Format (*.gif)"));
    CONTROLLER->SaveGif(gifName, m_ui->inputBox_fps->value());
}


void SE_UIMainWindow::SlotNewFileMenu()
{
    if(!CONTROLLER->IsProjectSaved())
    {
        QMessageBox msgBox;
        msgBox.setText("The file has not been saved yet.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
            {
                QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), "", tr("Spirte Sheet Project (*.ssp);;All Files (*)"));
                if(CONTROLLER->SaveProject(fileName))
                    m_newDialog.show();

                break;
            }
            case QMessageBox::Discard:
            {
                m_newDialog.show();
                break;
            }
            case QMessageBox::Cancel:
              break;
          default:
              break;
        }
    }
    else
    {
        m_newDialog.show();
    }
}

void SE_UIMainWindow::SlotHelpFileMenu()
{


    QTextEdit* help=new QTextEdit(this);
       help->setWindowFlag(Qt::Window); //or Qt::Tool, Qt::Dialog if you like
       help->setReadOnly(true);
       QSize m(400,600);
       help->setFixedSize(m);
       help->setFontPointSize(10);
       help->append("Click the file menu and select new to start a new project. "
                    "You will then be welcomed by a new file menu, where you may input a file name and sprite size. "
                    "after you have done this, you are ready to start your art! \n \n"
                    "There are a set of tools to your left to choose from, they are; undo, pen, line draw, rectangle draw, "
                    "filled rectangle draw, circle draw, filled circle draw, eraser, and color picker \n \n"
                    "On your right, you may traverse pixel frames and see a preview of your animation! "
                    "To traverse simply click on the frame after you have added a new one. \n \n"
                    "Once you are satisfied with your sprite, you can select export as gif from the file menu. "
                    "And that's it, you are done! \n \n"
                    "Bonus: You can also import an existing gif using the file menu!");
       help->show();


}

void SE_UIMainWindow::closeEvent(QCloseEvent* event)
{
    if(!CONTROLLER->IsProjectSaved())
    {
        QMessageBox msgBox;
        msgBox.setText("The file has not been saved yet.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
            {
                QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), "", tr("Spirte Sheet Project (*.ssp);;All Files (*)"));
                if(CONTROLLER->SaveProject(fileName))
                {
                }
                break;
            }
            case QMessageBox::Discard:
            {
                break;
            }
          default:
              break;
        }
    }
    else
    {

    }
    m_animePreview.close();
}

