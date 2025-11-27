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

#include "oac_tree_gui/views/composer/procedure_composer_combo_panel.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/views/composer/procedure_composer_combo_toolbar.h>

#include <sup/gui/app/null_command_service.h>

#include <mvvm/standarditems/container_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QStackedWidget>

Q_DECLARE_METATYPE(oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui::test
{

namespace
{
constexpr auto kNoProcedureSelected = " < no procedure >";
}

/**
 * @brief Tests for ProcedureComposerComboPanel class.
 */
class ProcedureComposerComboPanelTest : public ::testing::Test
{
public:
  SequencerModel m_model;
  sup::gui::NullCommandService m_command_service;
};

TEST_F(ProcedureComposerComboPanelTest, InitialState)
{
  const ProcedureComposerComboPanel widget(m_command_service, &m_model);
  EXPECT_EQ(widget.GetCurrentProcedure(), nullptr);
  EXPECT_EQ(widget.GetModel(), &m_model);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ProcedureComposerComboPanel::PlaceholderMode::kOn));

  auto toolbar = widget.findChild<ProcedureComposerComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);
  EXPECT_EQ(toolbar->GetSelectedProcedureName(), QString(kNoProcedureSelected));
}

TEST_F(ProcedureComposerComboPanelTest, SetModelAfterConstruction)
{
  ProcedureComposerComboPanel widget(m_command_service);
  EXPECT_EQ(widget.GetCurrentProcedure(), nullptr);
  EXPECT_EQ(widget.GetModel(), nullptr);

  widget.SetModel(&m_model);
  EXPECT_EQ(widget.GetModel(), &m_model);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ProcedureComposerComboPanel::PlaceholderMode::kOn));

  auto toolbar = widget.findChild<ProcedureComposerComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);
  EXPECT_EQ(toolbar->GetSelectedProcedureName(), QString(kNoProcedureSelected));
}

TEST_F(ProcedureComposerComboPanelTest, SetProcedure)
{
  ProcedureComposerComboPanel widget(m_command_service, &m_model);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure->SetDisplayName("Test procedure");

  QSignalSpy signal_spy(&widget, &ProcedureComposerComboPanel::selectedProcedureChanged);

  widget.SetProcedure(procedure);
  EXPECT_EQ(widget.GetCurrentProcedure(), procedure);

  ASSERT_EQ(signal_spy.count(), 1);
  EXPECT_EQ(signal_spy.takeFirst().at(0).value<ProcedureItem*>(), procedure);

  auto toolbar = widget.findChild<ProcedureComposerComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);
  EXPECT_EQ(toolbar->GetSelectedProcedureName(), QString("Test procedure"));

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ProcedureComposerComboPanel::PlaceholderMode::kOff));

  widget.SetProcedure(nullptr);
  EXPECT_EQ(widget.GetCurrentProcedure(), nullptr);

  ASSERT_EQ(signal_spy.count(), 1);
  EXPECT_EQ(signal_spy.takeFirst().at(0).value<ProcedureItem*>(), nullptr);

  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ProcedureComposerComboPanel::PlaceholderMode::kOn));
  EXPECT_EQ(toolbar->GetSelectedProcedureName(), QString(kNoProcedureSelected));
}

TEST_F(ProcedureComposerComboPanelTest, ChangeToPlaceholderWidgetOnProcedureDeletion)
{
  ProcedureComposerComboPanel widget(m_command_service, &m_model);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure->SetDisplayName("Test procedure");

  widget.SetProcedure(procedure);
  EXPECT_EQ(widget.GetCurrentProcedure(), procedure);

  auto stacked_widget = widget.findChild<QStackedWidget*>();
  ASSERT_NE(stacked_widget, nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ProcedureComposerComboPanel::PlaceholderMode::kOff));

  m_model.RemoveItem(procedure);
  EXPECT_EQ(widget.GetCurrentProcedure(), nullptr);
  EXPECT_EQ(stacked_widget->currentIndex(),
            static_cast<std::int32_t>(ProcedureComposerComboPanel::PlaceholderMode::kOn));
}

TEST_F(ProcedureComposerComboPanelTest, ChangeOfToolbarOnProcedureRename)
{
  ProcedureComposerComboPanel widget(m_command_service, &m_model);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure->SetDisplayName("Test procedure");

  widget.SetProcedure(procedure);
  EXPECT_EQ(widget.GetCurrentProcedure(), procedure);

  auto toolbar = widget.findChild<ProcedureComposerComboToolBar*>();
  ASSERT_NE(toolbar, nullptr);
  EXPECT_EQ(toolbar->GetSelectedProcedureName(), QString("Test procedure"));

  procedure->SetDisplayName("Renamed procedure");
  EXPECT_EQ(toolbar->GetSelectedProcedureName(), QString("Renamed procedure"));
}

}  // namespace oac_tree_gui::test
