/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
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
