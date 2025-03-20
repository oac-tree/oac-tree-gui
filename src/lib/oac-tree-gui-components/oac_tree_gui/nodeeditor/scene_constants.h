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

#ifndef OAC_TREE_GUI_NODEEDITOR_SCENE_CONSTANTS_H_
#define OAC_TREE_GUI_NODEEDITOR_SCENE_CONSTANTS_H_

//! @file
//! Collection of constants for graphics scene.

#include <vector>

namespace oac_tree_gui::constants
{

const double kWheelDefaultZoomInFactor = 1.075;
const double kMinZoomFactor = 0.1;
const double kMaxZoomFactor = 2.0;

const std::vector<double> kZoomStops = {constants::kMinZoomFactor, 0.25, 0.5, 0.75, 1.0, 1.5,
                                        constants::kMaxZoomFactor};

}  // namespace oac_tree_gui::constants

#endif  // OAC_TREE_GUI_NODEEDITOR_SCENE_CONSTANTS_H_
