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

#ifndef SEQUENCERGUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_
#define SEQUENCERGUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_

#include <QDialog>

class QLabel;

namespace sequencergui
{

class SettingsEditor;
class SettingsModel;

/**
 * @brief The SettingsEditorDialog class is a modal dialog that wraps SettingsEditor.
 */
class SettingsEditorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsEditorDialog(QWidget* parent = nullptr);
  ~SettingsEditorDialog() override;

  /**
   * @brief Sets initial values for editing.
   */
  void SetInitialValues(const SettingsModel& model);

private:
  /**
   * @brief Loads dialog persistence widget settings from disk.
   */
  void ReadSettings();

  /**
   * @brief Writes persistence widget settings on disk.
   */
  void WriteSettings();

  QLabel* m_label{nullptr};
  SettingsEditor* m_settings_editor{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SETTINGS_EDITOR_DIALOG_H_
