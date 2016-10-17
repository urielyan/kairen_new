#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

class AbstractComponentFactory : public QObject
{
    Q_OBJECT
public:
    explicit AbstractComponentFactory(QObject *parent = 0);
    virtual QLabel* getLabel(QString text, QWidget *parent = 0) = 0;
    virtual QPushButton* getButton(QString text, QWidget *parent = 0) = 0;
    virtual QBoxLayout* getBoxLayout(QBoxLayout::Direction direction, QWidget *parent = 0) = 0;

    virtual ~AbstractComponentFactory(){}
signals:

public slots:
};

class MainComponent : public AbstractComponentFactory
{
    Q_OBJECT
public:
    explicit MainComponent(QObject *parent = 0);
    QLabel* getLabel(QString text, QWidget *parent = 0) Q_DECL_OVERRIDE;
    QPushButton* getButton(QString text, QWidget *parent = 0) Q_DECL_OVERRIDE;
    QBoxLayout *getBoxLayout(QBoxLayout::Direction direction, QWidget *parent = 0) Q_DECL_OVERRIDE;

    ~MainComponent();
signals:

public slots:
};

class MeasureFrameComponent : public AbstractComponentFactory
{
    Q_OBJECT
public:
    explicit MeasureFrameComponent(QObject *parent = 0);
    QLabel* getLabel(QString text, QWidget *parent = 0) Q_DECL_OVERRIDE;
    QPushButton* getButton(QString text, QWidget *parent = 0) Q_DECL_OVERRIDE;
    QBoxLayout *getBoxLayout(QBoxLayout::Direction direction, QWidget *parent = 0) Q_DECL_OVERRIDE;

    ~MeasureFrameComponent();
signals:

public slots:
};

#endif // ABSTRACTFACTORY_H
