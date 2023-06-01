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

#ifndef SEQUENCERGUI_EXPLORER_SEQUENCER_EXPLORER_VIEW_H_
#define SEQUENCERGUI_EXPLORER_SEQUENCER_EXPLORER_VIEW_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{
class CodeView;
}

namespace sequencergui
{
class SequencerModel;
class ExplorerPanel;
class ProcedureTreesWidget;
class ProcedureItem;
class ItemStackWidget;

//! Main widget for XML browsing.
//! Contains file browser on the left, top items trees and property tree in the center, and XML
//! viewer on the right. Belongs to MainWindow.

class SequencerExplorerView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerExplorerView(QWidget* parent = nullptr);
  ~SequencerExplorerView() override;

  void SetModel(SequencerModel* model);

  void ShowXMLFile(const QString& file_name);

  void ShowSelectedProcedure(ProcedureItem* procedure_item);

private:
  void SetupConnections();

  ExplorerPanel* m_explorer_panel{nullptr};
  ProcedureTreesWidget* m_trees_widget{nullptr};
  sup::gui::CodeView* m_xml_view{nullptr};
  ItemStackWidget* m_right_panel{nullptr};
  QSplitter* m_splitter{nullptr};

  std::unique_ptr<SequencerModel> m_temp_model;  //!< temporary model for disk browsing
  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_EXPLORER_SEQUENCER_EXPLORER_VIEW_H_
