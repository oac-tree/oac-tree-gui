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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_PROCEDURE_OBSERVER_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_PROCEDURE_OBSERVER_H_

#include <sequencergui/jobsystem/domain_events.h>

#include <sup/sequencer/user_interface.h>

#include <functional>

namespace sequencergui
{

class DomainEventQueue;
class UserContext;
class UserChoiceProvider;
class UserInputProvider;

/**
 * @brief The DomainProcedureObserver class listens for changes in the domain procedure and
 * reports them to the event queue.
 */
class DomainProcedureObserver : public sup::sequencer::UserInterface
{
public:
  using post_event_callback_t = std::function<void(const domain_event_t& event)>;

  /**
   * @brief The main c-tor.
   *
   * @param post_event_callback A callback to report events to the GUI.
   * @param user_context Special user dialog callbacks to interact with the user.
   */
  explicit DomainProcedureObserver(post_event_callback_t post_event_callback,
                                   const UserContext& user_context);
  ~DomainProcedureObserver() override;

  void InitializeInstructionTree(const sup::sequencer::Instruction* root) override;

  void UpdateInstructionStatus(const ::sup::sequencer::Instruction* instruction) override;

  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                       bool connected) override;

  bool PutValue(const sup::dto::AnyValue& value, const std::string& description) override;

  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) override;

  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata) override;

  void Message(const std::string& message) override;

  void Log(int severity, const std::string& message) override;

private:
  post_event_callback_t m_post_event_callback;
  std::unique_ptr<UserChoiceProvider> m_choice_provider;
  std::unique_ptr<UserInputProvider> m_input_provider;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_PROCEDURE_OBSERVER_H_
