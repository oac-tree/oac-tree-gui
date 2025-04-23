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

#ifndef OAC_TREE_GUI_OPERATION_WORKSPACE_VIEW_COMPONENT_PROVIDER_H_
#define OAC_TREE_GUI_OPERATION_WORKSPACE_VIEW_COMPONENT_PROVIDER_H_

#include <mvvm/views/item_view_component_provider.h>

class QTreeView;

namespace mvvm
{
class ViewModel;
}  // namespace mvvm

namespace oac_tree_gui
{

class WorkspaceFilterViewModel;

/**
 * @brief The WorkspaceViewComponentProvider class provides filtering capabilities on top of the
 * given model.
 */
class WorkspaceViewComponentProvider : public mvvm::ItemViewComponentProvider
{
  Q_OBJECT

public:
  explicit WorkspaceViewComponentProvider(std::unique_ptr<mvvm::ViewModel> view_model,
                                          QTreeView* view);

  /**
   * @brief Sets filtering pattern to the display name in the first column.
   *
   * Item will be filtered out if its display name doesn't contain a given pattern (case
   * insensitive).
   */
  void SetFilterPattern(const QString& pattern);

private:
  WorkspaceFilterViewModel* m_filter_proxy_model{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_WORKSPACE_VIEW_COMPONENT_PROVIDER_H_
