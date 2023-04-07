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

#ifndef SEQUENCERGUI_JOBSYSTEM_USER_INPUT_PROVIDER_H_
#define SEQUENCERGUI_JOBSYSTEM_USER_INPUT_PROVIDER_H_

#include <sequencergui/jobsystem/request_handler_queue.h>
#include <sequencergui/jobsystem/request_types.h>

#include <QObject>

namespace sequencergui
{

/**
 * @brief The UserInputProvider class provides consumer thread with user input.
 *
 * @details The request for user input (issued from sequencer thread, for example) and actual answer
 * (provided by the GUI thread via callbacks) is disentangled via a queued connection. That allows
 * having consumer thread waiting for input, and GUI thread responsive.
 */

class UserInputProvider : public QObject
{
  Q_OBJECT

public:
  using provider_callback_t = std::function<UserInputResult(UserInputArgs)>;

  explicit UserInputProvider(provider_callback_t callback);

  /**
   * @brief Returns result of user input.
   *
   * @details The call is blocking and it is intended for call from consumer thread. Thread
   * will be released when the result is available.
   *
   * @param args Arguments to provide.
   *
   * @return Results of the user input.
   */
  UserInputResult GetUserInput(const UserInputArgs& args);

private slots:
  void OnInputRequest();

signals:
  void InputRequest();

private:
  RequestHandlerQueue<UserInputResult, UserInputArgs> m_request_queue;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_USER_INPUT_PROVIDER_H_
