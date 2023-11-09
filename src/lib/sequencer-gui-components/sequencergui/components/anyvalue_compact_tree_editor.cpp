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

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QTreeView>

namespace sequencergui
{

AnyValueCompactTreeEditor::AnyValueCompactTreeEditor(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_label(new QLabel)
    , m_tree_view(new QTreeView)
    , m_component_provider(
          mvvm::CreateProvider<mvvm::PropertyViewModel>(m_tree_view, m_model.get()))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_label);
  layout->addWidget(m_tree_view);

  m_model->RegisterItem<sup::gui::AnyValueStructItem>();
  m_model->RegisterItem<sup::gui::AnyValueArrayItem>();
  m_model->RegisterItem<sup::gui::AnyValueScalarItem>();
}

AnyValueCompactTreeEditor::~AnyValueCompactTreeEditor() = default;

void AnyValueCompactTreeEditor::SetDescription(const QString &text)
{
  m_label->setText(text);
}

void AnyValueCompactTreeEditor::SetInitialValue(const sup::gui::AnyValueItem *item)
{
  m_model->InsertItem(mvvm::utils::CloneItem(*item), m_model->GetRootItem(),
                      mvvm::TagIndex::Append());
}

std::unique_ptr<sup::gui::AnyValueItem> AnyValueCompactTreeEditor::GetResult()
{
  return mvvm::utils::CloneItem(*mvvm::utils::GetTopItem<sup::gui::AnyValueItem>(m_model.get()));
}

}  // namespace sequencergui
