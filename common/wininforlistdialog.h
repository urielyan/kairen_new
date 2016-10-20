#ifndef WININFORLISTDIALOG_H
#define WININFORLISTDIALOG_H

#include <QDialog>
#include <QLabel>

class WinInforListDialog : public QDialog
{
  Q_OBJECT
public:
  static WinInforListDialog *instance();

  void showMsg(QString text, QString informativeText = "");
signals:

public slots:

private:
  explicit WinInforListDialog(QWidget *parent = 0);
  void setStylesheet();

  QLabel m_label;
};
#endif // WININFORLISTDIALOG_H
