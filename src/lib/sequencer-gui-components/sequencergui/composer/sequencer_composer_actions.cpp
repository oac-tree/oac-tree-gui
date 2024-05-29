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

#include <sequencergui/mainwindow/app_constants.h>
#include <sequencergui/operation/procedure_action_handler.h>
#include <sup/gui/app/app_action_helper.h>

#include <QAction>
#include <QWidget>

namespace sequencergui
{

SequencerComposerActions::SequencerComposerActions(QWidget *parent) : QObject(parent)
{
  SetupActions();
}

SequencerComposerActions::~SequencerComposerActions() = default;

QList<QAction *> SequencerComposerActions::GetMenuActions()
{
  return {m_validate_procedure_action, m_export_xml_action};
}

void SequencerComposerActions::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure_item = procedure_item;
}

void SequencerComposerActions::SetupActions()
{
  m_validate_procedure_action = new QAction("Validate procedure", this);
  m_validate_procedure_action->setToolTip("Performs validation of currently selected procedure.");

  connect(m_validate_procedure_action, &QAction::triggered, this,
          [this]() { ProcedureActionHandler::OnValidateProcedureRequest(m_procedure_item); });

  m_export_xml_action = new QAction("Export to XML", this);
  m_export_xml_action->setToolTip("Exports currently selected procedure to Sequencer XML file");
  auto on_export = [this]()
  {
    ProcedureActionHandler handler;
    handler.OnExportToXmlRequest(m_procedure_item);
  };
  connect(m_export_xml_action, &QAction::triggered, this, on_export);

  sup::gui::AppAddActionToCommand(m_validate_procedure_action,
                                  app::constants::kValidateProcedureCommandId,
                                  app::constants::kComposerContext);

  sup::gui::AppAddActionToCommand(m_export_xml_action, app::constants::kExportXmlCommandId,
                                  app::constants::kComposerContext);
}

}  // namespace sequencergui
