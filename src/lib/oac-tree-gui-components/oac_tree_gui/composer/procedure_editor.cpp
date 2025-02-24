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

#include "procedure_editor.h"

#include <oac_tree_gui/composer/instruction_editor_action_handler.h>
#include <oac_tree_gui/model/aggregate_factory.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>

#include <QDebug>

namespace oac_tree_gui
{

ProcedureEditor::ProcedureEditor(QObject *parent_object)
    : QObject(parent_object)
    , m_action_handler(
          std::make_unique<InstructionEditorActionHandler>(CreateInstructionEditorContext()))
{
}

ProcedureEditor::~ProcedureEditor() = default;

void ProcedureEditor::SetProcedure(ProcedureItem *procedure_item)
{
  m_current_procedure = procedure_item;
}

void ProcedureEditor::SetInstructionsInFocus(const std::vector<InstructionItem *> &instructions)
{
  qDebug() << QList<InstructionItem *>(instructions.begin(), instructions.end());
  m_selected_instructions = instructions;
}

std::vector<InstructionItem *> ProcedureEditor::GetInstructionInFocus()
{
  return m_selected_instructions;
}

void ProcedureEditor::InsertInstructionFromToolBox(const QString &name)
{
  m_action_handler->InsertInstructionAfter(name.toStdString());
}

InstructionEditorContext ProcedureEditor::CreateInstructionEditorContext()
{
  InstructionEditorContext result;
  result.instruction_container = [this]()
  { return m_current_procedure ? m_current_procedure->GetInstructionContainer() : nullptr; };
  result.selected_instruction = [this]() { return GetSelectedInstruction(); };

  auto on_select_request = [this](mvvm::SessionItem *item) { emit SelectInstructionRequest(item); };
  result.select_notify = on_select_request;

  result.create_instruction = [](const std::string &name) { return CreateInstructionTree(name); };

  result.send_message = [](const auto &event)
  {
    (void)event;
    qDebug() << "unexpected event";
  };

  return result;
}

InstructionItem *ProcedureEditor::GetSelectedInstruction()
{
  return m_selected_instructions.empty() ? nullptr : m_selected_instructions.front();
}

}  // namespace oac_tree_gui
