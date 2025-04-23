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

#ifndef OAC_TREE_GUI_MODEL_JOB_ITEM_H_
#define OAC_TREE_GUI_MODEL_JOB_ITEM_H_

#include <oac_tree_gui/model/runner_status.h>

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The JobItem class is a base class for jobs to run a procedure.
 *
 * Job item links to ProcedureItem via item pool machinery, so it should be used only when it is a
 * part of the model. JobItem doesn't have its own logic to run a job and is handled by JobHandler.
 */
class JobItem : public mvvm::CompoundItem
{
public:
  explicit JobItem(const std::string& type);

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns job status.
   */
  RunnerStatus GetStatus() const;

  /**
   * @brief Sets the status of JobItem.
   */
  void SetStatus(RunnerStatus status);

  /**
   * @brief Returns the current value of sequencer ticks timeout in msec defined for this job.
   */
  int GetTickTimeout() const;

  /**
   * @brief Sets the value of sequencer ticks timeout that shall be used for this job execution.
   */
  void SetTickTimeout(int msec);

  /**
   * @brief Sets procedure to handle.
   */
  void SetProcedure(const ProcedureItem* item);

  /**
   * @brief Returns a pointer to a procedure which is handled by given job item.
   *
   * @details We store procedure via LinkIetm mechanism to provide persistence. It works only when
   * both JobItem and ProcedureItem are belongs to the same model, or handled by the same memory
   * pool.
   */
  ProcedureItem* GetProcedure() const;

  /**
   * @brief Returns underlying expanded procedure.
   *
   * Expanded procedure is a ProcedureItem which is obtained from the sequencer domain procedure
   * after its Setup. Expanded ProcedureItem is stored on board this JobItem, and may contain
   * breakpoint information, and is shown in a real-time instruction tree.
   */
  ProcedureItem* GetExpandedProcedure();
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_JOB_ITEM_H_
