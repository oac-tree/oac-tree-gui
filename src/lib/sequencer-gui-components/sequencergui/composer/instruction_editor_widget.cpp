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

#include "instruction_editor_widget.h"

#include "instruction_editor_context.h"

#include <sequencergui/components/message_helper.h>
#include <sequencergui/composer/instruction_editor_actions.h>
#include <sequencergui/composer/instruction_editor_controller.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/property_tree_view.h>
#include <mvvm/widgets/top_items_tree_view.h>

#include <QSettings>
#include <QSplitter>
#include <QVBoxLayout>

namespace
{
const QString kGroupName("InstructionEditorWidget");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";

}  // namespace

namespace sequencergui
{

InstructionEditorWidget::InstructionEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new mvvm::TopItemsTreeView)
    , m_property_tree(new mvvm::PropertyTreeView)
    , m_splitter(new QSplitter)
    , m_editor_actions(new InstructionEditorActions(this))
    , m_editor_controller(
          std::make_unique<InstructionEditorController>(CreateInstructionEditorContext()))
{
  setWindowTitle("Instruction Tree");
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_tree_view);
  m_splitter->addWidget(m_property_tree);
  m_splitter->setSizes(QList<int>() << 300 << 200);

  layout->addWidget(m_splitter);

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view->GetTreeView());

  SetupConnections();

  addActions(m_editor_actions->GetActions());

  ReadSettings();
}

InstructionEditorWidget::~InstructionEditorWidget()
{
  WriteSettings();
}

void InstructionEditorWidget::SetProcedure(ProcedureItem *procedure)
{
  m_procedure = procedure;
  m_tree_view->SetItem(procedure ? procedure->GetInstructionContainer() : nullptr);
}

void InstructionEditorWidget::SetSelectedInstructions(
    const std::vector<InstructionItem *> &instructions)
{
  m_tree_view->SetSelectedItems(::mvvm::utils::CastItems<mvvm::SessionItem>(instructions));
}

std::vector<InstructionItem *> InstructionEditorWidget::GetSelectedInstructions() const
{
  return ::mvvm::utils::CastItems<InstructionItem>(m_tree_view->GetSelectedItems());
}

InstructionItem *InstructionEditorWidget::GetSelectedInstruction() const
{
  auto selected = GetSelectedInstructions();
  return selected.empty() ? nullptr : selected.front();
}

void InstructionEditorWidget::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }
}

void InstructionEditorWidget::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
}

void InstructionEditorWidget::SetupConnections()
{
  auto on_selected_instruction_changed = [this](auto)
  {
    auto selected = GetSelectedInstruction();
    m_property_tree->SetItem(selected);
    emit InstructionSelected(selected);
  };
  connect(m_tree_view, &::mvvm::TopItemsTreeView::SelectedItemChanged, this,
          on_selected_instruction_changed);

  // propagate instruction related requests from InstructionTreeWidget to InstructionEditorActions
  connect(m_editor_actions, &InstructionEditorActions::InsertAfterRequest,
          m_editor_controller.get(), &InstructionEditorController::OnInsertInstructionAfterRequest);
  connect(m_editor_actions, &InstructionEditorActions::InsertIntoRequest, m_editor_controller.get(),
          &InstructionEditorController::OnInsertInstructionIntoRequest);
  connect(m_editor_actions, &InstructionEditorActions::RemoveSelectedRequest,
          m_editor_controller.get(), &InstructionEditorController::OnRemoveInstructionRequest);
}

InstructionEditorContext InstructionEditorWidget::CreateInstructionEditorContext()
{
  InstructionEditorContext result;
  result.selected_procedure = [this]() { return m_procedure; };
  result.selected_instruction = [this]() { return GetSelectedInstruction(); };

  auto send_message_callback = [](const auto &event) { SendWarningMessage(event); };
  result.send_message_callback = send_message_callback;

  return result;
}

}  // namespace sequencergui
