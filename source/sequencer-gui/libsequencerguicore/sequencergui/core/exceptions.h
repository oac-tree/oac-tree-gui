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

#ifndef SEQUENCERGUI_CORE_EXCEPTIONS_H
#define SEQUENCERGUI_CORE_EXCEPTIONS_H

#include <exception>
#include <string>

namespace sequencergui
{

//! Base Exception class with message.

class MessageException : public std::exception
{
public:
  MessageException(std::string message);
  const char* what() const noexcept override;

private:
  std::string message;
};

//! Base Exception class with message.

class TransformToDomainException : public MessageException
{
public:
  TransformToDomainException(const std::string& message);
};

}  // namespace sequencergui

#endif // SEQUENCERGUI_CORE_EXCEPTIONS_H

