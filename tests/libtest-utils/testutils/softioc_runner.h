/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Core System
 *
 * Description   : EPICS 7 C++ wrapper classes
 *
 * Author        : Gennady Pospelov
 *
 * Copyright (c) : 2010-2021 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SUP_EPICS_SOFTIOCRUNNER_H_
#define SUP_EPICS_SOFTIOCRUNNER_H_

#include <string>

namespace testutils
{

//! Provides possibility to start/stop 'softIoc' service on request.
//! Intended for use with google-test and its SetUpTestCase/TearDownTestCase machinery.
//! The goal is to isolate the user from startup/shutdown details of softIoc service
//! and provide a gurantee, that service will be available between Start/Stop calls.
class SoftIocRunner
{
public:
  SoftIocRunner(const std::string& session_name = {});
  ~SoftIocRunner();

  //! Starts the service.
  void Start(const std::string& db_file_content);

  //! Stops the service.
  void Stop();

  //! Returns true if SoftIoc service is active and running.
  bool IsActive() const;

  //! Returns the name of the EPICS database file name
  std::string GetDataBaseFileName() const;

protected:
  bool m_is_active;
  std::string m_session_name;
};

}

#endif  // SUP_EPICS_SOFTIOCRUNNER_H_
