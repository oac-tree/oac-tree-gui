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
 * it is a part of the model.
 */

class JobItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Job";

  using CompoundItem::CompoundItem;
  JobItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetStatus() const;

  void SetStatus(const std::string& status);

  /**
   * @brief Sets procedure to handle.
   */
  void SetProcedure(const ProcedureItem* item);

  /**
   * @brief Returns a pointer to a procedure which is handled by given job item.
   *
   * @details It works only when both JobItem and ProcedureItem are belongs to the same model, or
   * handled by the same memory pool.
   */
  ProcedureItem* GetProcedure() const;

  ProcedureItem* GetExpandedProcedure();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_JOB_ITEM_H_
