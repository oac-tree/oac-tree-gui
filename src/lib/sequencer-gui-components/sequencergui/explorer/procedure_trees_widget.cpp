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

#include "procedure_trees_widget.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/widgets/panel_toolbar.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/widgets/property_tree_view.h>
#include <mvvm/widgets/top_items_tree_view.h>

#include <QSettings>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{

const QString kGroupName("ProcedureTreesWidget");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";

}  // namespace

namespace sequencergui
{
ProcedureTreesWidget::ProcedureTreesWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new PanelToolBar)
    , m_procedure_tree(new mvvm::TopItemsTreeView)
    , m_property_tree(new mvvm::PropertyTreeView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_splitter = new QSplitter;
  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_procedure_tree);
  m_splitter->addWidget(m_property_tree);

  layout->addWidget(m_tool_bar);
  layout->addWidget(m_splitter);

  connect(m_procedure_tree, &mvvm::TopItemsTreeView::SelectedItemChanged, m_property_tree,
          &mvvm::PropertyTreeView::SetItem);

  sequencergui::styleutils::SetUnifiedPropertyStyle(m_procedure_tree->GetTreeView());

  ReadSettings();
}

ProcedureTreesWidget::~ProcedureTreesWidget()
{
  WriteSettings();
}

void ProcedureTreesWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure_tree->SetItem(procedure_item);
}

void ProcedureTreesWidget::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }
  else
  {
    m_splitter->setSizes(QList<int>() << 400 << 200);
  }
}

void ProcedureTreesWidget::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
}

}  // namespace sequencergui
