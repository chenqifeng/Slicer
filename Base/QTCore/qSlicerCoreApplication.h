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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerCoreApplication_h
#define __qSlicerCoreApplication_h

// Qt includes
#include <QApplication>
#include <QMetaType>

// CTK includes
#include <ctkVTKObject.h>

// SlicerCore includes
#include "vtkSlicerConfigure.h" // For Slicer_USE_PYTHONQT
#include "qSlicerBaseQTCoreExport.h"

class ctkErrorLogModel;
#ifdef Slicer_BUILD_DICOM_SUPPORT
class ctkDICOMDatabase;
#endif
class QSettings;
class qSlicerCoreIOManager;
class qSlicerCoreCommandOptions;
class qSlicerCoreApplicationPrivate;
class qSlicerModuleManager;
#ifdef Slicer_USE_PYTHONQT
class qSlicerCorePythonManager;
#endif
#ifdef Slicer_BUILD_EXTENSIONMANAGER_SUPPORT
class qSlicerExtensionsManagerModel;
#endif
class vtkSlicerApplicationLogic;
class vtkMRMLApplicationLogic;
class vtkMRMLScene;

class Q_SLICER_BASE_QTCORE_EXPORT qSlicerCoreApplication : public QApplication
{
  Q_OBJECT
  QVTK_OBJECT
  Q_ENUMS(ReturnCode)

  Q_PROPERTY(QString slicerHome READ slicerHome CONSTANT)
  Q_PROPERTY(QString temporaryPath READ temporaryPath WRITE setTemporaryPath)
  Q_PROPERTY(QString launcherExecutableFilePath READ launcherExecutableFilePath CONSTANT)
  Q_PROPERTY(QString launcherSettingsFilePath READ launcherSettingsFilePath CONSTANT)
  Q_PROPERTY(QString extensionsInstallPath READ extensionsInstallPath WRITE setExtensionsInstallPath)
  Q_PROPERTY(QString intDir READ intDir CONSTANT)
  Q_PROPERTY(bool isInstalled READ isInstalled CONSTANT)
  Q_PROPERTY(QString repositoryUrl READ repositoryUrl CONSTANT)
  Q_PROPERTY(QString repositoryBranch READ repositoryBranch CONSTANT)
  Q_PROPERTY(QString repositoryRevision READ repositoryRevision CONSTANT)
  Q_PROPERTY(int majorVersion READ majorVersion CONSTANT)
  Q_PROPERTY(int minorVersion READ minorVersion CONSTANT)
  Q_PROPERTY(QString platform READ platform CONSTANT)
  Q_PROPERTY(QString arch READ arch CONSTANT)
  Q_PROPERTY(QString os READ os CONSTANT)
public:

  typedef QApplication Superclass;
  qSlicerCoreApplication(int &argc, char **argv);
  virtual ~qSlicerCoreApplication();

  /// Return a reference to the application singleton
  static qSlicerCoreApplication* application();

  /// Used in addition to existing QCoreApplication attribute.
  /// \sa Qt::ApplicationAttribute
  enum ApplicationAttribute
    {
    AA_DisablePython = 1000,
    AA_EnableTesting
    };

  /// \sa QCoreApplication::setAttribute
  static void setAttribute(qSlicerCoreApplication::ApplicationAttribute attribute, bool on = true);

  /// \sa QCoreApplication::testAttribute
  static bool testAttribute(qSlicerCoreApplication::ApplicationAttribute attribute);

  /// Convenient function to set an environment variable
  /// \note Using this function will ensure that the environment is up-to-date for
  /// processes started using QProcess or other alternative methods.
  void setEnvironmentVariable(const QString& key, const QString& value);

  /// Returns True if environment variable identified by \a key is set to \a value
  bool isEnvironmentVariableValueSet(const QString& key, const QString& value);

  /// Convenient function allowing to prepend \a value to environment variable identified by
  /// by \a key using \a separator
  void prependEnvironmentVariable(const QString& key, const QString& value, QChar separator = ';');

  /// Convenient function allowing to append \a value to environment variable identified by
  /// by \a key using \a separator
  void appendEnvironmentVariable(const QString& key, const QString& value, QChar separator = ';');

  /// Parse arguments
  /// \note If exitWhenDone is True, it's your responsability to exit the application
  void parseArguments(bool& exitWhenDone);

  enum ReturnCode{
    ExitNotRequested = -1,
    ExitSuccess = EXIT_SUCCESS,
    ExitFailure = EXIT_FAILURE
  };
  /// After parsing arguments and before exec() is called, returnCode contains the
  /// return code if any (!= -1).
  /// -1 if the application has not been asked to exit.
  /// EXIT_SUCCESS (0) if the application must return in success.
  /// EXIT_FAILURE (1) if the application failed.
  int returnCode()const;

  /// Get MRML Scene
  Q_INVOKABLE vtkMRMLScene* mrmlScene() const;

  /// Get application logic
  Q_INVOKABLE vtkSlicerApplicationLogic* applicationLogic() const;

  /// Get slicer home directory
  QString slicerHome() const;

  /// Returns True if module identified by \a moduleFileName is a descendant of slicer home.
  /// \sa slicerHome()
  bool isEmbeddedModule(const QString& moduleFileName)const;

  /// Get slicer default temporary directory
  QString defaultTemporaryPath() const;

  /// Get slicer temporary directory
  QString temporaryPath() const;

  /// Set slicer temporary directory
  void setTemporaryPath(const QString& path);

