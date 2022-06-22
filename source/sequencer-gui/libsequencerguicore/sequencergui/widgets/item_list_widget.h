/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_WIDGETS_ITEM_LIST_WIDGET_H_
#define SEQUENCERGUI_WIDGETS_ITEM_LIST_WIDGET_H_

#include <QListWidget>

namespace sequencergui
{
//! The list widget with all pieces, representing connectable elements of the node editor.
//! Located at the left-hand side of the NodeEditorWidget.
//! Elements from the list can be drag-and-dropped on the graphics scene.

class ItemListWidget : public QListWidget
{
  Q_OBJECT

public:
  explicit ItemListWidget(QWidget* parent = nullptr);

  static QString piecesMimeType();

  QSize sizeHint() const override;

protected:
  void startDrag(Qt::DropActions supportedActions) override;

private:
  void populateList();
  void addEntry(const QString& name);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_ITEM_LIST_WIDGET_H_
