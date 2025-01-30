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

#ifndef OAC_TREE_GUI_JOBSYSTEM_REQUEST_HANDLER_QUEUE_H_
#define OAC_TREE_GUI_JOBSYSTEM_REQUEST_HANDLER_QUEUE_H_

#include <oac-tree-gui/jobsystem/request_handler.h>

#include <cassert>
#include <functional>
#include <mutex>
#include <queue>

namespace oac_tree_gui
{

/**
 * @brief The RequestHandlerQueue class provides consumer thread with the result of user input.
 * There can be more than one thread asking for input.
 *
 * @tparam DataT Type of input expected from the user.
 * @tparam ArgT Arguments that should be given to the user.
 */

template <typename DataT, typename ArgT>
class RequestHandlerQueue
{
public:
  using provider_callback_t = std::function<DataT(ArgT)>;
  using request_handler_t = RequestHandler<DataT>;

  explicit RequestHandlerQueue(provider_callback_t callback) : m_provider_callback(callback) {}

  /**
   * @brief Returns the data .
   *
   * @details The call is blocking and it is intended for call from consumer thread. Thread
   * will be released when the result is available. The method can be used from more than one
   * thread.
   *
   * @param args Arguments to provide.
   * @param notify_callback Special non-blocking callback that will notify the GUI that we need
   * data.
   *
   * @return Results of the user choice.
   */
  DataT GetData(const ArgT& args, std::function<void(void)> notify_callback)
  {
    RequestPack request_data;
    request_data.args = args;

    {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_stack.push(&request_data);
    }

    // asking the GUI for a data via queued connection
    notify_callback();

    // waiting for data to become available
    return request_data.request_handler.GetData();
  }

  /**
   * @brief Will call provider callback to get the data and will send the data to the consumer
   * thread. As a result of the call, consumer thread wating in ::GetData will be released.
   */
  void OnDataRequest()
  {
    RequestPack* request_data{nullptr};

    // accessing data request in a stack
    {
      std::unique_lock<std::mutex> lock(m_mutex);
      assert(!m_stack.empty());

      request_data = m_stack.front();
      // removing request from a stack before handling it further
      m_stack.pop();
    }

    // get the data from the provider callback
    auto args = request_data->args;
    auto data_to_send = m_provider_callback(args);

    // sending the data to waiting thread
    request_data->request_handler.SendData(data_to_send);
  }

private:
  struct RequestPack
  {
    //! Request arguments.
    ArgT args;

    //! Handler who knows how to pass data to the thread.
    request_handler_t request_handler;
  };

  mutable std::mutex m_mutex;

  //!< External provider which will return requested data for given set of arguments.
  provider_callback_t m_provider_callback;

  //!< Queue of requests from various threads.
  std::queue<RequestPack*> m_stack;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_JOBSYSTEM_REQUEST_HANDLER_QUEUE_H_
