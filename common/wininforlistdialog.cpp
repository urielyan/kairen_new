#include "global.h"

#include "wininforlistdialog.h"

#include <QMessageBox>

WinInforListDialog *WinInforListDialog::instance()
{
  static WinInforListDialog Instance;
  return &Instance;
}

void WinInforListDialog::showMsg(QString text, QString informativeText)
{
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setText(text);
  msgbox.setInformativeText(informativeText);
  msgbox.exec();
}

WinInforListDialog::WinInforListDialog(QWidget *parent) : QDialog(parent)
{
  this->setFixedHeight(DESKTOP_HEIGHT - 30);
  this->setFixedWidth(DESKTOP_WIDTH- 30);
  setStylesheet();
}

void WinInforListDialog::setStylesheet()
{
  //this->setStyleSheet("QMessageBox{min-height: 50}");
}

