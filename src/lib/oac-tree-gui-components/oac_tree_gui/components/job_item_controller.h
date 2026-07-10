/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_COMPONENTS_JOB_ITEM_CONTROLLER_H_
#define OAC_TREE_GUI_COMPONENTS_JOB_ITEM_CONTROLLER_H_

#include <oac_tree_gui/model/job_item.h>

#include <mvvm/signals/item_controller.h>

#include <functional>

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The JobItemController class notifies the client when expanded procedure on board of
 * JobItem changes.
 */
class JobItemController : public mvvm::ItemController<JobItem>
{
public:
  //! callback to notify the client about expanded procedure change
  using procedure_changed_t = std::function<void(ProcedureItem*)>;

  explicit JobItemController(const procedure_changed_t& procedure_changed);
  ~JobItemController() override;

  JobItemController(const JobItemController&) = delete;
  JobItemController& operator=(const JobItemController&) = delete;
  JobItemController(JobItemController&&) = delete;
  JobItemController& operator=(JobItemController&&) = delete;

protected:
  void Subscribe() override;
  void Unsubscribe() override;

private:
  void OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event);
  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event);
  void NotifyOnProcedureChange();

  procedure_changed_t m_procedure_changed;
  ProcedureItem* m_current_procedure_item{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_TEXT_EDIT_CONTROLLER_H_
