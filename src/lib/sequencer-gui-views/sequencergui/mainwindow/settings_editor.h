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

class QSplitter;
class QListView;
class QTreeView;

namespace sequencergui
{

/**
 * @brief The SettingsEditor class represents simple editor of application persistent settings.
 */
class SettingsEditor : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsEditor(QWidget* parent = nullptr);
  ~SettingsEditor() override;

private:
  QSplitter* m_splitter{nullptr};
  QListView* m_top_item_list_view{nullptr};
  QTreeView* m_settings_view;
};

 }  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SETTINGS_EDITOR_H_
