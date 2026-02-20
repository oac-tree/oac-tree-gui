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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_FAVORITE_INSTRUCTIONS_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_FAVORITE_INSTRUCTIONS_WIDGET_H_

#include <QWidget>

namespace oac_tree_gui
{

class ToolKitViewModel;
class AvailableInstructionsTreeView;

/**
 * @brief The FavoriteInstructionsWidget holds the list of user's favorite instructions.
 *
 * Located at the left bottom corner of SequencerComposerView.
 *
 * The user can drag-and-drop instructions from other places to here (this will create copies).
 * Then, the user can dran-and-drop from here to other places (this will create copies there, but
 * won't remove instructions from here).
 *
 * The user can also double-click instruction name to add it to the currently opened procedure.
 *
 * Favorite instructions are saved in the project file, so they are restored when the user opens the
 * project again.
 */
class FavoriteInstructionsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FavoriteInstructionsWidget(QWidget* parent_widget = nullptr);

signals:
  void InstructionDoubleClicked(const QString& name);

private:
  ToolKitViewModel* m_toolkit_viewmodel{nullptr};
  AvailableInstructionsTreeView* m_tree_view{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_FAVORITE_INSTRUCTIONS_WIDGET_H_
