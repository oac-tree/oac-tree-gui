/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalueeditor/editor_widget.h"

#include <anyvalueeditor/anyvalue_editor_actions.h>
#include <anyvalueeditor/anyvalue_editor_toolbar.h>
#include <anyvalueeditor/anyvalue_item.h>
#include <anyvalueeditor/conversion_utils.h>
#include <anyvalueeditor/highlighter/qsourcehighliter.h>
#include <mvvm/model/application_model.h>
#include <mvvm/widgets/all_items_tree_view.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

#include <QHBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QTreeView>

namespace anyvalueeditor
{

EditorWidget::EditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_actions(new AnyValueEditorActions(m_model.get(), this))
    , m_tool_bar(new AnyValueEditorToolBar(m_actions))
    , m_all_items_tree_view(new mvvm::AllItemsTreeView)
    , m_text_edit(new QTextEdit)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);

  m_splitter->addWidget(m_all_items_tree_view);
  m_splitter->addWidget(m_text_edit);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_model->RegisterItem<AnyValueStructItem>();
  m_model->RegisterItem<AnyValueArrayItem>();
  m_model->RegisterItem<AnyValueScalarItem>();

  PopulateModel();

  m_all_items_tree_view->SetApplicationModel(m_model.get());
  auto on_selected = [this](auto item)
  { OnSelectionChanged(dynamic_cast<AnyValueItem *>(const_cast<mvvm::SessionItem *>(item))); };
  connect(m_all_items_tree_view, &mvvm::AllItemsTreeView::SelectedItemChanged, this, on_selected);

  QFont textFont("Monospace");
  m_text_edit->setFont(textFont);
  m_text_edit->setLineWrapMode(QTextEdit::NoWrap);

  auto highlighter = new QSourceHighlite::QSourceHighliter(m_text_edit->document());
  highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeJSON);
}

void EditorWidget::ImportAnyValueFromFile(const QString &file_name)
{
  auto anyvalue = sup::dto::AnyValueFromJSONFile(file_name.toStdString());
  auto item =
      m_model->InsertItem(CreateItem(anyvalue), m_model->GetRootItem(), mvvm::TagIndex::Append());
  item->SetDisplayName("AnyValue");

  //   setting view back to the model
  m_all_items_tree_view->SetApplicationModel(m_model.get());
}

EditorWidget::~EditorWidget() = default;

void EditorWidget::PopulateModel()
{
  //  {  // two members
  //    sup::dto::AnyValue anyvalue = {
  //        {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType,
  //        true}}}};
  //    m_model->InsertItem(CreateItem(anyvalue), m_model->GetRootItem(), mvvm::TagIndex::Append());
  //  }

  //  {  // Nested structure
  //    sup::dto::AnyValue two_scalars = {
  //        {{"signed", {sup::dto::SignedInteger8Type, 1}}, {"bool", {sup::dto::BooleanType, 12}}}};
  //    sup::dto::AnyValue anyvalue{{
  //        {"scalars", two_scalars},
  //    }};
  //    m_model->InsertItem(CreateItem(anyvalue), m_model->GetRootItem(), mvvm::TagIndex::Append());
  //  }
}

void EditorWidget::OnSelectionChanged(AnyValueItem *item)
{
  if(!item)
  {
    return;
  }

  m_actions->SetSelectedItem(item);

  auto any_value = CreateAnyValue(*item);
  auto str = sup::dto::AnyValueToJSONString(any_value, true);
  m_text_edit->setText(QString::fromStdString(str));
}

}  // namespace anyvalueeditor
