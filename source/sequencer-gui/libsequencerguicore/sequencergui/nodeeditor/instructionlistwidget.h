/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_INSTRUCTIONLISTWIDGET_H
#define SEQUENCERGUI_NODEEDITOR_INSTRUCTIONLISTWIDGET_H

#include <QListWidget>

namespace sequi
{
//! The list widget with all pieces, representing connectable elements of the node editor.
//! Located at the left-hand side of the NodeEditorWidget.
//! Elements from the list can be drag-and-dropped on the graphics scene.

class InstructionListWidget : public QListWidget
{
  Q_OBJECT

public:
  InstructionListWidget(QWidget* parent = nullptr);

  static QString piecesMimeType();

  QSize sizeHint() const override;

protected:
  void startDrag(Qt::DropActions supportedActions) override;

private:
  void populateList();
  void addEntry(const QString& name);
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_INSTRUCTIONLISTWIDGET_H
