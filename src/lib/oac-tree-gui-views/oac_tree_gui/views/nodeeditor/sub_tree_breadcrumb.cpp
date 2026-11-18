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

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/nodeeditor/objects/node_editor_scene_controller.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/style/mvvm_style_helper.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QStringList>

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
    : QWidget(parent_widget), m_controller(controller), m_label(new QLabel)
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(mvvm::style::UnitSize(0.5), mvvm::style::UnitSize(0.2),
                             mvvm::style::UnitSize(0.5), mvvm::style::UnitSize(0.2));
  layout->addWidget(m_label);

  m_label->setTextFormat(Qt::RichText);
  m_label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
  // a long trail is cut, it doesn't widen the node editor
  m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
  setToolTip("SubTrees entered through Includes. Click an earlier SubTree to go back to it.");

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

}  // namespace oac_tree_gui
