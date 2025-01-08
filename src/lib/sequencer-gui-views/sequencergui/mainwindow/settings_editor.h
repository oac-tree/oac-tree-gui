/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SEQUENCERGUI_MAINWINDOW_SETTINGS_EDITOR_H_
#define SEQUENCERGUI_MAINWINDOW_SETTINGS_EDITOR_H_

#include <QWidget>
#include <memory>

class QSplitter;
class QListView;

namespace mvvm
{
class ViewModel;
class ItemViewComponentProvider;
class SessionItem;
class PropertyFlatView;
class PropertyViewModel;
}  // namespace mvvm

namespace sequencergui
{

class SettingsModel;

/**
 * @brief The SettingsEditor class represents a simple editor of application persistent settings.
 *
 * It consists of the list with group setting items on the left, and a property editor for
 * the currently selected group on the right.
 */
class SettingsEditor : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsEditor(QWidget* parent_widget = nullptr);
  ~SettingsEditor() override;

  /**
   * @brief Sets initial values for editing.
   */
  void SetInitialValues(const SettingsModel& model);

  /**
   * @brief Sets settings group item to show in property editor.
   */
  void SetSettingsGroup(mvvm::SessionItem* item);

  /**
   * @brief Returns the model representing result of editing.
   */
  const SettingsModel* GetResult() const;

private:
  QSplitter* m_splitter{nullptr};
  QListView* m_list_view{nullptr};
  mvvm::PropertyFlatView* m_settings_view{nullptr};

  std::unique_ptr<SettingsModel> m_settings_model;

  std::unique_ptr<mvvm::ItemViewComponentProvider> m_list_component_provider;
  std::unique_ptr<mvvm::PropertyViewModel> m_property_view_model;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SETTINGS_EDITOR_H_
