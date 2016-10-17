#include "abstractfactory.h"

AbstractComponentFactory::AbstractComponentFactory(QObject *parent) : QObject(parent)
{

}


MainComponent::MainComponent(QObject *parent)
    : AbstractComponentFactory(parent)
{
}

QLabel *MainComponent::getLabel(QString text, QWidget *parent)
{
    QLabel *label = new QLabel(text, parent);
    return label;
}

QPushButton *MainComponent::getButton(QString text, QWidget *parent)
{
    QPushButton *button = new QPushButton(text, parent);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    return button;
}

QBoxLayout *MainComponent::getBoxLayout(QBoxLayout::Direction directio, QWidget *parent)
{
    QBoxLayout *layout = new QBoxLayout(directio, parent);
    layout->setMargin(2);
    layout->setContentsMargins(5, 5, 5, 5);
    return layout;
}

MainComponent::~MainComponent()
{
}

MeasureFrameComponent::MeasureFrameComponent(QObject *parent)
    : AbstractComponentFactory(parent)
{
}

QLabel *MeasureFrameComponent::getLabel(QString text, QWidget *parent)
{
    QLabel *label = new QLabel(text, parent);
    return label;
}

QPushButton *MeasureFrameComponent::getButton(QString text, QWidget *parent)
{
    QPushButton *button = new QPushButton(text, parent);
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    return button;
}

QBoxLayout *MeasureFrameComponent::getBoxLayout(QBoxLayout::Direction direction, QWidget *parent)
{
    QBoxLayout *layout = new QBoxLayout(direction, parent);
    layout->setMargin(2);
    layout->setContentsMargins(5, 5, 5, 5);
    return layout;
}

MeasureFrameComponent::~MeasureFrameComponent()
{

}
