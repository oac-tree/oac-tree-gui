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

#include "dialog_message_handler.h"

#include <QMessageBox>

namespace sequencergui
{

void DialogMessageHandler::SendMessage(const std::string &text)
{
  QMessageBox msg_box;
  msg_box.setText(QString::fromStdString(text));
  msg_box.setIcon(msg_box.Critical);
  msg_box.exec();
}

}  // namespace sequencergui
