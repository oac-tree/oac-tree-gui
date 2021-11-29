/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/mainwindow/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QMetaType>

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QApplication app(argc, argv);

  Q_INIT_RESOURCE(sequencericons);

  sequi::MainWindow win;
  win.show();

  return app.exec();
}
