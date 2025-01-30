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

#ifndef OAC_TREE_GUI_VIEWMODEL_CUSTOM_ROW_STRATEGIES_H_
#define OAC_TREE_GUI_VIEWMODEL_CUSTOM_ROW_STRATEGIES_H_

//! @file
//! Various strategies to construct row of properties representing instructions and variables.

#include <mvvm/viewmodel/abstract_row_strategy.h>

namespace oac_tree_gui
{

/**
 * @brief The VariableRowStrategy generates the row of three elements representing a Variable.
 *
 * For VariableItem itself it will generate a row with [editable_display_name, empty space,
 * typeName label]. For everything beneath it it will generate [property_name, value, typeName
 * label].
 */
class VariableRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  int GetSize() const override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override;
};

/**
 * @brief The VariableTableRowStrategy generates the row of 5 elements representing a variable.
 *
 * The row contains "| Name | Value | Type | Channel | Connected |".
 */
class VariableTableRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  int GetSize() const override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override;
};

/**
 * @brief The InstructionEditorRowStrategy generates the row of 2 elements representing instruction
 * for instruction editor.
 *
 * The row contains instruction type, and editable name.
 */
class InstructionEditorRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  int GetSize() const override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_CUSTOM_ROW_STRATEGIES_H_
