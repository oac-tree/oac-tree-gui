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

#ifndef SEQUENCERGUI_EXPLORER_SEQUENCEREXPLORERVIEW_H
#define SEQUENCERGUI_EXPLORER_SEQUENCEREXPLORERVIEW_H

#include <QWidget>

class QSplitter;

namespace sequencergui
{
class SequencerModel;
class XMLEditor;
class ExplorerPanel;
class ProcedureTreesWidget;
class ProcedureItem;

class SequencerExplorerView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerExplorerView(QWidget* parent = nullptr);
  ~SequencerExplorerView() override;

  void SetModel(SequencerModel* model);

  void ShowXMLFile(const QString& file_name);

  void ShowSelectedProcedure(ProcedureItem* procedure_item);

  void CreateNewProcedure();

private:
  void SetupConnections();

  ExplorerPanel* m_explorer_panel{nullptr};
  ProcedureTreesWidget* m_trees_widget{nullptr};
  XMLEditor* m_xml_editor{nullptr};
  QSplitter* m_splitter{nullptr};

  std::unique_ptr<SequencerModel> m_temp_model;  //!< temporary model for disk browsing
  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_EXPLORER_SEQUENCEREXPLORERVIEW_H
