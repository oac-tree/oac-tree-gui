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

#include "sub_tree_breadcrumb.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/nodeeditor/objects/node_editor_scene_controller.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/style/mvvm_style_helper.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QStringList>
#include <QToolButton>

namespace oac_tree_gui
{

namespace
{

//! Returns the escaped display name of the instruction, or the given text if it is nullptr.
QString GetName(const InstructionItem* instruction, const QString& missing_text)
{
  return (instruction != nullptr)
             ? QString::fromStdString(instruction->GetDisplayName()).toHtmlEscaped()
             : missing_text;
}

}  // namespace

SubTreeBreadcrumb::SubTreeBreadcrumb(NodeEditorSceneController* controller, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_controller(controller)
    , m_label(new QLabel)
    , m_included_by_button(new QToolButton)
    , m_included_by_menu(new QMenu(this))
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(mvvm::style::UnitSize(0.5), mvvm::style::UnitSize(0.2),
                             mvvm::style::UnitSize(0.5), mvvm::style::UnitSize(0.2));
  layout->addWidget(m_label);
  layout->addWidget(m_included_by_button);
  layout->addStretch();

  m_label->setTextFormat(Qt::RichText);
  m_label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
  m_label->setToolTip(
      "SubTrees entered through Includes. Click an earlier SubTree to go back to it.");
  // a long trail is cut, it doesn't widen the node editor
  m_label->setMinimumWidth(1);

  m_included_by_button->setText("Included by");
  m_included_by_button->setToolTip("Includes pointing at this SubTree");
  m_included_by_button->setAutoRaise(true);
  m_included_by_button->setPopupMode(QToolButton::InstantPopup);
  m_included_by_button->setMenu(m_included_by_menu);
  connect(m_included_by_menu, &QMenu::aboutToShow, this, &SubTreeBreadcrumb::FillIncludedByMenu);

  connect(m_label, &QLabel::linkActivated, this,
          [this](const QString& link) { m_controller->GoToHistoryEntry(link.toULong()); });
  connect(m_controller, &NodeEditorSceneController::historyChanged, this,
          &SubTreeBreadcrumb::UpdateText);
  connect(m_controller, &NodeEditorSceneController::scopeChanged, this,
          &SubTreeBreadcrumb::UpdateText);

  UpdateText();
}

SubTreeBreadcrumb::~SubTreeBreadcrumb() = default;

QString SubTreeBreadcrumb::GetText() const
{
  return m_label->text();
}

void SubTreeBreadcrumb::UpdateText()
{
  m_included_by_button->setVisible(m_controller->GetScope() != nullptr);

  const auto& history = m_controller->GetHistory();
  const auto& entries = history.GetEntries();
  const auto* container = m_controller->GetInstructionContainer();
  if (entries.empty() || (container == nullptr) || (container->GetModel() == nullptr))
  {
    m_label->clear();
    return;
  }

  const auto trail_start = history.GetTrailStart();
  const auto current_index = history.GetCurrentIndex();
  QStringList segments;
  for (auto index = trail_start; index <= current_index; ++index)
  {
    const auto& entry = entries[index];
    QString segment;
    if (index > trail_start)
    {
      const auto* include = dynamic_cast<const InstructionItem*>(
          container->GetModel()->FindItem(entry.include_identifier));
      segment = GetName(include, "(deleted)") + " → ";
    }

    if (index == current_index)
    {
      segment += "<b>" + GetName(m_controller->GetScope(), "(none)") + "</b>";
    }
    else if (const auto* scope = m_controller->FindHistoryScope(entry); scope != nullptr)
    {
      segment += QString("<a href=\"%1\">%2</a>").arg(index).arg(GetName(scope, {}));
    }
    else
    {
      segment += "(deleted)";
    }
    segments.append(segment);
  }

  m_label->setText(segments.join(" › "));
}

void SubTreeBreadcrumb::FillIncludedByMenu()
{
  m_included_by_menu->clear();

  const auto* scope = m_controller->GetScope();
  const auto* container = m_controller->GetInstructionContainer();
  if ((scope == nullptr) || (container == nullptr))
  {
    return;
  }

  // Includes come in container order, so each top-level instruction is one group
  const InstructionItem* group = nullptr;
  for (const auto* include : FindIncludesOf(*scope, *container))
  {
    const auto* top_level = GetTopLevelInstruction(*include);
    if (top_level != group)
    {
      group = top_level;
      if (!m_included_by_menu->isEmpty())
      {
        (void)m_included_by_menu->addSeparator();
      }
      auto header = m_included_by_menu->addAction(QString::fromStdString(group->GetDisplayName()));
      header->setEnabled(false);
      auto font = header->font();
      font.setBold(true);
      header->setFont(font);
    }

    const auto path = include->GetAttribute<std::string>(domainconstants::kPathAttribute);
    auto action = m_included_by_menu->addAction(QString("%1 (%2)").arg(
        QString::fromStdString(include->GetDisplayName()), QString::fromStdString(path)));
    connect(action, &QAction::triggered, this,
            [this, include]() { m_controller->GoToInstruction(*include); });
  }

  if (m_included_by_menu->isEmpty())
  {
    m_included_by_menu->addAction("No Includes point here")->setEnabled(false);
  }
}

}  // namespace oac_tree_gui
