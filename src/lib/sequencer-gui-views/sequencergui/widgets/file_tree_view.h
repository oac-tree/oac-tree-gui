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

#ifndef SEQUENCERGUI_WIDGETS_FILE_TREE_VIEW_H_
#define SEQUENCERGUI_WIDGETS_FILE_TREE_VIEW_H_

#include <QWidget>

class QTreeView;
class QFileSystemModel;
class QLabel;
class QAction;
class QMenu;

namespace sup::gui
{
class RecentProjectSettings;
class ActionMenu;
}

namespace sequencergui
{

/**
 * @brief The FileTreeView class is a simple file browser with breadcrumb on top, and tree view at
 * the bottom.
 */
class FileTreeView : public QWidget
{
  Q_OBJECT

public:
  explicit FileTreeView(QWidget* parent = nullptr);
  ~FileTreeView() override;

  /**
   * @brief Sets the directory to be shown in a breadcrumb, and in a file tree view.
   */
  void SetCurrentDir(const QString& dirname);

signals:
  void ProcedureFileClicked(const QString& filename);
  void ProcedureFileDoubleClicked(const QString& filename);

private:
  /**
   * @brief Processes mouse click on a breadcrumb label.
   *
   * Navigates to a directory following the label link.
   *
   * @param link A special html text encoded in a label.
   */
  void OnLabelClick(const QString& link);

  /**
   * @brief Processes double click on a file tree.
   *
   * Used to import procedures, or change working directories.
   */
  void OnTreeDoubleClick(const QModelIndex& index);

  /**
   * @brief Process import from file button request.
   */
  void OnImportFromFileRequest();

  /**
   * @brief Processes single click on a file tree.
   *
   * Used to show content of the file in XML view, or in a tree view.
   */
  void OnTreeSingleClick(const QModelIndex& index);

  /**
   * @brief Setups actions for toolbar.
   */
  void SetupActions();

  /**
   * @brief Populates bookmark menu with entries.
   */
  void OnAboutToShowBookmarkMenu();

  QFileSystemModel* m_file_system_model{nullptr};
  QTreeView* m_tree_view{nullptr};
  QLabel* m_path_label{nullptr};
  QString m_current_xml_file;
  QAction* m_import_file_action{nullptr};
  sup::gui::ActionMenu* m_bookmark_action{nullptr};
  std::unique_ptr<sup::gui::RecentProjectSettings> m_recent_dirs;
  std::unique_ptr<QMenu> m_bookmark_menu;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_FILE_TREE_VIEW_H_
