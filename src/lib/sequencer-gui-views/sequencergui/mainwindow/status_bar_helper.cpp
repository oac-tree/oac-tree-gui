/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "status_bar_helper.h"

#include <sup/gui/app/app_action_helper.h>

#include <mvvm/widgets/appearance_helper.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QApplication>
#include <QPalette>
#include <QToolButton>

namespace sequencergui
{

void SetupStatusBarButton(QToolButton *button, const QString &command_id)
{
  const int size = mvvm::utils::UnitSize(1.3);  // size of the button
  const auto palette = QApplication::palette();
  const auto background_color = palette.color(QPalette::Window);

  // set button appearance
  button->setText("");
  button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  button->setStyleSheet(mvvm::GetFlatButtonStyleString(background_color));
  button->setFixedSize(size, size);
  button->setIconSize(QSize(size, size));

  // connect button with the proxy action in the toolbar
  auto action = sup::gui::FindProxyAction(command_id);
  QObject::connect(button, &QToolButton::clicked, action, &QAction::trigger);

  // synchronize enabled status of action and the button
  button->setEnabled(action->isEnabled());

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QObject::connect(action, &QAction::changed,
                   [button, action]() { button->setEnabled(action->isEnabled()); });
#else
  QObject::connect(action, &QAction::enabledChanged, button, &QToolButton::setEnabled);
#endif
}

}  // namespace sequencergui
