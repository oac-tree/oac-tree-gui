/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_attribute_editor.h"

#include "attribute_editor_actions.h"

#include <sequencergui/composer/attribute_editor_action_handler.h>
#include <sequencergui/composer/attribute_editor_context.h>
#include <sequencergui/viewmodel/attribute_editor_viewmodel.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/model/session_item.h>
#include <mvvm/providers/item_view_component_provider.h>

#include <QMenu>
#include <QSettings>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kHeaderStateSettingName = "InstructionAttributeEditor/header_state";
const std::vector<int> kDefaultColumnStretch({2, 1, 1});
}  // namespace

namespace sequencergui
{

InstructionAttributeEditor::InstructionAttributeEditor(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new QToolBar)
    , m_tree_view(new QTreeView)
    , m_custom_header(
          new sup::gui::CustomHeaderView(kHeaderStateSettingName, kDefaultColumnStretch, this))
    , m_component_provider(mvvm::CreateProvider<AttributeEditorViewModel>(m_tree_view))
    , m_attribute_action_handler(new AttributeEditorActionHandler(CreateActionContext(), this))
    , m_attribute_actions(new AttributeEditorActions(m_attribute_action_handler, this))
{
  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_tool_bar->setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  auto attribute_toolbar_actions =
      m_attribute_actions->GetActions({AttributeEditorActions::ActionKey::kModifyAttribute});
  m_tool_bar->addActions(attribute_toolbar_actions);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tree_view);

  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &InstructionAttributeEditor::OnTreeContextMenuRequest);

  connect(m_attribute_actions, &AttributeEditorActions::EditAnyvalueRequest, this,
          &InstructionAttributeEditor::EditAnyvalueRequest);
}

InstructionAttributeEditor::~InstructionAttributeEditor() = default;

void InstructionAttributeEditor::SetItem(mvvm::SessionItem *item)
{
  m_component_provider->SetItem(item);
  m_custom_header->AdjustColumnsWidth();
}

void InstructionAttributeEditor::OnTreeContextMenuRequest(const QPoint &point)
{
  QMenu menu;
  m_attribute_actions->SetupMenu(menu);
  menu.exec(m_tree_view->mapToGlobal(point));
}

AttributeEditorContext InstructionAttributeEditor::CreateActionContext()
{
  auto selected_item_callback = [this]() { return m_component_provider->GetSelectedItem(); };

  return {selected_item_callback};
}

}  // namespace sequencergui
