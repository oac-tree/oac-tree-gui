/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_APP_CONSTANTS_
#define SEQUENCERGUI_MAINWINDOW_APP_CONSTANTS_

#include <sup/gui/app/app_context.h>

#include <QString>

namespace sequencergui::app::constants
{

const sup::gui::AppContext kComposerContext("Composer");

const QString kCutProxyActionId = "Cut";
const QString kCopyProxyActionId = "Copy";
const QString kPasteProxyActionId = "Paste";

}  // namespace sequencergui::app::constants

#endif  // SEQUENCERGUI_MAINWINDOW_APP_CONSTANTS_
