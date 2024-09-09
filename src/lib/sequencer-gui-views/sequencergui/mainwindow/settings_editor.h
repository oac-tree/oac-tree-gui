/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
 * @brief The SettingsEditor class represents simple editor of application persistent settings.
 */
class SettingsEditor : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsEditor(QWidget* parent = nullptr);
  ~SettingsEditor() override;


  void SetSettingsItem(mvvm::SessionItem* item);

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
