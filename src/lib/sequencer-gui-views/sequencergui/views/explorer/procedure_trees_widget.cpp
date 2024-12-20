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

#include "procedure_trees_widget.h"

#include <sequencergui/model/procedure_item.h>
#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/custom_header_view.h>
#include <sup/gui/widgets/custom_splitter.h>

#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/property_tree_view.h>

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

}  // namespace sequencergui
