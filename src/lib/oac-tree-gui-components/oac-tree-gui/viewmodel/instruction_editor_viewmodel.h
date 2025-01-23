/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef SEQUENCERGUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class ISessionModel;
}

namespace sequencergui
{

/**
 * @brief The InstructionEditorViewModel class is a model with two columns: display name and
 * editable description.
 *
 * It allows to drag-and-drop instructions, and move instructions from one parent to another.
 */
class MVVM_VIEWMODEL_EXPORT InstructionEditorViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit InstructionEditorViewModel(mvvm::ISessionModel* model, QObject* parent_object = nullptr);

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  QMimeData* mimeData(const QModelIndexList& index_list) const override;

  Qt::DropActions supportedDragActions() const override;

  Qt::DropActions supportedDropActions() const override;

  bool canDropMimeData(const QMimeData* data, Qt::DropAction, int, int,
                       const QModelIndex&) const override;

  bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                    const QModelIndex& parent) override;

  QStringList mimeTypes() const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_INSTRUCTION_EDITOR_VIEWMODEL_H_
