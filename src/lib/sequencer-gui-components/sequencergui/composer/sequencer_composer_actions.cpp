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

#include <sequencergui/components/message_helper.h>
#include <sequencergui/model/xml_utils.h>
#include <sequencergui/transform/domain_procedure_builder.h>
#include <sequencergui/transform/domain_workspace_builder.h>
#include <sup/gui/components/message_event.h>

#include <mvvm/utils/file_utils.h>

#include <sup/sequencer/procedure.h>

#include <QAction>
#include <QFileDialog>
#include <QSettings>
#include <QWidget>
#include <fstream>

namespace
{
const QString kGroupName("SequencerComposerActions");
const QString kCurrentWorkdirSettingName = kGroupName + "/" + "workdir";
}  // namespace

namespace sequencergui
{

SequencerComposerActions::SequencerComposerActions(QWidget *parent) : QObject(parent)
{
  SetupActions();
  ReadSettings();
}

SequencerComposerActions::~SequencerComposerActions()
{
  WriteSettings();
}

QList<QAction *> SequencerComposerActions::GetMenuActions()
{
  return {m_validate_procedure_action, m_export_xml_action};
}

void SequencerComposerActions::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure_item = procedure_item;
}

void SequencerComposerActions::ReadSettings()
{
  const QSettings settings;
  m_current_workdir = settings.value(kCurrentWorkdirSettingName, QDir::homePath()).toString();
}

void SequencerComposerActions::WriteSettings()
{
  QSettings settings;
  settings.setValue(kCurrentWorkdirSettingName, m_current_workdir);
}

void SequencerComposerActions::SetupActions()
{
  m_validate_procedure_action = new QAction("Validate procedure", this);
  m_validate_procedure_action->setToolTip("Performs validation of currently selected procedure.");
  connect(m_validate_procedure_action, &QAction::triggered, this,
          &SequencerComposerActions::OnValidateProcedureRequest);

  m_export_xml_action = new QAction("Export to XML", this);
  m_export_xml_action->setToolTip("Exports currently selected procedure to Sequencer XML file");
  connect(m_export_xml_action, &QAction::triggered, this,
          &SequencerComposerActions::OnExportToXmlRequest);
}

void SequencerComposerActions::OnValidateProcedureRequest()
{
  if (!m_procedure_item)
  {
    SendWarningMessage({"Validate Procedure", "No procedure selected"});
    return;
  }

  std::unique_ptr<procedure_t> domain_procedure;
  try
  {
    DomainProcedureBuilder builder;
    domain_procedure = std::move(builder.CreateProcedure(m_procedure_item));
    domain_procedure->Setup();
  }
  catch (std::exception &ex)
  {
    SendWarningMessage({"Validate Procedure", "Validation of procedure failed", ex.what()});
    return;
  }

  if (domain_procedure->RootInstruction() == nullptr && domain_procedure->GetInstructionCount() > 1)
  {
    SendWarningMessage({"Validate Procedure", "Validation of procedure failed",
                        "None of existing top-level instructions is marked as root instruction"});
    return;
  }

  SendInfoMessage({"Validate Procedure", "Sequencer procedure is valid",
                   "Domain procedure setup has been completed successfully"});
}

//! Generates XML content from currently selected procedure and save it to file.
//! FIXME there is a code duplication with XMLEditor::SetupActions in the part
//! related to the file saving.

void SequencerComposerActions::OnExportToXmlRequest()
{
  if (!m_procedure_item)
  {
    SendWarningMessage({"Validate Procedure", "No procedure selected"});
    return;
  }

  std::string xml_content;
  try
  {
    xml_content = ExportToXMLString(m_procedure_item);
  }
  catch (const std::exception &ex)
  {
    SendWarningMessage(
        {"Export to XML", "Procedure is in unconsistent state, can't generate XML", ex.what()});
  }

  if (xml_content.empty())
  {
    return;
  }

  auto file_name = QFileDialog::getSaveFileName(
      nullptr, "Save File", m_current_workdir + "/untitled.xml", tr("Files (*.xml *.XML)"));

  if (!file_name.isEmpty())
  {
    auto parent_path = mvvm::utils::GetParentPath(file_name.toStdString());
    m_current_workdir = QString::fromStdString(parent_path);
    std::ofstream file_out(file_name.toStdString());
    file_out << xml_content;
    file_out.close();
  }
}

}  // namespace sequencergui
