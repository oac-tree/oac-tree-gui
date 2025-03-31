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

namespace oac_tree_gui
{

/**
 * @brief The CustomCellEditorFactory class is a custom factory to create sequencer specific cell
 * editors.
 *
 * Intended to create cell editors for InstructionItem attributes with variable name auto-complete
 * features.
 */
class CustomCellEditorFactory : public mvvm::DefaultEditorFactory
{
public:
  using string_list_func_t = std::function<std::vector<std::string>()>;

  explicit CustomCellEditorFactory(const string_list_func_t& string_list_func);

private:
  string_list_func_t m_string_list_func;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_CUSTOM_CELLEDITOR_FACTORY_H
