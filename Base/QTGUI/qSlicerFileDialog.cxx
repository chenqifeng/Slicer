/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>

// CTK includes
#include <ctkFileDialog.h>
#include <ctkUtils.h>

// SlicerQt includes
#include "qSlicerApplication.h"
#include "qSlicerFileReader.h"
#include "qSlicerIOManager.h"
#include "qSlicerIOOptionsWidget.h"

/*
//-----------------------------------------------------------------------------
class qSlicerFileDialogPrivate
{
public:
};
*/

//-----------------------------------------------------------------------------
qSlicerFileDialog::qSlicerFileDialog(QObject* _parent)
  :QObject(_parent)
{
}

//-----------------------------------------------------------------------------
qSlicerFileDialog::~qSlicerFileDialog()
{
}

//-----------------------------------------------------------------------------
QStringList qSlicerFileDialog::nameFilters(qSlicerIO::IOFileType fileType)
{
  QStringList filters;
  QStringList extensions;
  QList<qSlicerFileReader*> readers = 
    qSlicerApplication::application()->ioManager()->readers(fileType);
  foreach(const qSlicerFileReader* reader, readers)
    {
    foreach(const QString& filter, reader->extensions())
      {
      QString nameFilter = filter.contains('(') ? filter :
        reader->description() + " (" + filter + ")";
      filters << nameFilter;
      extensions << ctk::nameFilterToExtensions(nameFilter);
      }
    }
  filters.insert(0, QString("All (") + extensions.join(" ") + QString(")"));
  return filters;
}

//-----------------------------------------------------------------------------
class qSlicerStandardFileDialogPrivate
{
public:
  qSlicerStandardFileDialogPrivate();
  qSlicerIO::IOFileType       FileType;
  qSlicerFileDialog::IOAction Action;
};

//-----------------------------------------------------------------------------
qSlicerStandardFileDialogPrivate::qSlicerStandardFileDialogPrivate()
{
  this->FileType = QString("NoFile");
  this->Action = qSlicerFileDialog::Read;
}

