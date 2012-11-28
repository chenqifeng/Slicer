#include "qMRMLCustomWidgetPlugin.h"
#include "qMRMLCustomWidget.h"

//------------------------------------------------------------------------------
qMRMLCustomWidgetPlugin::qMRMLCustomWidgetPlugin(QObject *_parent)
        : QObject(_parent)
{

}

//------------------------------------------------------------------------------
QWidget *qMRMLCustomWidgetPlugin::createWidget(QWidget *_parent)
{
  qMRMLCustomWidget* _widget = new qMRMLCustomWidget(_parent);
  return _widget;
}

//------------------------------------------------------------------------------
QString qMRMLCustomWidgetPlugin::domXml() const
{
  return "<widget class=\"qMRMLCustomWidget\" \
          name=\"MRMLCustomWidget\">\n"
          "</widget>\n";
}

//------------------------------------------------------------------------------
QString qMRMLCustomWidgetPlugin::includeFile() const
{
  return "qMRMLCustomWidget.h";
}

//------------------------------------------------------------------------------
bool qMRMLCustomWidgetPlugin::isContainer() const
{
  return false;
}

//------------------------------------------------------------------------------
QString qMRMLCustomWidgetPlugin::name() const
{
  return "qMRMLCustomWidget";
}