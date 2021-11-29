/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MAINWINDOW_SEQUENCERXMLVIEW_H
#define SEQUENCERGUI_MAINWINDOW_SEQUENCERXMLVIEW_H

#include <QWidget>

class QSplitter;

namespace sequi
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

  std::unique_ptr<SequencerModel> m_temp_model; //!< temporary model for disk browsing
  SequencerModel* m_model{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_SEQUENCERXMLVIEW_H
