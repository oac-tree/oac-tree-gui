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

#include <sequencergui/jobsystem/request_handle.h>
#include <sequencergui/jobsystem/request_types.h>

#include <QObject>
#include <functional>

namespace sequencergui
{

class UserChoiceProvider : public QObject
{
  Q_OBJECT

public:
  using provider_callback_t = std::function<UserChoiceResult(UserChoiceArgs)>;
  using choice_request_t = RequestHandle<UserChoiceArgs, UserChoiceResult>;

  UserChoiceProvider(provider_callback_t callback);

  /**
   * @brief Returns result of user choice.
   *
   * @details The call is blocking and it is intended for call from sequencer observer. Thread
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
  provider_callback_t m_provider_callback;
  choice_request_t m_choice_request;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REQUEST_PROVIDER_H_
