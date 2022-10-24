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

#ifndef TESTS_LIBTESTMACHINERY_MOCKMODELLISTENER_H
#define TESTS_LIBTESTMACHINERY_MOCKMODELLISTENER_H

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

  MOCK_METHOD2(OnAboutToInsertItem,
               void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnItemInserted, void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnAboutToRemoveItem,
               void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnItemRemoved, void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

  MOCK_METHOD2(OnDataChanged, void(mvvm::SessionItem* item, int role));

  MOCK_METHOD1(OnModelAboutToBeReset, void(mvvm::SessionModelInterface* model));

  MOCK_METHOD1(OnModelReset, void(mvvm::SessionModelInterface* model));

  MOCK_METHOD1(OnModelAboutToBeDestroyed, void(mvvm::SessionModelInterface* model));
};

#endif  // TESTS_LIBTESTMACHINERY_MOCKMODELLISTENER_H
