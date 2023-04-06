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

#include "request_provider.h"

namespace sequencergui
{

UserChoiceProvider::UserChoiceProvider(provider_callback_t callback)
{
  connect(this, &UserChoiceProvider::ChoiceRequest, this, &UserChoiceProvider::OnChoiceRequest,
          Qt::QueuedConnection);
}

UserChoiceResult UserChoiceProvider::GetUserChoice(const UserChoiceArgs &args)
{
  emit ChoiceRequest();

  return m_choice_request.GetData(args);
}

void UserChoiceProvider::OnChoiceRequest()
{
  auto args = m_choice_request.GetArguments();
  m_choice_request.SendData(m_provider_callback(args));
}

}  // namespace sequencergui
