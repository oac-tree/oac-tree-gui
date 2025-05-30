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

#ifndef OAC_TREE_GUI_COMPOSER_PROCEDURE_LIST_CONTEXT_H_
#define OAC_TREE_GUI_COMPOSER_PROCEDURE_LIST_CONTEXT_H_

#include <functional>
#include <memory>

class QMimeData;

namespace mvvm
{
class ContainerItem;
}

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The ProcedureListContext struct holds collection of callbacks to retrieve current state
 * of ProcedureListWidget.
 *
 * It is used by ProcedureListActionHandler to handle context dependent actions.
 */
struct ProcedureListContext
{
  //! callback to get current procedure container
  std::function<mvvm::ContainerItem*()> procedure_container;

  //! callback to get currently selected procedure
  std::function<ProcedureItem*()> selected_procedure;

  //! callback to get mime data from the clipboard
  std::function<const QMimeData*()> get_mime_data;

  //! callback to set mime data to the clipboard
  std::function<void(std::unique_ptr<QMimeData>)> set_mime_data;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_PROCEDURE_LIST_CONTEXT_H_
