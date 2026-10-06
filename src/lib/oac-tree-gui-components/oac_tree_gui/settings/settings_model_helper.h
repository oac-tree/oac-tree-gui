/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_SETTINGS_SETTINGS_MODEL_HELPER_H_
#define OAC_TREE_GUI_SETTINGS_SETTINGS_MODEL_HELPER_H_

#include <memory>

namespace oac_tree_gui
{

class ISettingsManager;
class SequencerSettingsModel;

/**
 * @brief Creates the settings model populated from the application scope of the settings manager.
 *
 * @param manager The settings manager to read from.
 * @return The settings model.
 * @throw RuntimeException if no application storage is set.
 */
std::unique_ptr<SequencerSettingsModel> CreateSettingsModel(const ISettingsManager& manager);

/**
 * @brief Writes settings from the model to the application scope of the settings manager.
 *
 * Subscribers of written groups are notified.
 *
 * @param model The settings model to read from.
 * @param manager The settings manager to write to.
 * @throw RuntimeException if no application storage is set.
 */
void WriteSettingsModel(const SequencerSettingsModel& model, ISettingsManager& manager);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_SETTINGS_SETTINGS_MODEL_HELPER_H_
