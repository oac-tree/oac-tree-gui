/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/views/composer/procedure_list_widget.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>

#include <sup/gui/app/null_command_service.h>

#include <mvvm/model/item_selection.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

#include <QSignalSpy>
#include <QTreeView>

Q_DECLARE_METATYPE(const oac_tree_gui::ProcedureItem*)

// #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
// #endif

namespace oac_tree_gui::test
{

//! Tests for utility functions related to the domain to presentation transformations.

class ProcedureListWidgetTest : public test::FolderTest
{
public:
  ProcedureListWidgetTest() : test::FolderTest("ProcedureListWidgetTest")
  {
    qRegisterMetaType<oac_tree_gui::ProcedureItem*>("oac_tree_gui::ProcedureItem*");
  }

  sup::gui::NullCommandService m_command_service;
};

TEST_F(ProcedureListWidgetTest, InitialState)
{
  const ProcedureListWidget view(m_command_service);
  EXPECT_TRUE(view.GetSelection().IsEmpty());
}

TEST_F(ProcedureListWidgetTest, SelectProcedure)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  ProcedureListWidget view(m_command_service);
  QSignalSpy spy_selected(&view, &ProcedureListWidget::procedureSelectionChanged);

  view.SetModel(&model);
  EXPECT_TRUE(view.GetSelection().IsEmpty());

  // selecting an item and checking results
  view.SetSelection(mvvm::ItemSelection(procedure));
  EXPECT_EQ(view.GetSelection(), mvvm::ItemSelection(procedure));

  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(spy_selected),
            mvvm::ItemSelection(procedure));

  spy_selected.clear();

  // removing selection
  view.SetSelection(mvvm::ItemSelection());
  EXPECT_TRUE(view.GetSelection().IsEmpty());

  EXPECT_EQ(mvvm::test::TakeValue<const oac_tree_gui::ProcedureItem*>(spy_selected), nullptr);
}

TEST_F(ProcedureListWidgetTest, SelectionAfterRemoval)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  ProcedureListWidget view(m_command_service);
  view.SetModel(&model);

  // selecting single item
  view.SetSelection(mvvm::ItemSelection(procedure));

  // checking selections
  EXPECT_EQ(view.GetSelection(), mvvm::ItemSelection(procedure));

  QSignalSpy spy_selected(&view, &ProcedureListWidget::procedureSelectionChanged);

  // removing item
  model.RemoveItem(procedure);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(mvvm::test::TakeValue<const oac_tree_gui::ProcedureItem*>(spy_selected), nullptr);
}

TEST_F(ProcedureListWidgetTest, SetCurrentIndexViaView)
{
  SequencerModel model;
  auto procedure = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  ProcedureListWidget view(m_command_service);
  QSignalSpy spy_selected(&view, &ProcedureListWidget::procedureSelectionChanged);

  view.SetModel(&model);
  EXPECT_TRUE(view.GetSelection().IsEmpty());

  // selecting an item and checking results
  auto indexes = view.GetViewModel()->GetIndexOfSessionItem(procedure);
  ASSERT_EQ(indexes.size(), 2);  // display name and close button
  view.GetTreeView()->setCurrentIndex(indexes.at(0));

  EXPECT_EQ(view.GetSelection(), mvvm::ItemSelection(procedure));

  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(spy_selected),
            mvvm::ItemSelection(procedure));
}

}  // namespace oac_tree_gui::test
