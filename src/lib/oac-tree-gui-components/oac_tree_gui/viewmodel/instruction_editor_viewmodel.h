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

#ifndef OAC_TREE_GUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

class IItemDropHandler;

/**
 * @brief The InstructionEditorViewModel class is a model with two columns: display name and
 * editable description.
 *
 * It is used during instruction tree composition in InstructionEditorWidget.
 * It has following features:
 *
 * 1) Internal move within the same tree view.
 * Drag of item leads to children reordering, or move from one parent to another.
 *
 * 2) External move (from one panel to another, same view model type, different instances).
 * Viewmodels can look to the same instruction trees, or to the trees belonging to the different
 * procedures. Same behavior as in [1]. The item will be removed from the source and added to the
 * target.
 *
 * 3) New item drop.
 * Copy from the toolbox's tree of available instructions will lead to the creation of new
 * instruction.
 *
 * 4) Drag from here and drop to favorite instructions panel.
 * Will create a copy of the instruction in the favorites. This viewmodel will remain intact.
 *
 * 5) Drag from favorites and drop here.
 * Will create a copy of the instruction from favorites here. Favorites will remain intact.
 *
 * 6) Drag between different applications.
 * Unlike 1 and 2, will lead just to the copy of the instruction, without removing it from the
 * source.
 */
class InstructionEditorViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit InstructionEditorViewModel(mvvm::ISessionModel* model, QObject* parent_object = nullptr);
  ~InstructionEditorViewModel() override;

  InstructionEditorViewModel(const InstructionEditorViewModel&) = delete;
  InstructionEditorViewModel& operator=(const InstructionEditorViewModel&) = delete;
  InstructionEditorViewModel(InstructionEditorViewModel&&) = delete;
  InstructionEditorViewModel& operator=(InstructionEditorViewModel&&) = delete;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  QStringList mimeTypes() const override;

  QMimeData* mimeData(const QModelIndexList& index_list) const override;

  Qt::DropActions supportedDragActions() const override;

  Qt::DropActions supportedDropActions() const override;

  bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                       const QModelIndex& parent) const override;

  bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                    const QModelIndex& parent) override;

private:
  std::unique_ptr<IItemDropHandler> m_drop_handler;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_
