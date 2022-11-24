/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/core/standard_message_handlers.h"

#include <sequencergui/core/exceptions.h>

#include <QDebug>
#include <iostream>

namespace sequencergui
{

void StdMessageHandler::SendMessage(const std::string &text)
{
  qInfo("%s", text.c_str());
}

void NullMessageHandler::SendMessage(const std::string &text)
{
  std::cout << "ooooo" << std::endl;
  throw RuntimeException(text);
}

}  // namespace sequencergui
