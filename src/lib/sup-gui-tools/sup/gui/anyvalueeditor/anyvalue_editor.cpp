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

#include "anyvalue_editor.h"

#include "anyvalue_editor_actions.h"
#include "anyvalue_editor_textpanel.h"
#include "anyvalue_editor_toolbar.h"

#include <mvvm/model/application_model.h>
#include <mvvm/project/model_has_changed_controller.h>
#include <mvvm/widgets/all_items_tree_view.h>
#include <mvvm/widgets/item_view_component_provider.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/viewmodel/anyvalue_viewmodel.h>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QTreeView>

namespace sup::gui
{

AnyValueEditor::AnyValueEditor(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_actions(new AnyValueEditorActions(CreateActionContext(), m_model.get(), this))
    , m_tool_bar(new AnyValueEditorToolBar(m_actions))
    , m_tree_view(new QTreeView)
    , m_text_edit(new AnyValueEditorTextPanel(m_model.get()))
    , m_splitter(new QSplitter)
    , m_component_provider(mvvm::CreateProvider<sup::gui::AnyValueViewModel>(m_tree_view))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);

  m_splitter->addWidget(m_tree_view);
  m_splitter->addWidget(m_text_edit);

  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_model->RegisterItem<sup::gui::AnyValueStructItem>();
  m_model->RegisterItem<sup::gui::AnyValueArrayItem>();
  m_model->RegisterItem<sup::gui::AnyValueScalarItem>();

  m_component_provider->SetApplicationModel(m_model.get());
  m_tree_view->expandAll();

  SetupConnections();
}

void AnyValueEditor::ImportAnyValueFromFile(const QString &file_name)
{
  // temporarily disabling the model to speed-up loading of large files
  m_component_provider->SetApplicationModel(nullptr);
  m_actions->OnImportFromFileRequest(file_name.toStdString());
  m_component_provider->SetApplicationModel(m_model.get());
  m_tree_view->expandAll();
}

//! Returns AnyValueItem selected by the user in item tree.

sup::gui::AnyValueItem *AnyValueEditor::GetSelectedItem() const
{
  return m_component_provider->GetSelected<sup::gui::AnyValueItem>();
}

//! Sets initial value. The given value will be cloned inside the editor's model and used as
//! a starting point for editing.

void AnyValueEditor::SetInitialValue(const AnyValueItem &item)
{
  m_actions->SetInitialValue(item);
}

AnyValueItem *AnyValueEditor::GetTopItem()
{
  return m_actions->GetTopItem();
}

//! Set up all connections.

void AnyValueEditor::SetupConnections()
{
  auto on_panel = [this]() { m_text_edit->setVisible(!m_text_edit->isVisible()); };
  connect(m_tool_bar, &AnyValueEditorToolBar::HidePannelButtonRequest, this, on_panel);
}

AnyValueEditor::~AnyValueEditor() = default;

//! Creates a context with all callbacks necessary for AnyValueEditorActions to function.

AnyValueEditorContext AnyValueEditor::CreateActionContext() const
{
  auto get_selected_callback = [this]() { return GetSelectedItem(); };

  auto notify_warning_callback = [this](const sup::gui::MessageEvent &event)
  {
    QMessageBox msg_box;
    msg_box.setText(QString::fromStdString(event.text));
    msg_box.setInformativeText(QString::fromStdString(event.informative));
    msg_box.setDetailedText(QString::fromStdString(event.detailed));
    msg_box.setIcon(msg_box.Warning);
    msg_box.exec();
  };

  return {get_selected_callback, notify_warning_callback};
}

}  // namespace sup::gui
