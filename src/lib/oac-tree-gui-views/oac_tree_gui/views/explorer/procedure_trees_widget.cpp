/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "procedure_trees_widget.h"

#include <oac_tree_gui/model/procedure_item.h>

#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/custom_splitter.h>

#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>
#include <mvvm/views/property_tree_view.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace
{

const QString kGroupName("ProcedureTreesWidget");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";
const QString kProcedureHeaderStateSettingName = kGroupName + "/" + "header_state";

}  // namespace

namespace oac_tree_gui
{

ProcedureTreesWidget::ProcedureTreesWidget(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_procedure_tree(new QTreeView)
    , m_procedure_tree_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_procedure_tree))
    , m_procedure_custom_header(
          new sup::gui::CustomHeaderView(kProcedureHeaderStateSettingName, this))
    , m_property_tree(new mvvm::PropertyTreeView)
    , m_splitter(new sup::gui::CustomSplitter(kSplitterSettingName))
{
  setWindowTitle("Procedure View");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->addWidget(m_procedure_tree);
  m_splitter->addWidget(m_property_tree);
  m_splitter->setSizes({400, 200});

  m_splitter->setWindowTitle("Procedure View");
  layout->addWidget(m_splitter);

  m_procedure_tree->setHeader(m_procedure_custom_header);
  m_procedure_tree->setAlternatingRowColors(true);
  m_procedure_tree->header()->setStretchLastSection(true);

  connect(m_procedure_tree_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged,
          m_property_tree, &mvvm::PropertyTreeView::SetItem);

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
  m_procedure_custom_header->AdjustColumnsWidth();
}

void ProcedureTreesWidget::ReadSettings()
{
  m_splitter->ReadSettings();
}

void ProcedureTreesWidget::WriteSettings()
{
  m_splitter->WriteSettings();
}

}  // namespace oac_tree_gui
