/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_JOBSYSTEM_USER_CHOICE_PROVIDER_H_
#define SEQUENCERGUI_JOBSYSTEM_USER_CHOICE_PROVIDER_H_

#include <oac-tree-gui/jobsystem/request_handler_queue.h>
#include <oac-tree-gui/jobsystem/request_types.h>

#include <QObject>

namespace oac_tree_gui
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

  explicit UserChoiceProvider(provider_callback_t callback);

  /**
   * @brief Returns result of user choice.
   *
   * @details The call is blocking and it is intended for call from consumer thread. Thread
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
  RequestHandlerQueue<UserChoiceResult, UserChoiceArgs> m_request_queue;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_JOBSYSTEM_USER_CHOICE_PROVIDER_H_
