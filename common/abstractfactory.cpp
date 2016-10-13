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
