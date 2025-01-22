/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/views/composer/procedure_list_widget.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>

#include <mvvm/serialization/xml_document.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

#include <QListView>
#include <QSignalSpy>

Q_DECLARE_METATYPE(sequencergui::ProcedureItem*)

namespace sequencergui::test
{

//! Tests for utility functions related to the domain to presentation transformations.

class ProcedureListWidgetTest : public test::FolderTest
{
public:
  ProcedureListWidgetTest() : test::FolderTest("ProcedureListWidgetTest")
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

  EXPECT_EQ(mvvm::test::GetSendItem<sequencergui::ProcedureItem*>(spy_selected), procedure);

  spy_selected.clear();

  // removing selection
  view.SetSelectedProcedure(nullptr);
  EXPECT_EQ(view.GetSelectedProcedure(), nullptr);

  EXPECT_EQ(mvvm::test::GetSendItem<sequencergui::ProcedureItem*>(spy_selected), nullptr);
}

//! Removing selected and checking notifications

// FIXME find a way to enable test, or remove test
// Tests generates same pointers to the ProcedureListWidget objects, that leads to duplication of
// action context registreation

TEST_F(ProcedureListWidgetTest, DISABLED_SelectionAfterRemoval)
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
  EXPECT_EQ(mvvm::test::GetSendItem<sequencergui::ProcedureItem*>(spy_selected), nullptr);
}

//! Checking selection when acting through the view.

// FIXME find a way to enable test, or remove test
// Tests generates same pointers to the ProcedureListWidget objects, that leads to duplication of
// action context registration

TEST_F(ProcedureListWidgetTest, DISABLED_SetCurrentIndex)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  ProcedureListWidget view;
  QSignalSpy spy_selected(&view, &ProcedureListWidget::ProcedureSelected);

  view.SetModel(&model);
  EXPECT_EQ(view.GetSelectedProcedure(), nullptr);

  // selecting an item and checking results
  auto indexes = view.GetViewModel()->GetIndexOfSessionItem(procedure);
  ASSERT_EQ(indexes.size(),
            1);  // ProcedureViewModel for the moment generate the row with single entry only
  view.GetListView()->setCurrentIndex(indexes.at(0));

  EXPECT_EQ(view.GetSelectedProcedure(), procedure);
  EXPECT_EQ(view.GetSelectedProcedures(), std::vector<ProcedureItem*>({procedure}));

  EXPECT_EQ(mvvm::test::GetSendItem<sequencergui::ProcedureItem*>(spy_selected), procedure);
}

}  // namespace sequencergui::test
