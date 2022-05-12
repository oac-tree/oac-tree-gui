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

#include "sequencergui/components/procedurelistwidget.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/viewmodel/viewmodel.h"

#include <gtest/gtest.h>

#include <QDebug>
#include <QListView>
#include <QSignalSpy>

using namespace sequencergui;

Q_DECLARE_METATYPE(sequencergui::ProcedureItem*)

//! Tests for utility functions related to the domain to presentation transformations.

class ProcedureListWidgetTest : public ::testing::Test
{
public:
  ProcedureListWidgetTest()
  {
    qRegisterMetaType<sequencergui::ProcedureItem*>("sequencergui::ProcedureItem*");
  }
};

TEST_F(ProcedureListWidgetTest, InitialState)
{
  ProcedureListWidget view;
  EXPECT_EQ(view.GetSelectedProcedure(), nullptr);
  EXPECT_TRUE(view.GetSelectedProcedures().empty());
}

TEST_F(ProcedureListWidgetTest, SelectProcedure)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  ProcedureListWidget view;
  QSignalSpy spy_selected(&view, &ProcedureListWidget::ProcedureSelected);

  view.SetModel(&model);
  EXPECT_EQ(view.GetSelectedProcedure(), nullptr);

  // selecting an item and checking results
  view.SetSelectedProcedure(procedure);
  EXPECT_EQ(view.GetSelectedProcedure(), procedure);
  EXPECT_EQ(view.GetSelectedProcedures(), std::vector<ProcedureItem*>({procedure}));
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_procedure = arguments.at(0).value<sequencergui::ProcedureItem*>();
  EXPECT_EQ(selected_procedure, procedure);

  spy_selected.clear();

  // removing selection

  view.SetSelectedProcedure(nullptr);
  EXPECT_EQ(view.GetSelectedProcedure(), nullptr);
  EXPECT_EQ(spy_selected.count(), 1);

  arguments = spy_selected.takeFirst();
  selected_procedure = arguments.at(0).value<sequencergui::ProcedureItem*>();
  EXPECT_EQ(selected_procedure, nullptr);
}

//! Removing selected and checking notifications

TEST_F(ProcedureListWidgetTest, SelectionAfterRemoval)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  ProcedureListWidget view;
  view.SetModel(&model);

  // selecting single item
  view.SetSelectedProcedure(procedure);

  // checking selections
  EXPECT_EQ(view.GetSelectedProcedures(), std::vector<sequencergui::ProcedureItem*>({procedure}));

  QSignalSpy spy_selected(&view, &ProcedureListWidget::ProcedureSelected);

  // removing item
  model.RemoveItem(procedure);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(spy_selected.count(), 1);
  auto arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_procedure = arguments.at(0).value<sequencergui::ProcedureItem*>();
  EXPECT_EQ(selected_procedure, nullptr);
}

//! Checking selection when acting through the view.

TEST_F(ProcedureListWidgetTest, SetCurrentIndex)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  ProcedureListWidget view;
  QSignalSpy spy_selected(&view, &ProcedureListWidget::ProcedureSelected);

  view.SetModel(&model);
  EXPECT_EQ(view.GetSelectedProcedure(), nullptr);

  // selecting an item and checking results
  auto indexes = view.GetViewModel()->GetIndexOfSessionItem(procedure);
  ASSERT_EQ(indexes.size(), 2);
  view.GetListView()->setCurrentIndex(indexes.at(0));

  EXPECT_EQ(view.GetSelectedProcedure(), procedure);
  EXPECT_EQ(view.GetSelectedProcedures(), std::vector<ProcedureItem*>({procedure}));
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_procedure = arguments.at(0).value<sequencergui::ProcedureItem*>();
  EXPECT_EQ(selected_procedure, procedure);

  spy_selected.clear();
}
