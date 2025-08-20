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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPOSER_OBJECTS_PROCEDURE_LIST_ACTION_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_OBJECTS_PROCEDURE_LIST_ACTION_HANDLER_H_

#include <oac_tree_gui/composer/procedure_list_context.h>

#include <QObject>

class QMimeData;

namespace mvvm
{
class ContainerItem;
class ISessionModel;
class SessionItem;
}  // namespace mvvm

namespace oac_tree_gui
{

class ProcedureListActionHandler : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureListActionHandler(ProcedureListContext context,
                                      QObject* parent_object = nullptr);

  /**
   * @brief Creates empty procedure after the current selection.
   */
  void OnCreateNewProcedureRequest();

  /**
   * @brief Checks if removal is possible.
   */
  bool CanRemove() const;

  /**
   * @brief Removes currently selected procedure.
   */
  void OnRemoveProcedureRequest();

  /**
   * @brief Checks if cut operation is possible.
   */
  bool CanCut() const;

  /**
   * @brief Cut selected procedure.
   */
  void Cut();

  /**
   * @brief Checks if copy operation is possible.
   */
  bool CanCopy() const;

  /**
   * @brief Copy selected procedure.
   */
  void Copy();

  /**
   * @brief Checks if paste operation is possible.
   */
  bool CanPaste() const;

  /**
   * @brief Paste procedure.
   */
  void Paste();

signals:
  void SelectProcedureRequest(oac_tree_gui::ProcedureItem* item);

private:
  mvvm::ContainerItem* GetProcedureContainer() const;
  ProcedureItem* GetSelectedProcedure() const;
  mvvm::ISessionModel* GetModel();
  const QMimeData* GetMimeData() const;

  mvvm::SessionItem* InsertProcedure(std::unique_ptr<mvvm::SessionItem> item);

  ProcedureListContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_OBJECTS_PROCEDURE_LIST_ACTION_HANDLER_H_
