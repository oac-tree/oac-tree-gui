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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalue_compact_scalar_editor.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/providers/viewmodel_delegate.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/widgets/widget_utils.h>

#include <QDataWidgetMapper>
#include <QHBoxLayout>
#include <QLabel>

namespace sequencergui
{

AnyValueCompactScalarEditor::AnyValueCompactScalarEditor(QWidget *parent_widget)
    : AbstractAnyValueEditor(parent_widget)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
    , m_view_model(std::make_unique<mvvm::PropertyViewModel>(m_model.get()))
    , m_widget_mapper(std::make_unique<QDataWidgetMapper>())
    , m_layout(new QVBoxLayout(this))
    , m_label(new QLabel)
{
  setWindowTitle("AnyValueCompactScalarEditor");

  m_layout->setContentsMargins(mvvm::utils::UnitSize(0.5), 0, mvvm::utils::UnitSize(0.5), 0);
  m_layout->addSpacing(mvvm::utils::UnitSize(0.8));
  m_layout->addWidget(m_label);
}

AnyValueCompactScalarEditor::~AnyValueCompactScalarEditor() = default;

void AnyValueCompactScalarEditor::SetDescription(const QString &text)
{
  m_label->setText(text);
}

void AnyValueCompactScalarEditor::SetInitialValue(const sup::gui::AnyValueItem *item)
{
  m_model->InsertItem(mvvm::utils::CloneItem(*item), m_model->GetRootItem(),
                      mvvm::TagIndex::Append());

  // FIXME find better way to use our custom cell-editors
  // this one is way too verbose

  m_widget_mapper->setModel(m_view_model.get());
  m_widget_mapper->setItemDelegate(m_delegate.get());
  m_widget_mapper->setRootIndex(QModelIndex());
  m_widget_mapper->setCurrentModelIndex(m_view_model->index(0, 0));

  // index of value
  auto index = m_view_model->index(0, 1);

  const QStyleOptionViewItem view_item;
  auto editor = std::unique_ptr<QWidget>(m_delegate->createEditor(nullptr, view_item, index));
  m_delegate->setEditorData(editor.get(), index);
  m_widget_mapper->addMapping(editor.get(), 1);

  m_editor = editor.release();
  m_layout->addWidget(m_editor);
  m_layout->addStretch(1);
  m_layout->addSpacing(mvvm::utils::UnitSize(0.5));
}

std::unique_ptr<sup::gui::AnyValueItem> AnyValueCompactScalarEditor::GetResult()
{
  m_widget_mapper->submit();
  return mvvm::utils::CloneItem(*mvvm::utils::GetTopItem<sup::gui::AnyValueItem>(m_model.get()));
}

}  // namespace sequencergui