  /// If any, return slicer launcher executable file path.
  QString launcherExecutableFilePath()const;

  /// If any, return slicer launcher settings file path.
  QString launcherSettingsFilePath()const;

  /// Get slicer default extensions path
  QString defaultExtensionsInstallPath() const;

  /// Get slicer extension directory
  QString extensionsInstallPath() const;

  /// Set slicer extension directory
  void setExtensionsInstallPath(const QString& path);

  /// If any, this method return the build intermediate directory
  /// See $(IntDir) on http://msdn.microsoft.com/en-us/library/c02as0cs%28VS.71%29.aspx
  QString intDir()const;

  /// Return true is this instance of Slicer is running from an installed directory
  bool isInstalled()const;

#ifdef Slicer_USE_PYTHONQT
  /// Get python manager
  qSlicerCorePythonManager* corePythonManager()const;

  /// Set the IO manager
  /// \note qSlicerCoreApplication takes ownership of the object
  void setCorePythonManager(qSlicerCorePythonManager* pythonManager);
#endif

#ifdef Slicer_BUILD_EXTENSIONMANAGER_SUPPORT
  /// Get extension manager model
  Q_INVOKABLE qSlicerExtensionsManagerModel* extensionManagerModel()const;

  /// Set the extension manager model
  /// \note qSlicerCoreApplication takes ownership of the object
  void setExtensionManagerModel(qSlicerExtensionsManagerModel* model);
#endif

  /// Get the module manager
  Q_INVOKABLE qSlicerModuleManager* moduleManager()const;

  /// Get the IO manager
  Q_INVOKABLE qSlicerCoreIOManager* coreIOManager()const;

  /// Set the IO manager
  /// \note qSlicerCoreApplication takes ownership of the object
  void setCoreIOManager(qSlicerCoreIOManager* ioManager);

  /// Get coreCommandOptions
  qSlicerCoreCommandOptions* coreCommandOptions()const;

  /// Set coreCommandOptions
  /// \note qSlicerCoreApplication takes ownership of the object
  void setCoreCommandOptions(qSlicerCoreCommandOptions* options);

  /// Get errorLogModel
  Q_INVOKABLE ctkErrorLogModel* errorLogModel()const;

  /// Get application settings
  /// \note It will also instantiate a QSettings object if required.
  Q_INVOKABLE QSettings* settings()const;

  /// Disable application settings
  /// Instanciate a new empty ctkSettings object and associate it with this instance.
  /// \note The original settings won't deleted.
  void disableSettings();

  /// Clear application settings
  void clearSettings();

  /// Return the copyrights of Slicer
  virtual QString copyrights()const;

  /// Return the acknowledgment text of Slicer
  virtual QString acknowledgment()const;

  /// Return the libraries of Slicer
  virtual QString libraries()const;

  /// Return the source repository URL associated to this build
  /// \sa qSlicerCoreApplicationPrivate::discoverRepository
  QString repositoryUrl()const;

  /// Return the source repository Branch associated to this build
  /// \sa qSlicerCoreApplicationPrivate::discoverRepository
  QString repositoryBranch()const;

  /// Return the source repository Revision associated to this build
  /// \sa qSlicerCoreApplicationPrivate::discoverRepository
  QString repositoryRevision()const;

  /// Return the Slicer major version number
  int majorVersion() const;

  /// Return the Slicer minor version number
  int minorVersion() const;

  /// Return the \a platform associated to this build
  /// \sa qSlicerCoreApplicationPrivate::discoverRepository
  QString platform()const;

  /// Return the \a arch associated to this build
  /// \sa qSlicerCoreApplicationPrivate::discoverRepository
  QString arch()const;

  /// Return the \a os associated to this build
  /// \sa qSlicerCoreApplicationPrivate::discoverRepository
  QString os()const;

#ifdef Slicer_BUILD_DICOM_SUPPORT
  /// Return the active slicer dicom database (will be NULL until set by dicom
  /// management code).
  Q_INVOKABLE ctkDICOMDatabase* dicomDatabase() const;

  /// Set the current application dicomDatabase.
  Q_INVOKABLE void setDICOMDatabase(ctkDICOMDatabase* dicomDatabase);
#endif


public slots:

  /// Restart the application with the arguments passed at startup time
  /// \sa QCoreApplication::arguments()
  static void restart();

protected:
  ///
  virtual void handlePreApplicationCommandLineArguments();
  /// If fileName is set (not empty), a custom fileName is used
  /// otherwise it uses the default QSettings constructor.
  /// \sa QSettings::QSettings(QObject* parent)
  virtual QSettings* newSettings(const QString& fileName = QString());

  /// Set MRML Scene
  virtual void setMRMLScene(vtkMRMLScene * scene);

protected slots:

  ///
  virtual void handleCommandLineArguments();
  virtual void onSlicerApplicationLogicModified();
  void onSlicerApplicationLogicRequest(vtkObject*, void* , unsigned long);
  void processAppLogicModified();
  void processAppLogicReadData();
  void processAppLogicWriteData();

signals:
  void mrmlSceneChanged(vtkMRMLScene* mrmlScene);

protected:
  qSlicerCoreApplication(qSlicerCoreApplicationPrivate* pimpl, int &argc, char **argv);
  QScopedPointer<qSlicerCoreApplicationPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerCoreApplication);
  Q_DISABLE_COPY(qSlicerCoreApplication);
};

Q_DECLARE_METATYPE(qSlicerCoreApplication::ReturnCode)

#endif
