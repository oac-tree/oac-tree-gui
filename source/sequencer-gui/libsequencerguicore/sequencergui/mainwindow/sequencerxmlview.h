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

#ifndef SEQUENCERGUI_MAINWINDOW_SEQUENCERXMLVIEW_H
#define SEQUENCERGUI_MAINWINDOW_SEQUENCERXMLVIEW_H

#include <QWidget>

class QSplitter;

namespace sequencergui
{
class SequencerModel;
class XMLEditor;
class ExplorerView;
class ProcedureTreesWidget;
class ProcedureItem;

class SequencerXMLView : public QWidget
{
  Q_OBJECT

public:
  SequencerXMLView(QWidget* parent = nullptr);
  ~SequencerXMLView();

  void SetModel(SequencerModel* model);

  void SetXMLFile(const QString& file_name);

  void OnAddToScratchpad(const QString& file_name);

  void onCreateNewProcedure();

  void onSratchpadProcedureSelected(ProcedureItem* procedure_item);

private:
  void SetupConnections();

  ExplorerView* m_explorer_view{nullptr};
  ProcedureTreesWidget* m_trees_widget{nullptr};
  XMLEditor* m_xml_editor{nullptr};
  QSplitter* m_splitter{nullptr};

  std::unique_ptr<SequencerModel> m_temp_model;  //!< temporary model for disk browsing
  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCERXMLVIEW_H
