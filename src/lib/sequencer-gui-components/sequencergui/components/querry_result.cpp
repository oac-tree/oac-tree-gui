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

#include "querry_result.h"

namespace sequencergui
{

QuerryResult::QuerryResult(Status status, sup::gui::MessageEvent message)
    : m_status(status), m_message(std::move(message))
{
}

bool QuerryResult::IsSuccess() const
{
  return m_status == Status::kSuccess;
}

QuerryResult QuerryResult::Failure(const sup::gui::MessageEvent &message)
{
  return {Status::kFailure, message};
}

QuerryResult QuerryResult::Success()
{
  return {Status::kSuccess, {}};
}

sup::gui::MessageEvent QuerryResult::GetMessage() const
{
  return m_message;
}

}  // namespace sequencergui
