/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MONITOR_USERCONTROLLER_H
#define MVVM_MONITOR_USERCONTROLLER_H

#include "sequencergui/monitor/usercontext.h"

#include "mvvm/utils/threadsafestack.h"

#include <QObject>

namespace sequi
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

signals:
  void InputRequest(const QString& current_value, const QString& description);

private:
  void OnInputRequest(const QString& current_value, const QString& description);
  void SetAsUserInput(const std::string& value);
  void SetupConnections();

  ModelView::threadsafe_stack<std::string> m_input_data;
  UserContext m_user_context;
};

}  // namespace sequi

#endif  // MVVM_MONITOR_USERCONTROLLER_H
