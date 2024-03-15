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

#include "sequencergui/components/procedure_list_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/viewmodel/drag_and_drop_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

#include <QMimeData>
#include <QSignalSpy>

using namespace sequencergui;

Q_DECLARE_METATYPE(ProcedureItem*)

//! Tests for ProcedureListActionHandler class.

class ProcedureListActionHandlerTest : public ::testing::Test
{
public:
  ProcedureListActionHandlerTest()
  {
    m_procedure_container = m_model.InsertItem<mvvm::ContainerItem>();
  }

  /**
   * @brief Test helper to create context mimicking current widget state.
   *
   * @param current_selection Will be returned to the caller as current selection.
   * @param  current_mime Will be returned to the caller as current clipboard content.
   */
  ProcedureListContext CreateContext(ProcedureItem* current_selection,
                                     const QMimeData* current_mime = nullptr)
  {
    auto container_callback = [this]() { return m_procedure_container; };
    auto selected_calback = [current_selection]() { return current_selection; };
    auto get_mime_data_callback = [current_mime]() { return current_mime; };
    auto set_mime_data_callback = [this](std::unique_ptr<QMimeData> data)
    { m_copy_result = std::move(data); };
    return {container_callback, selected_calback, get_mime_data_callback, set_mime_data_callback};
  }

  /**
   * @brief Test helper to create action handler.
   */
  std::unique_ptr<ProcedureListActionHandler> CreateHandler(ProcedureItem* current_selection)
  {
    return std::make_unique<ProcedureListActionHandler>(CreateContext(current_selection));
  }

  mvvm::ContainerItem* m_procedure_container{nullptr};
  mvvm::ApplicationModel m_model;
  std::unique_ptr<QMimeData> m_copy_result;
};

//! Initial state of action handler.
TEST_F(ProcedureListActionHandlerTest, InitialState)
{
  // all callbacks should be initialised
  EXPECT_THROW(ProcedureListActionHandler({}), RuntimeException);

  // selected procedure callback is not initialised
  EXPECT_THROW(ProcedureListActionHandler({[this]() { return m_procedure_container; }}),
               RuntimeException);
}

//! Insert new procedure in empty container.
TEST_F(ProcedureListActionHandlerTest, InsertInEmptyContainer)
{
  auto handler = CreateHandler(nullptr);  // nothing is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_TRUE(m_procedure_container->IsEmpty());

  handler->OnCreateNewProcedureRequest();

  // new item appeared in the container
  ASSERT_FALSE(m_procedure_container->IsEmpty());
  auto inserted = m_procedure_container->GetAllItems().at(0);

  // request to select just inserted procedure
  EXPECT_EQ(testutils::GetSendItem<ProcedureItem*>(spy_selection_request), inserted);
}

//! Insert procedure after inserted procedure.
TEST_F(ProcedureListActionHandlerTest, AppendInContainerWhenNothingIsSelected)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  auto handler = CreateHandler(nullptr);  // nothing is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_EQ(m_procedure_container->GetSize(), 1);

  handler->OnCreateNewProcedureRequest();

  // new item appeared in the container
  ASSERT_EQ(m_procedure_container->GetSize(), 2);

  // request to select just inserted procedure
  auto send_item = testutils::GetSendItem<ProcedureItem*>(spy_selection_request);

  // in the absence of selection item was appended to the container
  EXPECT_EQ(m_procedure_container->GetAllItems(),
            std::vector<mvvm::SessionItem*>({proc0, send_item}));
}

//! Insert procedure between two procedures.
TEST_F(ProcedureListActionHandlerTest, InsertBetweenTwoProceduresContainer)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc1 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  auto handler = CreateHandler(proc0);  // first procedure is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_EQ(m_procedure_container->GetSize(), 2);

  handler->OnCreateNewProcedureRequest();

  // new item appeared in the container
  ASSERT_EQ(m_procedure_container->GetSize(), 3);

  // request to select just inserted procedure
  auto send_item = testutils::GetSendItem<ProcedureItem*>(spy_selection_request);

  // sent item should be located between two procedures
  EXPECT_EQ(m_procedure_container->GetAllItems(),
            std::vector<mvvm::SessionItem*>({proc0, send_item, proc1}));
}

//! Remove procedure from the middle.
TEST_F(ProcedureListActionHandlerTest, RemoveMiddleProcedure)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc1 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc2 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  auto handler = CreateHandler(proc1);  // second procedure is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_EQ(m_procedure_container->GetSize(), 3);

  handler->OnRemoveProcedureRequest();

  // new item disappeared from the container
  ASSERT_EQ(m_procedure_container->GetSize(), 2);

  // request to select a procedure just after the deleted one
  auto send_item = testutils::GetSendItem<ProcedureItem*>(spy_selection_request);
  EXPECT_EQ(send_item, proc2);

  EXPECT_EQ(m_procedure_container->GetAllItems(), std::vector<mvvm::SessionItem*>({proc0, proc2}));
}

//! Copy operation when nothing is selected.
TEST_F(ProcedureListActionHandlerTest, CopyPasteWhenNothingIsSelected)
{
  // nothing is selected
  auto handler = CreateHandler(nullptr);
  EXPECT_FALSE(handler->CanCopy());
}

//! Copy operation when item is selected.
TEST_F(ProcedureListActionHandlerTest, CopyOperation)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  proc0->SetName("abc");

  EXPECT_EQ(m_copy_result.get(), nullptr);

  auto handler = CreateHandler(proc0);  // selected procedure
  EXPECT_TRUE(handler->CanCopy());

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_copy_result.get(), nullptr);
  EXPECT_TRUE(m_copy_result->hasFormat(kCopyProcedureMimeType));
}
