#ifndef WININFORLISTDIALOG_H
#define WININFORLISTDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>

class WinInforListDialog : public QDialog
{
  Q_OBJECT
public:
  static WinInforListDialog *instance();

  void showMsg(QString text);
  void showMsg(qint32 id);

  QString doubleToCompleteDouble(double num);
  QString intToCompleteInt(int num);
signals:

public slots:

private:
  explicit WinInforListDialog(QWidget *parent = 0);
  void setStylesheet();

  QLabel m_label;
};
#endif // WININFORLISTDIALOG_H
