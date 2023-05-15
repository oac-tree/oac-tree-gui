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

#include "sequencer_composer_actions.h"

#include <sequencergui/transform/domain_procedure_builder.h>
#include <sequencergui/transform/domain_workspace_builder.h>
#include <sup/gui/components/message_event.h>

#include <sup/sequencer/procedure.h>

#include <QAction>
#include <QMessageBox>
#include <QWidget>

namespace
{
void SendMessage(const sup::gui::MessageEvent &event, QMessageBox::Icon icon_type)
{
  QMessageBox msg_box;
  msg_box.setWindowTitle(QString::fromStdString(event.title));
  msg_box.setText(QString::fromStdString(event.text));
  msg_box.setInformativeText(QString::fromStdString(event.informative));
  msg_box.setDetailedText(QString::fromStdString(event.detailed));
  msg_box.setIcon(icon_type);
  msg_box.exec();
}

}  // namespace

namespace sequencergui
{

SequencerComposerActions::SequencerComposerActions(QWidget *parent) : QObject(parent)
{
  SetupActions();
}

QList<QAction *> SequencerComposerActions::GetMenuActions()
{
  return {m_validate_procedure_action, m_export_xml_action};
}

void SequencerComposerActions::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure_item = procedure_item;
}

void SequencerComposerActions::SendInfoMessage(const sup::gui::MessageEvent &event)
{
  SendMessage(event, QMessageBox::Information);
}

void SequencerComposerActions::SendWarningMessage(const sup::gui::MessageEvent &event)
{
  SendMessage(event, QMessageBox::Warning);
}

void SequencerComposerActions::SetupActions()
{
  m_validate_procedure_action = new QAction("Validate procedure", this);
  m_validate_procedure_action->setToolTip("Performs validation of currently selected procedure.\n");
  connect(m_validate_procedure_action, &QAction::triggered, this,
          &SequencerComposerActions::OnValidateProcedureRequest);

  m_export_xml_action = new QAction("Export to XML", this);
  m_export_xml_action->setToolTip("Exports currently selected procedure to Sequencer XML file");
}

void SequencerComposerActions::OnValidateProcedureRequest()
{
  if (!m_procedure_item)
  {
    SendWarningMessage({"Validate Procedure", "No procedure selected"});
    return;
  }

  try
  {
    DomainProcedureBuilder builder;
    auto domain_procedure = builder.CreateProcedure(m_procedure_item);
    domain_procedure->Setup();
    SendInfoMessage(
        {"Validate Procedure", "Domain procedure setup has been completed successfully"});
  }
  catch (std::exception &ex)
  {
    SendWarningMessage({"Validate Procedure", "Validation of procedure failed", ex.what()});
    return;
  }
}

}  // namespace sequencergui
