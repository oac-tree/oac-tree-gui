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
#include <sequencergui/components/visibility_agent_base.h>
#include <sequencergui/composer/instruction_editor_action_handler.h>
#include <sequencergui/composer/instruction_editor_actions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/viewmodel/instruction_viewmodel.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/item_view_component_provider.h>
#include <mvvm/widgets/property_tree_view.h>

#include <sup/gui/widgets/custom_header_view.h>

#include <QSettings>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>
#include <iostream>

bool FixColumnAppearance(QTreeView *tree)
{
  tree->setColumnHidden(2, true);  // status
  tree->setColumnHidden(3, true);  // breakpoint
  tree->header()->setStretchLastSection(true);
  return true;
}

namespace
{
const QString kGroupName("InstructionEditorWidget");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kHeaderStateSettingName = kGroupName + "/" + "header_state";
}  // namespace

namespace sequencergui
{

InstructionEditorWidget::InstructionEditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_custom_header(new sup::gui::CustomHeaderView(this))
    , m_component_provider(mvvm::CreateProvider<InstructionViewModel>(m_tree_view))
    , m_property_tree(new mvvm::PropertyTreeView)
    , m_splitter(new QSplitter)
    , m_editor_actions(new InstructionEditorActions(this))
    , m_action_handler(
          std::make_unique<InstructionEditorActionHandler>(CreateInstructionEditorContext()))
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

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_tree_view);
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->setHeader(m_custom_header);

  SetupConnections();

  addActions(m_editor_actions->GetActions());

  ReadSettings();

  auto on_subscribe = [this]() { SetProcedureIntern(m_procedure); };

  auto on_unsubscribe = [this]() { SetProcedureIntern(nullptr); };

  // will be deleted as a child of QObject
  m_visibility_agent = new VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

InstructionEditorWidget::~InstructionEditorWidget()
{
  WriteSettings();
}

void InstructionEditorWidget::SetProcedure(ProcedureItem *procedure)
{
  if (procedure == m_procedure)
  {
    return;
  }

  m_procedure = procedure;

  if (m_procedure && isVisible())
  {
    SetProcedureIntern(m_procedure);
  }
}

void InstructionEditorWidget::SetSelectedInstructions(
    const std::vector<InstructionItem *> &instructions)
{
  if (isHidden())
  {
    return;
  }
  m_component_provider->SetSelectedItems(::mvvm::utils::CastItems<mvvm::SessionItem>(instructions));
}

std::vector<InstructionItem *> InstructionEditorWidget::GetSelectedInstructions() const
{
  return ::mvvm::utils::CastItems<InstructionItem>(m_component_provider->GetSelectedItems());
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
  if (settings.contains(kHeaderStateSettingName))
  {
    m_custom_header->SetAsFavoriteState(settings.value(kHeaderStateSettingName).toByteArray());
  }
}

void InstructionEditorWidget::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
  if (m_custom_header->HasFavoriteState())
  {
    settings.setValue(kHeaderStateSettingName, m_custom_header->GetFavoriteState());
  }
}

void InstructionEditorWidget::AdjustTreeAppearance()
{
  if (m_custom_header->HasFavoriteState())
  {
    m_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_tree_view->resizeColumnToContents(0);
  }
  static const bool adjust_columns_once = FixColumnAppearance(m_tree_view);
}

void InstructionEditorWidget::SetProcedureIntern(ProcedureItem *procedure)
{
  if (procedure)
  {
    m_component_provider->SetItem(procedure->GetInstructionContainer());
    AdjustTreeAppearance();
  }
  else
  {
    m_component_provider->SetItem(nullptr);
  }
}

void InstructionEditorWidget::SetupConnections()
{
  auto on_selected_instruction_changed = [this](auto)
  {
    auto selected = GetSelectedInstruction();
    m_property_tree->SetItem(selected);
    emit InstructionSelected(selected);
  };
  connect(m_component_provider.get(), &::mvvm::ItemViewComponentProvider::SelectedItemChanged, this,
          on_selected_instruction_changed);

  // propagate instruction related requests from InstructionTreeWidget to InstructionEditorActions
  connect(m_editor_actions, &InstructionEditorActions::InsertAfterRequest, m_action_handler.get(),
          &InstructionEditorActionHandler::OnInsertInstructionAfterRequest);
  connect(m_editor_actions, &InstructionEditorActions::InsertIntoRequest, m_action_handler.get(),
          &InstructionEditorActionHandler::OnInsertInstructionIntoRequest);
  connect(m_editor_actions, &InstructionEditorActions::RemoveSelectedRequest,
          m_action_handler.get(), &InstructionEditorActionHandler::OnRemoveInstructionRequest);
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
