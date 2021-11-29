/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERTREEWIDGET_H
#define SEQUENCERGUI_COMPOSER_COMPOSERTREEWIDGET_H

#include "sequencergui/model/sequencer_types.h"

#include <QWidget>

class QSplitter;
class QTreeView;
class QTabWidget;

namespace ModelView
{
class TopItemsTreeView;
class PropertyTreeView;
class AllItemsTreeView;
}  // namespace ModelView

namespace sequi
{
class SequencerModel;
class InstructionItem;
class ComposerTreeToolBar;
class ProcedureItem;

//! The panel with two trees occupying right part of SequencerComposerView.
//! Containts TopItemsTreeView for the whole procedure, and PropertyTreeView for currently selected
//! instruction.

class ComposerTreeWidget : public QWidget
{
  Q_OBJECT

public:
  ComposerTreeWidget(QWidget* parent = nullptr);
  ~ComposerTreeWidget();

  void SetModel(SequencerModel* model, ProcedureItem* procedure);

  void SetSelected(InstructionItem* instruction);

private:
  void SetupConnections();

  ComposerTreeToolBar* m_tool_bar{nullptr};
  QTabWidget* m_tab_widget{nullptr};
  ModelView::TopItemsTreeView* m_instruction_tree{nullptr};
  ModelView::AllItemsTreeView* m_workspace_tree{nullptr};
  ModelView::PropertyTreeView* m_property_tree{nullptr};
  QSplitter* m_splitter{nullptr};

  SequencerModel* m_model{nullptr};
  ProcedureItem* m_procedure{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERTREEWIDGET_H
