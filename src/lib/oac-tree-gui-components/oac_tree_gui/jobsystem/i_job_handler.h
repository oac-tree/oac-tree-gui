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

#ifndef OAC_TREE_GUI_JOBSYSTEM_I_JOB_HANDLER_H_
#define OAC_TREE_GUI_JOBSYSTEM_I_JOB_HANDLER_H_

#include <oac_tree_gui/model/runner_status.h>

namespace oac_tree_gui
{

class InstructionItem;
class ProcedureItem;
class JobLog;
class JobItem;

/**
 * @brief The IJobHandler class is a an interface to run a job represented by the JobItem.
 */
class IJobHandler
{
public:
  virtual ~IJobHandler() = default;

  /**
   * @brief Run sequencer procedure in continuous manner, if allowed.
   */
  virtual void Start() = 0;

  /**
   * @brief Pause procedure's execution.
   */
  virtual void Pause() = 0;

  /**
   * @brief Execute a single step of the procedure, if allowed.
   */
  virtual void Step() = 0;

  /**
   * @brief Stops the procedure execution.
   */
  virtual void Stop() = 0;

  /**
   * @brief Reset not-running procedure to initial state.
   *
   * Call domains' AsyncRunner::Reset
   */
  virtual void Reset() = 0;

  /**
   * @brief Checks if the job is running, which is one of pause/stepping/running states.
   */
  virtual bool IsRunning() const = 0;

  /**
   * @brief Returns runner status.
   */
  virtual RunnerStatus GetRunnerStatus() const = 0;

  /**
   * @brief Returns a job log.
   */
  virtual JobLog* GetJobLog() const = 0;

  /**
   * @brief Set/unset a breakpoint for given instruction.
   *
   * It is used to handle double-clicks in a tree of instructions belonging to ExpandedProcedure
   * item. Each double-click will toggle breakpoint status in a circle set/disabled/not-set.
   *
   * @param instruction Instruction item from expanded procedure tree.
   *
   */
  virtual void OnToggleBreakpointRequest(InstructionItem* instruction) = 0;

  /**
   * @brief Returns job item served by this handler.
   */
  virtual JobItem* GetJobItem() = 0;

  /**
   * @brief Returns expanded ProcedureItem.
   */
  virtual ProcedureItem* GetExpandedProcedure() const = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_I_JOB_HANDLER_H_
