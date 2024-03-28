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

#include "instruction_attribute_editor.h"

#include "attribute_editor_actions.h"
#include "instruction_attribute_editor_context.h"

#include <sequencergui/viewmodel/attribute_editor_viewmodel.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/model/sessionitem.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QMenu>
#include <QSettings>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("UniversalPropertyEditor");
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";
}  // namespace

namespace sequencergui
{

InstructionAttributeEditor::InstructionAttributeEditor(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new QToolBar)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(mvvm::CreateProvider<AttributeEditorViewModel>(m_tree_view))
    , m_attribute_actions(new AttributeEditorActions(CreateActionContext(), this))
{
  m_tool_bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_tool_bar->setIconSize(sup::gui::utils::NarrowToolBarIconSize());
  m_tool_bar->addActions(m_attribute_actions->GetToolBarActions());

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_tree_view);

  m_custom_header->setStretchLastSection(true);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &InstructionAttributeEditor::OnTreeContextMenuRequest);

  ReadSettings();
  AdjustTreeAppearance();

  connect(m_attribute_actions, &AttributeEditorActions::EditAnyvalueRequest, this,
          &InstructionAttributeEditor::EditAnyvalueRequest);
}

InstructionAttributeEditor::~InstructionAttributeEditor()
{
  WriteSettings();
}

void InstructionAttributeEditor::SetItem(mvvm::SessionItem *item)
{
  m_component_provider->SetItem(item);
  AdjustTreeAppearance();
}

void InstructionAttributeEditor::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void InstructionAttributeEditor::WriteSettings()
{
  QSettings settings;
  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

void InstructionAttributeEditor::AdjustTreeAppearance()
{
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
}

void InstructionAttributeEditor::OnTreeContextMenuRequest(const QPoint &point)
{
  auto index = m_tree_view->indexAt(point);
  auto item = dynamic_cast<sup::gui::AnyValueItem *>(
      m_component_provider->GetViewModel()->GetSessionItemFromIndex(index));

  QMenu menu;
  m_attribute_actions->SetupMenu(menu, item);
  menu.exec(m_tree_view->mapToGlobal(point));
}

InstructionAttributeEditorContext InstructionAttributeEditor::CreateActionContext()
{
  auto selected_item_callback = [this]() { return m_component_provider->GetSelectedItem(); };

  return {selected_item_callback};
}

}  // namespace sequencergui
