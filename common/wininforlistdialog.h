#ifndef WININFORLISTDIALOG_H
#define WININFORLISTDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

class QHBoxLayout;
class QButtonGroup;
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

/**
 * @class WinSureOperateDialog
 * @brief 弹出消息对话框，确认是否需要执行操作.
 */
class WinSureOperateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WinSureOperateDialog(QString text, QWidget *parent = 0);

private:
    QLabel *p_labelTitle;
    QLabel *p_labelContent;

    QHBoxLayout *p_VLayoutBottom;

private:
    void init();
};
#endif // WININFORLISTDIALOG_H
