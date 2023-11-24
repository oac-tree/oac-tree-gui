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
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/panel_toolbar.h>
#include <sup/gui/widgets/style_utils.h>

#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/widgets/item_view_component_provider.h>
#include <mvvm/widgets/property_tree_view.h>

#include <QSettings>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace
{

const QString kGroupName("ProcedureTreesWidget");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kProcedureHeaderStateSettingName = kGroupName + "/" + "header_state";

}  // namespace

namespace sequencergui
{
ProcedureTreesWidget::ProcedureTreesWidget(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new sup::gui::PanelToolBar)
    , m_procedure_tree(new QTreeView)
    , m_procedure_tree_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_procedure_tree))
    , m_procedure_custom_header(new sup::gui::CustomHeaderView(this))
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

  m_procedure_tree->setHeader(m_procedure_custom_header);
  m_procedure_tree->setAlternatingRowColors(true);
  m_procedure_tree->header()->setStretchLastSection(true);

  connect(m_procedure_tree_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged,
          m_property_tree, &mvvm::PropertyTreeView::SetItem);

  sup::gui::utils::BeautifyTreeStyle(m_procedure_tree);

  ReadSettings();
}

ProcedureTreesWidget::~ProcedureTreesWidget()
{
  WriteSettings();
}

void ProcedureTreesWidget::SetProcedure(ProcedureItem *procedure_item)
{
  m_procedure_tree_provider->SetItem(procedure_item);
  m_procedure_tree->expandAll();
  AdjustColumnWidth();
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
  if (settings.contains(kProcedureHeaderStateSettingName))
  {
    m_procedure_custom_header->SetAsFavoriteState(
        settings.value(kProcedureHeaderStateSettingName).toByteArray());
  }
}

void ProcedureTreesWidget::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
  if (m_procedure_custom_header->HasFavoriteState())
  {
    settings.setValue(kProcedureHeaderStateSettingName,
                      m_procedure_custom_header->GetFavoriteState());
  }
}

void ProcedureTreesWidget::AdjustColumnWidth()
{
  if (m_procedure_custom_header->HasFavoriteState())
  {
    m_procedure_custom_header->RestoreFavoriteState();
  }
  else
  {
    m_procedure_tree->resizeColumnToContents(0);
  }
}

}  // namespace sequencergui
