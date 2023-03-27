/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

namespace testutils
{

//! Mocking class to test events coming from the model.

class MockModelListener : public mvvm::ModelListener<mvvm::SessionModelInterface>
{
public:
  explicit MockModelListener(const mvvm::SessionModelInterface* model);

  MOCK_METHOD(void, OnEvent, (const mvvm::event_variant_t& event));
};

}  // namespace testutils

#endif  // TESTS_LIBTESTMACHINERY_MOCK_MODEL_LISTENER_H_
