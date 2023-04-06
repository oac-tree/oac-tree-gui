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

#ifndef SEQUENCERGUI_JOBSYSTEM_REQUEST_PROVIDER_H_
#define SEQUENCERGUI_JOBSYSTEM_REQUEST_PROVIDER_H_

#include <sequencergui/jobsystem/request_handler.h>
#include <sequencergui/jobsystem/request_types.h>

#include <QObject>
#include <functional>
#include <mutex>
#include <queue>

namespace sequencergui
{

/**
 * @brief The UserChoiceProvider class provides consumer thread with the result of user choice.
 *
 * @details The request for user input (issued from sequencer thread, for example) and actual answer
 * (provided by the GUI thread via callbacks) is disentangled via a queued connection. That allows
 * having consumer thread waiting for input, and GUI thread responsive.
 */

class UserChoiceProvider : public QObject
{
  Q_OBJECT

public:
  using provider_callback_t = std::function<UserChoiceResult(UserChoiceArgs)>;
  using request_handler_t = RequestHandler<UserChoiceResult>;

  UserChoiceProvider(provider_callback_t callback);

  /**
   * @brief Returns result of user choice.
   *
   * @details The call is blocking and it is intended for call from concumer thread. Thread
   * will be released when the result is available.
   *
   * @param args Arguments to provide
   *
   * @return Results of the user choice.
   */
  UserChoiceResult GetUserChoice(const UserChoiceArgs& args);

private slots:
  void OnChoiceRequest();

signals:
  void ChoiceRequest();

private:
  struct RequestData
  {
    UserChoiceArgs args;
    request_handler_t request_handler;
  };

  mutable std::mutex m_mutex;
  provider_callback_t m_provider_callback;
  std::queue<RequestData*> m_stack;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REQUEST_PROVIDER_H_
