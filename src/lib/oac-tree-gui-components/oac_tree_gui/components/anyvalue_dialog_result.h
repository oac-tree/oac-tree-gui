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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPONENTS_ANYVALUE_DIALOG_RESULT_H_
#define OAC_TREE_GUI_COMPONENTS_ANYVALUE_DIALOG_RESULT_H_

#include <memory>

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace oac_tree_gui
{

/**
 * @brief The AnyValueDialogResult struct is intended to return AnyValueItem from the dialog.
 */
struct AnyValueDialogResult
{
  //!< Flag showing if operation was finished (OK button), or dialog was canceled. Canceled dialog
  //!< means that the user whants to leave previous AnyValueItem intact.
  bool is_accepted;
  //!< result of editing; empty object means that the user wants to remove previous item
  std::unique_ptr<sup::gui::AnyValueItem> result;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_ANYVALUE_DIALOG_RESULT_H_
