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

#ifndef SEQUENCERGUI_AUTOMATION_AUTOMATION_JOB_HANDLER_H_
#define SEQUENCERGUI_AUTOMATION_AUTOMATION_JOB_HANDLER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/jobsystem/domain_events.h>

#include <QObject>
#include <memory>

namespace sup::auto_server
{
class IJobInfoIO;
}

namespace sequencergui
{

class JobItem;
class ProcedureItemAutomationBuilder;
class RemoteJobObserver;
class DomainEventQueue;
class DomainEventDispatcher;
class DomainEventDispatcherContext;

/**
 * @brief The AutomationJobHandler class provides all machinery to propagate changes from remote
 * automation job to JobModel.
 *
 * It contains a machinery to generate ProcedureItem from automation job information, and then
 * update item on remote job observer (IJobInfoIO) calls.
 *
 * It doesn't contain any machinery to start/stop remote jobs.
 */
class AutomationJobHandler : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param job_item Empty job item.
   * @param job_info Job info from automation server.
   */
  AutomationJobHandler(JobItem* job_item, const sup::auto_server::JobInfo& job_info);
  ~AutomationJobHandler() override;

  /**
   * @brief GetJobObserver
   */
  sup::auto_server::IJobInfoIO* GetJobObserver() const;

private:
  /**
   * @brief Setups ProcedureItem on board of JobItem using provided automation job info.
   */
  void SetupProcedureItem(const sup::auto_server::JobInfo& job_info);

  /**
   * @brief Creates context necessary for DomainEventDispatcher.
   */
  DomainEventDispatcherContext CreateDispatcherContext();

  /**
   * @brief Creates a callback to publish domain events.
   */
  std::function<void(const domain_event_t& event)> CreatePostEventCallback() const;

  /**
   * @brief Creates a callback to get events from event queue.
   */
  std::function<domain_event_t()> CreateGetEventCallback() const;

  /**
   * @brief Processes instruction status change in the domain, and update InstructionItem's status
   * accordingly.
   */
  void OnInstructionStateUpdated(const InstructionStateUpdatedEvent& event);

  /**
   * @brief Processes job status changed from the domain, and update JobItem status accordingly.
   */
  void OnJobStateChanged(const JobStateChangedEvent& event);

  /**
   * @brief Processes variable change and update workspace accordingly.
   */
  void OnVariableUpdated(const VariableUpdatedEvent& event);

  std::unique_ptr<ProcedureItemAutomationBuilder> m_builder;
  std::unique_ptr<RemoteJobObserver> m_job_observer;
  std::unique_ptr<DomainEventQueue> m_event_queue;
  std::unique_ptr<DomainEventDispatcher> m_event_dispatcher;
  JobItem* m_job_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_AUTOMATION_AUTOMATION_JOB_HANDLER_H_
