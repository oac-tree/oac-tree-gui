/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#ifndef SUP_GUI_MODEL_SETTINGS_MODEL_H_
#define SUP_GUI_MODEL_SETTINGS_MODEL_H_

#include <sup/gui/model/settings_constants.h>
#include <sup/gui/model/settings_item.h>

#include <mvvm/model/application_model.h>

namespace oac_tree_gui
{

/**
 * @brief Contains common application settings.
 *
 * It provides a collection of editable setting items, which is written in persistent storage thanks
 * to QSettings machinery.
 */
class SequencerSettingsModel : public mvvm::ApplicationModel
{
public:
  SequencerSettingsModel();

  std::unique_ptr<ISessionModel> Clone() const override;

  /**
   * @details Clears the model and populates it with the default settings.
   */
  void Clear() override;

  std::vector<mvvm::CompoundItem*> GetSettingsItems() const;

  /**
   * @brief Returns data inside property item registered under the given setting name.
   */
  template <typename T = mvvm::variant_t>
  T Data(const std::string& setting_name) const;

private:
  void PopulateModel();
};

template <typename T>
inline T SequencerSettingsModel::Data(const std::string& setting_name) const
{
  // for the moment we assume that there is only one settings item, might change in the future
  return GetSettingsItems().at(0)->Property<T>(setting_name);
}

}  // namespace oac_tree_gui

#endif  // SUP_GUI_MODEL_SETTINGS_MODEL_H_
