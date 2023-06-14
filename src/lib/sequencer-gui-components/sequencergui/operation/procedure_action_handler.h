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

#ifndef SEQUENCERGUI_OPERATION_PROCEDURE_ACTION_HANDLER_H_
#define SEQUENCERGUI_OPERATION_PROCEDURE_ACTION_HANDLER_H_

#include <QObject>
#include <memory>

class QWidget;

namespace sequencergui
{

class ProcedureItem;

//! Implements following actions related to ProcedureItem: import from XML, export to XML,
//! validation for domain.

/**
 * @brief The ProcedureActionHandler class implements following actions: import from XML, export to
 * XML, validation of ProcedureItem.
 *
 * @details All errors are reported to the user via modal dialogs.
 */

class ProcedureActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureActionHandler(QWidget* parent = nullptr);
  ~ProcedureActionHandler() override;

  void OnExportToXmlRequest(ProcedureItem* procedure_item);

  static void OnValidateProcedureRequest(ProcedureItem* procedure_item);

  /**
   * @brief Loads procedure from Sequencer XML file.
   *
   * @param file_name The name of XML file.
   *
   * @return Valid procedure in the case of success, empty ptr otherwise.
   */
  static std::unique_ptr<sequencergui::ProcedureItem> LoadProcedureFromFile(
      const QString& file_name);

  /**
   * @brief Loads procedure from Sequencer XML file using open file dialog.
   *
   * @return Valid procedure in the case of success, empty ptr otherwise.
   */
  std::unique_ptr<sequencergui::ProcedureItem> LoadProcedureFromFile();

private:
  void ReadSettings();
  void WriteSettings();
  void UpdateCurrentWorkdir(const QString& file_name);

  QString m_current_workdir;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_PROCEDURE_ACTION_HANDLER_H_
