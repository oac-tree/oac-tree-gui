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

#include "sequencer_composer_view.h"

#include <sequencergui/composer/composer_panel.h>
#include <sequencergui/composer/composer_widget_panel.h>
#include <sequencergui/composer/sequencer_composer_actions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/widgets/widget_utils.h>

#include <QSettings>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTreeView>

namespace
{
const QString kGroupName("SequencerComposerView");
const QString kSplitterSettingName = kGroupName + "/" + "splitter";

}  // namespace

namespace sequencergui
{
SequencerComposerView::SequencerComposerView(QWidget *parent)
    : QWidget(parent)
    , m_composer_panel(new ComposerPanel)
    , m_central_panel(new ComposerWidgetPanel)
    , m_right_panel(new ComposerWidgetPanel)
    , m_splitter(new QSplitter)
    , m_composer_actions(new SequencerComposerActions(this))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 1, 4, 4);

  m_splitter->addWidget(m_composer_panel);
  m_splitter->addWidget(m_central_panel);
  m_splitter->addWidget(m_right_panel);

  m_central_panel->SetCurrentWidget(ComposerWidgetPanel::kInstructionTree);
  m_right_panel->SetCurrentWidget(ComposerWidgetPanel::kWorkspace);

  m_splitter->setSizes(QList<int>() << mvvm::utils::UnitSize(40) << mvvm::utils::UnitSize(80)
                                    << mvvm::utils::UnitSize(80));

  layout->addWidget(m_splitter);

  SetupConnections();

  // Add actions from SequencerComposerActions to the list of as actions of this widget.
  // They will be used by MainWindow for QMenuBar
  addActions(m_composer_actions->GetMenuActions());

  ReadSettings();
}

SequencerComposerView::~SequencerComposerView()
{
  WriteSettings();
}

void SequencerComposerView::SetModel(SequencerModel *model)
{
  m_model = model;
  m_composer_panel->SetModel(model);
  m_central_panel->SetModel(model);
  m_right_panel->SetModel(model);
}

void SequencerComposerView::showEvent(QShowEvent *event)
{
  Q_UNUSED(event);
  if (!m_composer_panel->GetSelectedProcedure())
  {
    m_composer_panel->SetSelectedProcedure(GetFirstProcedure());
  }
}

void SequencerComposerView::ReadSettings()
{
  const QSettings settings;

  if (settings.contains(kSplitterSettingName))
  {
    m_splitter->restoreState(settings.value(kSplitterSettingName).toByteArray());
  }
}

void SequencerComposerView::WriteSettings()
{
  QSettings settings;
  settings.setValue(kSplitterSettingName, m_splitter->saveState());
}

void SequencerComposerView::SetupConnections()
{
  auto on_procedure_selected = [this](auto procedure_item)
  {
    m_central_panel->SetProcedure(procedure_item);
    m_right_panel->SetProcedure(procedure_item);
    m_composer_actions->SetProcedure(procedure_item);
  };
  connect(m_composer_panel, &ComposerPanel::ProcedureSelected, this, on_procedure_selected);

  auto on_create_procedure = [this]()
  {
    auto procedure_item = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
    m_composer_panel->SetSelectedProcedure(procedure_item);
  };
  connect(m_composer_panel, &ComposerPanel::CreateNewProcedureRequest, this, on_create_procedure);

  auto on_remove_procedure = [this](auto procedure)
  {
    if (procedure)
    {
      m_model->RemoveItem(procedure);
    }
  };
  connect(m_composer_panel, &ComposerPanel::RemoveProcedureRequest, this, on_remove_procedure);
}

//! Returns first procedure from the procedure container, if exist.
ProcedureItem *SequencerComposerView::GetFirstProcedure()
{
  auto procedure_container = m_model->GetProcedureContainer();
  return procedure_container->IsEmpty() ? nullptr : procedure_container->GetItem<ProcedureItem>("");
}

}  // namespace sequencergui
