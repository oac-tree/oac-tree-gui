/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "main_window_helper.h"

#include <sequencergui/core/version.h>

#include <mvvm/widgets/app_utils.h>

#include <QApplication>
#include <QLocale>
#include <QStyleFactory>

namespace
{
const QString PreferredCodacStyle = "Adwaita";
}

namespace sequencergui
{

void InitCoreApplication(const QString &app_name)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QCoreApplication::setApplicationName(app_name);
  QCoreApplication::setApplicationVersion(QString::fromStdString(sequencergui::ProjectVersion()));
  QCoreApplication::setOrganizationName("coa");
}

void SetWindowStyle(const QString &app_style, int system_font_psize)
{
  if (!app_style.isEmpty())
  {
    QApplication::setStyle(QStyleFactory::create(app_style));
  }
  else
  {
#ifdef SEQUENCERGUI_CODAC
    // If no special request from the user, and we are on CODAC, use Adwaita style
    // which provide tolerable gnome-like UI
    QApplication::setStyle(QStyleFactory::create(PreferredCodacStyle));
#endif
  }

  mvvm::utils::SetApplicationFontSize(system_font_psize);
}

}  // namespace sequencergui
