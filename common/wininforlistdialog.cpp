#include <QMessageBox>

#include "wininforlistdialog.h"
#include "global.h"

WinInforListDialog *WinInforListDialog::instance()
{
  static WinInforListDialog Instance;
  return &Instance;
}

void WinInforListDialog::showMsg(QString text)
{
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setText(text);
  msgbox.exec();
}

void WinInforListDialog::showMsg(qint32 id)
{
  switch (id) {
    case 0:
      break;
    default:
      break;
    }

  this->show();
}

QString WinInforListDialog::doubleToCompleteDouble(double num)
{
  return QString::number(num, 'f', 4);
}

QString WinInforListDialog::intToCompleteInt(int num)
{
  QString tmp =  QString::number(num);
  while(num < 10000)
    {
      tmp = "0" + tmp;
    }
  return tmp;
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

