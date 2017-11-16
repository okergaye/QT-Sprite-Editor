// CS3505 Sprite Editor
// "A Good Time"
// Haoran Chen        u1060286
// Yijie Guo          u1056745
// Osama Kergaye      u0767339
// Stephan Stankovic  u0872863
// Jeremy Wu          u0895969

#include "se_uinewdialog.h"
#include "ui_se_uinewdialog.h"
#include "ui_se_uimainwindow.h"

//Constucts a new dialog window
SE_UINewDialog::SE_UINewDialog(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::SE_UINewDialog)
{
    m_ui->setupUi(this);
    m_ui->fileNameInputBox->setText("A Good Default Name");
    m_ui->spinBoxX->setMinimum(1);
    m_ui->spinBoxY->setMinimum(1);
    m_ui->spinBoxX->setMaximum(768);
    m_ui->spinBoxY->setMaximum(768);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &SE_UINewDialog::SlotButtonAcceptedClicked);
}

SE_UINewDialog::~SE_UINewDialog()
{
    delete m_ui;
}

//Once the user has chosen a file name and pixel size. That info is emited to the UI
void SE_UINewDialog::SlotButtonAcceptedClicked()
{
    QSize pixelSize(m_ui->spinBoxX->value(), m_ui->spinBoxY->value());
    emit SignalNewFile(m_ui->fileNameInputBox->text(), pixelSize);
}


