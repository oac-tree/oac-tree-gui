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

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_TOOLKIT_TREE_VIEW_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_TOOLKIT_TREE_VIEW_H_

#include <QTreeView>

namespace sequencergui
{

/**
 * @brief The InsructionToolKitTreeView class is a tree view with instruction groups to
 * drag-and-drop on the graphics scene.
 *
 * Its only purpose is to tweak the drag-and-drop indicator.
 */
class InsructionToolKitTreeView : public QTreeView
{
  Q_OBJECT
public:
  explicit InsructionToolKitTreeView(QWidget* parent = nullptr);

signals:
  void InstructionDoubleClicked(const QString& name);

protected:
  void startDrag(Qt::DropActions supported_actions) override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_TOOLKIT_TREE_VIEW_H_
