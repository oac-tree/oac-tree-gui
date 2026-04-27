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

#include "oac_tree_gui/composer/objects/procedure_list_action_handler.h"

#include <oac_tree_gui/components/mime_constants.h>
#include <oac_tree_gui/composer/objects/procedure_list_action_handler.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>

#include <sup/gui/components/mime_conversion_helper.h>

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>

#include <QMimeData>
#include <QSignalSpy>

Q_DECLARE_METATYPE(const oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui::test
{

/**
 * @brief Tests for ProcedureListActionHandler class.
 */
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
  std::unique_ptr<ProcedureListActionHandler> CreateActionHandler(
      ProcedureItem* current_selection, const QMimeData* current_mime = nullptr)
  {
    return std::make_unique<ProcedureListActionHandler>(
        CreateContext(current_selection, current_mime));
  }

  mvvm::ContainerItem* m_procedure_container{nullptr};
  mvvm::ApplicationModel m_model;
  std::unique_ptr<QMimeData> m_copy_result;
};

TEST_F(ProcedureListActionHandlerTest, InitialState)
{
  // all callbacks should be initialised
  EXPECT_THROW(ProcedureListActionHandler({}), RuntimeException);

  // selected procedure callback is not initialised
  EXPECT_THROW(ProcedureListActionHandler({[this]() { return m_procedure_container; }}),
               RuntimeException);
}

TEST_F(ProcedureListActionHandlerTest, InsertInEmptyContainer)
{
  auto handler = CreateActionHandler(nullptr);  // nothing is selected

  EXPECT_FALSE(handler->CanRemove());

  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_TRUE(m_procedure_container->IsEmpty());

  handler->OnCreateNewProcedureRequest();

  // new item appeared in the container
  ASSERT_FALSE(m_procedure_container->IsEmpty());
  auto inserted = m_procedure_container->GetAllItems().at(0);

  // request to select just inserted procedure
  EXPECT_EQ(mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request), inserted);
}

TEST_F(ProcedureListActionHandlerTest, AppendProcedureWhenNothingIsSelected)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  auto handler = CreateActionHandler(nullptr);  // nothing is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_EQ(m_procedure_container->GetSize(), 1);

  handler->OnCreateNewProcedureRequest();

  // new item appeared in the container
  ASSERT_EQ(m_procedure_container->GetSize(), 2);

  // request to select just inserted procedure
  const auto* send_item = mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request);

  // in the absence of selection item was appended to the container
  EXPECT_TRUE(mvvm::utils::AreTheSame(m_procedure_container->GetAllItems(),
                                      std::vector<const mvvm::SessionItem*>({proc0, send_item})));
}

TEST_F(ProcedureListActionHandlerTest, InsertBetweenTwoProcedures)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc1 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  auto handler = CreateActionHandler(proc0);  // first procedure is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_EQ(m_procedure_container->GetSize(), 2);

  handler->OnCreateNewProcedureRequest();

  // new item appeared in the container
  ASSERT_EQ(m_procedure_container->GetSize(), 3);

  // request to select just inserted procedure
  auto send_item = mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request);

  // sent item should be located between two procedures
  EXPECT_TRUE(
      mvvm::utils::AreTheSame(m_procedure_container->GetAllItems(),
                              std::vector<const mvvm::SessionItem*>({proc0, send_item, proc1})));
}

TEST_F(ProcedureListActionHandlerTest, RemoveMiddleProcedure)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc1 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc2 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  auto handler = CreateActionHandler(proc1);  // second procedure is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_EQ(m_procedure_container->GetSize(), 3);

  EXPECT_TRUE(handler->CanRemove());
  handler->OnRemoveProcedureRequest();

  // request to select a procedure just after the deleted one
  auto send_item = mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request);
  EXPECT_EQ(send_item, proc2);

  // middle procedure has disappeared from the container
  EXPECT_EQ(m_procedure_container->GetAllItems(), std::vector<mvvm::SessionItem*>({proc0, proc2}));
}

TEST_F(ProcedureListActionHandlerTest, RemoveLastProcedure)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  auto handler = CreateActionHandler(proc0);  // second procedure is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_TRUE(handler->CanRemove());
  handler->OnRemoveProcedureRequest();

  EXPECT_TRUE(m_procedure_container->IsEmpty());

  // request to select a procedure just after the deleted one, nullptr is expected
  auto send_item = mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request);
  EXPECT_EQ(send_item, nullptr);
}

