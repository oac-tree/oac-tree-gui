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

#ifndef SEQUENCERGUI_VIEWMODEL_STANDARD_ROW_STRATEGIES_H
#define SEQUENCERGUI_VIEWMODEL_STANDARD_ROW_STRATEGIES_H

//! @file
//! Various strategies to construct row of properties representing instructions and variables.

#include <mvvm/interfaces/row_strategy_interface.h>

namespace sequencergui
{

/**
 * @brief The VariableRowStrategy generates the row of two elements representing a Variable.
 *
 * @details For VariableItem itself it will generate a row with [display_name, editable_name].
 * For everything beneath it it will return usual [property_name, value] pair.
 */

class VariableRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override;

  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRow(mvvm::SessionItem *item) override;
};

/**
 * @brief The VariableTableRowStrategy generates the row of 5 elements representing a variable.
 *
 * @details The row will containe "| Name | Value | Type | Channel | Connected |".
 */

class VariableTableRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override;

  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRow(mvvm::SessionItem *item) override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_STANDARD_ROW_STRATEGIES_H
