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

#ifndef SEQUENCERGUI_VIEWS_EXPLORER_SEQUENCER_EXPLORER_VIEW_H_
#define SEQUENCERGUI_VIEWS_EXPLORER_SEQUENCER_EXPLORER_VIEW_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{
class CodeView;
class ItemStackWidget;
}

namespace sequencergui
{

class SequencerModel;
class ExplorerPanel;
class ProcedureTreesWidget;
class ProcedureItem;

/**
 * @brief The SequencerExplorerView class is a main widget to browse procedures on disk.
 *
 * It is a first main view of sequencer main window. Contains 3 vertical panels:
 * - file browser on the left
 * - two trees with procedure structure and item properties
 * - XML viewer
 */
class SequencerExplorerView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerExplorerView(QWidget* parent = nullptr);
  ~SequencerExplorerView() override;

  void SetModel(SequencerModel* model);

  void ImportProcedure(const QString& file_name);

  void ShowXMLFile(const QString& file_name);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();

  ExplorerPanel* m_explorer_panel{nullptr};
  ProcedureTreesWidget* m_trees_widget{nullptr};
  sup::gui::CodeView* m_xml_view{nullptr};
  sup::gui::ItemStackWidget* m_right_panel{nullptr};
  QSplitter* m_splitter{nullptr};

  std::unique_ptr<SequencerModel> m_temp_model;  //!< temporary model for disk browsing
  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_EXPLORER_SEQUENCER_EXPLORER_VIEW_H_
