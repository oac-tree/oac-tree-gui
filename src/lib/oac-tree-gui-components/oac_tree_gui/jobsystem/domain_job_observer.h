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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_
#define OAC_TREE_GUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_

#include <oac_tree_gui/jobsystem/domain_events.h>

#include <sup/oac-tree/i_job_info_io.h>

#include <condition_variable>
#include <functional>
#include <mutex>

namespace sup::oac_tree
{
class ActiveInstructionMonitor;
}

namespace oac_tree_gui
{

struct UserContext;
class UserChoiceProvider;
class UserInputProvider;

/**
 * @brief The DomainJobObserver class listens for changes in the domain procedure and
 * reports them to the event queue.
 */
class DomainJobObserver : public sup::oac_tree::IJobInfoIO
{
public:
  using post_event_callback_t = std::function<void(const domain_event_t& event)>;
  using active_monitor_t = sup::oac_tree::ActiveInstructionMonitor;

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
                               sup::oac_tree::InstructionState state) override;

  void BreakpointInstructionUpdated(sup::dto::uint32 instr_idx) override;

  void VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue& value,
                       bool connected) override;

  void JobStateUpdated(sup::oac_tree::JobState state) override;

  void PutValue(const sup::dto::AnyValue& value, const std::string& description) override;

  bool GetUserValue(sup::dto::uint64 id, sup::dto::AnyValue& value,
                    const std::string& description) override;

  int GetUserChoice(sup::dto::uint64 id, const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata) override;

  void Interrupt(sup::dto::uint64 id) override;

  void Message(const std::string& message) override;

  void Log(int severity, const std::string& message) override;

  void NextInstructionsUpdated(const std::vector<sup::dto::uint32>& instr_indices);

  void ProcedureTicked() override;

  /**
   * @brief Returns last reported job state.
   */
  sup::oac_tree::JobState GetCurrentState() const;

  /**
   * @brief Waits for given state.
   */
  bool WaitForState(sup::oac_tree::JobState state, double msec) const;

  /**
   * @brief Waits for job finished state, which is one of succeeded/failed/halted.
   */
  sup::oac_tree::JobState WaitForFinished() const;

  /**
   * @brief Sets sleep time on every tick.
   */
  void SetTickTimeout(int msec);

  /**
   * @brief Sets filter to suppres active instruction filter notifications.
   */
  void SetInstructionActiveFilter(const active_filter_t& filter);

private:
  std::unique_ptr<active_monitor_t> CreateActiveInstructionMonitor(const active_filter_t& filter);

  post_event_callback_t m_post_event_callback;
  std::unique_ptr<UserChoiceProvider> m_choice_provider;
  std::unique_ptr<UserInputProvider> m_input_provider;
  std::unique_ptr<active_monitor_t> m_active_instruction_monitor;

  sup::oac_tree::JobState m_state{sup::oac_tree::JobState::kInitial};
  mutable std::mutex m_mutex;
  mutable std::condition_variable m_cv;
  int m_tick_timeout_msec{0};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_DOMAIN_JOB_OBSERVER_H_
