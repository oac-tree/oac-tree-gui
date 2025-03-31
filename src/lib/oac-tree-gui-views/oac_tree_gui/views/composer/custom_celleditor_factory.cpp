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

#include "custom_celleditor_factory.h"

#include <oac_tree_gui/domain/domain_constants.h>

namespace oac_tree_gui
{

CustomCellEditorFactory::CustomCellEditorFactory(const string_list_func_t &string_list_func)
    : m_string_list_func(string_list_func)
{
}

}  // namespace oac_tree_gui
