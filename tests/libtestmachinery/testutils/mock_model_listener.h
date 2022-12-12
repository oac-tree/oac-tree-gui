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

#ifndef TESTS_LIBTESTMACHINERY_MOCK_MODEL_LISTENER_H_
#define TESTS_LIBTESTMACHINERY_MOCK_MODEL_LISTENER_H_

#include <gmock/gmock.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/signals/model_listener.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

//! Mocking class to test ModelEventListenerInterface reactions on notifications issued by
//! ModelEventNotifier.

class MockModelListener : public mvvm::ModelListener<mvvm::SessionModelInterface>
{
public:
  explicit MockModelListener(mvvm::SessionModelInterface* model);

  MOCK_METHOD(void, OnAboutToInsertItem,
              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD(void, OnItemInserted, (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD(void, OnAboutToRemoveItem,
              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD(void, OnItemRemoved, (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD(void, OnDataChanged, (mvvm::SessionItem * item, int role));

  MOCK_METHOD(void, OnModelAboutToBeReset, (mvvm::SessionModelInterface * model));

  MOCK_METHOD(void, OnModelReset, (mvvm::SessionModelInterface * model));

  MOCK_METHOD(void, OnModelAboutToBeDestroyed, (mvvm::SessionModelInterface * model));
};

#endif  // TESTS_LIBTESTMACHINERY_MOCK_MODEL_LISTENER_H_
