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

#ifndef SEQUENCERGUI_CORE_EXCEPTIONS_H_
#define SEQUENCERGUI_CORE_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace sequencergui
{

//! Base Exception class with message.

class MessageException : public std::exception
{
public:
  explicit MessageException(std::string message);
  const char* what() const noexcept override;

private:
  std::string message;
};

//! Generic exception to throw at runtime.

class RuntimeException : public MessageException
{
public:
  explicit RuntimeException(const std::string& message);
};

//! Generic exception to throw as a consequence of faulty logic (viloated preconditions etc).

class LogicErrorException : public MessageException
{
public:
  explicit LogicErrorException(const std::string& message);
};

//! Error detected in the Sequencer domain during the attempt to convert UI classes to
//! sequencer classes.

class TransformToDomainException : public MessageException
{
public:
  explicit TransformToDomainException(const std::string& message);
};

//! Exception is thrown when no such key exists in a map.

class NotFoundKeyException : public MessageException
{
public:
  explicit NotFoundKeyException(const std::string& message);
};

//! Exception is thrown when no such value exists in a map.

class NotFoundValueException : public MessageException
{
public:
  explicit NotFoundValueException(const std::string& message);
};

//! Exception is thrown when such key already exists in the map.

class ExistingKeyException : public MessageException
{
public:
  explicit ExistingKeyException(const std::string& message);
};

//! Exception is thrown when the argument is nullptr.

class NullException : public MessageException
{
public:
  explicit NullException(const std::string& message);
};

//! Exception is thrown on invalid operation.

class InvalidOperationException : public MessageException
{
public:
  explicit InvalidOperationException(const std::string& message);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_CORE_EXCEPTIONS_H_
