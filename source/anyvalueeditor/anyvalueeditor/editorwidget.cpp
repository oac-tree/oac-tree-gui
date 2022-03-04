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

#include "anyvalueeditor/editorwidget.h"

#include "AnyValue.h"
#include "anyvalueeditor/anyvalueitem.h"
#include "anyvalueeditor/transformutils.h"

#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace anyvalueeditor
{

EditorWidget::EditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tree_view);

  m_model->RegisterItem<AnyValueItem>();
  PopulateModel();

  m_view_model = std::make_unique<mvvm::AllItemsViewModel>(m_model.get());
  m_tree_view->setModel(m_view_model.get());
  m_tree_view->setItemDelegate(m_delegate.get());
}

EditorWidget::~EditorWidget() = default;

void EditorWidget::PopulateModel()
{
  {  // two members
    sup::dto::AnyValue anyvalue = {
        {{"signed", {sup::dto::SignedInteger32, 42}}, {"bool", {sup::dto::Boolean, true}}}};
    auto item = m_model->InsertItem<AnyValueItem>();
    PopulateItem(&anyvalue, item);
  }

  {  // Nested structure
    sup::dto::AnyValue two_scalars = {
        {{"signed", {sup::dto::SignedInteger8, 1}}, {"bool", {sup::dto::Boolean, 12}}}};
    sup::dto::AnyValue anyvalue{{
        {"scalars", two_scalars},
    }};
    auto item = m_model->InsertItem<AnyValueItem>();
    PopulateItem(&anyvalue, item);
  }

  {  // Two nested structures
    const std::string nested_name = "nested_struct";
    sup::dto::AnyValue two_scalars = {{{"signed", {sup::dto::SignedInteger8, 1}},
                                       {"unsigned", {sup::dto::UnsignedInteger8, 12}}}};
    sup::dto::AnyValue anyvalue{
        {{"scalars", two_scalars},
         {"single",
          {{"first", {sup::dto::SignedInteger8, 0}}, {"second", {sup::dto::SignedInteger8, 5}}}}},
        nested_name};
    auto item = m_model->InsertItem<AnyValueItem>();
    PopulateItem(&anyvalue, item);
  }
}

}  // namespace anyvalueeditor
