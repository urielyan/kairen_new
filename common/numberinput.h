#ifndef INPUT_H
#define INPUT_H

#include <QDialog>

class AbstractComponentFactory;
class QBoxLayout;
class QLineEdit;
class NumberInput : public QDialog
{
  Q_OBJECT
public:
    NumberInput(QWidget *parent = 0);
    QString getString();

private slots:
    void slotOkClicked();
    void slotCancelClicked();
    void slotBackspaceClicked();
    void slotClearClicked();
    void slotNumberClicked(int index);
    void slotPointClicked();

private:
    AbstractComponentFactory *p_componentFactory;
    QBoxLayout *p_mainBoxLayout;
    QLineEdit *p_lineEdit;

    void initKey();
};

#endif // INPUT_H
