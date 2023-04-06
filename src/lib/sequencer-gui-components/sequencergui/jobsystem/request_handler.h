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

#ifndef SEQUENCERGUI_JOBSYSTEM_REQUEST_HANDLE_H_
#define SEQUENCERGUI_JOBSYSTEM_REQUEST_HANDLE_H_

#include <functional>
#include <future>
#include <thread>

namespace sequencergui
{

/**
 * @brief The RequestHandler class allow to request the data from another thread.
 *
 * @tparam ArgT Type of input arguments to get the data.
 * @tparam DataT Type of the data to return.
 */
template <typename DataT, typename ArgT>
class RequestHandler
{
public:
  RequestHandler() = default;

  /**
   * Gets the data for given arguments. This is a blocking method which shall be used from a
   * thread that needs the data. Will return the data when it become available.
   *
   * @param arg Necessary arguments to get the data.
   * @return Returns requested data.
   *
   */
  DataT GetData(const ArgT& args)
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REQUEST_HANDLE_H_
