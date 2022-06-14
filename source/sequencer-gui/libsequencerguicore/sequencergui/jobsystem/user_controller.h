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

#ifndef SEQUENCERGUI_MONITOR_USERCONTROLLER_H
#define SEQUENCERGUI_MONITOR_USERCONTROLLER_H

#include "sequencergui/jobsystem/user_context.h"

#include "mvvm/utils/threadsafe_stack.h"

#include <QObject>

namespace sequencergui
{

//! Provides user response for ProcedureRunner using callbacks.
//! The request for user input (issued from runner thread) and actual answer (provided by the GUI
//! thread via callbacks) is disentangled via a queued connection. That allows having runner thread
//! waiting for input, and GUI thread responsive.

class UserController : public QObject
{
  Q_OBJECT

public:
  explicit UserController(QObject* parent = nullptr);

  void SetUserContext(const UserContext& context);

  std::string GetUserInput(const std::string& current_value, const std::string& description);

  int GetUserChoice(const std::vector<std::string>& choices, const std::string& description);

  void SetAsUserInput(const std::string& value);

  void SetAsUserChoice(int value);

signals:
  void InputRequest(const QString& current_value, const QString& description);
  void ChoiceRequest(const QStringList& choices, const QString& description);

private:
  void OnInputRequest(const QString& current_value, const QString& description);
  void OnUserChoiceRequest(const QStringList& choices, const QString& description);
  void SetupConnections() const;

  mvvm::threadsafe_stack<std::string> m_input_data;
  mvvm::threadsafe_stack<int> m_choice_data;
  UserContext m_user_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_USERCONTROLLER_H
