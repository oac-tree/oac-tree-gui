/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "procedure_action_handler.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/xml_utils.h>
#include <oac_tree_gui/transform/domain_procedure_builder.h>
#include <oac_tree_gui/transform/domain_workspace_builder.h>

#include <sup/gui/core/message_event.h>
#include <sup/gui/widgets/message_helper.h>

#include <mvvm/utils/file_utils.h>

#include <sup/oac-tree/procedure.h>

#include <QFileDialog>
#include <QSettings>
#include <QWidget>
#include <fstream>

namespace
{
const QString kGroupName("ProcedureActionHandler");
const QString kCurrentWorkdirSettingName = kGroupName + "/" + "workdir";
}  // namespace

namespace oac_tree_gui
{

ProcedureActionHandler::ProcedureActionHandler(QWidget* parent_widget) : QObject(parent_widget)
{
  ReadSettings();
}

ProcedureActionHandler::~ProcedureActionHandler()
{
  WriteSettings();
}

//! Generates XML content from currently selected procedure and save it to file.

void ProcedureActionHandler::OnExportToXmlRequest(ProcedureItem* procedure_item)
{
  if (procedure_item == nullptr)
  {
    sup::gui::SendWarningMessage({"Validate Procedure", "No procedure selected", "", ""});
    return;
  }

  std::string xml_content;
  try
  {
    xml_content = ExportToXMLString(*procedure_item);
  }
  catch (const std::exception& ex)
  {
    sup::gui::SendWarningMessage(
        {"Export to XML", "Procedure is in unconsistent state, can't generate XML", ex.what(), ""});
  }

  if (xml_content.empty())
  {
    return;
  }

  auto file_name = QFileDialog::getSaveFileName(
      nullptr, "Save File", m_current_workdir + "/untitled.xml", tr("Files (*.xml *.XML)"));

  if (!file_name.isEmpty())
  {
    UpdateCurrentWorkdir(file_name);
    std::ofstream file_out(file_name.toStdString());
    file_out << xml_content;
    file_out.close();
  }
}

void ProcedureActionHandler::OnValidateProcedureRequest(ProcedureItem* procedure_item)
{
  if (procedure_item == nullptr)
  {
    sup::gui::SendWarningMessage({"Validate Procedure", "No procedure selected", "", ""});
    return;
  }

  std::unique_ptr<procedure_t> domain_procedure;
  try
  {
    domain_procedure = CreateDomainProcedure(*procedure_item);
    domain_procedure->Setup();
  }
  catch (std::exception& ex)
  {
    sup::gui::SendWarningMessage(
        {"Validate Procedure", "Validation of procedure failed", ex.what(), ""});
    return;
  }

  if (domain_procedure->RootInstruction() == nullptr && domain_procedure->GetInstructionCount() > 1)
  {
    sup::gui::SendWarningMessage(
        {"Validate Procedure", "Validation of procedure failed",
         "None of existing top-level instructions is marked as root instruction", ""});
    return;
  }

  sup::gui::SendInfoMessage({"Validate Procedure", "Sequencer procedure is valid",
                             "Domain procedure setup has been completed successfully", ""});
}

std::unique_ptr<ProcedureItem> ProcedureActionHandler::LoadProcedureFromFile(QString file_name)
{
  if (file_name.isEmpty())
  {
    file_name = QFileDialog::getOpenFileName(nullptr, "Open file", m_current_workdir,
                                             tr("Files (*.xml *.XML)"));
  }

  if (!file_name.isEmpty())
  {
    UpdateCurrentWorkdir(file_name);
    return LoadProcedureFromFileIntern(file_name);
  }

  return {};
}

std::unique_ptr<ProcedureItem> ProcedureActionHandler::LoadProcedureFromFileIntern(
    const QString& file_name)
{
  std::unique_ptr<ProcedureItem> result;

  try
  {
    auto procedure_name = mvvm::utils::GetPathStem(file_name.toStdString());
    result = oac_tree_gui::ImportFromFile(file_name.toStdString());
    result->SetDisplayName(procedure_name);
  }
  catch (const std::exception& ex)
  {
    sup::gui::SendWarningMessage(
        {"Import from file", "Procedure import has failed", ex.what(), ""});
  }

  return result;
}

void ProcedureActionHandler::ReadSettings()
{
  const QSettings settings;
  m_current_workdir = settings.value(kCurrentWorkdirSettingName, QDir::homePath()).toString();
}

void ProcedureActionHandler::WriteSettings()
{
  QSettings settings;
  settings.setValue(kCurrentWorkdirSettingName, m_current_workdir);
}

void ProcedureActionHandler::UpdateCurrentWorkdir(const QString& file_name)
{
  auto parent_path = mvvm::utils::GetParentPath(file_name.toStdString());
  m_current_workdir = QString::fromStdString(parent_path);
}

}  // namespace oac_tree_gui
