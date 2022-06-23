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

#ifndef SEQUENCERGUI_MONITOR_MONITOR_REALTIME_TREE_WIDGET_H_
#define SEQUENCERGUI_MONITOR_MONITOR_REALTIME_TREE_WIDGET_H_

#include <QWidget>
#include <memory>

namespace mvvm
{
class ItemViewComponentProvider;
}

class QTreeView;

namespace sequencergui
{

class ProcedureItem;
class CustomHeaderView;
class InstructionItem;

//! Widget with expanded instruction tree for realtime job execution.
//! Located at the central panel of SequencerMonitorView.

class MonitorRealTimeTreeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MonitorRealTimeTreeWidget(QWidget* parent = nullptr);
  ~MonitorRealTimeTreeWidget() override;

  void SetProcedure(ProcedureItem* procedure_item);

  void SetSelectedInstruction(InstructionItem* item);

private:
  void AdjustColumnWidth();

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  CustomHeaderView* m_custom_header{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MONITOR_MONITOR_REALTIME_TREE_WIDGET_H_
