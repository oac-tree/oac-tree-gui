/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "instruction_editor_viewmodel.h"

#include "drag_and_drop_helper.h"

#include <sequencergui/model/sequencer_item_helper.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <QDebug>
#include <QMimeData>

namespace sequencergui
{

class InstructionEditorRowStrategy : public mvvm::RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static QStringList result = {"Type", "Name"};
    return result;
  }

  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRow(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;

    if (!item)
    {
      return result;
    }

    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));

    if (auto instruction = dynamic_cast<InstructionItem *>(item); instruction)
    {
      result.emplace_back(mvvm::CreateDataViewItem(GetNameItem(*instruction)));
    }
    return result;
  }
};

InstructionEditorViewModel::InstructionEditorViewModel(mvvm::SessionModelInterface *model,
                                                       QObject *parent)
    : ViewModel(parent)
{
  SetController(
      mvvm::factory::CreateController<mvvm::TopItemsStrategy, InstructionEditorRowStrategy>(model,
                                                                                            this));
}

Qt::ItemFlags InstructionEditorViewModel::flags(const QModelIndex &index) const
{
  auto default_flags = ViewModel::flags(index);
  if (index.isValid())
  {
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | default_flags;
  }

  return Qt::ItemIsDropEnabled | default_flags;
}

QMimeData *InstructionEditorViewModel::mimeData(const QModelIndexList &index_list) const
{
  qDebug() << "mimeData" << index_list;

  // ownership will be taken by QDrag operation
  return CreateInstructionMoveMimeData(index_list).release();
}

Qt::DropActions InstructionEditorViewModel::supportedDragActions() const
{
  return Qt::MoveAction;
}

Qt::DropActions InstructionEditorViewModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

/*
Here we explain parameters reported by canDropMimeData method. Suppose we have the following tree
hierarchy:

Message
Sequence
  Wait
Message

Schema below shows drop areas and reported parameters. Horizontal dashed lines appears as drop
indicators on attempt to drop between cells.

[0]  --------------    row_col=( 0,  0)     QModelIndex(-1, -1)
[1]  Message           row_col=(-1, -1)     QModelIndex(0, 0)
[2]  --------------    row_col=( 1,  0)     QModelIndex(-1, -1)
[3]  Sequence          row_col=(-1, -1)     QModelIndex(1, 0)
[4]  --------------    row_col=( 2,  0)     QModelIndex(-1, -1)    <-- problematic
[5]     -----------    row_col=( 0,  0)     QModelIndex(1, 0)
[6]     Wait           row_col=(-1, -1)     QModelIndex(0, 0)
[7]     -----------    row_col=( 1,  0)     QModelIndex(1, 0)
[8]  --------------    row_col=( 2,  0)     QModelIndex(-1, -1)
[9]  Message           row_col=(-1, -1)     QModelIndex(2, 0)
[10] --------------    row_col=( 3,  0)     QModelIndex(-1, -1)

Area #4 appears to be the same as area #8. Thus, from the model perspective it is not possible to
distinguish


*/

bool InstructionEditorViewModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
                                                 int row, int column,
                                                 const QModelIndex &parent) const
{
  qDebug() << "canDropMimeData" << data << action << row << column << parent;
  if (parent.isValid())
  {
    auto items = mvvm::utils::ItemsFromIndex({parent});
    qDebug() << "     " << items.size() << QString::fromStdString(items.at(0)->GetDisplayName());
  }
  return true;
}

bool InstructionEditorViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row,
                                              int column, const QModelIndex &parent)
{
  qDebug() << "dropMimeData" << data << action << row << column << parent;
  if (parent.isValid())
  {
    auto items = mvvm::utils::ItemsFromIndex({parent});
    qDebug() << "     " << items.size() << QString::fromStdString(items.at(0)->GetDisplayName());
  }
  return false;
}

}  // namespace sequencergui
