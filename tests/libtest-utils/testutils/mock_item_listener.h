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

#ifndef TESTS_LIBTESTMACHINERY_MOCK_ITEM_LISTENER_H_
#define TESTS_LIBTESTMACHINERY_MOCK_ITEM_LISTENER_H_

#include <gmock/gmock.h>
#include <mvvm/signals/item_listener.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

namespace testutils
{

//! Mocking class to test ModelEventListenerInterface reactions on notifications issued by
//! ModelEventNotifier.

class MockItemListener : public mvvm::ItemListener<mvvm::SessionItem>
{
public:
  explicit MockItemListener(mvvm::SessionItem* item) { SetItem(item); }

  MOCK_METHOD(void, OnItemInserted, (mvvm::SessionItem * item, const mvvm::TagIndex& tagindex));
  MOCK_METHOD(void, OnAboutToRemoveItem,
              (mvvm::SessionItem * item, const mvvm::TagIndex& tagindex));
  MOCK_METHOD(void, OnItemRemoved, (mvvm::SessionItem * item, const mvvm::TagIndex& tagindex));
  MOCK_METHOD(void, OnDataChanged, (mvvm::SessionItem * item, int role));
  MOCK_METHOD(void, OnPropertyChanged, (mvvm::SessionItem * item, std::string name));
  MOCK_METHOD(void, Unsubscribe, ());

protected:
  void Subscribe() override;
};

}  // namespace testutils

#endif  // TESTS_LIBTESTMACHINERY_MOCK_ITEM_LISTENER_H_
