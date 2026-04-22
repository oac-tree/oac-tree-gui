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

namespace mvvm
{
class ItemViewComponentProvider;
}  // namespace mvvm

namespace oac_tree_gui
{

class InstructionContainerItem;
class FlatListView;

/**
 * @brief The FavoriteInstructionsWidget holds the list of user's favorite instructions.
 *
 * Located at the left bottom corner of SequencerComposerView.
 *
 * The user can drag-and-drop instructions from other places to here (this will create copies).
 * The user can dran-and-drop from here to other places (this will create copies there, but
 * won't remove instructions from here).
 *
 * Favorite instructions are saved in the project file, so they are restored when the user opens the
 * project again.
 */
class FavoriteInstructionsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FavoriteInstructionsWidget(QWidget* parent_widget = nullptr);
  ~FavoriteInstructionsWidget() override;

  FavoriteInstructionsWidget(const FavoriteInstructionsWidget&) = delete;
  FavoriteInstructionsWidget& operator=(const FavoriteInstructionsWidget&) = delete;
  FavoriteInstructionsWidget(FavoriteInstructionsWidget&&) = delete;
  FavoriteInstructionsWidget& operator=(FavoriteInstructionsWidget&&) = delete;

  void SetInstructionContainer(InstructionContainerItem* instruction_container);

private:
  std::unique_ptr<mvvm::ItemViewComponentProvider> CreateProvider() const;

  FlatListView* m_list_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_FAVORITE_INSTRUCTIONS_WIDGET_H_
