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

#ifndef SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_ACTION_HANDLER_H_
#define SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_ACTION_HANDLER_H_

#include <sequencergui/components/procedure_list_context.h>

#include <QObject>

namespace mvvm
{
class ContainerItem;
class SessionModelInterface;
}  // namespace mvvm

namespace sequencergui
{

class ProcedureListActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureListActionHandler(ProcedureListContext context, QObject* parent = nullptr);

  /**
   * @brief Creates empty procedure after the current selection.
   */
  void OnCreateNewProcedureRequest();

  /**
   * @brief Removes currently selected procedure.
   */
  void OnRemoveProcedureRequest();

signals:
  void SelectProcedureRequest(ProcedureItem* item);

private:
  mvvm::ContainerItem* GetProcedureContainer() const;
  ProcedureItem* GetSelectedProcedure() const;
  mvvm::SessionModelInterface* GetModel();

  ProcedureListContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_ACTION_HANDLER_H_
