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

#ifndef SEQUENCERGUI_COMPONENTS_MESSAGE_HELPER_H_
#define SEQUENCERGUI_COMPONENTS_MESSAGE_HELPER_H_

//! Collection of helper functions to send messages.

#include <sup/gui/components/message_event.h>

#include <QMessageBox>

namespace sequencergui
{

/**
 * @brief Send the message via the message box.
 */
void SendMessage(const sup::gui::MessageEvent& event, QMessageBox::Icon icon_type);

/**
 * @brief Send warning message via the message box.
 */
void SendWarningMessage(const sup::gui::MessageEvent& event);

/**
 * @brief Send info message via the message box.
 */
void SendInfoMessage(const sup::gui::MessageEvent& event);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_MESSAGE_HELPER_H_