TEST_F(ProcedureListActionHandlerTest, CopyPasteWhenNothingIsSelected)
{
  // nothing is selected
  auto handler = CreateActionHandler(nullptr);
  EXPECT_FALSE(handler->CanCopy());
  EXPECT_FALSE(handler->CanPaste());  // because mime data is empty
  EXPECT_FALSE(handler->CanCut());
}

TEST_F(ProcedureListActionHandlerTest, CopyOperationWhenItemIsSelected)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  proc0->SetDisplayName("abc");

  EXPECT_EQ(m_copy_result.get(), nullptr);

  auto handler = CreateActionHandler(proc0);  // selected procedure
  EXPECT_TRUE(handler->CanCopy());

  handler->Copy();

  // As a result of copy QMimeData object was created
  ASSERT_NE(m_copy_result.get(), nullptr);
  EXPECT_TRUE(m_copy_result->hasFormat(kProcedureCopyMimeType));
}

TEST_F(ProcedureListActionHandlerTest, PasteWrongMimeType)
{
  const QMimeData mime_data;

  EXPECT_TRUE(m_procedure_container->IsEmpty());

  // nothing is selected, invalid clipboard
  auto handler = CreateActionHandler(nullptr, &mime_data);

  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_FALSE(handler->CanPaste());

  handler->Paste();

  EXPECT_EQ(spy_selection_request.count(), 0);
}

TEST_F(ProcedureListActionHandlerTest, PasteOperationIntoEmptyModel)
{
  ProcedureItem item_to_paste;
  item_to_paste.SetDisplayName("abc");

  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kProcedureCopyMimeType);

  EXPECT_TRUE(m_procedure_container->IsEmpty());

  // nothing is selected, valid clipboard
  auto handler = CreateActionHandler(nullptr, mime_data.get());

  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_TRUE(handler->CanPaste());

  handler->Paste();

  // as a result of operation we have new procedure in a model
  ASSERT_EQ(m_procedure_container->GetSize(), 1);
  auto pasted_item = dynamic_cast<ProcedureItem*>(m_procedure_container->GetAllItems().at(0));
  ASSERT_NE(pasted_item, nullptr);

  EXPECT_EQ(pasted_item->GetDisplayName(), std::string("abc"));
  // request to select just inserted procedure
  EXPECT_EQ(mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request), pasted_item);
}

TEST_F(ProcedureListActionHandlerTest, PasteBetweenTwoItems)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc1 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  ProcedureItem item_to_paste;
  item_to_paste.SetDisplayName("abc");

  auto mime_data = sup::gui::CreateCopyMimeData(item_to_paste, kProcedureCopyMimeType);

  EXPECT_EQ(m_procedure_container->GetSize(), 2);

  // first procedure is selected, valid clipboard
  auto handler = CreateActionHandler(proc0, mime_data.get());

  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_TRUE(handler->CanPaste());

  handler->Paste();

  // as a result of operation we have new procedure in a model
  ASSERT_EQ(m_procedure_container->GetSize(), 3);
  auto pasted_item = dynamic_cast<ProcedureItem*>(m_procedure_container->GetAllItems().at(1));
  ASSERT_NE(pasted_item, nullptr);

  EXPECT_EQ(pasted_item->GetDisplayName(), std::string("abc"));
  // request to select just inserted procedure
  EXPECT_EQ(mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request), pasted_item);
}

TEST_F(ProcedureListActionHandlerTest, CutOperationWhenItemIsSelected)
{
  auto proc0 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  auto proc1 = m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());

  EXPECT_EQ(m_copy_result.get(), nullptr);

  auto handler = CreateActionHandler(proc0);  // first item is selected
  QSignalSpy spy_selection_request(handler.get(),
                                   &ProcedureListActionHandler::SelectProcedureRequest);

  EXPECT_TRUE(handler->CanCut());

  handler->Cut();

  ASSERT_EQ(m_procedure_container->GetSize(), 1);

  // request to select remaining item
  EXPECT_EQ(mvvm::test::GetSendItem<const ProcedureItem*>(spy_selection_request), proc1);
}

}  // namespace oac_tree_gui::test
