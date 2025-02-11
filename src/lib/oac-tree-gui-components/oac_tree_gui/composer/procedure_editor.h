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

#ifndef OAC_TREE_GUI_COMPOSER_PROCEDURE_EDITOR_H_
#define OAC_TREE_GUI_COMPOSER_PROCEDURE_EDITOR_H_

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The ProcedureEditor class is an envelope around all action handlers participating in
 * procedure editing.
 *
 * Contains all logic to add/remove instructions and variables, provide copy and paste, undo-redo,
 * etc.
 */
class ProcedureEditor
{
public:

  void SetProcedure(ProcedureItem* procedure_item);

private:
  ProcedureItem* m_current_procedure{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_PROCEDURE_EDITOR_H_
