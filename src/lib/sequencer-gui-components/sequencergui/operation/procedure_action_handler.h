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

class ProcedureActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureActionHandler(QWidget* parent = nullptr);
  ~ProcedureActionHandler() override;

  void SetProcedure(ProcedureItem* procedure_item);

  void OnImportFromXmlRequest();

  void OnExportToXmlRequest();

  void OnValidateProcedureRequest();

private:
  void ReadSettings();
  void WriteSettings();

  ProcedureItem* m_procedure_item{nullptr};
  QString m_current_workdir;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_PROCEDURE_ACTION_HANDLER_H_