//-----------------------------------------------------------------------------
qSlicerStandardFileDialog::qSlicerStandardFileDialog(QObject* _parent)
  :qSlicerFileDialog(_parent)
  , d_ptr(new qSlicerStandardFileDialogPrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerStandardFileDialog::~qSlicerStandardFileDialog()
{
}

//-----------------------------------------------------------------------------
void qSlicerStandardFileDialog::setFileType(qSlicerIO::IOFileType _fileType)
{
  Q_D(qSlicerStandardFileDialog);
  d->FileType = _fileType;
}

//-----------------------------------------------------------------------------
qSlicerIO::IOFileType qSlicerStandardFileDialog::fileType()const
{
  Q_D(const qSlicerStandardFileDialog);
  return d->FileType;
}


//-----------------------------------------------------------------------------
void qSlicerStandardFileDialog::setAction(qSlicerFileDialog::IOAction dialogAction)
{
  Q_D(qSlicerStandardFileDialog);
  d->Action = dialogAction;
}

//-----------------------------------------------------------------------------
qSlicerFileDialog::IOAction qSlicerStandardFileDialog::action()const
{
  Q_D(const qSlicerStandardFileDialog);
  return d->Action;
}

//-----------------------------------------------------------------------------
ctkFileDialog* qSlicerStandardFileDialog::createFileDialog(
    const qSlicerIO::IOProperties& ioProperties)
{
  if(ioProperties["objectName"].toString().isEmpty())
    {
    qWarning() << "Impossible to create the ctkFileDialog - No object name has been set";
    return 0;
    }

  qSlicerIOManager* ioManager = qSlicerApplication::application()->ioManager();
  ctkFileDialog* fileDialog = new ctkFileDialog();

  // We don't want the odd dialog in Mac: it shows in gray filenames that
  // don't match the filter -> we don't want to show files that don't match
  // the filter.
  fileDialog->setOption(QFileDialog::DontUseNativeDialog);
  if(ioProperties["fileType"].toBool())
    {
    fileDialog->setNameFilters(
      qSlicerFileDialog::nameFilters(
        (qSlicerIO::IOFileType)ioProperties["fileType"].toString()));
    }
  fileDialog->setHistory(ioManager->history());
  if (ioManager->favorites().count())
    {
    fileDialog->setSidebarUrls(ioManager->favorites());
    }
  if (ioProperties["multipleFiles"].toBool())
    {
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    }
  if (ioProperties["fileMode"].toBool())
    {
    fileDialog->setOption(QFileDialog::ShowDirsOnly);
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    }

  fileDialog->setObjectName(ioProperties["objectName"].toString());

  return fileDialog;
}

//-----------------------------------------------------------------------------
bool qSlicerStandardFileDialog::exec(const qSlicerIO::IOProperties& ioProperties)
{
  Q_D(qSlicerStandardFileDialog);
  Q_ASSERT(!ioProperties.contains("fileName"));

  qSlicerIO::IOProperties properties = ioProperties;
  properties["fileType"] = d->FileType;
  ctkFileDialog* fileDialog = qSlicerStandardFileDialog::createFileDialog(
                                properties);

  qSlicerIOManager* ioManager = qSlicerApplication::application()->ioManager();

  // warning: we are responsible for the memory of options
  QStringList fileDescriptions =
    ioManager->fileDescriptionsByType(this->fileType());
  qSlicerIOOptions* options = fileDescriptions.count() ?
    ioManager->fileOptions(fileDescriptions[0]) : 0;
  qSlicerIOOptionsWidget* optionsWidget =
    dynamic_cast<qSlicerIOOptionsWidget*>(options);
  // options is not necessary a qSlicerIOOptionsWidget (for the case of
  // readers/modules with no UI. If there is a UI then add it inside the  file
  // dialog.
  if (optionsWidget)
    {
    // fileDialog will reparent optionsWidget and take care of deleting
    // optionsWidget for us.
    fileDialog->setBottomWidget(optionsWidget, tr("Options:"));
    connect(fileDialog, SIGNAL(fileSelectionChanged(QStringList)),
            optionsWidget, SLOT(setFileNames(QStringList)));
    connect(optionsWidget, SIGNAL(validChanged(bool)),
            fileDialog, SLOT(setAcceptButtonEnable(bool)));
    fileDialog->setAcceptButtonEnable(optionsWidget->isValid());
    }
  // we do not delete options now as it is still useful later (even if there is
  // no UI.) they are the options of the reader, UI or not.
  bool res = fileDialog->exec();
  if (res)
    {
    properties = ioProperties;
    if (options)
      {
      properties.unite(options->properties());
      }
    else
      {
      properties["fileName"] = fileDialog->selectedFiles();
      }
    if (d->Action == qSlicerFileDialog::Read)
      {
      ioManager->loadNodes(this->fileType(), properties);
      }
    else if(d->Action == qSlicerFileDialog::Write)
      {
      ioManager->saveNodes(this->fileType(), properties);
      }
    else
      {
      Q_ASSERT(d->Action == qSlicerFileDialog::Read ||
               d->Action == qSlicerFileDialog::Write);
      }
    }

  ioManager->setFavorites(fileDialog->sidebarUrls());

  // If options is not a qSlicerIOOptionsWidget, we are responsible for
  // deleting options. If it is, then fileDialog would have reparent
  // the options and take care of its destruction
  if (!optionsWidget)
    {
    delete options;
    options = 0;
    }
  delete fileDialog;
  return res;
}

//-----------------------------------------------------------------------------
QStringList qSlicerStandardFileDialog::getOpenFileName(
    qSlicerIO::IOProperties ioProperties)
{
  QStringList files;
  ioProperties["multipleFiles"] = QFileDialog::ExistingFiles;
  ioProperties["objectName"] = "getOpenFileName";
  ctkFileDialog* fileDialog = qSlicerStandardFileDialog::createFileDialog(
                                ioProperties);
  qSlicerIOManager* ioManager = qSlicerApplication::application()->ioManager();

  if(fileDialog->exec() == QDialog::Accepted)
    {
    files = fileDialog->selectedFiles();
    }
  ioManager->setFavorites(fileDialog->sidebarUrls());
  delete fileDialog;
  return files;
}

//-----------------------------------------------------------------------------
QString qSlicerStandardFileDialog::getExistingDirectory(
    qSlicerIO::IOProperties ioProperties)
{
  QString directory;
  ioProperties["fileMode"] = QFileDialog::Directory;
  ioProperties["objectName"] = "getExistingDirectory";
  ctkFileDialog* fileDialog = qSlicerStandardFileDialog::createFileDialog(
                                ioProperties);
  qSlicerIOManager* ioManager = qSlicerApplication::application()->ioManager();

  if (fileDialog->exec() == QDialog::Accepted)
    {
    directory = fileDialog->selectedFiles().value(0);
    }
  ioManager->setFavorites(fileDialog->sidebarUrls());
  delete fileDialog;
  return directory;
}
