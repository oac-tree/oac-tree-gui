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

#include "custom_celleditor_factory.h"

#include <oac_tree_gui/domain/domain_constants.h>

#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QWidget>

namespace oac_tree_gui
{

CustomCellEditorFactory::CustomCellEditorFactory(
    const std::function<QStringList()> &string_list_func)
    : m_string_list_func(string_list_func)
{
}

mvvm::editor_t CustomCellEditorFactory::CreateEditor(const QModelIndex &index) const
{
  if (IsVariableNameRelatedCell(index))
  {
    return std::make_unique<mvvm::StringCompleterComboEditor>(m_string_list_func);
  }

  return mvvm::DefaultEditorFactory::CreateEditor(index);
}

bool IsVariableNameRelatedDisplayName(const std::string &name)
{
  static const std::vector<std::string> kVariableNameRelated = {
      domainconstants::kEqualVariableAttributeName,
      domainconstants::kFromVariableAttributeName,
      domainconstants::kGenericVariableNameAttribute,
      domainconstants::kInputVariableNameAttribute,
      domainconstants::kLeftVariableNameAttribute,
      domainconstants::kOutputVariableNameAttribute,
      domainconstants::kRequestAttribute,
      domainconstants::kRightVariableNameAttribute
  };
  return mvvm::utils::Contains(kVariableNameRelated, name);
}

bool IsVariableNameRelatedProperty(const mvvm::SessionItem &item)
{
  return IsVariableNameRelatedDisplayName(item.GetDisplayName());
}

bool IsVariableNameRelatedCell(const QModelIndex &index)
{
  // NOTE: in the case of PropertyViewModel, current implementation will report both cells
  // (with col=0 and col=1) as related to variable property
  auto item = mvvm::utils::ItemFromProxyIndex(index);
  return item ? IsVariableNameRelatedProperty(*item) : false;
}

}  // namespace oac_tree_gui
