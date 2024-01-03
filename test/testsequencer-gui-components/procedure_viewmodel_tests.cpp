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

#include "sequencergui/viewmodel/procedure_viewmodel.h"

#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace sequencergui;

//! Tests for ProcedureViewModel class.

class ProcedureViewModelTest : public ::testing::Test
{
public:
  SequencerModel m_model;
};

TEST_F(ProcedureViewModelTest, SingleProcedure)
{
  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  ProcedureViewModel viewmodel(&m_model);
  viewmodel.SetRootSessionItem(m_model.GetProcedureContainer());

  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 1);

  auto procedure_displayname_index = viewmodel.index(0, 0);

  auto views = viewmodel.FindViews(procedure);
  EXPECT_EQ(views.size(), 1);
  EXPECT_EQ(viewmodel.indexFromItem(views[0]), procedure_displayname_index);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(procedure_displayname_index), procedure);

  EXPECT_EQ(viewmodel.data(procedure_displayname_index, Qt::DisplayRole).toString().toStdString(),
            std::string("Procedure"));
  EXPECT_EQ(viewmodel.data(procedure_displayname_index, Qt::EditRole).toString().toStdString(),
            std::string("Procedure"));

  EXPECT_TRUE(viewmodel.setData(procedure_displayname_index, QVariant("abc"), Qt::EditRole));

  EXPECT_EQ(procedure->GetDisplayName(), std::string("abc"));
}

TEST_F(ProcedureViewModelTest, TwoProcedures)
{
  ProcedureViewModel viewmodel(&m_model);
  viewmodel.SetRootSessionItem(m_model.GetProcedureContainer());

  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure0->SetDisplayName("abc");
  auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  EXPECT_EQ(viewmodel.rowCount(), 2);
  EXPECT_EQ(viewmodel.columnCount(), 1);

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(viewmodel.index(0, 0)), procedure0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(viewmodel.index(1, 0)), procedure1);

  EXPECT_EQ(viewmodel.data(viewmodel.index(0, 0), Qt::DisplayRole).toString().toStdString(),
            std::string("abc"));
  EXPECT_EQ(viewmodel.data(viewmodel.index(1, 0), Qt::DisplayRole).toString().toStdString(),
            std::string("Procedure1"));  // automaticaly generated display name
}

TEST_F(ProcedureViewModelTest, NotificationOnDisplayNameChange)
{
  ProcedureViewModel viewmodel(&m_model);
  viewmodel.SetRootSessionItem(m_model.GetProcedureContainer());

  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure0->SetDisplayName("abc");
  auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  QSignalSpy spy_data_changed(&viewmodel, &ProcedureViewModel::dataChanged);

  EXPECT_EQ(viewmodel.rowCount(), 2);
  EXPECT_EQ(viewmodel.columnCount(), 1);

  procedure0->SetDisplayName("efg");
  EXPECT_EQ(spy_data_changed.count(), 1);

  QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), viewmodel.index(0, 0));
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), viewmodel.index(0, 0));
  QVector<int> expected_roles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expected_roles);
}
