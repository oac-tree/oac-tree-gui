/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_

#include <sequencergui/jobsystem/domain_events.h>

#include <sup/sequencer/i_job_info_io.h>

#include <condition_variable>
#include <functional>
#include <mutex>

namespace sequencergui
{

struct UserContext;
class UserChoiceProvider;
class UserInputProvider;

/**
 * @brief The DomainJobObserver class listens for changes in the domain procedure and
 * reports them to the event queue.
 */
class DomainJobObserver : public sup::sequencer::IJobInfoIO
{
public:
  using post_event_callback_t = std::function<void(const domain_event_t& event)>;

  /**
   * @brief The main c-tor.
   *
   * @param post_event_callback A callback to report events to the GUI.
   * @param user_context Special user dialog callbacks to interact with the user.
   */
  explicit DomainJobObserver(post_event_callback_t post_event_callback,
                             const UserContext& user_context);
  ~DomainJobObserver() override;

  void InitNumberOfInstructions(sup::dto::uint32 n_instr) override;

  void InstructionStateUpdated(sup::dto::uint32 instr_idx,
                               sup::sequencer::InstructionState state) override;

  void VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue& value,
                       bool connected) override;

  void JobStateUpdated(sup::sequencer::JobState state) override;

  void PutValue(const sup::dto::AnyValue& value, const std::string& description) override;

  bool GetUserValue(sup::dto::uint64 id, sup::dto::AnyValue& value,
                    const std::string& description) override;

  int GetUserChoice(sup::dto::uint64 id, const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata) override;

  void Interrupt(sup::dto::uint64 id) override;

  void Message(const std::string& message) override;

  void Log(int severity, const std::string& message) override;

  void NextInstructionsUpdated(const std::vector<sup::dto::uint32>& instr_indices) override;

  /**
   * @brief Returns last reported job state.
   */
  sup::sequencer::JobState GetCurrentState() const;

  /**
   * @brief Waits for given state.
   */
  bool WaitForState(sup::sequencer::JobState state, double msec) const;

  /**
   * @brief Waits for job finished state, which is one of succeeded/failed/halted.
   */
  sup::sequencer::JobState WaitForFinished() const;

  /**
   * @brief Sets sleep time on every tick.
   */
  void SetTickTimeout(int msec);

private:
  post_event_callback_t m_post_event_callback;
  std::unique_ptr<UserChoiceProvider> m_choice_provider;
  std::unique_ptr<UserInputProvider> m_input_provider;
  sup::sequencer::JobState m_state{sup::sequencer::JobState::kInitial};
  mutable std::mutex m_mutex;
  mutable std::condition_variable m_cv;
  int m_tick_timeout_msec{0};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_
