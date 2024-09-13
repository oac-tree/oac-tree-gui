/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencer_composer_actions.h"

#include <sequencergui/components/component_helper.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/views/operation/procedure_action_handler.h>
#include <sup/gui/app/app_action_helper.h>
#include <sup/gui/app/app_constants.h>

#include <mvvm/commands/i_command_stack.h>

#include <QAction>
#include <QWidget>

namespace sequencergui
{

SequencerComposerActions::SequencerComposerActions(QWidget *parent) : QObject(parent)
{
  SetupActions();
}

void SequencerComposerActions::SetModel(SequencerModel *model)
{
  m_model = model;
}

SequencerComposerActions::~SequencerComposerActions() = default;

void SequencerComposerActions::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure_item = procedure_item;
}

void SequencerComposerActions::RegisterActionsForContext(const sup::gui::AppContext &context)
{
  sup::gui::AppAddActionToCommand(m_validate_procedure_action,
                                  constants::kValidateProcedureCommandId, context);

  sup::gui::AppAddActionToCommand(m_export_xml_action, constants::kExportXmlCommandId,
                                  context);

  sup::gui::AppAddActionToCommand(m_undo_action, sup::gui::constants::kUndoCommandId, context);
  sup::gui::AppAddActionToCommand(m_redo_action, sup::gui::constants::kRedoCommandId, context);
}

void SequencerComposerActions::SetupActions()
{
  m_validate_procedure_action = new QAction("Validate procedure", this);
  m_validate_procedure_action->setToolTip("Performs validation of currently selected procedure.");

  connect(m_validate_procedure_action, &QAction::triggered, this,
          [this]() { ProcedureActionHandler::OnValidateProcedureRequest(m_procedure_item); });

  m_export_xml_action = new QAction("Save procedure to XML", this);
  m_export_xml_action->setToolTip("Save currently selected procedure to Sequencer XML file");
  auto on_export = [this]()
  {
    ProcedureActionHandler handler;
    handler.OnExportToXmlRequest(m_procedure_item);
  };
  connect(m_export_xml_action, &QAction::triggered, this, on_export);

  m_undo_action = new QAction("Undo", this);
  auto on_undo = [this]()
  {
    if (m_model && m_model->GetCommandStack())
    {
      m_model->GetCommandStack()->Undo();
    }
  };
  connect(m_undo_action, &QAction::triggered, this, on_undo);

  m_redo_action = new QAction("Redo", this);
  auto on_redo = [this]()
  {
    if (m_model && m_model->GetCommandStack())
    {
      m_model->GetCommandStack()->Redo();
    }
  };

  connect(m_redo_action, &QAction::triggered, this, on_redo);
}

}  // namespace sequencergui
