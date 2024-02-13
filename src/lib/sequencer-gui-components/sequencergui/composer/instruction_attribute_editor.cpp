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

#include "instruction_attribute_editor_actions.h"

#include <sequencergui/model/attribute_item.h>
#include <sup/gui/widgets/custom_header_view.h>

#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QDebug>
#include <QMenu>
#include <QSettings>
#include <QTreeView>
#include <QVBoxLayout>
#include <iostream>

namespace
{
const QString kGroupName("UniversalPropertyEditor");
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";
}  // namespace

namespace sequencergui
{

InstructionAttributeEditor::InstructionAttributeEditor(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(mvvm::CreateProvider<mvvm::PropertyViewModel>(m_tree_view))
    , m_actions(new InstructionAttributeEditorActions(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);

  m_custom_header->setStretchLastSection(true);
  m_tree_view->setHeader(m_custom_header);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_tree_view, &QTreeView::customContextMenuRequested, this,
          &InstructionAttributeEditor::SummonCustomMenu);

  ReadSettings();
  AdjustTreeAppearance();
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

void InstructionAttributeEditor::SummonCustomMenu(const QPoint &point)
{
  auto index = m_tree_view->indexAt(point);
  auto item = dynamic_cast<AttributeItem *>(
      m_component_provider->GetViewModel()->GetSessionItemFromIndex(index));
  if (!item)
  {
    return;
  }

  QMenu menu;
  menu.setToolTipsVisible(true);

  auto action = menu.addAction("Attribute is enabled flag");
  action->setToolTip("Attribute with enabled flag set will be propagated to domain.");
  action->setCheckable(true);
  action->setChecked(item->IsPresent());
  auto on_unset = [item]() { item->SetPresentFlag(!item->IsPresent()); };
  connect(action, &QAction::triggered, this, on_unset);

  menu.addSeparator();

  action = menu.addAction("Set default value");
  action->setToolTip("The attribute will be set to its default value");
  auto on_default_attribute = [item]() { item->SetAttributeFromTypeName(); };
  connect(action, &QAction::triggered, this, on_default_attribute);

  action = menu.addAction("Set placeholder attribute");
  action->setToolTip(
      "Attribute will be defined as string, allowing to use placeholders $par and references @par");
  auto on_placeholder = [item]() { item->SetAttributeAsString("$par"); };
  connect(action, &QAction::triggered, this, on_placeholder);

  menu.exec(m_tree_view->mapToGlobal(point));
}

}  // namespace sequencergui
