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

#ifndef SEQUENCERGUI_COMPONENTS_LOAD_RESOURCES_H_
#define SEQUENCERGUI_COMPONENTS_LOAD_RESOURCES_H_

//! @file
//! Main helper function to load resources

#include <oac-tree-gui/components/custom_meta_types.h>

namespace oac_tree_gui
{

/**
 * @brief Loads plugins, item factory and Qt metatypes.
 */
void LoadResources();

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_COMPONENTS_CUSTOM_META_TYPES_H_
