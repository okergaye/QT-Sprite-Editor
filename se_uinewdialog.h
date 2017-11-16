// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#ifndef __SE_UI_NEW_DIALOG_H__
#define __SE_UI_NEW_DIALOG_H__

#include <QDialog>
#include <QAbstractButton>
#include <QSize>

namespace Ui
{
    class SE_UINewDialog;
}

// The window for users to create a new project
class SE_UINewDialog : public QDialog
{
    Q_OBJECT

public:
    // Default Constructor for create a empty NewDialog window
    explicit SE_UINewDialog(QWidget* parent = nullptr);

    // Default Deconstructor to release the memory
    virtual ~SE_UINewDialog();

private slots:
    // Gathers file info upon button click
    void SlotButtonAcceptedClicked();

signals:
    // Sends project name and pixel size
    void SignalNewFile(QString name, QSize size);

private:
    Ui::SE_UINewDialog* m_ui;

};

#endif // __SE_UI_NEW_DIALOG_H__

