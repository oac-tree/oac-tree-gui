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

#include "sub_tree_navigation_actions.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/nodeeditor/objects/node_editor_scene_controller.h>
#include <oac_tree_gui/style/style_helper.h>

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

namespace oac_tree_gui
{

SubTreeNavigationActions::SubTreeNavigationActions(NodeEditorSceneController* controller,
                                                   QObject* parent_object)
    : QObject(parent_object)
    , m_controller(controller)
    , m_subtree_menu(std::make_unique<QMenu>())
    , m_subtree_button(new QToolButton)
    , m_subtree_action(new QWidgetAction(this))
{
  m_subtree_button->setIcon(FindIcon("file-tree-outline"));
  m_subtree_button->setToolTip("Top-level instruction shown in the node editor");
  m_subtree_button->setPopupMode(QToolButton::InstantPopup);
  m_subtree_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_subtree_button->setMenu(m_subtree_menu.get());
  m_subtree_action->setText("SubTree");
  m_subtree_action->setDefaultWidget(m_subtree_button);

  connect(m_subtree_menu.get(), &QMenu::aboutToShow, this,
          &SubTreeNavigationActions::FillSubTreeMenu);
  connect(m_controller, &NodeEditorSceneController::scopeChanged, this,
          &SubTreeNavigationActions::UpdateSubTreeText);

  UpdateSubTreeText();
}

SubTreeNavigationActions::~SubTreeNavigationActions() = default;

QList<QAction*> SubTreeNavigationActions::GetActions() const
{
  return {m_subtree_action};
}

void SubTreeNavigationActions::FillSubTreeMenu()
{
  m_subtree_menu->clear();
  UpdateSubTreeText();

  const auto* container = m_controller->GetInstructionContainer();
  if (container == nullptr)
  {
    return;
  }

  const auto* root = FindRootInstruction(*container);
  const auto* scope = m_controller->GetScope();

  auto add_action = [this, scope](const InstructionItem& instruction)
  {
    auto action = m_subtree_menu->addAction(QString::fromStdString(instruction.GetDisplayName()));
    action->setCheckable(true);
    action->setChecked(&instruction == scope);
    connect(action, &QAction::triggered, this,
            [this, identifier = instruction.GetIdentifier()]() { SetScope(identifier); });
    return action;
  };

  if (root != nullptr)
  {
    auto action = add_action(*root);
    auto font = action->font();
    font.setBold(true);
    action->setFont(font);
    (void)m_subtree_menu->addSeparator();
  }

  for (const auto* instruction : container->GetInstructions())
  {
    if (instruction != root)
    {
      (void)add_action(*instruction);
    }
  }
}

void SubTreeNavigationActions::UpdateSubTreeText()
{
  const auto* scope = m_controller->GetScope();
  m_subtree_button->setText((scope != nullptr) ? QString::fromStdString(scope->GetDisplayName())
                                               : QString("(none)"));
}

void SubTreeNavigationActions::SetScope(const std::string& identifier)
{
  const auto* container = m_controller->GetInstructionContainer();
  if (container == nullptr)
  {
    return;
  }

  for (const auto* top_level : container->GetInstructions())
  {
    if (top_level->GetIdentifier() == identifier)
    {
      m_controller->SetScope(top_level);
      return;
    }
  }
}

}  // namespace oac_tree_gui
