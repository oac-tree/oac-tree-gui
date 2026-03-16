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

#include "favorite_instructions_widget.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/viewmodel/favorite_instructions_viewmodel.h>
#include <oac_tree_gui/widgets/flatlist_delegate.h>
#include <oac_tree_gui/widgets/flatlist_view.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/views/component_provider_helper.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

FavoriteInstructionsWidget::FavoriteInstructionsWidget(QWidget* parent_widget)
    : QWidget(parent_widget), m_list_view(new FlatListView), m_component_provider(CreateProvider())
{
  setWindowTitle("FAVORITE");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_list_view);

  // handles click on close button in column=1
  auto on_close = [this](const QModelIndex& index)
  {
    auto item = const_cast<mvvm::SessionItem*>(m_component_provider->GetItemFromViewIndex(index));
    if (item != nullptr)
    {
      mvvm::utils::RemoveItem(*item);
    }
  };

  connect(m_list_view, &FlatListView::closeHandleClicked, this, on_close);
}

FavoriteInstructionsWidget::~FavoriteInstructionsWidget() = default;

void FavoriteInstructionsWidget::SetInstructionContainer(
    InstructionContainerItem* instruction_container)
{
  m_component_provider->SetItem(instruction_container);
}

std::unique_ptr<mvvm::ItemViewComponentProvider> FavoriteInstructionsWidget::CreateProvider() const
{
  auto delegate = std::make_unique<FlatListDelegate>();
  auto viewmodel = std::make_unique<FavoriteInstructionsViewModel>(nullptr);

  return std::make_unique<mvvm::ItemViewComponentProvider>(std::move(delegate),
                                                           std::move(viewmodel), m_list_view);
}

}  // namespace oac_tree_gui
