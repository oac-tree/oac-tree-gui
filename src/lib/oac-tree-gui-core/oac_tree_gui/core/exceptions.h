/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_CORE_EXCEPTIONS_H_
#define OAC_TREE_GUI_CORE_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace oac_tree_gui
{

/**
 * @brief The MessageException class is a base exception with the message.
 */
class MessageException : public std::exception
{
public:
  explicit MessageException(std::string message);
  ~MessageException() override = default;

  const char* what() const noexcept override;

protected:
  MessageException(const MessageException& other) = default;
  MessageException& operator=(const MessageException& other) & = default;
  MessageException(MessageException&&) noexcept = default;
  MessageException& operator=(MessageException&&) & noexcept = default;

private:
  std::string m_message;
};

/**
 * @brief The RuntimeException class shall be used for generic errors happening at runtime.
 */
class RuntimeException : public MessageException
{
public:
  explicit RuntimeException(const std::string& message);
  ~RuntimeException() override = default;
  RuntimeException(const RuntimeException& other) = default;
  RuntimeException& operator=(const RuntimeException& other) & = default;
  RuntimeException(RuntimeException&&) noexcept = default;
  RuntimeException& operator=(RuntimeException&&) & noexcept = default;
};

/**
 * @brief The NullArgumentException class shall be used when the argument is nullptr.
 */
class NullArgumentException : public MessageException
{
public:
  explicit NullArgumentException(const std::string& message);
  ~NullArgumentException() override = default;
  NullArgumentException(const NullArgumentException& other) = default;
  NullArgumentException& operator=(const NullArgumentException& other) & = default;
  NullArgumentException(NullArgumentException&&) noexcept = default;
  NullArgumentException& operator=(NullArgumentException&&) & noexcept = default;
};

/**
 * @brief The NotImplementedException class shall be used when code is not implemented.
 */
class NotImplementedException : public MessageException
{
public:
  explicit NotImplementedException(const std::string& message);
  ~NotImplementedException() override = default;
  NotImplementedException(const NotImplementedException& other) = default;
  NotImplementedException& operator=(const NotImplementedException& other) & = default;
  NotImplementedException(NotImplementedException&&) noexcept = default;
  NotImplementedException& operator=(NotImplementedException&&) & noexcept = default;
};

/**
 * @brief The InvalidOperationException class shall be used on attempt to perform invalid operation.
 */
class InvalidOperationException : public MessageException
{
public:
  explicit InvalidOperationException(const std::string& message);
  ~InvalidOperationException() override = default;
  InvalidOperationException(const InvalidOperationException& other) = default;
  InvalidOperationException& operator=(const InvalidOperationException& other) & = default;
  InvalidOperationException(InvalidOperationException&&) noexcept = default;
  InvalidOperationException& operator=(InvalidOperationException&&) & noexcept = default;
};

/**
 * @brief The LogicErrorException class shall be used when faulty logic is discovered.
 */
class LogicErrorException : public MessageException
{
public:
  explicit LogicErrorException(const std::string& message);
  ~LogicErrorException() override = default;
  LogicErrorException(const LogicErrorException& other) = default;
  LogicErrorException& operator=(const LogicErrorException& other) & = default;
  LogicErrorException(LogicErrorException&&) noexcept = default;
  LogicErrorException& operator=(LogicErrorException&&) & noexcept = default;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_CORE_EXCEPTIONS_H_
