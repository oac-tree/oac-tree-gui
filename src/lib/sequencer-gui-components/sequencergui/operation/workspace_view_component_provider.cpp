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

#include "workspace_view_component_provider.h"

#include <mvvm/viewmodel/filter_name_viewmodel.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <QTreeView>

namespace sequencergui
{

WorkspaceViewComponentProvider::WorkspaceViewComponentProvider(
    std::unique_ptr<mvvm::ViewModel> view_model, QTreeView *view)
    : mvvm::ItemViewComponentProvider(std::move(view_model), view)
{
  auto proxy_model = std::make_unique<mvvm::FilterNameViewModel>();
  m_filter_proxy_model = proxy_model.get();
  AddProxyModel(std::move(proxy_model));
}

void WorkspaceViewComponentProvider::SetFilterPattern(const QString &pattern)
{
  m_filter_proxy_model->SetPattern(pattern);
}

}  // namespace sequencergui
