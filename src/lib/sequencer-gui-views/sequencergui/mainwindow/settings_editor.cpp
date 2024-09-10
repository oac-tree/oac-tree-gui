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

#include "settings_editor.h"

#include <sequencergui/model/settings_model.h>

#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/property_flat_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QListView>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

SettingsEditor::SettingsEditor(QWidget *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter)
    , m_list_view(new QListView)
    , m_settings_view(new mvvm::PropertyFlatView)
    , m_list_component_provider(mvvm::CreateProvider<mvvm::TopItemsViewModel>(m_list_view))
    , m_property_view_model(std::make_unique<mvvm::PropertyViewModel>(nullptr))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_splitter);

  m_splitter->addWidget(m_list_view);
  m_splitter->addWidget(m_settings_view);
  m_splitter->setSizes(QList<int>() << 200 << 400);

  connect(m_list_component_provider.get(), &mvvm::ItemViewComponentProvider::SelectedItemChanged,
          this, [this](auto item) { SetSettingsGroup(item); });

  m_settings_view->layout()->setContentsMargins(mvvm::utils::UnitSize(1), 0, 0, 0);
}

void SettingsEditor::SetInitialValues(const SettingsModel &model)
{
  // in the absnce of ISessionModel::Clone, we just clone root item
  m_settings_model = std::make_unique<SettingsModel>();
  m_settings_model->ReplaceRootItem(model.GetRootItem()->Clone(/*unique_id*/ false));

  m_list_component_provider->SetApplicationModel(m_settings_model.get());
  m_settings_view->SetViewModel(m_property_view_model.get());
  m_list_component_provider->SetSelectedItem(m_settings_model->GetSettingsItems().at(0));
}

SettingsEditor::~SettingsEditor() = default;

void SettingsEditor::SetSettingsGroup(mvvm::SessionItem *item)
{
  m_property_view_model->SetRootSessionItem(item);
}

const SettingsModel *SettingsEditor::GetResult() const
{
  return m_settings_model.get();
}

}  // namespace sequencergui
