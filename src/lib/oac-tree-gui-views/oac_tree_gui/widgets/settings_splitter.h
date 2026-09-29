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

#ifndef OAC_TREE_GUI_WIDGETS_SETTINGS_SPLITTER_H_
#define OAC_TREE_GUI_WIDGETS_SETTINGS_SPLITTER_H_

#include <oac_tree_gui/settings/scoped_callback.h>

#include <QByteArray>
#include <QSplitter>
#include <cstdint>
#include <string>
#include <vector>

namespace oac_tree_gui
{

class ISettingsManager;

/**
 * @brief The SplitterInfo struct holds the persistent state of a splitter.
 */
struct SplitterInfo
{
  QByteArray state;                      //!< result of QSplitter::saveState()
  std::vector<std::int32_t> visibility;  //!< visibility flag of each child widget
};

template <typename Archive>
void Visit(Archive& archive, SplitterInfo& info)
{
  archive("state", info.state);
  archive("visibility", info.visibility);
}

/**
 * @brief The SettingsSplitter class is a splitter storing its state in application settings.
 *
 * The state is provided to the settings manager on Collect. It is restored on RestoreState().
 * The settings manager must outlive the splitter.
 */
class SettingsSplitter : public QSplitter
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * @param group The settings group, unique for each splitter instance.
   * @param settings_manager The manager to store settings.
   * @param parent_widget The parent widget.
   */
  SettingsSplitter(std::string group, ISettingsManager& settings_manager,
                   QWidget* parent_widget = nullptr);

  /**
   * @brief Returns the current state of the splitter.
   */
  SplitterInfo GetSplitterInfo() const;

  /**
   * @brief Restores the state from settings. Call after all child widgets are added.
   */
  void RestoreState();

private:
  std::string m_group;
  ISettingsManager& m_settings_manager;
  ScopedCallback m_provider_connection;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_WIDGETS_SETTINGS_SPLITTER_H_
