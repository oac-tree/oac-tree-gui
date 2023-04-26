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

#include <QApplication>
#include <QStyleFactory>

#include <iostream>

namespace
{
const QString PreferredCodacStyle = "Adwaita";
}

namespace sequencergui
{

void SetWindowStyle(const QString &user_style)
{
  if (!user_style.isEmpty())
  {
    QApplication::setStyle(QStyleFactory::create(user_style));
  }
  else
  {
#ifdef SEQUENCERGUI_CODAC
    // If no special request from the user, and we are on CODAC, use Adwaita style
    // which provide tolerable gnome-like UI
    QApplication::setStyle(QStyleFactory::create(PreferredCodacStyle));
#endif
  }
}

}  // namespace sequencergui
