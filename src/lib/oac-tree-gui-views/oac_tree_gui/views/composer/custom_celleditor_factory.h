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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_CUSTOM_CELLEDITOR_FACTORY_H
#define OAC_TREE_GUI_VIEWS_COMPOSER_CUSTOM_CELLEDITOR_FACTORY_H

#include <mvvm/editors/default_editor_factory.h>

#include <functional>
#include <string>
#include <vector>

#include <QStringList>

namespace mvvm
{
class SessionItem;
}

namespace oac_tree_gui
{

/**
 * @brief The CustomCellEditorFactory creates sequencer specific cell editors.
 *
 * Intended to create cell editors for InstructionItem attributes with variable name auto-complete
 * features.
 */
class CustomCellEditorFactory : public mvvm::DefaultEditorFactory
{
public:
  explicit CustomCellEditorFactory(const std::function<QStringList()>& string_list_func);

  mvvm::editor_t CreateEditor(const QModelIndex& index) const override;

private:
  std::function<QStringList()> m_string_list_func;
};

/**
 * @brief Checks if the given display name repersents a property carrying a variable name.
 */
bool IsVariableNameRelatedDisplayName(const std::string& name);

/**
 * @brief Checks if the given item repersents a property carrying a variable name.
 */
bool IsVariableNameRelatedProperty(const mvvm::SessionItem& item);

/**
 * @brief Checks if the given index is related to the variable name property.
 */
bool IsVariableNameRelatedCell(const QModelIndex& index);


}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_CUSTOM_CELLEDITOR_FACTORY_H
