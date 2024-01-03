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

#include "exceptions.h"

namespace sequencergui
{

MessageException::MessageException(std::string message_) : message{std::move(message_)} {}

const char* MessageException::what() const noexcept
{
  return message.c_str();
}

NotImplementedException::NotImplementedException(const std::string& message)
    : MessageException{message}
{
}

RuntimeException::RuntimeException(const std::string& message) : MessageException{message} {}

LogicErrorException::LogicErrorException(const std::string& message) : MessageException{message} {}

TransformToDomainException::TransformToDomainException(const std::string& message)
    : MessageException{message}
{
}

NotFoundKeyException::NotFoundKeyException(const std::string& message) : MessageException{message}
{
}

NotFoundValueException::NotFoundValueException(const std::string& message)
    : MessageException{message}
{
}

ExistingKeyException::ExistingKeyException(const std::string& message) : MessageException{message}
{
}

NullException::NullException(const std::string& message) : MessageException{message} {}

InvalidOperationException::InvalidOperationException(const std::string& message)
    : MessageException{message}
{
}

}  // namespace sequencergui
