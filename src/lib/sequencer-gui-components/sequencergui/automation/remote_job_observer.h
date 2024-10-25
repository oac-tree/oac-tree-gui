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

#ifndef SEQUENCERGUI_AUTOMATION_REMOTE_JOB_OBSERVER_H_
#define SEQUENCERGUI_AUTOMATION_REMOTE_JOB_OBSERVER_H_

#include <sup/sequencer/i_job_info_io.h>
#include <sequencergui/jobsystem/domain_events.h>

namespace sequencergui
{

class RemoteJobObserver : public sup::sequencer::IJobInfoIO
{
public:
  using post_event_callback_t = std::function<void(const domain_event_t& event)>;

  explicit RemoteJobObserver(post_event_callback_t post_event_callback);

  void InitNumberOfInstructions(sup::dto::uint32 n_instr) override;

  void InstructionStateUpdated(sup::dto::uint32 instr_idx,
                               sup::sequencer::InstructionState state) override;

  void VariableUpdated(sup::dto::uint32 var_idx, const sup::dto::AnyValue& value,
                       bool connected) override;

  void JobStateUpdated(sup::sequencer::JobState state) override;

  void PutValue(const sup::dto::AnyValue& value, const std::string& description) override;

  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) override;

  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata) override;

  void Message(const std::string& message) override;

  void Log(int severity, const std::string& message) override;

  void NextInstructionsUpdated(const std::vector<sup::dto::uint32>& instr_indices) override;

private:
  post_event_callback_t m_post_event_callback;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_AUTOMATION_REMOTE_JOB_OBSERVER_H_
