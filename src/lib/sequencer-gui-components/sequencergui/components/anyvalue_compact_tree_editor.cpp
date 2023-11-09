/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_compact_tree_editor.h"

#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <sup/gui/model/anyvalue_item.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QTreeView>

namespace sequencergui
{

AnyValueCompactTreeEditor::AnyValueCompactTreeEditor(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel)
    , m_tree_view(new QTreeView)
    , m_component_provider(mvvm::CreateProvider<mvvm::PropertyViewModel>(m_tree_view))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_label);
  layout->addWidget(m_tree_view);
}

AnyValueCompactTreeEditor::~AnyValueCompactTreeEditor() = default;

void AnyValueCompactTreeEditor::SetDescription(const QString &text)
{
  m_label->setText(text);
}

void AnyValueCompactTreeEditor::SetInitialValue(const sup::gui::AnyValueItem *item) {}

std::unique_ptr<sup::gui::AnyValueItem> AnyValueCompactTreeEditor::GetResult()
{
  return {};
}

}  // namespace sequencergui
