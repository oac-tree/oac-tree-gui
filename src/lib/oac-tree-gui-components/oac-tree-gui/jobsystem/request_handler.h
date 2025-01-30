/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef SEQUENCERGUI_JOBSYSTEM_REQUEST_HANDLER_H_
#define SEQUENCERGUI_JOBSYSTEM_REQUEST_HANDLER_H_

#include <future>

namespace oac_tree_gui
{

/**
 * @brief The RequestHandler class allow to request the data from another thread.
 *
 * @tparam DataT Type of the data to return.
 */
template <typename DataT>
class RequestHandler
{
public:
  RequestHandler() = default;

  /**
   * Gets the data from another thread (blocking). Will return the data when it become available.
   *
   * @return Returns requested data.
   *
   */
  DataT GetData()
  {
    auto future = promise.get_future();
    return future.get();
  }

  /**
   * Sends the data to the waiting thread.
   *
   * @param value The value which will be send.
   */
  void SendData(const DataT& value) { promise.set_value(value); }

private:
  std::promise<DataT> promise;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_JOBSYSTEM_REQUEST_HANDLER_H_
