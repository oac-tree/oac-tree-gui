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

#ifndef OAC_TREE_GUI_JOBSYSTEM_ABSTRACT_JOB_HANDLER_H_
#define OAC_TREE_GUI_JOBSYSTEM_ABSTRACT_JOB_HANDLER_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/jobsystem/domain_events.h>
#include <oac_tree_gui/jobsystem/i_job_handler.h>
#include <oac_tree_gui/operation/breakpoint_types.h>

#include <QObject>
#include <memory>

namespace oac_tree_gui
{

class ProcedureItemJobInfoBuilder;
struct LogEvent;
class BreakpointController;
class AbstractDomainRunner;
struct DomainEventDispatcherContext;

/**
 * @brief The AbstractJobHandler class is an abstract class that partially implements functionality
 * to run a job represented by the JobItem.
 *
 * Depending on the implementation, it can run jobs either localy, or remotely. The class is used by
 * the JobManager, where each JobItem is handled by its own JobHandler. JobHandler is responsible
 * for:
 *
 * - handling start/stop/pause/step requests
 * - listening for all sequencer domain events and updating the GUI items accordingly
 */
class AbstractJobHandler : public QObject, public IJobHandler
{
  Q_OBJECT

public:
  explicit AbstractJobHandler(JobItem* job_item);
  ~AbstractJobHandler() override;

  void Start() override;

  void Pause() override;

  void Step() override;

  void Stop() override;

  void Reset() override;

  bool IsRunning() const override;

  RunnerStatus GetRunnerStatus() const override;

  JobLog* GetJobLog() const override;

  void OnToggleBreakpointRequest(oac_tree_gui::InstructionItem* instruction) override;

  JobItem* GetJobItem() override;

  ProcedureItem* GetExpandedProcedure() const override;

signals:
  void InstructionStatusChanged(oac_tree_gui::InstructionItem* instruction);
  void NextLeavesChanged(const std::vector<oac_tree_gui::InstructionItem*>&);

protected:
  /**
   * @brief Returns domain  runner.
   */
  AbstractDomainRunner* GetDomainRunner();

  /**
   * @brief Returns a context necessary for sequencer event queue to function.
   */
  DomainEventDispatcherContext CreateEventDispatcherContext();

  /**
   * @brief Set-up domain runner.
   *
   * Depending on the type of the runner, the job will be either local, or remote.
   * Call to this method creates expanded ProcedureItem and populates it with the JobInfo content
   * obtained from the runner.
   */
  void Setup(std::unique_ptr<AbstractDomainRunner> runner);

  /**
   * @brief Returns item builder.
   */
  ProcedureItemJobInfoBuilder* GetItemBuilder();

private:
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
   * @brief Processes log events from the domain, and put them in GUI JobLog.
   */
  void onLogEvent(const oac_tree_gui::LogEvent& event);

  /**
   * @brief Handles events reporting for changes in domain's next leaves.
   */
  void OnNextLeavesChangedEvent(const NextLeavesChangedEvent& event);

  /**
   * @brief Handles events reporting update in the domain variable.
   */
  virtual void OnVariableUpdatedEvent(const VariableUpdatedEvent& event);

  /**
   * @brief Setups breakpoint controller.
   */
  void SetupBreakpointController();

  /**
   * @brief Setup expanded procedure item.
   *
   * It will reflect the content of domain procedure after its Setup.
   */
  void SetupExpandedProcedureItem();

  /**
   * @brief Set breakpoint to the domain instruction with the given index.
   */
  void SetDomainBreakpoint(std::size_t index, BreakpointStatus breakpoint_status);

  /**
   * @brief Propagates breakpoints from expanded procedure to domain.
   */
  void PropagateBreakpointsToDomain();

  //!< GUI object builder holding domain/GUI object correspondance
  std::unique_ptr<ProcedureItemJobInfoBuilder> m_procedure_item_builder;

  //!< main runner to start/stop jobs
  std::unique_ptr<AbstractDomainRunner> m_domain_runner;

  //!< main controller to handle breakpoints toggling
  std::unique_ptr<BreakpointController> m_breakpoint_controller;

  //!< the job log
  JobLog* m_job_log{nullptr};

  //!< the JobItem being handled
  JobItem* m_job_item{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_ABSTRACT_JOB_HANDLER_H_
