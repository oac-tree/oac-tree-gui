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

#include "workspace_view_component_provider.h"

#include <oac_tree_gui/viewmodel/workspace_filter_viewmodel.h>

#include <mvvm/viewmodel/viewmodel.h>

#include <QTreeView>

namespace oac_tree_gui
{

WorkspaceViewComponentProvider::WorkspaceViewComponentProvider(
    std::unique_ptr<mvvm::ViewModel> view_model, QTreeView *view)
    : mvvm::ItemViewComponentProvider(std::move(view_model), view)
{
  auto proxy_model = std::make_unique<WorkspaceFilterViewModel>();
  m_filter_proxy_model = proxy_model.get();
  AddProxyModel(std::move(proxy_model));
}

void WorkspaceViewComponentProvider::SetFilterPattern(const QString &pattern)
{
  m_filter_proxy_model->SetPattern(pattern);
}

}  // namespace oac_tree_gui
