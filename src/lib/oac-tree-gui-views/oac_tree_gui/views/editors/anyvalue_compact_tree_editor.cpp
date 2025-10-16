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

#include "anyvalue_compact_tree_editor.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>
#include <mvvm/widgets/widget_utils.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QTreeView>

namespace oac_tree_gui
{

namespace
{
const QString kGroupName("AnyValueCompactTreeEditor");
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";
}  // namespace

AnyValueCompactTreeEditor::AnyValueCompactTreeEditor(QWidget *parent_widget)
    : AbstractAnyValueEditor(parent_widget)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_label(new QLabel)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(kHeaderStateSettingName, this))
    , m_component_provider(
          mvvm::CreateProvider<mvvm::PropertyViewModel>(m_tree_view, m_model.get()))
{
  setWindowTitle("AnyValueCompactTreeEditor");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(mvvm::utils::UnitSize(0.5), 0, mvvm::utils::UnitSize(0.5), 0);
  layout->addSpacing(mvvm::utils::UnitSize(0.8));
  layout->addWidget(m_label);
  layout->addSpacing(mvvm::utils::UnitSize(0.5));
  layout->addWidget(m_tree_view);

  m_label->setHidden(true);

  m_tree_view->setHeader(m_custom_header);
}

AnyValueCompactTreeEditor::~AnyValueCompactTreeEditor() = default;

void AnyValueCompactTreeEditor::SetDescription(const QString &text)
{
  m_label->setHidden(false);
  m_label->setText(text);
}

void AnyValueCompactTreeEditor::SetInitialValue(const sup::gui::AnyValueItem *item)
{
  m_model->InsertItem(mvvm::utils::CloneItem(*item), m_model->GetRootItem(),
                      mvvm::TagIndex::Append());
  m_tree_view->expandAll();
}

std::unique_ptr<sup::gui::AnyValueItem> AnyValueCompactTreeEditor::GetResult()
{
  return mvvm::utils::CloneItem(*mvvm::utils::GetTopItem<sup::gui::AnyValueItem>(*m_model));
}

}  // namespace oac_tree_gui
