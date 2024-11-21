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

#ifndef SEQUENCERGUI_MODEL_JOB_ITEM_H_
#define SEQUENCERGUI_MODEL_JOB_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sequencergui
{

class ProcedureItem;

/**
 * @brief The JobItem class represents a job to run a procedure.
 *
 * @details Job item links to ProcedureItem via item pool machinery, so it should be used only when
 * it is a part of the model. JobItem doesn't have own logic to run a job and is handled by
 * JobHandler.
 */

class JobItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Job";
  using CompoundItem::CompoundItem;
  explicit JobItem(const std::string& type = Type);

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  /**
   * @brief Returns job status.
   */
  std::string GetStatus() const;

  /**
   * @brief Sets the job status.
   */
  void SetStatus(const std::string& status);

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
   * breakpoint information, and is shown in a real-time instruction tree. The whole chain looks
   * like the following.
   *
   * - ProcedureItem is what the user has created in the editor
   * - Sequencer domain procedure
   * - Sequencer domain procedure after Setup call
   * - Expanded ProcedureItem
   */
  ProcedureItem* GetExpandedProcedure();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_JOB_ITEM_H_
